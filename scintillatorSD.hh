#ifndef SCINTILLATOR_HH
#define SCINTILLATOR_HH

#include "G4VSensitiveDetector.hh"
// #include "g4root.hh" // ok with geant version 10.7
#include "g4root_defs.hh" // ok with geant version 11.1
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"


class MySensitiveScintillator : public G4VSensitiveDetector
{
public:
	MySensitiveScintillator(G4String);
	~MySensitiveScintillator();

private:
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};

#endif
