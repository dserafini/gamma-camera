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
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"
#include "G4SDManager.hh"
#include "detectorSD.hh"
#include "scintillatorSD.hh"
#include "infoPhantom.hh"
#include "parameters.hh"

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
	G4Box *solidScintillatorPinhole, *solidScintillatorPixel, *solidScintillatorArray, *solidScintillatorMatrix, *solidScintillatorReflectorCase, *solidScintillatorAluminumCase;
	G4LogicalVolume *logicScintillatorMatrix, *logicScintillatorArray, *logicScintillatorPixel, *logicScintillatorPinhole, *logicScintillatorReflectorCase, *logicScintillatorAluminumCase;
	G4VPhysicalVolume *physScintillatorPinhole, *physScintillatorPixel, *physScintillatorMatrix, *physScintillatorReflectorCase, *physScintillatorAluminumCase;

	// optical photons coupling slab
	G4Box *solidCoupler;
	G4LogicalVolume *logicCoupler;
	G4VPhysicalVolume *physCoupler;

	// detector slab
	G4Box *solidDetector;
	G4LogicalVolume *logicDetector;
	G4VPhysicalVolume *physDetector;

	// detector pixel
	G4Box *solidDetectorPixel, *solidDetectorFullPixel, *solidDetectorArray, *solidDetectorMatrix, *solidDetectorPwbCase, *solidDetectorSeal;
	G4LogicalVolume *logicDetectorPixel, *logicDetectorFullPixel, *logicDetectorArray, *logicDetectorMatrix, *logicDetectorPwbCase, *logicDetectorSeal;
	G4VPhysicalVolume *physDetectorPixel, *physDetectorFullPixel, *physDetectorMatrix;

	// collimator pixel
	G4Box *solidCollimatorPinhole, *solidCollimatorPixel, *solidCollimatorArray, *solidCollimatorMatrix;
	G4LogicalVolume *logicCollimatorMatrix, *logicCollimatorPixel, *logicCollimatorArray, *logicCollimatorPinhole;
	G4VPhysicalVolume *physCollimator, *physCollimatorMatrix;

	// materials
	G4Material *materialAir, *materialTungsten, *materialPMT, *materialAluminum, *materialLanthanumBromide, *materialGAGG, *materialSilicon;
	G4Material *materialPlastic, *materialBariumSulfate, *materialScintillatorReflector, *materialScintillatorCase;
	G4Material *materialSiliconRubber, *materialOpticalCoupler;
	G4Element *elLa, *elBr;

	void DefineMaterials();
	void DefineMaterialsProperties();
	void DefineOpticalSurfaceProperties();

	void ConstructSlabScintillator();
	void ConstructEpicPixelScintillator();
	void ConstructCollimator();
	void ConstructSlabDetector();
	void ConstructCoupler();
	void ConstructHamaPixelDetector();
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
	G4double scinti_case_side, scinti_case_depth, scinti_outer_reflector_side, scinti_outer_reflector_depth, scinti_matrix_side, scinti_matrix_depth;
	G4double scinti_pixel_depth, scinti_gagg_side, scinti_gagg_depth, scinti_reflector_thickness, scinti_aluminum_thickness;
	G4int scinti_holes_number;
	G4bool scintillatorExist;
	G4String scintiPixelNoSlab;
	
	// detector
	G4double det_pixel_size, detector_depth, detector_side, detector_scintillator_distance, det_fill_factor, channel_dead_space, det_pixel_active_size;
	G4int det_pixels_number, energyThreshold;
	G4String detPixelNoSlab;
	G4ThreeVector detector_centre_position;
	G4bool detSaveAllOpticals;
	G4double det_pwb_case_side,	det_pwb_case_thickness,	det_seal_side, det_seal_thickness, det_matrix_side, det_matrix_thickness, det_channel_dead_space, det_channel_active_side;
	G4double det_channel_number, det_channel_pitch;
	
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
