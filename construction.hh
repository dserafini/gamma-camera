// trick to not include multiple times
#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
// per le unità di misura tipo metri m
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
// per cambiare geometria on the way
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	// need contructor and destructor
	MyDetectorConstruction();
	~MyDetectorConstruction();

	// create a scoring volume to store only the energy deposited in the radiator
	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

	virtual G4VPhysicalVolume *Construct();
	// virtual because it is defined in G4USerDetectorConstruction

private:
	// the logicDetector defined in Construct method must be available also in other methods
	// so we define it here
	G4LogicalVolume *logicDetector;

	virtual void ConstructSDandField();

	// declare some things here to modify geometry on the run
	G4Box *solidWorld, *solidDetector, *solidScintillator, *solidCollimatorPixel, *solidCollimatorArray, *solidCase, *solidCollimatorPinhole;
	G4LogicalVolume *logicWorld, *logicCase, *logicScintillator, *logicCollimator, *logicCollimatorPixel, *logicCollimatorArray, *logicCollimatorPinhole;
	G4VPhysicalVolume *physWorld, *physCollimator, *physCase, *physScintillator;

	G4Material *materialAir, *materialTungsten, *materialPMT, *materialAluminum, *materialLanthanumBromide, *materialGAGG;
	G4Element *elLa, *elBr;

	void DefineMaterials();
	void DefineMaterialsProperties();

	void ConstructScintillator();
	void ConstructCollimator();
	void ConstructCase();
	void SetVisualizationFeatures();

	// we do not need to access the messenger from outside
	G4GenericMessenger *fMessenger;

	G4LogicalVolume *fScoringVolume;

	// declare variables used for the messenger
	G4int nCols, nRows;

	G4double xWorld, yWorld, zWorld;
	G4bool isCherenkov;
	G4bool isScintillator;
	G4bool isTOF;

	G4OpticalSurface *mirrorSurface;

	G4double collimator_thickness, case_side, case_wall_thickness, hole_thickness, pixel_size, hole_length, septa_thickness, holes_number;
	G4int pinhole_number;
};

#endif
