#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
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
    G4cout << wlen << " " << queff << G4endl;

    quEff->InsertValues(wlen, queff/100.);
  }

  // close data file
  datafile.close();

  quEff->SetSpline(false);
  // in standard configuration Spline is set to on
  // it means that interpolates distances between data points with splines
  // he said it sometimes creates problem if u use splines
  // he suggests to have measurements with very small distances in wavelength
  // and then using linear interpolation in between the points
  // this will lead to less problems
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
  // we want information about the position of our photons

  // first we get the track from the step
  G4Track* track = aStep->GetTrack();

  // track->SetTrackStatus(fStopAndKill);
  // do not kill the particle in TOF setup

  // we define 2 step points
  // there are only 2 steps for photons in the SD because they are not charged so they do not deposit energy
  // when the photon is entering the SD
  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
  // when the photon is leaving the SD
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

  // photon wavelength in nm
  G4ThreeVector momPhoton = preStepPoint->GetMomentum();

  G4double time = preStepPoint->GetGlobalTime();
  // I shold decide if which time to take: preStep, posStep or average
  // anyway the tile is thin so it does not matter too much
  // local time: counter starts when the particle is created
  // global time: counter reset to 0 at the start of each new event
  // local and global times differ in case of decaying particles
  // usually in experiments only global time can be accessed

  G4double wlen = (1.239841939*eV/momPhoton.mag())*1e3;

  G4ThreeVector posPhoton = preStepPoint->GetPosition();

  // G4cout << "photon position: " << posPhoton << G4endl;
  // we do not have access to the real photon position in a real experiment
  // we only have access to the detector copy number
  // rootsee with Photons->Draw("fX:fY:fZ","","surf")

  const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

  G4int copyNo = touchable->GetCopyNumber();

  // G4cout << "copy number: " << copyNo << G4endl;

  // you can also have the position of the detector
  G4VPhysicalVolume *physVol = touchable->GetVolume();
  G4ThreeVector posDetector = physVol->GetTranslation();

  // G4cout << "Detector position: " << posDetector << G4endl;

  // get the event number
  G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // store the information of the photon (Monte Carlo truth)
  man->FillNtupleIColumn(0, 0, evt);
  man->FillNtupleDColumn(0, 1, posPhoton[0]);
  man->FillNtupleDColumn(0, 2, posPhoton[1]);
  man->FillNtupleDColumn(0, 3, posPhoton[2]);
  man->FillNtupleDColumn(0, 4, wlen);
  man->FillNtupleDColumn(0, 5, time);
  man->AddNtupleRow(0);

  // store the information of the real experiment
  if (G4UniformRand() < quEff->Value(wlen))
  {
    man->FillNtupleIColumn(1, 0, evt);
    man->FillNtupleDColumn(1, 1, posDetector[0]);
    man->FillNtupleDColumn(1, 2, posDetector[1]);
    man->FillNtupleDColumn(1, 3, posDetector[2]);
    man->AddNtupleRow(1);
  }

  return true;
}
