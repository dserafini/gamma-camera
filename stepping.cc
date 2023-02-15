#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
  fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
  // we take the energy of the whole volume
  // or we take the energy of a single scoring volume

  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if the volume where the step is in is also our scoring volume
  const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

  if(volume != fScoringVolume)
    return; // not saving energy

  G4double edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edep);
}
