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
	// Collimator Characteristics
	collimator_thickness = 30*mm;
	collimator_size = 48.5*mm;
	pinhole_size = 1.5*mm;
	pinhole_number = 21;

	// PinHole Geomtry
	G4Box * pinhole = new G4Box("pinhole", pinhole_size/2., pinhole_size/2., collimator_thickness/2.+2);

	// Holes Positioning
	std::vector<std::pair<double,double>> holes_coordinates;

	for(int i = 0; i < pinhole_number; i++)
	{
		for(int j = 0; j < pinhole_number; j++)
		{
			holes_coordinates.emplace_back(-collimator_size/2.+(4.25+i*2)*mm, -collimator_size/2.+(4.25+j*2)*mm);
		}
	}

	// Multiunion Solid Definition
	G4MultiUnion* sHole_placement = new G4MultiUnion("sHole_placement");

	for(auto & p : holes_coordinates)
	{
			G4RotationMatrix rot = G4RotationMatrix();
			G4ThreeVector    pos = G4ThreeVector(p.first, p.second, 0);
			G4Transform3D    tr  = G4Transform3D(rot, pos);
			sHole_placement->AddNode(*pinhole, tr);
	}

	sHole_placement->Voxelize();

	// Collimator solid definition
	G4Box* sCollimatorCore = new G4Box("Collimator Core Solid", collimator_size/2, collimator_size/2, collimator_thickness/2);

	G4SubtractionSolid * sCollimator = new G4SubtractionSolid("Collimator Solid", sCollimatorCore, sHole_placement, 0, G4ThreeVector());

	logicCollimator = new G4LogicalVolume(sCollimator, materialTungsten, "logicCollimator");

	// Collimator
	physCollimator = new G4PVPlacement(0,  // no rotation
																											G4ThreeVector(0.,0.,0.), // at (0,0,0)
																											logicCollimator,             // its logical volume
																											"physCollimator",           // its name
																											logicWorld,                  // its mother volume
																											false,                   // no boolean operations
																											0,                       // copy number
																											1); // checking overlaps
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

	return physWorld;
}

void MyDetectorConstruction::SetVisualizationFeatures()
{
	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	logicCollimator->SetVisAttributes(new G4VisAttributes(G4Colour(200./255, 200./255, 200./255,1)));
	logicCase->SetVisAttributes(new G4VisAttributes(G4Colour(50./255, 100./255, 200./255,1)));
	logicScintillator->SetVisAttributes(new G4VisAttributes(G4Colour(100./255, 100./255, 100./255,1)));
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	if(logicScintillator != NULL)
		logicScintillator->SetSensitiveDetector(sensDet);
}
