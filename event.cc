#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // whenever a new event starts the energy accumulated in the previous event should be set to 0
  fEdep = 0.;
  fPosition = G4ThreeVector(0.,0.,0.);
  savedPosition = false;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  // G4cout << "Energy deposition: " << fEdep << " keV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->FillNtupleDColumn(0, 0, fEdep);
  man->FillNtupleDColumn(0, 1, fPosition.getX());
  man->FillNtupleDColumn(0, 2, fPosition.getY());
  man->FillNtupleDColumn(0, 3, fPosition.getZ());

  man->AddNtupleRow(0);
}
