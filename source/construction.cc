#include "construction.hh"

G4double HalfVoxelSize;
G4double HalfPhantomDepth;
G4double mouseCollimatorDistance;

MyDetectorConstruction::MyDetectorConstruction()
{
	G4cout << "MyDetectorConstruction::MyDetectorConstruction" << G4endl;
	fMessengerCollimator = new G4GenericMessenger(this, "/collimator/", "Collimator Construction");
	// first argument is the object to which it refers, this class
	// the slashes in the folder argument are mandatory
	// third argument is help text
	
	fMessengerCollimator->DeclareProperty("exist", collimatorExist, "true or false");

	// define our command
	// first argument is the name of the command
	// second argument is a variable
	// third argument is help text
	fMessengerCollimator->DeclarePropertyWithUnit("hole_length", "mm", hole_length, "Length of the collimator holes");
	fMessengerCollimator->DeclarePropertyWithUnit("hole_thickness", "mm", hole_thickness, "Thickness of the collimator holes");
	fMessengerCollimator->DeclarePropertyWithUnit("septa_thickness", "mm", septa_thickness, "Thickness of the collimator septa");
	fMessengerCollimator->DeclarePropertyWithUnit("case_side", "mm", case_side, "Side of the collimator case");
	fMessengerCollimator->DeclarePropertyWithUnit("positionX", "mm", collimator_posX, "X position of the collimator");
	fMessengerCollimator->DeclarePropertyWithUnit("positionY", "mm", collimator_posY, "X position of the collimator");

	// Collimator parameters
	collimatorExist = true; // by default the collimator is built
	hole_length = 30.*mm;
	septa_thickness = 1.*mm; // 2*mm;
	hole_thickness = 2.*mm; // 3*mm;
	case_side = 10.*cm; // fixed but not necessarily precise
	collimator_posX = 0.*mm;
	collimator_posY = 0.*mm;
	
	// scintillator commands
	fMessengerScintillator = new G4GenericMessenger(this, "/scintillator/", "Scintillator Construction");
	fMessengerScintillator->DeclarePropertyWithUnit("slab_side", "mm", slab_side, "Side of the scintillator");
	fMessengerScintillator->DeclarePropertyWithUnit("slab_depth", "mm", slab_depth, "Depth of the scintillator");
	fMessengerScintillator->DeclareProperty("pixel", scintiPixelNoSlab, "matrix or otherwise slab");
	fMessengerScintillator->DeclarePropertyWithUnit("scinti_pixel_size", "mm", scinti_pixel_size, "Size of the scintillator pixels");
	fMessengerScintillator->DeclareProperty("exist", scintillatorExist, "true or false");
	
	// scintillator parameters
	scintillatorExist = true; // by default the scintillator is built
	slab_side  = case_side;
	slab_depth = hole_length/3.;
	scintiPixelNoSlab = "matrix";
	scinti_pixel_size = hole_thickness + septa_thickness;
	scinti_septa_thickness = 10*um;
	scinti_hole_thickness = scinti_pixel_size - scinti_septa_thickness;
	fScoringScintillator = 0;

	// coupler commands
	fMessengerCoupler = new G4GenericMessenger(this, "/coupler/", "Optical coupling element");
	fMessengerCoupler->DeclarePropertyWithUnit("thickness", "mm", detector_scintillator_distance, "Optical coupling distance");

	// coupler parameters
	detector_scintillator_distance = 1*mm;
	
	// detector commands
	fMessengerDetector = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	fMessengerDetector->DeclarePropertyWithUnit("det_pixel_size", "mm", det_pixel_size, "Size of the detector pixels");
	fMessengerDetector->DeclarePropertyWithUnit("detector_side", "mm", detector_side, "Side of the SiPM");
	fMessengerDetector->DeclareProperty("pixel", detPixelNoSlab, "matrix or otherwise");
	fMessengerDetector->DeclarePropertyWithUnit("det_scinti_distance", "mm", detector_scintillator_distance, "Optical coupling distance");
	fMessengerDetector->DeclarePropertyWithUnit("channel_dead_space", "mm", channel_dead_space, "Dead space between channels");
	fMessengerDetector->DeclareProperty("fill_factor", det_fill_factor, "Ratio active over total pixel area");
	fMessengerDetector->DeclareProperty("threshold", energyThreshold, "Minimum number of photons to be detected");

	// detector parameters
	det_pixel_size = 3*mm;
	detPixelNoSlab = "matrix";
	detector_side = slab_side;
	detector_depth = 10*um;
	fScoringDetector = 0;
	det_fill_factor = .74;
	channel_dead_space = .2 * mm;
	energyThreshold = 1;
	
	// moby commands
	fMessengerMoby = new G4GenericMessenger(this, "/moby/", "MOBY parameters");
	fMessengerMoby->DeclarePropertyWithUnit("distance", "mm", mouseCollimatorDistance, "Distance MOBY - collimator surface");
	fMessengerMoby->DeclareProperty("voxelX", nVoxelX, "1 or more");
	fMessengerMoby->DeclareProperty("build", buildMoby, "yes (y) or no (n)");

	// moby parameters
	buildMoby = "yes";
	nVoxelX = 430;
	nVoxelY = 115;
	nVoxelZ = 115;
	mouseCollimatorDistance = 0.*mm;
	HalfVoxelSize = 0.18*mm/2.;
	HalfPhantomDepth = nVoxelZ*HalfVoxelSize;
	
	// define materials just once
	DefineMaterials();
	// Define MOBY materials
	DefineMaterialsMOBY();
	
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
	delete fMessengerScintillator;
	delete fMessengerDetector;
	delete fMessengerCoupler;
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
	materialPlastic = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
	materialSilicon = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

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

	// gagg
	G4double refractiveIndexGAGG[nEntries] = {1.9,1.9};
	G4double absorptionLengthGAGG[nEntries] = {645.*mm,645.*mm};

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
	
	// air
	G4double refractiveIndexAir[nEntries] = {1.000,1.000};
	G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", PhotonEnergy, refractiveIndexAir, nEntries);
	materialAir->SetMaterialPropertiesTable(mptAir);
	
	// plastic
	G4double refractiveIndexPlastic[nEntries] = {1.5,1.5};
	G4double absorptionLengthPlastic[nEntries] = {.1*mm,.1*mm}; // per polietilene
	G4MaterialPropertiesTable* mptPlastic = new G4MaterialPropertiesTable();
	mptPlastic->AddProperty("RINDEX", PhotonEnergy, refractiveIndexPlastic, nEntries);
	mptPlastic->AddProperty("ABSLENGTH", PhotonEnergy, absorptionLengthPlastic, nEntries);
	materialPlastic->SetMaterialPropertiesTable(mptPlastic);

	// silicon
	G4double refractiveIndexSilicon[nEntries] = {1.45,1.54};
	G4double absorptionLengthSilicon[nEntries] = {1e-8*m,1*m}; // per silicon dioxide
	G4MaterialPropertiesTable* mptSilicon = new G4MaterialPropertiesTable();
	mptSilicon->AddProperty("RINDEX", PhotonEnergy, refractiveIndexSilicon, nEntries);
	mptSilicon->AddProperty("ABSLENGTH", PhotonEnergy, absorptionLengthSilicon, nEntries);
	materialSilicon->SetMaterialPropertiesTable(mptSilicon);
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

void MyDetectorConstruction::DefineMaterialsMOBY()
{	
    G4NistManager *nistManager = G4NistManager::Instance();

    // Define Elements (for lung)
    G4double a;
    G4double z;
    G4double density;
    G4String name;
    G4String symbol;
    a = 1.01*g/mole;
    G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1., a);
    a = 12.01*g/mole;
    G4Element* elC  = new G4Element(name="Carbon", symbol="C", z=6., a);
    a = 14.01*g/mole;
    G4Element* elN  = new G4Element(name="Nitrogen", symbol="N", z=7., a);
    a = 16.00*g/mole;
    G4Element* elO  = new G4Element(name="Oxygen", symbol="O", z=8., a);
    a = 22.99*g/mole;
    G4Element* elNa = new G4Element(name="Sodium", symbol="Na", z=11., a);
    a = 30.97*g/mole;
    G4Element* elP = new G4Element(name="Phosphorus", symbol="P", z=15., a);
    a = 32.07*g/mole;
    G4Element* elS = new G4Element(name="Sulfur", symbol="S", z=16., a);
    a = 35.45*g/mole;
    G4Element* elCl = new G4Element(name="Chlorine", symbol="Cl", z=17., a);
    a = 39.10*g/mole;
    G4Element* elK = new G4Element(name="Potassium", symbol="K", z=19., a);
    a = 40.08*g/mole;

    // Define MOBY lung
    G4int ncomponents;
    G4double fractionmass;
    density = 0.3*g/cm3;
    fLungM = new G4Material(name="LUNG_MOBY", density, ncomponents=9);
    fLungM->AddElement(elH, fractionmass=0.103);
    fLungM->AddElement(elC, fractionmass=0.105);
    fLungM->AddElement(elN, fractionmass=0.031);
    fLungM->AddElement(elO, fractionmass=0.749);
    fLungM->AddElement(elNa, fractionmass=0.002);
    fLungM->AddElement(elP, fractionmass=0.002);
    fLungM->AddElement(elS, fractionmass=0.003);
    fLungM->AddElement(elCl, fractionmass=0.003);
    fLungM->AddElement(elK, fractionmass=0.002);

    // List of Materials
    //fLung = nistManager->FindOrBuildMaterial("G4_LUNG_ICRP");
    fSoft = nistManager->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
    fCBone = nistManager->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");

    // Define bone (compact + spongeous)
    density = 1.4*g/cm3;
    fBone = new G4Material(name="BONE", density, ncomponents=2);
    fBone->AddMaterial(fCBone, fractionmass=0.5);
    fBone->AddMaterial(fSoft, fractionmass=0.5);

    // Building Material "DataBase"
    theMaterials.push_back(materialAir);
    theMaterials.push_back(fLungM);
    theMaterials.push_back(fSoft);
    theMaterials.push_back(fBone);
}

