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
  
  const G4ReferenceCountedHandle<G4VTouchable> touch = step->GetPreStepPoint()->GetTouchableHandle();
  G4cout << "copyno: " << touch->GetCopyNumber(0) << G4endl;
  G4VPhysicalVolume *physvolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if the volume where the step is in is also our scoring volume
  const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  
  if(step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition())
  {
    G4cout << "collimator is mother? " << volume->IsDaughter(detectorConstruction->GetCollimatorPhysVolume()) << G4endl;
    G4cout << "copy number: " << physvolume->GetCopyNo() << G4endl;
  }
  
  if((volume->IsDaughter(detectorConstruction->GetCollimatorPhysVolume())) && 
     (step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition()) && 
     (fEventAction->GetCross() < 1))
  {
    if(fEventAction->GetCopyNumber() == -1)
      fEventAction->SetCopyNumber(physvolume->GetCopyNo());
    else
    {
      if(fEventAction->GetCopyNumber() != physvolume->GetCopyNo())
        fEventAction->SetCross(1);
    }
  }

  G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
  // G4cout << "volume: " << volume->GetName() << G4endl;
  // G4cout << "fScoringVolume: " << fScoringVolume->GetName() << G4endl;

  if((volume == fScoringVolume) && (step->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition()))
  {
    G4double edep = step->GetTotalEnergyDeposit() / keV;
    fEventAction->AddEdep(edep);
    // G4cout << "adding " << edep << " keV" << G4endl;

    G4ThreeVector preposition = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector postposition = step->GetPostStepPoint()->GetPosition();
    fEventAction->AddPrePosition(preposition, edep);
    fEventAction->AddPostPosition(postposition, edep);
    
    return;
  }
  
  if((volume == detectorConstruction->GetDetectorVolume()) && (step->GetTrack()->GetParticleDefinition() == G4OpticalPhoton::Definition()))
  {
    G4int num = 1;
    fEventAction->AddNum(num);
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
    fEventAction->AddPhotonPosition(position, num);
    // G4cout << "step position " << position << " vector" << G4endl;
    
    return;
  }
}
