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
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

// to define material only once
void MyDetectorConstruction::DefineMaterials()
{
	G4cout << "MyDetectorConstruction::DefineMaterials" << G4endl;
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
	G4cout << "Define Material GAGG" << G4endl;
	materialGAGG = new G4Material("materialGAGG", 6.6*g/cm3, 4);
	// materialGAGG = nist->FindOrBuildMaterial("G4_BGO");
	G4cout << "Add Element Gd" << G4endl;
	materialGAGG->AddElement(nist->FindOrBuildElement("Gd"), 3);
	G4cout << "Add Element Al" << G4endl;
	materialGAGG->AddElement(nist->FindOrBuildElement("Al"), 2);
	G4cout << "Add Element Ga" << G4endl;
	materialGAGG->AddElement(nist->FindOrBuildElement("Ga"), 3);
	G4cout << "Add Element O" << G4endl;
	materialGAGG->AddElement(nist->FindOrBuildElement("O"), 12);
	G4cout << "Added Element O" << G4endl;
	
	//G4Material *materialWater = new G4Material("water", 1.0*g/cm3, 3);
	//materialWater->AddElement(nist->FindOrBuildElement("G4_H"), 2);
	//materialWater->AddElement(nist->FindOrBuildElement("G4_O"), 1);
}


void MyDetectorConstruction::DefineMaterialsProperties()
{
	G4cout << "MyDetectorConstruction::DefineMaterialsProperties" << G4endl;
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
	G4cout << "MyDetectorConstruction::ConstructCollimator" << G4endl;
	// Collimator parameters
	hole_length = 30.*mm;
	septa_thickness = 10.*mm; // 2*mm;
	hole_thickness = 20.*mm; // 3*mm;
	case_side = 10.*cm; // fixed but not necessarily precise
	
	// Derived parameters
	pixel_size = hole_thickness + septa_thickness;
	holes_number = (G4int) case_side / pixel_size;
	case_side = (G4double) pixel_size * holes_number;
	G4cout << "pixel_size: " << pixel_size << " mm" << G4endl;
	G4cout << "holes_number: " << holes_number << " " << G4endl;
	G4cout << "case_side: " << case_side << " mm" << G4endl;
	
	// case
	G4cout << "defining the collimator case" << G4endl;
	solidCollimatorMatrix = new G4Box("solidCollimatorMatrix", case_side/2., case_side/2., hole_length/2.);
	logicCollimatorMatrix = new G4LogicalVolume(solidCollimatorMatrix, materialTungsten, "logicCollimatorMatrix");
	new G4PVPlacement(0, G4ThreeVector(0,0,-hole_length/2.), logicCollimatorMatrix, "physCollimatorMatrix", logicWorld, false, 0, true);
	
	// array
	G4cout << "defining the collimator array element" << G4endl;
	solidCollimatorArray = new G4Box("solidCollimatorArray", case_side/2., pixel_size/2., hole_length/2.);
	logicCollimatorArray = new G4LogicalVolume(solidCollimatorArray, materialTungsten, "logicCollimatorArray");
	new G4PVReplica("physCollimatorArray", logicCollimatorArray, logicCollimatorMatrix, kYAxis, holes_number, pixel_size, 0);
	
	// pixel
	G4cout << "defining the collimator pixel element" << G4endl;
	solidCollimatorPixel = new G4Box("solidCollimatorPixel", pixel_size/2., pixel_size/2., hole_length/2.);
	logicCollimatorPixel = new G4LogicalVolume(solidCollimatorPixel, materialTungsten, "logicCollimatorPixel");
	new G4PVReplica("physCollimatorPixel", logicCollimatorPixel, logicCollimatorArray, kXAxis, holes_number, pixel_size, 0);
	
	// pinhole
	solidCollimatorPinhole = new G4Box("solidCollimatorPinhole", hole_thickness/2., hole_thickness/2., hole_length/2.);
	logicCollimatorPinhole = new G4LogicalVolume(solidCollimatorPinhole, materialAir, "logicCollimatorPinhole");
	new G4PVPlacement(0, G4ThreeVector(), logicCollimatorPinhole, "physCollimatorPinhole", logicCollimatorPixel, false, 0, true);
}

void MyDetectorConstruction::ConstructScintillator()
{
	G4double slab_width  = 50 * mm;
	G4double slab_heigth = 50 * mm;
	G4double slab_thickness = 5 * mm;

	solidScintillator = new G4Box("solidScintillator", slab_width/2, slab_heigth/2, slab_thickness/2);
	logicScintillator = new G4LogicalVolume(solidScintillator, materialGAGG, "logicScintillator");

	physScintillator = new G4PVPlacement(0,  // no rotation
		G4ThreeVector(0.,0.,slab_thickness/2.), // at (0,0,0)
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
	G4cout << "MyDetectorConstruction::Construct" << G4endl;
	G4double world_half_Z  = 10*cm;
	G4double world_half_XY = 10*cm;

	solidWorld = new G4Box("solidWorld", world_half_XY, world_half_XY, world_half_Z);
	logicWorld = new G4LogicalVolume(solidWorld, materialAir, "logicWorld", 0, 0, 0, true);

	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, true);

	ConstructScintillator();
	ConstructCollimator();
	// SetVisualizationFeatures();

	return physWorld;
}

void MyDetectorConstruction::SetVisualizationFeatures()
{
	if (logicWorld)
		logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	if (logicCollimatorPixel)
		logicCollimatorPixel->SetVisAttributes(new G4VisAttributes(G4Colour(200./255, 200./255, 200./255,1)));
	if (logicCollimatorPinhole)
		logicCollimatorPinhole->SetVisAttributes(new G4VisAttributes(G4Colour(1.,.5,1.)));
	if (logicScintillator)
		logicScintillator->SetVisAttributes(new G4VisAttributes(G4Colour(100./255, 100./255, 100./255,1)));
}

void MyDetectorConstruction::ConstructSDandField()
{
	G4cout << "MyDetectorConstruction::ConstructSDandField" << G4endl;
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	if(logicScintillator != NULL)
		logicScintillator->SetSensitiveDetector(sensDet);
}
