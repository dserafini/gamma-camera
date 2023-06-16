#include "stacking.hh"

MyStackingAction::MyStackingAction() {}

MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  G4cout << "MyStackingAction::ClassifyNewTrack" << G4endl;
  
  if(aTrack->GetParentID() == 1) // Ag-111 daughter
    {
    
      if(aTrack->GetDefinition() == G4Electron::Definition())
      {  // particle is electron
            return fKill;
      }
    
      if(aTrack->GetDefinition() == G4AntiNeutrinoE::Definition())
      {  // particle is anti_nu_e
            return fKill;
      }
    
    }
  return fUrgent;
}
