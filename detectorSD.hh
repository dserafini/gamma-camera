#ifndef DETECTORSD_HH
#define DETECTORSD_HH

#include "G4VSensitiveDetector.hh"
// #include "g4root.hh" // ok with geant version 10.7
#include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include "detectorHit.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();
	void   Initialize(G4HCofThisEvent* hitCollection) override;
	G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
	void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	TrackerHitsCollection* fHitsCollection = nullptr;

	// quantum efficiency
	G4PhysicsOrderedFreeVector *quEff;
};

#endif
