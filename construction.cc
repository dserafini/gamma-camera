#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessengerCollimator = new G4GenericMessenger(this, "/collimator/", "Collimator Construction");
	// first argument is the object to which it refers, this class
	// the slashes in the folder argument are mandatory
	// third argument is help text

	// define our command
	fMessengerCollimator->DeclarePropertyWithUnit("hole_length", "mm", hole_length, "Length of the collimator holes");
	// first argument is the name of the command
	// second argument is a variable
	// third argument is help text

	fMessengerCollimator->DeclarePropertyWithUnit("hole_thickness", "mm", hole_thickness, "Thickness of the collimator holes");

	fMessengerCollimator->DeclarePropertyWithUnit("septa_thickness", "mm", septa_thickness, "Thickness of the collimator septa");

	fMessengerCollimator->DeclarePropertyWithUnit("case_side", "mm", case_side, "Side of the collimator case");

	// Collimator parameters
	hole_length = 30.*mm;
	septa_thickness = 1.*mm; // 2*mm;
	hole_thickness = 2.*mm; // 3*mm;
	case_side = 10.*cm; // fixed but not necessarily precise
	
	// scintillator commands
	fMessengerCollimator = new G4GenericMessenger(this, "/scintillator/", "Scintillator Construction");
	fMessengerCollimator->DeclarePropertyWithUnit("slab_side", "mm", slab_side, "Side of the collimator");
	fMessengerCollimator->DeclarePropertyWithUnit("slab_depth", "mm", slab_depth, "Depth of the collimator");
	
	// scintillator parameters
	slab_side  = case_side;
	slab_depth = hole_length/3.;

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
{
	delete fMessengerCollimator;
}

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

	G4double refractiveIndexGAGG[nEntries] = {1.9,1.9};
	G4double absorptionLengthGAGG[nEntries] = {50.*cm,50.*cm};

	G4MaterialPropertiesTable* mptGAGG = new G4MaterialPropertiesTable();

	mptGAGG->AddProperty("RINDEX", PhotonEnergy, refractiveIndexGAGG, nEntries);
	mptGAGG->AddProperty("ABSLENGTH", PhotonEnergy, absorptionLengthGAGG, nEntries);

	G4double ScintEnergy[nEntries] = {3.25*eV, 3.44*eV};
	G4double ScintFast[nEntries] = {1.0,1.0};

	mptGAGG->AddProperty("FASTCOMPONENT",ScintEnergy, ScintFast, nEntries);

	mptGAGG->AddConstProperty("SCINTILLATIONYIELD", 42000/MeV);
	mptGAGG->AddConstProperty("RESOLUTIONSCALE", 1.0);
	mptGAGG->AddConstProperty("FASTTIMECONSTANT",90.0*ns);
	mptGAGG->AddConstProperty("YIELDRATIO",1.0);

	materialGAGG->SetMaterialPropertiesTable(mptGAGG);
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
	solidScintillator = new G4Box("solidScintillator", slab_side/2., slab_side/2., slab_depth/2.);
	logicScintillator = new G4LogicalVolume(solidScintillator, materialGAGG, "logicScintillator");

	physScintillator = new G4PVPlacement(0,  // no rotation
		G4ThreeVector(0.,0.,slab_depth/2.), // at (0,0,0)
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

	ConstructCollimator();
	ConstructScintillator();
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
