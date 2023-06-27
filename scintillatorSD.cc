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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool MySensitiveScintillator::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // G4cout << "MySensitiveScintillator::ProcessHits" << G4endl;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveScintillator::EndOfEvent(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveScintillator::EndOfEvent" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
