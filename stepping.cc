#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
{
  fEventAction = eventAction;
  copyObject = new MyCopyNumber();
  enterCopyNo = 0;
  exitCopyNo = 0;
}

MySteppingAction::~MySteppingAction()
{
  delete copyObject;
}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
  // G4cout << "MySteppingAction::UserSteppingAction" << G4endl;
  // we take the energy of the whole volume
  // or we take the energy of a single scoring volume
  
  // G4cout << "physvol:\t" << touch->GetVolume()->GetName() << G4endl;
  // G4cout << "copy: " << touch->GetCopyNumber() << G4endl;
  // G4cout << "copy0: " << touch->GetCopyNumber(0) << G4endl;
  // G4VPhysicalVolume *physvolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  // G4cout << "logicvol:\t" << volume->GetName() << G4endl;
  
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  // G4cout << "vec: " << pos << G4endl;
  


  
 /* if(step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition())
  {
    G4cout << "collimator is mother? " << volume->IsDaughter(detectorConstruction->GetCollimatorPhysVolume()) << G4endl;
  G4cout << "holesnumber: " << detectorConstruction->GetHolesSideNumber() <<
    ",\tx: " << step->GetPreStepPoint()->GetPosition().getX() <<
    ",\ty: " << step->GetPreStepPoint()->GetPosition().getY() << G4endl;
    G4cout << "copy number: " << copyno << 
    ",\tx: " << copyObject->GetCopyNoX() << 
    ",\ty: " << copyObject->GetCopyNoY() << 
    ",\txy: " << copyObject->GetCopyNo(pos.getX(), pos.getY()) << G4endl;
  }*/
  
  if (step->GetTrack()->GetParticleDefinition() == G4Gamma::Definition())
  {
    // check if the volume where the step is in is also our scoring volume
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4LogicalVolume *logicPinhole = detectorConstruction->GetPinholeVolume();
    G4LogicalVolume *logicPixel = detectorConstruction->GetPixelVolume();
    G4LogicalVolume *logicScintillator = detectorConstruction->GetScoringVolume();
    
    copyObject->SetMaxX(((G4Box*)detectorConstruction->GetCollimatorVolume()->GetSolid())->GetXHalfLength()*2.);
    copyObject->SetMaxY(((G4Box*)detectorConstruction->GetCollimatorVolume()->GetSolid())->GetYHalfLength()*2.);
    copyObject->SetMaxNoX(detectorConstruction->GetHolesSideNumber());
    copyObject->SetMaxNoY(detectorConstruction->GetHolesSideNumber());
    G4int copyno = copyObject->GetCopyNo(step->GetPreStepPoint()->GetPosition().getX(), step->GetPreStepPoint()->GetPosition().getY());
    
    // G4cout << "a gamma" << G4endl;
    if ((volume == logicPixel || volume == logicPinhole) && 
       (pos.getZ() == 0.*mm))
    {
      // G4cout << "enter" << G4endl;
      enterCopyNo = copyno;
      fEventAction->SetCopyNumber(copyno);
    }

    if ((volume == logicScintillator) &&
       (pos.getZ() == 30.*mm))
    {
        exitCopyNo = copyno;
        G4cout << "start: " << enterCopyNo;
        G4cout << ",\t stop: " << exitCopyNo << G4endl;
        if(enterCopyNo != exitCopyNo)
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
