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

  G4HCofThisEvent *hce = anEvent->GetHCofThisEvent();
  G4VHitsCollection *hc0;
  if (hce)
    hc0 = hce->GetHC(0);
    if (!hc0) 
    {
      G4cout << "No hit collection for this event!!!" << G4endl;
      return;
    }
  else
  {
    G4cout << "No hit collection for this event!!!" << G4endl;
    return;
  }
  
  if (hc0->entries() > 0)
  {
    // G4cout << "numberDetectedPhotons " << numberDetectedPhotons << G4endl;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->AddNtupleRow(Tuples::kGeneration);
    man->AddNtupleRow(Tuples::kScintillator);
    man->AddNtupleRow(Tuples::kOptical);
    man->AddNtupleRow(Tuples::kSipm);

    G4cout << "hc0->entries " << hc0->entries << ", numberDetectedPhotons " << numberDetectedPhotons << G4endl;
  }
}
