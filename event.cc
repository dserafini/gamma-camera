#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  // gammas
  fEdep = 0.;
  fPrePosition = G4ThreeVector(0.,0.,0.);
  fPostPosition = G4ThreeVector(0.,0.,0.);
  fCross = 0;
  fCopyNumber = -1;
  
  // optical photons
  pNum = 0;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // whenever a new event starts the energy accumulated in the previous event should be set to 0
  fEdep = 0.;
  fPrePosition = G4ThreeVector(0.,0.,0.);
  fPostPosition = G4ThreeVector(0.,0.,0.);
  fCross = -2;
  pNum = 0;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
  fCopyNumber = -1;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  // G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // gammas
  man->FillNtupleDColumn(0, 0, fEdep);
  
  // G4cout << "fPosition: " << fPosition << " vector" << G4endl;
  if (fEdep>0)
  {
    fPrePosition = fPrePosition/fEdep; // normalize on the total energy
    fPostPosition = fPostPosition/fEdep;
  }
  
  man->FillNtupleDColumn(0, 1, fPrePosition.getX());
  man->FillNtupleDColumn(0, 2, fPrePosition.getY());
  man->FillNtupleDColumn(0, 3, fPrePosition.getZ());
  
  // optical photons
  man->FillNtupleIColumn(0, 4, fCross);
  
  // G4cout << "pPosition: " << pPosition << " vector" << G4endl;
  // G4cout << "pNumber: " << pNum << G4endl;
  if (pNum>0)
  {
    pPosition = pPosition/pNum; // normalize on the number of optical photons
    pSigma = pSigma/pNum;
  }
  
  man->FillNtupleDColumn(0, 5, fPostPosition.getX());
  man->FillNtupleDColumn(0, 6, fPostPosition.getY());
  man->FillNtupleDColumn(0, 7, fPostPosition.getZ());

  man->AddNtupleRow(0);
}
