#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name) : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
  fEdep = 0.;
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

void MySensitiveScintillator::Initialize(G4HCofThisEvent*)
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

void MySensitiveScintillator::EndOfEvent(G4HCofThisEvent*)
{
  G4cout << "MySensitiveScintillator::EndOfEvent" << G4endl;
  G4cout << "Energy deposited: " << fEdep/keV << " keV" << G4endl;
  return;
}
