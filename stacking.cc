#include "stacking.hh"

MyStackingAction::MyStackingAction()
{
  G4cout << "ClassifyNewTrack is set on aTrack->GetParentID() == 1" << G4endl;
}

MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  // G4cout << "MyStackingAction::ClassifyNewTrack" << G4endl;
  
  if(aTrack->GetParentID() == 1) // Ag-111 daughter
    {
    
      // particle is electron
      if(aTrack->GetDefinition() == G4Electron::Definition())
        return fKill;
      
      // particle is anti_nu_e
      if(aTrack->GetDefinition() == G4AntiNeutrinoE::Definition())
            return fKill;
    }
  
  if(aTrack->GetParentID() == 2) // excited Cd-111 daughter
    {
      G4cout << "figlia di Cd-111" << G4endl;
      // particle is gamma
      if(aTrack->GetDefinition() == G4Gamma::Definition())
      {
        G4cout << "gamma e figlia di Cd-111" << G4endl;
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->FillNtupleDColumn(0, 0, aTrack->GetKineticEnergy()/keV); // [keV]
      }
    }
    
  return fUrgent;
}
