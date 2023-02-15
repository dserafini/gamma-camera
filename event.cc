#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // whenever a new event starts the energy accumulated in the previous event should be set to 0
  fEdep = 0.;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  G4cout << "Energy deposition: " << fEdep << " MeV" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->FillNtupleDColumn(2, 0, fEdep);

  man->AddNtupleRow(2);
}
