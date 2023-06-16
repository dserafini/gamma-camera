#include "stacking.hh"

MyStackingAction::MyStackingAction() {}

MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  if(aTrack->GetDefinition() == G4Electron::Definition())
  {  // particle is electron
    if(aTrack->GetParentID() == 0)
    {  // beta is Ag-111 daughter
      return fKill;
    }
  }
  return fUrgent;
}