void MyDetectorConstruction::ConstructMOBY()
{	
    // Defining the voxelPhantom parametrisation
    G4PhantomParameterisation* voxelizedPhantom = new G4PhantomParameterisation();

    voxelizedPhantom->SetVoxelDimensions(HalfVoxelSize, HalfVoxelSize, HalfVoxelSize);
    voxelizedPhantom->SetNoVoxel(nVoxelX, nVoxelY, nVoxelZ);
    voxelizedPhantom->SetMaterials(theMaterials);

    size_t* materialIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];

    // Importing CT
    // std::ifstream ctfile("../moby_20_atn.txt",std::ios::in);
    // std::ifstream ctfile("../moby_background_prova_atn.txt",std::ios::in);
    std::ifstream ctfile("../prova_ds_all_atn.txt",std::ios::in);

    if(ctfile.fail()) std::cout << "no file for MOBY ct\n";

    std::string buffer;
	
	G4int binxmin, binxmax, binymin, binymax, binzmin, binzmax;
	binxmin = 120;
	binxmax = 550;
	binymin = 40;
	binymax = 155;
	binzmin = 35;
	binzmax = 150;

	G4int i, j, k, n;
	G4double HU;

    while (ctfile >> buffer)
    {
        i = std::stoi(buffer);
        ctfile >> buffer;
        j = std::stoi(buffer);
        ctfile >> buffer;
        k = std::stoi(buffer);
        ctfile >> buffer;
        HU = std::stod(buffer);

	// turn the mouse in space
	if (i>=binxmin && i<binxmax && j>=binymin && j<binymax && k>=binzmin && k<binzmax)
	{
		i = i - binxmin;
		j = j - binymin;
		k = k - binzmin;
		// G4cout << i << "\t" << j << "\t" << k << G4endl;
	        n = i+nVoxelX*j+nVoxelX*nVoxelY*k;
		
		if (HU < 0) G4cout << "errore!!!" << G4endl;
		else
		{
	        	if (HU == 0) materialIDs[n] = 0; // air
			else 
			{
				if (HU < 30) materialIDs[n] = 1; // lung
				else 
				{
					if (HU < 85) materialIDs[n] = 2; // soft tissue
					else
					{
						if (HU >= 85) materialIDs[n] = 3; // bone
					}
				}
			}
		}
	}
 }

    voxelizedPhantom->SetMaterialIndices(materialIDs);

    G4ThreeVector moby_position = G4ThreeVector(0, 0, - HalfPhantomDepth - mouseCollimatorDistance);
    G4Box* cont_solid = new G4Box("PhantomContainer", nVoxelX*HalfVoxelSize, nVoxelY*HalfVoxelSize, HalfPhantomDepth);
    G4LogicalVolume* cont_logic = new G4LogicalVolume( cont_solid, materialAir, "PhantomContainer", 0, 0, 0 );
    G4VPhysicalVolume * cont_phys = new G4PVPlacement(0, moby_position, cont_logic, "PhantomContainer", logicWorld, false, true);

    voxelizedPhantom->BuildContainerSolid(cont_phys);

    // Assure that the voxels are completely filling the container volume
    voxelizedPhantom->CheckVoxelsFillContainer(cont_solid->GetXHalfLength(), cont_solid->GetYHalfLength(), cont_solid->GetZHalfLength() );
    voxelizedPhantom->SetSkipEqualMaterials(false);

    // The parameterised volume which uses this parameterisation is placed in the container logical volume
    G4VSolid* solVoxel = new G4Box("phantom", HalfVoxelSize, HalfVoxelSize, HalfVoxelSize);
    G4LogicalVolume* logicVoxel = new G4LogicalVolume(solVoxel, materialAir, "phantom", 0, 0, 0);
    G4PVParameterised * patient_phys = new G4PVParameterised("Patient", logicVoxel, cont_logic, kUndefined, nVoxelX*nVoxelY*nVoxelZ, voxelizedPhantom);

    G4VisAttributes *vis = new G4VisAttributes();
    vis->SetVisibility(false);
    logicVoxel->SetVisAttributes(vis);
    //cont_logic->SetVisAttributes(vis);

    // Indicate that this physical volume is having a regular structure
    patient_phys->SetRegularStructureId(0);
}


