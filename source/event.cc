#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* run)
{
  idH3 = 0;
  if (run)
    idH3 = run->idH3;
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

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  if (event->GetPrimaryVertex())
  {
    G4ThreeVector v1 = event->GetPrimaryVertex()->GetPosition();
    G4cout << "event: " << v1 << G4endl;
    man->FillH3(idH3, v1.getX(), v1.getY(), v1.getZ());
  }
  else
    G4cout << "no vertex" << G4endl;

  man->AddNtupleRow(0);
}
