#include "detectorSD.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveDetector::MySensitiveDetector" << G4endl;
    
  fHitsCollection = nullptr;
  collectionName.insert(hitsCollectionName);
  fMeanPos = G4ThreeVector();
  fSigmaPos = G4ThreeVector();
  fSigmaMod = 0.;
  nofHits = 0;
}

MySensitiveDetector::~MySensitiveDetector()
{}

void MySensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  // G4cout << "MySensitiveDetector::Initialize" << G4endl;
  
  // Create hits collection
  fHitsCollection = new detectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
  
  // reset for each event
  fMeanPos = G4ThreeVector();
  fSigmaPos = G4ThreeVector();
  fSigmaMod = 0.;
  nofHits = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4cout << "MySensitiveDetector::ProcessHits" << G4endl;
  
  // process hits only for optical photons
  if (aStep->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition())
    return false;
  
  // process hits only at z = 61 mm
  if (aStep->GetPostStepPoint()->GetPosition().getZ() != 61*mm)
    return false;
  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  // if (edep==0.) return false;

  auto newHit = new detectorHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  if (fHitsCollection)
    fHitsCollection->insert( newHit );

  // newHit->Print();
  // newHit->Draw();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  // G4cout << "MySensitiveDetector::EndOfEvent" << G4endl;
  
  nofHits = fHitsCollection->entries();
  if ( verboseLevel>1 ) {
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the SiPM detector: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
  
  if (nofHits > 0)
  {
    // calculate mean position
    for ( G4int i=0; i<nofHits; i++ )
      fMeanPos += (*fHitsCollection)[i]->GetPos();
    fMeanPos /= nofHits;

    // calculate standard deviation of position
    for ( G4int i=0; i<nofHits; i++ )
    {
      G4ThreeVector addend = fMeanPos - (*fHitsCollection)[i]->GetPos();
      fSigmaPos.setX( fSigmaPos.getX() + addend.getX()*addend.getX() );
      fSigmaPos.setY( fSigmaPos.getY() + addend.getY()*addend.getY() );
      fSigmaMod += (fMeanPos - (*fHitsCollection)[i]->GetPos()).mag2();
    }
    fSigmaPos.setX(sqrt(fSigmaPos.getX() / (nofHits - 1)));
    fSigmaPos.setY(sqrt(fSigmaPos.getY() / (nofHits - 1)));
    fSigmaMod = sqrt(fSigmaMod / (nofHits - 1));
  }
  
  // G4cout << "mean: " << fMeanPos << ",\t sigma: " << fSigma 
  // << ",\t nofHits: " << nofHits << G4endl;
  
  // fill the Ntuple
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->FillNtupleIColumn(0, 4, nofHits);
  man->FillNtupleDColumn(0, 5, fMeanPos.getX());
  man->FillNtupleDColumn(0, 6, fMeanPos.getY());
  man->FillNtupleDColumn(0, 7, fMeanPos.getZ());
  man->FillNtupleDColumn(0, 8, fSigmaPos.getX());
  man->FillNtupleDColumn(0, 9, fSigmaPos.getY());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

