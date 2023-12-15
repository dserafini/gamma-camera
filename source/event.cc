#include "event.hh"

int numberDetectedPhotons;

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

  if (numberDetectedPhotons>0)
  {
    G4cout << "numberDetectedPhotons " << numberDetectedPhotons << G4endl;
    man->AddNtupleRow(Tuples::kGeneration);
    man->AddNtupleRow(Tuples::kScintillator);
    man->AddNtupleRow(Tuples::kOptical);
    man->AddNtupleRow(Tuples::kSipm);
  }
}
