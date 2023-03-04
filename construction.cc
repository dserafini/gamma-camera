#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	// first argument is the object to which it refers, this class
	// the slashes in the folder argument are mandatory
	// third argument is help text

	// define our command
	// we want to change the number of photon sensors in our column
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	// first argument is the name of the command
	// second argument is a variable
	// third argument is help text

	// same for the rows
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

	fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
	fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");
	fMessenger->DeclareProperty("isTOF", isTOF, "Construct Time Of Flight");

	// defualt values for the number of rows and columns
	nCols = 10;
	nRows = 10;

	// define materials just once
	DefineMaterials();
	DefineMaterialsProperties();

	// define world lengths
	xWorld = 5*m;
	yWorld = 5*m;
	zWorld = 5*m;

	isCherenkov = false;
	isScintillator = false;
	isTOF = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

// to define material only once
void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	materialAir      = nist->FindOrBuildMaterial("G4_AIR");
	materialTungsten = nist->FindOrBuildMaterial("G4_W");
	materialPMT      = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
	materialAluminum = nist->FindOrBuildMaterial("G4_Al");

	elLa = new G4Element("Lanthanum", "La", 57, 138.905*g/mole);
	elBr = new G4Element("Bromine", "Br", 35, 79.904*g/mole);

	G4double density     = 5.08 * g / cm3;
	G4int    ncomponents = 2;
	G4int natoms;
	G4String name;

	materialLanthanumBromide = new G4Material(name="LaBr3", density, ncomponents);
	materialLanthanumBromide->AddElement(elLa,natoms=1);
	materialLanthanumBromide->AddElement(elBr, natoms=3);
	
	// GaGG
	// the difference between the types of GaGGs involves only the properties and not the constituents
	materialGAGG = new G4Material("GaGG", 6.6*g/cm3, 4);
	materialGAGG->AddElement(nist->FindOrBuildElement("G4_Gd"), 3);
	materialGAGG->AddElement(nist->FindOrBuildElement("G4_Al"), 2);
	materialGAGG->AddElement(nist->FindOrBuildElement("G4_Ga"), 3);
	materialGAGG->AddElement(nist->FindOrBuildElement("G4_O"), 12);
}


void MyDetectorConstruction::DefineMaterialsProperties()
{
  const G4int nEntries = 2;

  G4double PhotonEnergy[nEntries] = {1.0*eV, 7.0*eV};

  G4double LaBr3RefracionIndex[nEntries] = {1.9,1.9};
  G4double LaBr3AbsorptionLength[nEntries] = {50.*cm,50.*cm};

  G4MaterialPropertiesTable* materialLanthanumBromideMPT = new G4MaterialPropertiesTable();

  materialLanthanumBromideMPT->AddProperty("RINDEX", PhotonEnergy, LaBr3RefracionIndex, nEntries);
  materialLanthanumBromideMPT->AddProperty("ABSLENGTH", PhotonEnergy, LaBr3AbsorptionLength, nEntries);

  G4double ScintEnergy[nEntries] = {3.25*eV, 3.44*eV};
  G4double ScintFast[nEntries] = {1.0,1.0};

  materialLanthanumBromideMPT->AddProperty("FASTCOMPONENT",ScintEnergy, ScintFast, nEntries);

  materialLanthanumBromideMPT->AddConstProperty("SCINTILLATIONYIELD", 63./keV);
  materialLanthanumBromideMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  materialLanthanumBromideMPT->AddConstProperty("FASTTIMECONSTANT",20.0*ns);
  materialLanthanumBromideMPT->AddConstProperty("YIELDRATIO",1.0);

  materialLanthanumBromide->SetMaterialPropertiesTable(materialLanthanumBromideMPT);
}

// when u change something in the detector construction u have to tell Geant4 to construct the whole world again
// so basically to start the method Construct again
// when u do that you have some memory problems cuz u create new volumes with the same name of previous existing ones
// we have to change the code in Construct method so that some parts are only created once
// this method is also proposed by the documentation of Geant4
// method:
// first move the declarations of G4Material, G4Solid, G4LogicalVolume into the header file
// then move material commands into another method that u call only once in the constructor
// in the gui change the number of columns or rows using the custom commands
// use /run/reinitializeGeometry
// run one event
// now you should see the changes

