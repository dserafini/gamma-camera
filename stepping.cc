#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
  fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
  // G4cout << "MySteppingAction::UserSteppingAction" << G4endl;
  // we take the energy of the whole volume
  // or we take the energy of a single scoring volume
  
  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if the volume where the step is in is also our scoring volume
  const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
  // G4cout << "volume: " << volume->GetName() << G4endl;
  // G4cout << "fScoringVolume: " << fScoringVolume->GetName() << G4endl;

  if((volume == fScoringVolume) && (step->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition()))
  {
    G4double edep = step->GetTotalEnergyDeposit() / keV;
    fEventAction->AddEdep(edep);
    // G4cout << "adding " << edep << " keV" << G4endl;

    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    fEventAction->AddPosition(position, edep);
  }
  
  if((volume == detectorConstruction->GetDetectorVolume()) && (step->GetTrack()->GetParticleDefinition() == G4OpticalPhoton::Definition()))
  {
    G4int num = 1;
    fEventAction->AddNum(num);
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    fEventAction->AddPosition(position, num);
    G4cout << "position " << position << " vector" << G4endl;
  }
}
