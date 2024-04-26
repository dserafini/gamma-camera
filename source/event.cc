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

void MyEventAction::EndOfEventAction(const G4Event* currentEvent)
{
  G4cout << "MyEventAction::EndOfEventAction" << G4endl;
  
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  MySensitiveDetector *sdet = (MySensitiveDetector*)fSDM->FindSensitiveDetector("SensitiveDetector");
  
  G4cout << "Should I save the event? ";
  if (sdet->ShouldISaveEvent())
  {
    G4cout << "yes" << G4endl;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow(Tuples::kGeneration);
    man->AddNtupleRow(Tuples::kScintillator);
    man->AddNtupleRow(Tuples::kOptical);
    man->AddNtupleRow(Tuples::kSipm);
  }
  else
    G4cout << "no" << G4endl;
}
