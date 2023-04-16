#ifndef DETECTORSD_HH
#define DETECTORSD_HH

#include "G4VSensitiveDetector.hh"
// #include "g4root.hh" // ok with geant version 10.7
#include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4OpticalPhoton.hh"

#include "detectorHit.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String, const G4String&);
	~MySensitiveDetector();
	void Initialize(G4HCofThisEvent* hitCollection) override;

private:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
	void EndOfEvent(G4HCofThisEvent* hitCollection) override;
	
	detectorHitsCollection* fHitsCollection;

	// quantum efficiency
	G4PhysicsOrderedFreeVector *quEff;
};

#endif
