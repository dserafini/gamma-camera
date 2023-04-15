#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  // gammas
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  
  // optical photons
  pNumber = 0.;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // whenever a new event starts the energy accumulated in the previous event should be set to 0
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  pNumber = 0.;
  pPosition = G4ThreeVector(0.,0.,0.);
  pSigma = G4ThreeVector(0.,0.,0.);
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  // G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // gammas
  man->FillNtupleDColumn(0, 0, fEdep);
  
  if (fEdep>0)
    fPosition = fPosition/fEdep; // normalize on the total energy
  
  man->FillNtupleDColumn(0, 1, fPosition.getX());
  man->FillNtupleDColumn(0, 2, fPosition.getY());
  man->FillNtupleDColumn(0, 3, fPosition.getZ());
  
  // optical photons
  man->FillNtupleIColumn(0, 4, pNumber);
  
  if (pNumber>0)
  {
    G4cout << "pPosition: " << pPosition << " vector" << G4endl;
    pPosition = pPosition/pNumber; // normalize on the number of optical photons
    pSigma = pSigma/pNumber;
  }
  
  man->FillNtupleDColumn(0, 5, pPosition.getX());
  man->FillNtupleDColumn(0, 6, pSigma.getX());
  man->FillNtupleDColumn(0, 7, pPosition.getY());
  man->FillNtupleDColumn(0, 8, pSigma.getY());

  man->AddNtupleRow(0);
}
