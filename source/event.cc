#include "event.hh"

int numberDetectedPhotons;
double energyDeposited;

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

void MyEventAction::EndOfEventAction(const G4Event* anEvent)
{
  // G4cout << "MyEventAction::EndOfEventAction" << G4endl;

  G4HCofThisEvent *hcSipm = anEvent->GetHCofThisEvent();
  
  if (numberDetectedPhotons>0 || energyDeposited>0.)
  {
    // G4cout << "numberDetectedPhotons " << numberDetectedPhotons << G4endl;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow(Tuples::kGeneration);
    man->AddNtupleRow(Tuples::kScintillator);
    man->AddNtupleRow(Tuples::kOptical);
    man->AddNtupleRow(Tuples::kSipm);

    G4cout << "numberDetectedPhotons " << numberDetectedPhotons << ", energyDeposited " << energyDeposited << G4endl;
  }
}
