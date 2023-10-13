#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* )
{
  // G4cout << "MyEventAction::BeginOfEventAction" << G4endl;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
  // G4cout << "MyEventAction::EndOfEventAction" << G4endl;
  if (event->GetPrimaryVertex())
  {
    G4ThreeVector v1 = event->GetPrimaryVertex()->GetPosition();
    G4cout << "event: " << v1 << G4endl;
    man->FillH3(0, v1.getX(), v1.getY(), v1.getZ());
  }

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->AddNtupleRow(0);
}
