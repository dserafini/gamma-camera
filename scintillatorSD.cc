#include "scintillatorSD.hh"

MySensitiveScintillator::MySensitiveScintillator(G4String name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveScintillator::MySensitiveScintillator" << G4endl;
    
  fHitsCollection = nullptr;
  collectionName.insert(hitsCollectionName);
}

MySensitiveScintillator::~MySensitiveScintillator()
{}

void MySensitiveScintillator::Initialize(G4HCofThisEvent* hce)
{
  // G4cout << "MySensitiveScintillator::Initialize" << G4endl;
  
  // Create hits collection
  fHitsCollection = new scintillatorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );

  // reset gamma information
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  
  // default is no initial energy
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn(0, 0, 0.*eV); // [eV]
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool MySensitiveScintillator::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4cout << "MySensitiveScintillator::ProcessHits" << G4endl;

  if(aStep->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition())
  {
    G4double edep = aStep->GetTotalEnergyDeposit() / keV;
    fEventAction->AddEdep(edep);
    G4cout << "adding " << edep << " keV" << G4endl;

    G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
    fEventAction->AddPosition(position, edep);
    
    return true;
  }
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveScintillator::EndOfEvent(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveScintillator::EndOfEvent" << G4endl;
  G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // gammas
  man->FillNtupleDColumn(0, 1, fEdep/keV); // [keV]
  
  G4cout << "fPosition: " << fPosition << " vector" << G4endl;
  if (fEdep>0)
    fPosition = fPosition/fEdep; // normalize on the total energy
  
  man->FillNtupleDColumn(0, 2, fPosition.getX());
  man->FillNtupleDColumn(0, 3, fPosition.getY());
  man->FillNtupleDColumn(0, 4, fPosition.getZ());
  man->AddNtupleRow(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
