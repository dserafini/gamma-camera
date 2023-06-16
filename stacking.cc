#include "stacking.hh"

MyStackingAction::MyStackingAction() {}

MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  G4cout << "MyStackingAction::ClassifyNewTrack" << G4endl;
  
  if(aTrack->GetParentID() == 1) // Ag-111 daughter
    {
    
      // particle is electron
      if(aTrack->GetDefinition() == G4Electron::Definition())
        return fKill;
      
      // particle is anti_nu_e
      if(aTrack->GetDefinition() == G4AntiNeutrinoE::Definition())
            return fKill;
    
    }
  return fUrgent;
}
