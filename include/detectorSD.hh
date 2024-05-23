#ifndef DETECTORSD_HH
#define DETECTORSD_HH

#include "G4VSensitiveDetector.hh"
#include "g4root.hh" // ok with geant version 10.7
// #include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4OpticalPhoton.hh"
#include "enums.hh"

#include "detectorHit.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String, const G4String&);
	~MySensitiveDetector();
	void Initialize(G4HCofThisEvent* hitCollection) override;
	void SetDetectionThreshold(G4int threshold);
	void SetFillFactor(G4double value);
	G4int GetDetectionThreshold() {return nofHitsThreshold;}
	G4double GetFillFactor() {return fillfactor;}
	G4bool ShouldISaveEvent() {return fSaveEvent;}
	void IShouldSaveEvent() {fSaveEvent = true;}
	void IShouldNotSaveEvent() {fSaveEvent = false;}

private:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
	void EndOfEvent(G4HCofThisEvent* hitCollection) override;
	
	detectorHitsCollection* fHitsCollection;
	G4ThreeVector fMeanPos;
	G4ThreeVector fSigmaPos;
	G4double fSigmaMod;
	G4int nofHits;

	// quantum efficiency
	G4PhysicsOrderedFreeVector *quEff;
	G4double fillfactor;

	// energy threshold
	G4int nofHitsThreshold;

	// save or not the event
	G4bool fSaveEvent;
};

#endif
