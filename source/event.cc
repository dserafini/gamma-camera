#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
  idH3 = 0;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* )
{
  // G4cout << "MyEventAction::BeginOfEventAction" << G4endl;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
  G4cout << "MyEventAction::EndOfEventAction" << G4endl;

  man->AddNtupleRow(Tuples::kGeneration);
  man->AddNtupleRow(Tuples::kScintillator);
  man->AddNtupleRow(Tuples::kOptical);
  man->AddNtupleRow(Tuples::kSipm);
}
