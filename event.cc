#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
  // G4cout << "MyEventAction::BeginOfEventAction" << G4endl;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  // G4cout << "MyEventAction::EndOfEventAction" << G4endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->AddNtupleRow(0);
  man->AddNtupleRow(1);
}
