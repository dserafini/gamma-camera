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

  man->AddNtupleRow(0);
}
