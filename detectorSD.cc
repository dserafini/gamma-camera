#include "detectorSD.hh"

MySensitiveDetector::MySensitiveDetector(G4String name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveDetector::MySensitiveDetector" << G4endl;
  quEff = new G4PhysicsOrderedFreeVector();

  // read the data files
  std::ifstream datafile;
  datafile.open("eff.dat");

  // put the read data files into the quEff vector
  while(1)
  {
    G4double wlen, queff;

    datafile >> wlen >> queff;

    if(datafile.eof())
      break;

    // make sure everything work fine by printing
    // G4cout << wlen << " " << queff << G4endl;

    quEff->InsertValues(wlen, queff/100.);
  }

  // close data file
  datafile.close();

  // quEff->SetSpline(false);
  // in standard configuration Spline is set to on
  // it means that interpolates distances between data points with splines
  // he said it sometimes creates problem if u use splines
  // he suggests to have measurements with very small distances in wavelength
  // and then using linear interpolation in between the points
  // this will lead to less problems
  
  fHitsCollection = nullptr;
  collectionName.insert(hitsCollectionName);
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // G4cout << "MySensitiveDetector::ProcessHits" << G4endl;
  
  // process hits only for optical photons
  if (aStep->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition())
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

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  G4cout << "MySensitiveDetector::EndOfEvent" << G4endl;
  
  G4int nofHits = fHitsCollection->entries();
  if ( verboseLevel>1 ) {
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
  
  // calculate mean position
  G4ThreeVector mean = G4ThreeVector(0.,0.,0.);
  for ( G4int i=0; i<nofHits; i++ )
    mean += (*fHitsCollection)[i]->GetPos();
  mean /= nofHits;
  
  // calculate standard deviation of position
  G4double sigma = 0.;
  for ( G4int i=0; i<nofHits; i++ )
    sigma += (mean - (*fHitsCollection)[i]->GetPos()).mag();
  sigma = sqrt(sigma / (nofHits - 1));
  
  G4cout << "mean: " << mean << ",\t sigma: " << sigma << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

