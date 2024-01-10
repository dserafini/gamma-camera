#ifndef SCINTILLATOR_HH
#define SCINTILLATOR_HH

#include "G4VSensitiveDetector.hh"
#include "g4root.hh" // ok with geant version 10.7
// #include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "enums.hh"

class MySensitiveScintillator : public G4VSensitiveDetector
{
public:
	MySensitiveScintillator(G4String);
	~MySensitiveScintillator();

private:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
	void Initialize(G4HCofThisEvent*) override;
	void EndOfEvent(G4HCofThisEvent*) override;
	G4double fEdep;
	G4ThreeVector fPosition;

};

#endif
