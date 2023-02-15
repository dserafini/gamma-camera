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
		// do not reinvent the wheel (materials)
		G4NistManager *nist = G4NistManager::Instance();

		// define material to use in the aerogel detector
		// fused silica
		SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
		SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
		SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

		// water
		H2O = new G4Material("H2O", 1.000*g/cm3, 2);
		H2O->AddElement(nist->FindOrBuildElement("H"), 2);
		H2O->AddElement(nist->FindOrBuildElement("O"), 1);

		// carbon
		C = nist->FindOrBuildElement("C");

		// build the aerogel material
		Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
		Aerogel->AddMaterial(SiO2, 62.5*perCent);
		Aerogel->AddMaterial(H2O, 37.4*perCent);
		Aerogel->AddElement(C, 0.1*perCent);

		worldMat = nist->FindOrBuildMaterial("G4_AIR");
		// all physics happens within a boundary called world volume
		// the world volume is what we have to return at the end of this method so it is necessary

		// I do not need to move the properties of the materials into DefineMaterials method
		// but I put also these commands regarding material in this method so that the code is more clear
		// set optical properties of aerogel
		G4double energy[2] = {1.2398*eV*um/0.2/um, 1.2398*eV*um/0.9/um};
		G4double rindexAerogel[2] = {1.1, 1.1};
		G4double rindexWorld[2] = {1.0, 1.0};
		G4double rindexNaI[2] = {1.78, 1.78};
		G4double reflectivity[2] = {1., 1.};
		// wa want all photons reflected

		G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
		mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

		Aerogel->SetMaterialPropertiesTable(mptAerogel);

		G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
		mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

		worldMat->SetMaterialPropertiesTable(mptWorld);

		Na = nist->FindOrBuildElement("Na");

		I = nist->FindOrBuildElement("I");

		NaI = new G4Material("NaI", 3.67*g/cm3, 2);
		NaI->AddElement(Na, 1);
		NaI->AddElement(I, 1);

		// assume same amount of red and blue photons
		G4double fraction[2] = {1.0, 1.0};

		G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
		mptNaI->AddProperty("RINDEX", energy, rindexNaI, 2);
		mptNaI->AddProperty("FASTCOMPONENT", energy, fraction, 2);

		// how many photons per energy loss of the particle we create
		// no array, it is a constant property
		mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38./keV);

		// the photons are emitted in time according to an exponential function
		// this is practically the decay time of the scintillator
		mptNaI->AddConstProperty("FASTTIMECONSTANT", 250*ns);

		mptNaI->AddConstProperty("YIELDRATIO", 1.);
		mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.);

		NaI->SetMaterialPropertiesTable(mptNaI);

		// we want to make a reflective coating for the scintillators
		mirrorSurface = new G4OpticalSurface("mirrorSurface");

		mirrorSurface->SetType(dielectric_metal);
		// he finds dielectric_metal type works the best
		mirrorSurface->SetFinish(ground);
		mirrorSurface->SetModel(unified);

		G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();

		mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);

		mirrorSurface->SetMaterialPropertiesTable(mptMirror);
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

void MyDetectorConstruction::ConstructCherenkov()
{
	// create the physical radiator
	solidRadiator = new G4Box("solidRadiator", .4*m, .4*m, .01*m);

	logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicRadiator");

	// the scoring volume is the radiator
	fScoringVolume = logicRadiator;

	physRadiator = new G4PVPlacement(0, G4ThreeVector(.0, .0, .25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);

	// I need it to understand
	G4double xCell = xWorld/nCols;
	G4double yCell = yWorld/nRows;
	G4double zCell = 0.01*m;

	// construction of the sensitive volume
	solidDetector = new G4Box("solidDetector", xCell, yCell, zCell);

	// the material of the detector it does not matter because we only need it to sense the optical photons
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	// we have to create arrays of sensitive detectors
	for(G4int i = 0; i < nRows; i++)
	{
		for(G4int j = 0; j < nCols; j++)
		{
			physDetector = new G4PVPlacement(0, G4ThreeVector(-xWorld + 2*xCell*(.5 + j), -yWorld + 2*yCell*(.5 + i), zWorld - zCell), logicDetector, "physDetector", logicWorld, false, j + i*nCols, false);
		}
	}
}

void MyDetectorConstruction::ConstructScintillator()
{
	solidScintillator = new G4Box("solidScintillator", 5*cm, 5*cm, 6*cm);

	logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicScintillator");

	// we make the world reflecting so that it keeps the photons trapped in the scintillator
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);

	solidDetector = new G4Box("solidDetector", 1*cm, 5*cm, 6*cm);

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	fScoringVolume = logicScintillator;

	for (G4int i=0; i<6; i++)
	{
		for (G4int j=0; j<16; j++)
		{
			G4Rotate3D rotZ(j*22.5*deg, G4ThreeVector(0,0,1));
			G4Translate3D transXScint(G4ThreeVector(5./tan(22.5/2*deg)*cm + 5.*cm, 0*cm, -40*cm +i*15*cm));
			G4Translate3D transXDet(G4ThreeVector(5./tan(22.5/2*deg)*cm + 6.*cm + 5.*cm, 0*cm, -40*cm +i*15*cm));

			G4Transform3D transformScint = (rotZ)*(transXScint);
			G4Transform3D transformDet = (rotZ)*(transXDet);

			physScintillator = new G4PVPlacement(transformScint, logicScintillator, "physScintillator", logicWorld, false, 16*i + j, true);
			physDetector = new G4PVPlacement(transformDet, logicDetector, "physDetector", logicWorld, false, 16*i + j, true);
		}
	}
}

void MyDetectorConstruction::ConstructTOF()
{
	solidDetector = new G4Box("solidDetector", 1.*m, 1.*m, 0.1*m);

	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

	physDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, -4.*m), logicDetector, "physDetector", logicWorld, false, 0, true);
	physDetector = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, 3.*m), logicDetector, "physDetector", logicWorld, false, 1, true);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

	// we want the world to be centered in the origin of our coordinate system
	// always check for overlaps in the last argument
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "PhysWorld", 0, false, 0, true);

	if(isCherenkov)
		ConstructCherenkov();

	if(isScintillator)
		ConstructScintillator();

	if(isTOF)
		ConstructTOF();

	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	if(logicDetector != NULL)
		logicDetector->SetSensitiveDetector(sensDet);
}
