#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
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
	fMessengerScintillator->DeclarePropertyWithUnit("slab_side", "mm", slab_side, "Side of the collimator");
	fMessengerScintillator->DeclarePropertyWithUnit("slab_depth", "mm", slab_depth, "Depth of the collimator");
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
	fMessengerDetector->DeclareProperty("pixel", detPixelNoSlab, "matrix or otherwise");
	fMessengerDetector->DeclarePropertyWithUnit("det_scinti_distance", "mm", detector_scintillator_distance, "Optical coupling distance");
	fMessengerDetector->DeclareProperty("fill_factor", det_fill_factor, "Ratio active over total pixel area");

	// detector parameters
	det_pixel_size = 3*mm;
	detPixelNoSlab = "matrix";
	detector_side = slab_side;
	detector_depth = 10*um;
	fScoringDetector = 0;
	det_fill_factor = .8;

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
    // Lambda function to set RINDEX and ABSLENGTH in a MPT and assign it to a material
    auto SetMaterialProperties = [](G4MaterialPropertiesTable* mpt, G4Material* material, bool isSoft = true) {

        /*std::vector<G4double> energies = {1.2*eV, 1.91*eV, 2.25*eV, 2.76*eV, 6.2*eV};
        std::vector<G4double> absorptionSoft = {0.2*cm, 0.2*cm, 0.04*cm, 0.04*cm, 0.03*cm}; // from Optical properties of biological tissues: a review
        std::vector<G4double> absorptionBone = {0.16*cm, 0.16*cm, 0.077*cm, 0.05*cm, 0.04*cm}; // https://hal.science/hal-02335596/document*/

        // dati sono risistemati altrimenti se metto un'altra energia nel mezzo salta tutto...
        // dati a 1.2eV = 1000 nm e 6.2eV = 200 nm sono inventati
        std::vector<G4double> energies = {1.2*eV, 1.77*eV, 2.25*eV, 2.76*eV, 6.2*eV};
        std::vector<G4double> absorptionSoft = {0.5*cm, 0.33*cm, 0.1*cm, 0.04*cm, 0.001*cm}; // from Experimental and analytical comparative study of optical coefficient of fresh and frozen rat tissues
        std::vector<G4double> absorptionBone = {0.2*cm, 0.16*cm, 0.09*cm, 0.05*cm, 0.001*cm}; // from Optical properties of mice skull bone in the 455- to 705-nm range

        
        G4double rIndex = 0.3323 + 0.3422*(material->GetDensity()/(g/cm3)-1) +1;
        mpt->AddProperty("RINDEX", energies, std::vector<G4double>(energies.size(), rIndex), true, true);
        if(isSoft){
            mpt->AddProperty("ABSLENGTH", energies, absorptionSoft, true, true);
            //mpt->AddProperty("RAYLEIGH", energies, std::vector<G4double>({0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm}), true, true);
            //mpt->AddProperty("RAYLEIGH", std::vector<G4double>({1.2*eV, 2.25*eV, 6.2*eV}), std::vector<G4double>({0.14*mm, 0.69*mm, 2.2*mm}), true, true); // from Experimental and analytical comparative study of optical coefficient of fresh and frozen rat tissues
        }
        else{
            mpt->AddProperty("ABSLENGTH", energies, absorptionBone, true, true);
            //mpt->AddProperty("RAYLEIGH", energies, std::vector<G4double>({0.05*cm, 0.05*cm, 0.05*cm, 0.05*cm, 0.05*cm}), true, true);
            //mpt->AddProperty("RAYLEIGH", std::vector<G4double>({1.2*eV, 2.25*eV, 6.2*eV}), std::vector<G4double>({0.33*mm, 0.35*mm, 0.4*mm}), true, true); // from Optical properties of mice skull bone in the 455- to 705-nm range
        }

        material->SetMaterialPropertiesTable(mpt);
    };

    G4NistManager *nistManager = G4NistManager::Instance();

    // Setting up default voxels
    fVoxelMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

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
    G4Element* elCa = new G4Element(name="Calcium", symbol="Ca", z=20., a);
    a = 55.85*g/mole;
    G4Element* elFe = new G4Element(name="Iron", symbol="Fe", z=26., a);
    a = 126.90*g/mole;
    G4Element* elI = new G4Element(name="Iodium", symbol="I", z=53., a);

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
    G4MaterialPropertiesTable* mptLungM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptLungM, fLungM);

    // Define MOBY stomach
    density = 1.04*g/cm3;
    fStomachM = new G4Material(name="STOMACH_MOBY", density, ncomponents=9);
    fStomachM->AddElement(elH, fractionmass=0.106);
    fStomachM->AddElement(elC, fractionmass=0.115);
    fStomachM->AddElement(elN, fractionmass=0.022);
    fStomachM->AddElement(elO, fractionmass=0.751);
    fStomachM->AddElement(elNa, fractionmass=0.001);
    fStomachM->AddElement(elP, fractionmass=0.001);
    fStomachM->AddElement(elS, fractionmass=0.001);
    fStomachM->AddElement(elCl, fractionmass=0.002);
    fStomachM->AddElement(elK, fractionmass=0.001);
    G4MaterialPropertiesTable* mptStomachM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptStomachM, fStomachM);

    // Define MOBY smintestine
    density = 1.04*g/cm3;
    fSmIntestineM = new G4Material(name="SMINTESTINE_MOBY", density, ncomponents=9);
    fSmIntestineM->AddElement(elH, fractionmass=0.106);
    fSmIntestineM->AddElement(elC, fractionmass=0.115);
    fSmIntestineM->AddElement(elN, fractionmass=0.022);
    fSmIntestineM->AddElement(elO, fractionmass=0.751);
    fSmIntestineM->AddElement(elNa, fractionmass=0.001);
    fSmIntestineM->AddElement(elP, fractionmass=0.001);
    fSmIntestineM->AddElement(elS, fractionmass=0.001);
    fSmIntestineM->AddElement(elCl, fractionmass=0.002);
    fSmIntestineM->AddElement(elK, fractionmass=0.001);
    G4MaterialPropertiesTable* mptSmIntestineM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptSmIntestineM, fSmIntestineM);

    // Define MOBY lgintestine
    density = 1.04*g/cm3;
    fLgIntestineM = new G4Material(name="LGINTESTINE_MOBY", density, ncomponents=9);
    fLgIntestineM->AddElement(elH, fractionmass=0.106);
    fLgIntestineM->AddElement(elC, fractionmass=0.115);
    fLgIntestineM->AddElement(elN, fractionmass=0.022);
    fLgIntestineM->AddElement(elO, fractionmass=0.751);
    fLgIntestineM->AddElement(elNa, fractionmass=0.001);
    fLgIntestineM->AddElement(elP, fractionmass=0.001);
    fLgIntestineM->AddElement(elS, fractionmass=0.001);
    fLgIntestineM->AddElement(elCl, fractionmass=0.002);
    fLgIntestineM->AddElement(elK, fractionmass=0.001);
    G4MaterialPropertiesTable* mptLgIntestineM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptLgIntestineM, fLgIntestineM);
    

    // Define MOBY liver
    density = 1.05*g/cm3;
    fLiverM = new G4Material(name="LIVER_MOBY", density, ncomponents=9);
    fLiverM->AddElement(elH, fractionmass=0.103);
    fLiverM->AddElement(elC, fractionmass=0.186);
    fLiverM->AddElement(elN, fractionmass=0.028);
    fLiverM->AddElement(elO, fractionmass=0.671);
    fLiverM->AddElement(elNa, fractionmass=0.002);
    fLiverM->AddElement(elP, fractionmass=0.002);
    fLiverM->AddElement(elS, fractionmass=0.003);
    fLiverM->AddElement(elCl, fractionmass=0.002);
    fLiverM->AddElement(elK, fractionmass=0.003);
    G4MaterialPropertiesTable* mptLiverM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptLiverM, fLiverM);

    // Define MOBY pancreas
    density = 1.05*g/cm3;
    fPancreasM = new G4Material(name="PANCREAS_MOBY", density, ncomponents=9);
    fPancreasM->AddElement(elH, fractionmass=0.106);
    fPancreasM->AddElement(elC, fractionmass=0.169);
    fPancreasM->AddElement(elN, fractionmass=0.022);
    fPancreasM->AddElement(elO, fractionmass=0.694);
    fPancreasM->AddElement(elNa, fractionmass=0.002);
    fPancreasM->AddElement(elP, fractionmass=0.002);
    fPancreasM->AddElement(elS, fractionmass=0.001);
    fPancreasM->AddElement(elCl, fractionmass=0.002);
    fPancreasM->AddElement(elK, fractionmass=0.002);
    G4MaterialPropertiesTable* mptPancreasM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptPancreasM, fPancreasM);

    // Define MOBY bladder
    density = 1.04*g/cm3;
    fBladderM = new G4Material(name="BLADDER_MOBY", density, ncomponents=9);
    fBladderM->AddElement(elH, fractionmass=0.105);
    fBladderM->AddElement(elC, fractionmass=0.096);
    fBladderM->AddElement(elN, fractionmass=0.026);
    fBladderM->AddElement(elO, fractionmass=0.761);
    fBladderM->AddElement(elNa, fractionmass=0.002);
    fBladderM->AddElement(elP, fractionmass=0.002);
    fBladderM->AddElement(elS, fractionmass=0.002);
    fBladderM->AddElement(elCl, fractionmass=0.003);
    fBladderM->AddElement(elK, fractionmass=0.003);
    G4MaterialPropertiesTable* mptBladderM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBladderM, fBladderM);

    // Define MOBY spleen
    density = 1.06*g/cm3;
    fSpleenM = new G4Material(name="SPLEEN_MOBY", density, ncomponents=9);
    fSpleenM->AddElement(elH, fractionmass=0.103);
    fSpleenM->AddElement(elC, fractionmass=0.113);
    fSpleenM->AddElement(elN, fractionmass=0.032);
    fSpleenM->AddElement(elO, fractionmass=0.741);
    fSpleenM->AddElement(elNa, fractionmass=0.001);
    fSpleenM->AddElement(elP, fractionmass=0.003);
    fSpleenM->AddElement(elS, fractionmass=0.002);
    fSpleenM->AddElement(elCl, fractionmass=0.002);
    fSpleenM->AddElement(elK, fractionmass=0.003);
    G4MaterialPropertiesTable* mptSpleenM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptSpleenM, fSpleenM);

    // Define MOBY blood
    density = 1.06*g/cm3;
    fBloodM = new G4Material(name="BLOOD_MOBY", density, ncomponents=10);
    fBloodM->AddElement(elH, fractionmass=0.102);
    fBloodM->AddElement(elC, fractionmass=0.110);
    fBloodM->AddElement(elN, fractionmass=0.033);
    fBloodM->AddElement(elO, fractionmass=0.745);
    fBloodM->AddElement(elNa, fractionmass=0.001);
    fBloodM->AddElement(elP, fractionmass=0.001);
    fBloodM->AddElement(elS, fractionmass=0.002);
    fBloodM->AddElement(elCl, fractionmass=0.003);
    fBloodM->AddElement(elK, fractionmass=0.002);
    fBloodM->AddElement(elFe, fractionmass=0.001);
    G4MaterialPropertiesTable* mptBloodM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBloodM, fBloodM);

    // Define MOBY brain
    density = 1.06*g/cm3;
    fBrainM = new G4Material(name="BRAIN_MOBY", density, ncomponents=9);
    fBrainM->AddElement(elH, fractionmass=0.107);
    fBrainM->AddElement(elC, fractionmass=0.145);
    fBrainM->AddElement(elN, fractionmass=0.022);
    fBrainM->AddElement(elO, fractionmass=0.712);
    fBrainM->AddElement(elNa, fractionmass=0.002);
    fBrainM->AddElement(elP, fractionmass=0.004);
    fBrainM->AddElement(elS, fractionmass=0.002);
    fBrainM->AddElement(elCl, fractionmass=0.003);
    fBrainM->AddElement(elK, fractionmass=0.003);
    G4MaterialPropertiesTable* mptBrainM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBrainM, fBrainM);

    // Define MOBY heart
    density = 1.06*g/cm3;
    fHeartM = new G4Material(name="HEART_MOBY", density, ncomponents=9);
    fHeartM->AddElement(elH, fractionmass=0.104);
    fHeartM->AddElement(elC, fractionmass=0.139);
    fHeartM->AddElement(elN, fractionmass=0.029);
    fHeartM->AddElement(elO, fractionmass=0.718);
    fHeartM->AddElement(elNa, fractionmass=0.001);
    fHeartM->AddElement(elP, fractionmass=0.002);
    fHeartM->AddElement(elS, fractionmass=0.002);
    fHeartM->AddElement(elCl, fractionmass=0.002);
    fHeartM->AddElement(elK, fractionmass=0.003);
    G4MaterialPropertiesTable* mptHeartM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptHeartM, fHeartM);

    // Define MOBY thyroid
    density = 1.05*g/cm3;
    fThyroidM = new G4Material(name="THYROID_MOBY", density, ncomponents=10);
    fThyroidM->AddElement(elH, fractionmass=0.104);
    fThyroidM->AddElement(elC, fractionmass=0.119);
    fThyroidM->AddElement(elN, fractionmass=0.024);
    fThyroidM->AddElement(elO, fractionmass=0.745);
    fThyroidM->AddElement(elNa, fractionmass=0.002);
    fThyroidM->AddElement(elP, fractionmass=0.001);
    fThyroidM->AddElement(elS, fractionmass=0.001);
    fThyroidM->AddElement(elCl, fractionmass=0.002);
    fThyroidM->AddElement(elK, fractionmass=0.001);
    fThyroidM->AddElement(elI, fractionmass=0.001);
    G4MaterialPropertiesTable* mptThyroidM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptThyroidM, fThyroidM);

    // Define MOBY skin
    density = 1.1*g/cm3;
    fSkinM = new G4Material(name="SKIN_MOBY", density, ncomponents=9);
    fSkinM->AddElement(elH, fractionmass=0.100);
    fSkinM->AddElement(elC, fractionmass=0.204);
    fSkinM->AddElement(elN, fractionmass=0.042);
    fSkinM->AddElement(elO, fractionmass=0.645);
    fSkinM->AddElement(elNa, fractionmass=0.002);
    fSkinM->AddElement(elP, fractionmass=0.001);
    fSkinM->AddElement(elS, fractionmass=0.002);
    fSkinM->AddElement(elCl, fractionmass=0.003);
    fSkinM->AddElement(elK, fractionmass=0.001);
    G4MaterialPropertiesTable* mptSkinM = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptSkinM, fSkinM);

    // List of Materials
    //fLung = nistManager->FindOrBuildMaterial("G4_LUNG_ICRP");
    fSoft = nistManager->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
    G4MaterialPropertiesTable* mptSoft = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptSoft, fSoft);
    fCBone = nistManager->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
    G4MaterialPropertiesTable* mptCBone = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptCBone, fCBone, false);
    fAdipose = nistManager->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
    G4MaterialPropertiesTable* mptAdipose = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptAdipose, fAdipose);
    fBrain = nistManager->FindOrBuildMaterial("G4_BRAIN_ICRP");
    G4MaterialPropertiesTable* mptBrain = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBrain, fBrain);
    fBlood = nistManager->FindOrBuildMaterial("G4_BLOOD_ICRP");
    G4MaterialPropertiesTable* mptBlood = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBlood, fBlood);
    fMuscle = nistManager->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");
    G4MaterialPropertiesTable* mptMuscle = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptMuscle, fMuscle);

    // Define bone (compact + spongeous)
    density = 1.4*g/cm3;
    fBone = new G4Material(name="BONE", density, ncomponents=2);
    fBone->AddMaterial(fCBone, fractionmass=0.5);
    fBone->AddMaterial(fSoft, fractionmass=0.5);
    G4MaterialPropertiesTable* mptBone = new G4MaterialPropertiesTable();
    SetMaterialProperties(mptBone, fBone, false);

    // Building Material "DataBase"
    theMaterials.push_back(fMaterialAir);
    theMaterials.push_back(fLungM);
    theMaterials.push_back(fSoft);
    theMaterials.push_back(fBone);
    theMaterials.push_back(fAdipose);
    theMaterials.push_back(fBrain);
    theMaterials.push_back(fBlood);
    theMaterials.push_back(fMuscle);
    theMaterials.push_back(fStomachM);
    theMaterials.push_back(fSmIntestineM);
    theMaterials.push_back(fLgIntestineM);
    theMaterials.push_back(fLiverM);
    theMaterials.push_back(fPancreasM);
    theMaterials.push_back(fBladderM);
    theMaterials.push_back(fSpleenM);
    theMaterials.push_back(fBloodM);
    theMaterials.push_back(fBrainM);
    theMaterials.push_back(fHeartM);
    theMaterials.push_back(fThyroidM);
    theMaterials.push_back(fSkinM);

    // Set optical properties
    /*std::vector<G4double> energies = {1.2*eV, 1.91*eV, 2.25*eV, 2.76*eV, 6.2*eV};
    std::vector<G4double> absorptionSoft = {0.2*cm, 0.2*cm, 0.04*cm, 0.04*cm, 0.03*cm}; // from Optical properties of biological tissues: a review
    std::vector<G4double> absorptionBone = {0.16*cm, 0.16*cm, 0.077*cm, 0.05*cm, 0.04*cm}; // https://hal.science/hal-02335596/document*/
    /*std::vector<G4double> energy = {1.2*eV, 1.91*eV, 2.25*eV, 2.48*eV, 2.76*eV, 6.2*eV}
    std::vector<G4double> absorptionHeart = {0.2*cm, 0.2*cm, 0.04*cm, 0.1*cm, 0.04*cm, 0.04*cm};
    std::vector<G4double> absorptionKidney = {0.2*cm, 0.2*cm, 0.04*cm, 0.1*cm, 0.04*cm, 0.04*cm};
    std::vector<G4double> absorptionBrain = {0.25*cm, 0.25*cm, 0.16*cm, 0.25*cm, 0.0125*cm, 0.0125*cm};
    std::vector<G4double> absorptionBone = {0.16*cm, 0.16*cm, 0.077*cm, 0.1*cm, 0.005*cm, 0.005*cm};*/
   
    //std::vector<G4MaterialPropertiesTable*> theMPTables;
    /*for(G4int i=0; i<theMaterials.size(); i++) {
        G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
        G4double rIndex = 0.3323 + 0.3422*(theMaterials[i]->GetDensity()/(g/cm3)-1) +1; // from Optical properties of biological tissues: a review
        std::vector<G4double> rIndexVec = {rIndex, rIndex, rIndex, rIndex, rIndex};
        if(theMaterials[i]==fBone) mpt->AddProperty("ABSLENGTH", energies, absorptionBone, true, true);
        else mpt->AddProperty("ABSLENGTH", energies, absorptionSoft, true, true);
        theMPTables.push_back(mpt);
        //theMaterials[i]->SetMaterialPropertiesTable(mpt);
    }*/

    /*for(auto& material: theMaterials) {
        G4double rIndex = 0.3323 + 0.3422*(material->GetDensity()/(g/cm3)-1) +1; // from Optical properties of biological tissues: a review
        G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
        std::vector<G4double> rIndexVec = {rIndex, rIndex, rIndex, rIndex, rIndex};
        mpt->AddProperty("RINDEX", energies, rIndexVec, true, true);
        mpt->AddProperty("ABSLENGTH", energies, absorptionBone, true, true);
        //theMPTables.push_back(mpt);
        //material->SetMaterialPropertiesTable(theMPTables.back());
        material->SetMaterialPropertiesTable(mpt);
    }*/

    /*G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX", energies, std::vector<G4double>(energies.size(), Get_rIndex(fBone->GetDensity())), true, true);
    mpt->AddProperty("ABSLENGTH", energies, absorptionBone, true, true);
    fBone->SetMaterialPropertiesTable(mpt);*/

    /*rIndex = 0.3323 + 0.3422*(fSoft->GetDensity()/(g/cm3)-1) +1; // from Optical properties of biological tissues: a review
    G4MaterialPropertiesTable *mpt_2 = new G4MaterialPropertiesTable();
    std::vector<G4double> rIndexVec_2(energies.size(), rIndex);
    mpt_2->AddProperty("RINDEX", energies, rIndexVec_2, true, true);
    mpt_2->AddProperty("ABSLENGTH", energies, absorptionSoft, true, true);
    fSoft->SetMaterialPropertiesTable(mpt_2);*/
       
    //std::cout << "TEST: " << fBone->GetMaterialPropertiesTable()->GetProperty("RINDEX")[0] << std::endl;

}void MyDetectorConstruction::ConstructMOBY()
{
    // Define MOBY materials
    DefineMaterialsMOBY();

    // Defining the voxelPhantom parametrisation
    G4PhantomParameterisation* voxelizedPhantom = new G4PhantomParameterisation();

    // Setting up voxel
    //HalfVoxelSize = 0.2*mm/2.;
    G4double HalfVoxelSize = 0.18*mm/2.;
    G4int nVoxelX = 550;
    G4int nVoxelY = 200;
    G4int nVoxelZ = 200;

    voxelizedPhantom->SetVoxelDimensions(HalfVoxelSize, HalfVoxelSize, HalfVoxelSize);
    voxelizedPhantom->SetNoVoxels(nVoxelX, nVoxelY, nVoxelZ);
    voxelizedPhantom->SetMaterials(theMaterials);

    size_t* materialIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];

    // Importing CT
    std::ifstream ctfile("../moby_20_atn.txt",std::ios::in);

    if(ctfile.fail()) std::cout << "no file for MOBY ct\n";

    std::string buffer;

    while (ctfile >> buffer) {

        G4int i = std::stoi(buffer);
        ctfile >> buffer;
        G4int j = std::stoi(buffer);
        ctfile >> buffer;
        G4int k = std::stoi(buffer);
        ctfile >> buffer;
        G4double HU = std::stod(buffer);

        G4int n = i+nVoxelX*j+nVoxelX*nVoxelY*k;

        /*if (HU == 0) {materialIDs[n] = 0;} // air
        else if (HU > 57.4 and HU < 57.7) {materialIDs[n] = 1;} // lung
        else if (HU > 220) {materialIDs[n] = 3;} // bone
        else if (HU > 22.9 and HU < 23.1) {materialIDs[n] = 1;} // lung
        else if (HU > 90) {materialIDs[n] = 3;} // bone
        else if (HU > 45.9 and HU < 46) {materialIDs[n] = 4;} // adipose
        else if (HU > 76.888 and HU < 76.890) {materialIDs[n] = 16;} // brain
        else if (HU > 80.680 and HU < 80.682) {materialIDs[n] = 15;} // blood
        else if (HU > 78.099 and HU < 78.101) {materialIDs[n] = 7;} // muscle
        else if (HU > 78.064 and HU < 78.065) {materialIDs[n] = 9;} // intestine (sm)
        else if (HU > 78.988 and HU < 78.989) {materialIDs[n] = 11;} // liver
        else if (HU > 76.001 and HU < 76.002) {materialIDs[n] = 12;} // pancreas
        else if (HU > 80.277 and HU < 80.278) {materialIDs[n] = 14;} // spleen
        else if (HU > 78.261 and HU < 78.262) {materialIDs[n] = 17;} // heart
        else {materialIDs[n] = 2;} // soft tissue*/
        if(HU>85) {materialIDs[n] = 3;}
        else {materialIDs[n] = 2;}
    }

    voxelizedPhantom->SetMaterialIndices(materialIDs);

    G4double HalfPhantomDepth = nVoxelZ*HalfVoxelSize;
    G4Box* cont_solid = new G4Box("PhantomContainer", nVoxelX*HalfVoxelSize, nVoxelY*HalfVoxelSize, HalfPhantomDepth);
    G4LogicalVolume* cont_logic = new G4LogicalVolume( cont_solid, fMaterialAir, "PhantomContainer", 0, 0, 0 );
    G4VPhysicalVolume * cont_phys = new G4PVPlacement(0, G4ThreeVector(0, 0, HalfPhantomDepth), cont_logic, "PhantomContainer", flogicalWorld, false, true);

    voxelizedPhantom->BuildContainerSolid(cont_phys);

    // Assure that the voxels are completely filling the container volume
    voxelizedPhantom->CheckVoxelsFillContainer(cont_solid->GetXHalfLength(), cont_solid->GetYHalfLength(), cont_solid->GetZHalfLength() );
    voxelizedPhantom->SetSkipEqualMaterials(false);

    // The parameterised volume which uses this parameterisation is placed in the container logical volume
    G4VSolid* solVoxel = new G4Box("phantom", HalfVoxelSize, HalfVoxelSize, HalfVoxelSize);
    G4LogicalVolume* logicVoxel = new G4LogicalVolume(solVoxel, fMaterialAir, "phantom", 0, 0, 0);
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
	scinti_holes_number = (G4int) case_side / scinti_pixel_size;
	
	// check proper parameters
	if (scinti_holes_number < 1)
	{
		G4cout << "Error: pixel larger than case!" << G4endl;
		G4cout << "return to default values" << G4endl;
		scinti_septa_thickness = 10.*um;
		scinti_hole_thickness = 2.*mm - scinti_septa_thickness;
		scinti_pixel_size = scinti_hole_thickness + scinti_septa_thickness;
		scinti_holes_number = (G4int) case_side / scinti_pixel_size;
	}
	if ((scinti_holes_number % 2) < 1)
		scinti_holes_number = scinti_holes_number - 1;
	
	// derived parameters
	case_side = (G4double) scinti_pixel_size * scinti_holes_number;
	G4cout << "scinti_pixel_size: " << scinti_pixel_size << " mm" << G4endl;
	G4cout << "scinti_holes_number: " << scinti_holes_number << " " << G4endl;
	G4cout << "scinti_case_side: " << case_side << " mm" << G4endl;
	
	// case
	G4cout << "defining the scintillator case" << G4endl;
	solidScintillatorMatrix = new G4Box("solidScintillatorMatrix", case_side/2., case_side/2., scinti_hole_length/2.);
	logicScintillatorMatrix = new G4LogicalVolume(solidScintillatorMatrix, materialPlastic, "logicScintillatorMatrix");
	physScintillator = new G4PVPlacement(0, G4ThreeVector(0.,0.,hole_length + slab_depth/2.), logicScintillatorMatrix, "physScintillatorMatrix", logicWorld, false, 0, true);
	
	// array
	G4cout << "defining the scintillator array element" << G4endl;
	solidScintillatorArray = new G4Box("solidScintillatorArray", case_side/2., scinti_pixel_size/2., scinti_hole_length/2.);
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
	if ((det_pixels_number % 2) < 1)
		det_pixels_number = det_pixels_number - 1;
	if (det_fill_factor>1)
	{
		G4cout << "Error: fill factor larger than 1" << G4endl;
		det_fill_factor = 1;
	}
	if (det_fill_factor<=0)
	{
		G4cout << "Error: fill factor smaller than or equal to 0" << G4endl;
		det_fill_factor = .1;
	}
	
	// derived parameters
	detector_side = (G4double) det_pixel_size * det_pixels_number;
	G4double det_pixel_active_size = sqrt(det_fill_factor) * det_pixel_size;
	G4cout << "det_pixel_size: " << det_pixel_size / mm << " mm" << G4endl;
	G4cout << "det_fill_factor: " << det_fill_factor * 100. << " %" << G4endl;
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

	if (det_fill_factor < 1)
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
	G4cout << "Optical coupling distance: " << detector_scintillator_distance << G4endl;
	if (detPixelNoSlab == "matrix")
		ConstructPixelDetector();
	else
		ConstructSlabDetector();

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
	}

	if(fScoringScintillator != NULL)
	{
		MySensitiveScintillator *sensScinti = new MySensitiveScintillator("SensitiveScintillator");
		G4SDManager::GetSDMpointer()->AddNewDetector(sensScinti);
		fScoringScintillator->SetSensitiveDetector(sensScinti);
	}
}
