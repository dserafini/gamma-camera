#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name) : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

G4bool MySensitiveScintillator::ProcessHits(G4Step *, G4TouchableHistory *)
{

  return true;
}
