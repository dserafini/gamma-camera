#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name) : G4VSensitiveDetector(name)
{
  // G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

void MySensitiveScintillator::Initialize(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveScintillator::Initialize" << G4endl;
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  return;
}

G4bool MySensitiveScintillator::ProcessHits(G4Step * aStep, G4TouchableHistory *)
{
  // G4cout << "MySensitiveScintillator::ProcessHits" << G4endl;

  if (aStep->GetTrack()->GetParticleDefinition() == G4OpticalPhoton::Definition())
    return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  fEdep += edep;

  G4ThreeVector delta = aStep->GetPreStepPoint()->GetPosition() * edep;
  fPosition += delta;
  
  return true;
}

void MySensitiveScintillator::EndOfEvent(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveScintillator::EndOfEvent" << G4endl;
  
  // normalize fPosition on total energy deposit
  if (fEdep>0)
    fPosition /= fEdep;
  
  // G4cout << "Energy deposited: " << fEdep/keV << " keV" << G4endl;
  // G4cout << "Position: " << fPosition << " keV" << G4endl;
  
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  man->FillNtupleDColumn(Tuples::kScintillator, TScintillator::kEdep, fEdep/keV); // [keV]
  man->FillNtupleDColumn(Tuples::kScintillator, TScintillator::kGammaX, fPosition.getX()/mm); // [mm]
  man->FillNtupleDColumn(Tuples::kScintillator, TScintillator::kGammaY, fPosition.getY()/mm);
  man->FillNtupleDColumn(Tuples::kScintillator, TScintillator::kGammaZ, fPosition.getZ()/mm);
  
  return;
}
