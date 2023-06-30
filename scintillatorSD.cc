#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name) : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

G4bool MySensitiveScintillator::ProcessHits(G4Step *, G4TouchableHistory *)
{
  G4cout << "MySensitiveScintillator::ProcessHits" << G4endl;
  return true;
}
