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
  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit(); // [keV]

  if (edep==0.) return false;

  auto newHit = new detectorHit();

  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  if (fHitsCollection)
    fHitsCollection->insert( newHit );
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveScintillator::EndOfEvent(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveScintillator::EndOfEvent" << G4endl;

  nofHits = fHitsCollection->entries();
  
  // calculate mean position and energy deposit
  for ( G4int i=0; i<nofHits; i++ )
  {
    fEdep += (*fHitsCollection)[i]->GetEdep();
    fPosition += (*fHitsCollection)[i]->GetPos()*(*fHitsCollection)[i]->GetEdep();
  }

  if (fEdep>0)
    fPosition /= fEdep; // normalize on the total energy
  
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;
  G4cout << "fPosition: " << fPosition << " vector" << G4endl;
  
  man->FillNtupleDColumn(0, 1, fEdep/keV); // [keV]
  man->FillNtupleDColumn(0, 2, fPosition.getX()/mm); // [mm]
  man->FillNtupleDColumn(0, 3, fPosition.getY()/mm);
  man->FillNtupleDColumn(0, 4, fPosition.getZ()/mm);
  man->AddNtupleRow(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
