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
  fSaveEvent = false;

  // quantum efficiency
  quEff = new G4PhysicsOrderedFreeVector();
  // read the data files
  std::ifstream datafile;
  datafile.open("eff.dat");
  // put the read data files into the quEff vector
  while(datafile.good())
  {
    G4double wlen, queff;

    datafile >> wlen >> queff;

    if(datafile.eof())
      break;

    quEff->InsertValues(wlen, queff/100.);
  }
  // close data file
  datafile.close();
  quEff->SetSpline(false);
  quEff->DumpValues();

  // default energy detection threshold
  nofHitsThreshold = 1;

  // default fill factor
  fillfactor = 1;
}

MySensitiveDetector::~MySensitiveDetector() {}

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
  fSaveEvent = false;

  // G4cout << "SiPM reads at least " << nofHitsThreshold << " photons" << G4endl;
  // G4cout << "SiPM fill factor " << fillfactor << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // G4cout << "MySensitiveDetector::ProcessHits" << G4endl;
  
  // process hits only for optical photons
  if (aStep->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition())
    return false;

  // kill every detected optical photon
  aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  // filter on geometrical fill factor
  if ( G4UniformRand() > fillfactor )
    return false;

  // filter on optical photon wavelength
  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
  G4double enePhoton = aStep->GetTrack()->GetKineticEnergy();
  G4double wlen = (1.239841939*eV/enePhoton)*1e3;
  if (G4UniformRand() > quEff->Value(wlen))
    return false;
  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit(); // [keV]

  // if (edep==0.) return false;

  auto newHit = new detectorHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
  newHit->SetPixelPos (preStepPoint->GetTouchable()->GetTranslation());
  // G4cout << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << ": " << aStep->GetPreStepPoint()->GetTouchable()->GetTranslation() << G4endl;

  if (fHitsCollection)
    fHitsCollection->insert( newHit );
  
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

  // at least I need a total number of hits equal to the energy threshold
  if (nofHits >= nofHitsThreshold)
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
  
    // G4cout << "nofHits: " << nofHits << G4endl;
    
    // fill the Ntuple of optical photons
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(Tuples::kOptical, TOptical::kNumber, nofHits);
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kMeanX, fMeanPos.getX());
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kMeanY, fMeanPos.getY());
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kMeanZ, fMeanPos.getZ());
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kSigmaX, fSigmaPos.getX());
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kSigmaY, fSigmaPos.getY());
    man->FillNtupleDColumn(Tuples::kOptical, TOptical::kSigmaR, fSigmaMod);
  
    // save pixel tree
    std::vector <G4ThreeVector> pixelPos = {};
    std::vector <G4int> pixelCount = {};
    for ( G4int i=0; i<nofHits; i++ )
    {
      auto it = std::find(pixelPos.begin(),pixelPos.end(),(*fHitsCollection)[i]->GetPixelPos());
      if(it == pixelPos.end())
      {
        pixelPos.push_back((*fHitsCollection)[i]->GetPixelPos());
        pixelCount.push_back(1);
      }
      else
        pixelCount.at(std::distance(pixelPos.begin(),it)) += 1;
    }
    // G4cout << "pixelCount.size: " << pixelCount.size() << G4endl;
    // G4cout << "pixelPos.size: " << pixelPos.size() << G4endl;
    
    G4int totalGoodCounts = 0;
    G4ThreeVector meanPixelPos = G4ThreeVector();
    G4ThreeVector mostPixelPos = G4ThreeVector();
    G4int imax = 0;
    if (pixelCount.size() > 0)
    {
    for (unsigned long i=0; i<pixelCount.size();)
    {
     // G4cout << pixelCount.at(i) << G4endl;
     //  G4cout << pixelPos.at(i) << G4endl;

      // threshold must be applied pixel wise
      if (pixelCount.at(i) < nofHitsThreshold)
      {
        pixelCount.erase(pixelCount.begin() + i);
        pixelPos.erase(pixelPos.begin() + i);
      }
      else
      {
        totalGoodCounts += pixelCount.at(i);
        meanPixelPos += pixelCount.at(i)*pixelPos.at(i);
        if (pixelCount.at(i) > pixelCount.at(imax))
          imax = i;
        i++;
      }
    }
    
    if (pixelCount.size() > 0)
    {
      IShouldSaveEvent();
      meanPixelPos /= totalGoodCounts;
      mostPixelPos = pixelPos.at(imax);
    }
    }
  /*
    // print to check
    // for ( G4int i=0; i<((G4int)pixelPos.size()); i++)
    //   G4cout << pixelPos.at(i) << "\t" << pixelCount.at(i) << G4endl;
  
    // find maximum pixel
    G4ThreeVector mostPixelPos = pixelPos.at(std::distance(pixelCount.begin(),std::max_element(pixelCount.begin(),pixelCount.end())));
  */
    // G4cout << "found most pixel pos: " << mostPixelPos << G4endl;
    // G4cout << "found mean pixel pos: " << meanPixelPos << G4endl;
    man->FillNtupleIColumn(Tuples::kSipm, Tsipm::kNumber, totalGoodCounts);
    man->FillNtupleDColumn(Tuples::kSipm, Tsipm::kMostX, mostPixelPos.getX());
    man->FillNtupleDColumn(Tuples::kSipm, Tsipm::kMostY, mostPixelPos.getY());
    man->FillNtupleDColumn(Tuples::kSipm, Tsipm::kMeanX, meanPixelPos.getX());
    man->FillNtupleDColumn(Tuples::kSipm, Tsipm::kMeanY, meanPixelPos.getY());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MySensitiveDetector::SetDetectionThreshold(G4int aThreshold)
{
  nofHitsThreshold = aThreshold;
}

void MySensitiveDetector::SetFillFactor(G4double aFactor)
{
  if (aFactor<0 || aFactor >1)
    G4cout << "Error: " << aFactor << " is invalid as fill factor value!!!" << G4endl;
  else
    fillfactor = aFactor;
  return;
}
