#ifndef SCINTILLATORSD_HH
#define SCINTILLATORSD_HH

#include "G4VSensitiveDetector.hh"
#include "g4root.hh" // ok with geant version 10.7
// #include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "event.hh"

#include "scintillatorHit.hh"

class MySensitiveScintillator : public G4VSensitiveDetector
{
public:
	MySensitiveScintillator(G4String, const G4String&);
	~MySensitiveScintillator();
	void Initialize(G4HCofThisEvent* hitCollection) override;

private:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
	void EndOfEvent(G4HCofThisEvent* hitCollection) override;
	
	scintillatorHitsCollection* fHitsCollection;
	MyEventAction *fEventAction;
};

#endif