void MyDetectorConstruction::ConstructCollimator()
{
	G4cout << "MyDetectorConstruction::ConstructCollimator" << G4endl;
	
	// always restart the case side
	case_side = 10.*cm;
	
	// Derived parameters
	pixel_size = hole_thickness + septa_thickness;
	holes_number = (G4int) case_side / pixel_size;
	
	// check proper parameters
	if (holes_number < 1)
	{
		G4cout << "Error: pixel larger than case!" << G4endl;
		G4cout << "return to default values" << G4endl;
		septa_thickness = 1.*mm;
		hole_thickness = 2.*mm;
		pixel_size = hole_thickness + septa_thickness;
		holes_number = (G4int) case_side / pixel_size;
	}
	if ((holes_number % 2) < 1)
		holes_number = holes_number - 1;
	
	// derived parameters
	case_side = (G4double) pixel_size * holes_number;
	G4cout << "pixel_size: " << pixel_size << " mm" << G4endl;
	G4cout << "holes_number: " << holes_number << " " << G4endl;
	G4cout << "case_side: " << case_side << " mm" << G4endl;

	// position
	G4ThreeVector collimator_position = G4ThreeVector(collimator_posX, collimator_posY, +hole_length/2.);
	
	// case
	G4cout << "defining the collimator case" << G4endl;
	solidCollimatorMatrix = new G4Box("solidCollimatorMatrix", case_side/2., case_side/2., hole_length/2.);
	logicCollimatorMatrix = new G4LogicalVolume(solidCollimatorMatrix, materialTungsten, "logicCollimatorMatrix");
	new G4PVPlacement(0, collimator_position, logicCollimatorMatrix, "physCollimatorMatrix", logicWorld, false, 0, true);
	
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

void MyDetectorConstruction::ConstructSlabScintillator()
{
	solidScintillator = new G4Box("solidScintillator", slab_side/2., slab_side/2., slab_depth/2.);
	logicScintillator = new G4LogicalVolume(solidScintillator, materialGAGG, "logicScintillator");

	physScintillator = new G4PVPlacement(0,  // no rotation
		G4ThreeVector(0.,0.,hole_length + slab_depth/2.),
		logicScintillator,             // its logical volume
		"physScintillator",           // its name
		logicWorld,                  // its mother volume
		false,                   // no boolean operations
		0,                       // copy number
		1); // checking overlaps

	fScoringScintillator = logicScintillator;
	physScoringScintillator = physScintillator;
}

void MyDetectorConstruction::ConstructPixelScintillator()
{
	G4cout << "MyDetectorConstruction::ConstructPixelScintillator" << G4endl;
	
	// Derived parameters
	scinti_hole_thickness = scinti_pixel_size - scinti_septa_thickness;
	scinti_hole_length = slab_depth;
	scinti_holes_number = (G4int) slab_side / scinti_pixel_size;
	
	// check proper parameters
	if (scinti_holes_number < 1)
	{
		G4cout << "Error: pixel larger than case!" << G4endl;
		G4cout << "return to default values" << G4endl;
		scinti_septa_thickness = 10.*um;
		scinti_hole_thickness = 2.*mm - scinti_septa_thickness;
		scinti_pixel_size = scinti_hole_thickness + scinti_septa_thickness;
		scinti_holes_number = (G4int) slab_side / scinti_pixel_size;
	}
	// if ((scinti_holes_number % 2) < 1)
	// 	scinti_holes_number = scinti_holes_number - 1;
	
	// derived parameters
	slab_side = (G4double) scinti_pixel_size * scinti_holes_number;
	G4cout << "scinti_pixel_size: " << scinti_pixel_size << " mm" << G4endl;
	G4cout << "scinti_holes_number: " << scinti_holes_number << " " << G4endl;
	G4cout << "scinti_slab_side: " << slab_side << " mm" << G4endl;
	
	// case
	G4cout << "defining the scintillator case" << G4endl;
	solidScintillatorMatrix = new G4Box("solidScintillatorMatrix", slab_side/2., slab_side/2., scinti_hole_length/2.);
	logicScintillatorMatrix = new G4LogicalVolume(solidScintillatorMatrix, materialPlastic, "logicScintillatorMatrix");
	physScintillator = new G4PVPlacement(0, G4ThreeVector(0.,0.,hole_length + slab_depth/2.), logicScintillatorMatrix, "physScintillatorMatrix", logicWorld, false, 0, true);
	
	// array
	G4cout << "defining the scintillator array element" << G4endl;
	solidScintillatorArray = new G4Box("solidScintillatorArray", slab_side/2., scinti_pixel_size/2., scinti_hole_length/2.);
	logicScintillatorArray = new G4LogicalVolume(solidScintillatorArray, materialPlastic, "logicScintillatorArray");
	new G4PVReplica("physScintillatorArray", logicScintillatorArray, logicScintillatorMatrix, kYAxis, scinti_holes_number, scinti_pixel_size, 0);
	
	// pixel
	G4cout << "defining the scintillator pixel element" << G4endl;
	solidScintillatorPixel = new G4Box("solidScintillatorPixel", scinti_pixel_size/2., scinti_pixel_size/2., scinti_hole_length/2.);
	logicScintillatorPixel = new G4LogicalVolume(solidScintillatorPixel, materialPlastic, "logicScintillatorPixel");
	physScintillatorPixel = new G4PVReplica("physScintillatorPixel", logicScintillatorPixel, logicScintillatorArray, kXAxis, scinti_holes_number, scinti_pixel_size, 0);
	
	// pinhole
	solidScintillatorPinhole = new G4Box("solidScintillatorPinhole", scinti_hole_thickness/2., scinti_hole_thickness/2., scinti_hole_length/2.);
	logicScintillatorPinhole = new G4LogicalVolume(solidScintillatorPinhole, materialGAGG, "logicScintillatorPinhole");
	physScintillatorPinhole = new G4PVPlacement(0, G4ThreeVector(), logicScintillatorPinhole, "physScintillatorPinhole", logicScintillatorPixel, false, 0, true);

	fScoringScintillator = logicScintillatorPinhole;
	physScoringScintillator = physScintillatorPinhole;
}

void MyDetectorConstruction::ConstructCoupler()
{
	G4cout << "Optical coupling distance: " << detector_scintillator_distance / mm << " mm" << G4endl;
	
	solidCoupler = new G4Box("solidScintillator", slab_side/2., slab_side/2., detector_scintillator_distance/2.);
	logicCoupler = new G4LogicalVolume(solidCoupler, materialAir, "logicCoupler");
	physCoupler = new G4PVPlacement(0,  // no rotation
		G4ThreeVector(0.,0.,hole_length + slab_depth + detector_scintillator_distance/2.),
		logicCoupler,             // its logical volume
		"physCoupler",           // its name
		logicWorld,                  // its mother volume
		false,                   // no boolean operations
		0,                       // copy number
		1); // checking overlaps
}

void MyDetectorConstruction::ConstructSlabDetector()
{
	G4cout << "MyDetectorConstruction::ConstructDetector" << G4endl;
	
	solidDetector = new G4Box("solidDetector", detector_side/2., detector_side/2., detector_depth/2.);
	logicDetector = new G4LogicalVolume(solidDetector, materialPlastic, "logicDetector");

	physDetector = new G4PVPlacement(0,  // no rotation
		detector_centre_position,
		logicDetector,             // its logical volume
		"physDetector",           // its name
		logicWorld,                  // its mother volume
		false,                   // no boolean operations
		0,                       // copy number
		1); // checking overlaps

	fScoringDetector = logicDetector;
}

void MyDetectorConstruction::ConstructPixelDetector()
{
	G4cout << "MyDetectorConstruction::ConstructPixelDetector" << G4endl;
	
	// Derived parameters
	det_pixels_number = (G4int) detector_side / det_pixel_size;
	
	// check proper parameters
	if (det_pixels_number < 1)
	{
		G4cout << "Error: pixel larger than case!" << G4endl;
		G4cout << "return to default values" << G4endl;;
		det_pixel_size = 3*mm;
		det_pixels_number = (G4int) detector_side / det_pixel_size;
	}
	// if ((det_pixels_number % 2) < 1)
	// 	det_pixels_number = det_pixels_number - 1;
	if (det_pixel_size < channel_dead_space)
	{
		G4cout << "Error: dead space larger than sensitive space!" << G4endl;
		G4cout << "return to default values" << G4endl;;
		det_pixel_size = 3*mm;
		det_pixels_number = (G4int) detector_side / det_pixel_size;
	}	
	
	// derived parameters
	detector_side = (G4double) det_pixel_size * det_pixels_number;
	G4double det_pixel_active_size = det_pixel_size - channel_dead_space;
	G4cout << "det_pixel_size: " << det_pixel_size / mm << " mm" << G4endl;
	G4cout << "channel_dead_space: " << channel_dead_space / mm << " mm" << G4endl;
	G4cout << "det_fill_factor: " << det_fill_factor * 100. << " %" << G4endl;
	G4cout << "det_pixel_size: " << det_pixel_size / mm << " mm" << G4endl;
	G4cout << "det_pixel_active_size: " << det_pixel_active_size / mm << " mm" << G4endl;
	G4cout << "det_pixels_number: " << det_pixels_number << " " << G4endl;
	G4cout << "detector_side: " << detector_side / mm << " mm" << G4endl;
	
	// case
	G4cout << "defining the detector case" << G4endl;
	solidDetectorMatrix = new G4Box("solidDetectorMatrix", detector_side/2., detector_side/2., detector_depth/2.);
	logicDetectorMatrix = new G4LogicalVolume(solidDetectorMatrix, materialPlastic, "logicDetectorMatrix");
	physDetector = new G4PVPlacement(0, detector_centre_position, logicDetectorMatrix, "physDetectorMatrix", logicWorld, false, 0, true);
	
	// array
	G4cout << "defining the detector array element" << G4endl;
	solidDetectorArray = new G4Box("solidDetectorArray", detector_side/2., det_pixel_size/2., detector_depth/2.);
	logicDetectorArray = new G4LogicalVolume(solidDetectorArray, materialPlastic, "logicDetectorArray");
	new G4PVReplica("physDetectorArray", logicDetectorArray, logicDetectorMatrix, kYAxis, det_pixels_number, det_pixel_size, 0);

	if (channel_dead_space > 0)
	{
		// dead boundary of a pixel
		G4cout << "defining the detector pixel element, both active and not-active" << G4endl;
		solidDetectorFullPixel = new G4Box("solidDetectorFullPixel", det_pixel_size/2., det_pixel_size/2., detector_depth/2.);
		logicDetectorFullPixel = new G4LogicalVolume(solidDetectorFullPixel, materialPlastic, "logicDetectorFullPixel");
		physDetectorFullPixel = new G4PVReplica("physDetectorFullPixel", logicDetectorFullPixel, logicDetectorArray, kXAxis, det_pixels_number, det_pixel_size, 0);
		
		// active pixel
		G4cout << "defining the detector pixel active element" << G4endl;
		solidDetectorPixel = new G4Box("solidDetectorPixel", det_pixel_active_size/2., det_pixel_active_size/2., detector_depth/2.);
		logicDetectorPixel = new G4LogicalVolume(solidDetectorPixel, materialSilicon, "logicDetectorPixel");
		physDetectorPixel = new G4PVPlacement(0, G4ThreeVector(), logicDetectorPixel, "physDetectorActivePixel", logicDetectorFullPixel, false, 0, true);
	}
	else
	{
		// pixel
		G4cout << "defining the detector pixel element" << G4endl;
		solidDetectorPixel = new G4Box("solidDetectorPixel", det_pixel_size/2., det_pixel_size/2., detector_depth/2.);
		logicDetectorPixel = new G4LogicalVolume(solidDetectorPixel, materialSilicon, "logicDetectorPixel");
		physDetectorPixel = new G4PVReplica("physDetectorPixel", logicDetectorPixel, logicDetectorArray, kXAxis, det_pixels_number, det_pixel_size, 0);
	}
	
	fScoringDetector = logicDetectorPixel;
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	G4cout << "MyDetectorConstruction::Construct" << G4endl;
	G4double world_half_Z  = 10*cm;
	G4double world_half_XY = 10*cm;

	solidWorld = new G4Box("solidWorld", world_half_XY, world_half_XY, world_half_Z);
	logicWorld = new G4LogicalVolume(solidWorld, materialAir, "logicWorld", 0, 0, 0, true);

	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, true);

	G4cout << "Do I construct the collimator? " << collimatorExist << G4endl;
	if(collimatorExist)
		ConstructCollimator();

	if(scintillatorExist)
	{
		if (scintiPixelNoSlab == "matrix")
			ConstructPixelScintillator();
		else
			ConstructSlabScintillator();
	}

	if(scintillatorExist && detector_scintillator_distance > 0)
		ConstructCoupler();

	detector_centre_position = G4ThreeVector(0.,0.,hole_length + slab_depth + detector_scintillator_distance + detector_depth/2.);
	if (detPixelNoSlab == "matrix")
		ConstructPixelDetector();
	else
		ConstructSlabDetector();

	if ((buildMoby == "yes") || (buildMoby == "y"))
		ConstructMOBY();
	else
		G4cout << "not building MOBY" << G4endl;

	DefineOpticalSurfaceProperties();
	// SetVisualizationFeatures();

	return physWorld;
}