void MyDetectorConstruction::ConstructCollimator()
{
	// Collimator parameters
	hole_length = 30*mm;
	septa_thickness = 2*mm;
	hole_thickness = 3*mm;
	case_side = 10*cm; // fixed
	case_wall_thickness = 5*mm; // fixed
	
	// Derived parameters
	case_wall_thickness -= septa_thickness / 2.; // the wall comprehend the most outer pixels
	holes_number = TMath::Floor((case_side - 2*case_wall_thickness) / (hole_thickness + septa_thickness));
	pixel_size = hole_thickness + septa_thickness;
	
	// case
	solidCase = new G4Box("solidCase", case_side/2., case_side/2., hole_length/2.);
	logicCase = new G4LogicalVolume(solidCase, materialTungsten, "logicCase");
	G4PVPlacement(0, logicCase, "physCase", logicWorld, false, 0, true);
	
	// pixel
	solidCollimatorPixel = new G4Box("solidCollimatorPixel", pixel_size/2., pixel_size/2., hole_length/2.);
	logicCollimatorPixel = new G4LogicalVolume(solidCollimatorPixel, materialTungsten, "logicCollimatorPixel");
	
	// matrix
	G4PVReplica("physicalCollimatorString", logicCollimatorPixel, logicCase, kXAxis, holes_number, pixel_size, 0);
}

void MyDetectorConstruction::ConstructCase()
{
	G4double case_width  = 51 * mm;
	G4double case_heigth = 51 * mm;
	G4double case_thickness = 9 * mm;

	G4Box * sCase = new G4Box("Case solid", case_width/2, case_heigth/2, case_thickness/2);

	logicCase =  new G4LogicalVolume(sCase, materialAluminum, "logicCase");

	physCase = new G4PVPlacement(0,  // no rotation
																								G4ThreeVector(0.,0.,-collimator_thickness/2-case_thickness/2), // at (0,0,0)
																								logicCase,             // its logical volume
																								"physCase",           // its name
																								logicWorld,                  // its mother volume
																								false,                   // no boolean operations
																								0,                       // copy number
																							1); // checking overlaps
}

void MyDetectorConstruction::ConstructScintillator()
{
	G4double slab_width  = 50 * mm;
	G4double slab_heigth = 50 * mm;
	G4double slab_thickness = 5 * mm;

	G4Box * sLaBr3 = new G4Box("LYSO solid", slab_width/2, slab_heigth/2, slab_thickness/2);
	logicScintillator = new G4LogicalVolume(sLaBr3, materialLanthanumBromide, "logicScintillator");

	physScintillator = new G4PVPlacement(0,  // no rotation
																				G4ThreeVector(0.,0.,1.5*mm), // at (0,0,0)
																				logicScintillator,             // its logical volume
																				"physScintillator",           // its name
																				logicWorld,                  // its mother volume
																				false,                   // no boolean operations
																				0,                       // copy number
																				1); // checking overlaps

	fScoringVolume = logicScintillator;
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	G4double world_half_Z  = 5*cm;
	G4double world_half_XY = 5*cm;

	G4Box * sWorld = new G4Box("solidWorld", world_half_XY, world_half_XY, world_half_Z);
	logicWorld = new G4LogicalVolume(sWorld, materialAir, "logicWorld", 0, 0, 0, true);

	physWorld = new G4PVPlacement(0,
																G4ThreeVector(),
																logicWorld,
																"physWorld",
																0,
																false,
																0,
																true);

	ConstructScintillator();
	// ConstructCollimator();
	// ConstructCase(); // se lo attivi ricordati di metterlo come mother del logicScintillator
	// SetVisualizationFeatures();

	return physWorld;
}

void MyDetectorConstruction::SetVisualizationFeatures()
{
	if (logicWorld)
		logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	if (logicCollimator)
		logicCollimator->SetVisAttributes(new G4VisAttributes(G4Colour(200./255, 200./255, 200./255,1)));
	if (logicCase)
		logicCase->SetVisAttributes(new G4VisAttributes(G4Colour(50./255, 100./255, 200./255,1)));
	if (logicScintillator)
		logicScintillator->SetVisAttributes(new G4VisAttributes(G4Colour(100./255, 100./255, 100./255,1)));
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	if(logicScintillator != NULL)
		logicScintillator->SetSensitiveDetector(sensDet);
}
