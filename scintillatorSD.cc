#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name) : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
  fEdep = 0.;
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

void MySensitiveScintillator::Initialize()
{
  G4cout << "MySensitiveScintillator::Initialize" << G4endl;
  fEdep = 0.;
  return;
}

G4bool MySensitiveScintillator::ProcessHits(G4Step * aStep, G4TouchableHistory *)
{
  G4cout << "MySensitiveScintillator::ProcessHits" << G4endl;

  fEdep += aStep->GetTotalEnergyDeposit();
  
  return true;
}