void MyDetectorConstruction::DefineOpticalSurfaceProperties()
{
	// scintillator pixel surfaces
	G4cout << "MyDetectorConstruction::DefineOpticalSurfaceProperties" << G4endl;
	// reflectivity 0 and transmission 0 means total absorption
	// reflectivity 0 and transmission 1 means total transmission
	// reflectivity 1 and transmission 0 (or whatever) means Fresnel
	
	// define the material properties table for a fully transmitting surface
	std::vector<G4double> ephoton = {1.0*eV, 7.0*eV};
	std::vector<G4double> reflectivity = { 0., 0. };
	std::vector<G4double> transmittance = { 1., 1. };
	G4MaterialPropertiesTable* MPTtransmitting = new G4MaterialPropertiesTable();
	MPTtransmitting->AddProperty("REFLECTIVITY", ephoton, reflectivity);
	MPTtransmitting->AddProperty("TRANSMITTANCE", ephoton, transmittance);
	
	// define he material properties table for a fully absorbing surface
	std::vector<G4double> reflectivity2 = { 0., 0. };
	std::vector<G4double> transmittance2 = { 0., 0. };
	G4MaterialPropertiesTable* MPTabsorbing = new G4MaterialPropertiesTable();
	MPTabsorbing->AddProperty("REFLECTIVITY", ephoton, reflectivity2);
	MPTabsorbing->AddProperty("TRANSMITTANCE", ephoton, transmittance2);
	
	// define he material properties table for a fully Fresnel surface
	std::vector<G4double> reflectivity3 = { 1., 1. };
	std::vector<G4double> transmittance3 = { 0., 0. };
	G4MaterialPropertiesTable* MPTfresnel = new G4MaterialPropertiesTable();
	MPTfresnel->AddProperty("REFLECTIVITY", ephoton, reflectivity3);
	MPTfresnel->AddProperty("TRANSMITTANCE", ephoton, transmittance3);

	// build reflective skin surface
	G4OpticalSurface* opGaggPlasticSurface = new G4OpticalSurface("opGaggPlasticSurface");
	opGaggPlasticSurface->SetModel(unified);
	opGaggPlasticSurface->SetType(dielectric_metal);
	opGaggPlasticSurface->SetFinish(polished);
	opGaggPlasticSurface->SetMaterialPropertiesTable(MPTfresnel);
	
	// build fully transmitting surface
	G4OpticalSurface* opGaggDetectorSurface = new G4OpticalSurface("opGaggDetectorSurface");
	opGaggDetectorSurface->SetMaterialPropertiesTable(MPTtransmitting);


	if(scintillatorExist)
	{
		// build reflective skin surface around the scintillator pixel hole
		new G4LogicalSkinSurface("skin",fScoringScintillator, opGaggPlasticSurface);

		if (detector_scintillator_distance > 0)
		{
			// fully transmit optical photons escaping toward the detector
			new G4LogicalBorderSurface("logicBorderGaggCouplerSurface", 
						   physScoringScintillator, physCoupler, opGaggDetectorSurface);
			
			// fully transmit optical photons escaping toward the detector
			new G4LogicalBorderSurface("logicBorderCouplerDetectorSurface", 
				  physCoupler, physDetector, opGaggDetectorSurface);
		}
		else
		{
			// fully transmit optical photons escaping toward the detector
			new G4LogicalBorderSurface("logicBorderGaggDetectorSurface", 
						   physScoringScintillator, physDetector, opGaggDetectorSurface);
			G4cout << "Optical coupling distance is zero!!!" << G4endl;
		}
	}
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

	if(fScoringDetector != NULL)
	{
		MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector","SensitiveDetectorHitsCollection");
		G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
		fScoringDetector->SetSensitiveDetector(sensDet);
		sensDet->SetDetectionThreshold(energyThreshold);
		sensDet->SetFillFactor(det_fill_factor);
	}

	if(fScoringScintillator != NULL)
	{
		MySensitiveScintillator *sensScinti = new MySensitiveScintillator("SensitiveScintillator");
		G4SDManager::GetSDMpointer()->AddNewDetector(sensScinti);
		fScoringScintillator->SetSensitiveDetector(sensScinti);
	}
}
