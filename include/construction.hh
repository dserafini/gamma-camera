// trick to not include multiple times
#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PhantomParameterisation.hh"
#include "G4PVParameterised.hh"
// per le unità di misura tipo metri m
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VisAttributes.hh"
// per cambiare geometria on the way
#include "G4GenericMessenger.hh"
#include "G4SDManager.hh"
#include "detectorSD.hh"
#include "scintillatorSD.hh"
#include "infoPhantom.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	// need contructor and destructor
	MyDetectorConstruction();
	~MyDetectorConstruction();

	// create a scoring volume to store only the energy deposited in the radiator
	G4LogicalVolume *GetScoringScintillator() const { return fScoringScintillator; }
	G4LogicalVolume *GetDetectorVolume() const { return logicDetector; }

	virtual G4VPhysicalVolume *Construct();
	// virtual because it is defined in G4USerDetectorConstruction

	// to build MOBY
	void DefineMaterialsMOBY();
    	void ConstructMOBY();

private:
	virtual void ConstructSDandField();

	// world
	G4Box *solidWorld;
	G4LogicalVolume *logicWorld;
	G4VPhysicalVolume *physWorld;

	// scintillator slab
	G4Box *solidScintillator;
	G4LogicalVolume *logicScintillator;
	G4VPhysicalVolume *physScintillator;

	// scintillator pixel
	G4Box *solidScintillatorPinhole, *solidScintillatorPixel, *solidScintillatorArray, *solidScintillatorMatrix;
	G4LogicalVolume *logicScintillatorMatrix, *logicScintillatorArray, *logicScintillatorPixel, *logicScintillatorPinhole;
	G4VPhysicalVolume *physScintillatorPinhole, *physScintillatorPixel, *physScintillatorMatrix;

	// optical photons coupling slab
	G4Box *solidCoupler;
	G4LogicalVolume *logicCoupler;
	G4VPhysicalVolume *physCoupler;

	// detector slab
	G4Box *solidDetector;
	G4LogicalVolume *logicDetector;
	G4VPhysicalVolume *physDetector;

	// detector pixel
	G4Box *solidDetectorPixel, *solidDetectorFullPixel, *solidDetectorArray, *solidDetectorMatrix;
	G4LogicalVolume *logicDetectorPixel, *logicDetectorFullPixel, *logicDetectorArray, *logicDetectorMatrix;
	G4VPhysicalVolume *physDetectorPixel, *physDetectorFullPixel, *physDetectorMatrix;

	// collimator pixel
	G4Box *solidCollimatorPinhole, *solidCollimatorPixel, *solidCollimatorArray, *solidCollimatorMatrix;
	G4LogicalVolume *logicCollimatorMatrix, *logicCollimatorPixel, *logicCollimatorArray, *logicCollimatorPinhole;
	G4VPhysicalVolume *physCollimator, *physCollimatorMatrix;

	// materials
	G4Material *materialAir, *materialTungsten, *materialPMT, *materialAluminum, *materialLanthanumBromide, *materialGAGG, *materialSilicon;
	G4Material *materialPlastic;
	G4Element *elLa, *elBr;

	void DefineMaterials();
	void DefineMaterialsProperties();
	void DefineOpticalSurfaceProperties();

	void ConstructSlabScintillator();
	void ConstructPixelScintillator();
	void ConstructCollimator();
	void ConstructSlabDetector();
	void ConstructCoupler();
	void ConstructPixelDetector();
	void SetVisualizationFeatures();

	// we do not need to access the messenger from outside
	G4GenericMessenger *fMessengerCollimator;
	G4GenericMessenger *fMessengerScintillator;
	G4GenericMessenger *fMessengerDetector;
	G4GenericMessenger *fMessengerCoupler;
	G4GenericMessenger *fMessengerMoby;

	G4LogicalVolume *fScoringScintillator, *fScoringDetector;
	G4VPhysicalVolume *physScoringScintillator;

	// declare variables used for the messenger
	G4int nCols, nRows;

	G4double xWorld, yWorld, zWorld;
	G4bool isCherenkov;
	G4bool isScintillator;
	G4bool isTOF;

	G4OpticalSurface *mirrorSurface;

	// collimator
	G4double collimator_thickness, case_side, case_wall_thickness, hole_thickness, pixel_size, hole_length, septa_thickness;
	G4int holes_number;
	G4bool collimatorExist;
	G4double collimator_posX, collimator_posY;
	
	// scintillator
	G4double slab_depth, slab_side, scinti_pixel_size, scinti_septa_thickness, scinti_hole_thickness, scinti_hole_length;
	G4int scinti_holes_number;
	G4bool scintillatorExist;
	G4String scintiPixelNoSlab;
	
	// detector
	G4double det_pixel_size, detector_depth, detector_side, detector_scintillator_distance, det_fill_factor, channel_dead_space;
	G4int det_pixels_number, energyThreshold;
	G4String detPixelNoSlab;
	G4ThreeVector detector_centre_position;
	G4bool detSaveAllOpticals;
	
	// Materials MOBY
	G4Material* fVoxelMaterial;
	G4Material* fSoft;
	G4Material* fCBone;
	G4Material* fLungM;
	G4Material* fBone;
	std::vector<G4Material*> theMaterials;

	// moby
	G4int nVoxelX, nVoxelY, nVoxelZ;
	G4double mouseXsize;
	G4String buildMoby;
};

#endif
