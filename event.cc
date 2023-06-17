#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  // gammas
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  
  // optical photons
  pNum = 0;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // G4cout << "MyEventAction::BeginOfEventAction" << G4endl;
  
  // whenever a new event starts the energy accumulated in the previous event should be set to 0
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  pNum = 0;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  // G4cout << "MyEventAction::EndOfEventAction" << G4endl;
  // G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // gammas
  man->FillNtupleDColumn(0, 1, fEdep/keV); // [keV]
  
  // G4cout << "fPosition: " << fPosition << " vector" << G4endl;
  if (fEdep>0)
    fPosition = fPosition/fEdep; // normalize on the total energy
  
  man->FillNtupleDColumn(0, 2, fPosition.getX());
  man->FillNtupleDColumn(0, 3, fPosition.getY());
  man->FillNtupleDColumn(0, 4, fPosition.getZ());
  
  if (pNum>0)
  {
    pPosition = pPosition/pNum; // normalize on the number of optical photons
    pSigma = pSigma/pNum;
  }

  man->AddNtupleRow(0);
}
