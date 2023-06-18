#include "stacking.hh"

MyStackingAction::MyStackingAction()
{
	G4cout << "ClassifyNewTrack is set on aTrack->GetParentID() == 1" << G4endl;
  
	fMessengerPrimary = new G4GenericMessenger(this, "/primary/", "Primary selection for stacking action");
	
	fMessengerPrimary->DeclareProperty("ion", fIonIsPrimary, "0 gamma, 1 ion");

	fIonIsPrimary = 0;
}

MyStackingAction::~MyStackingAction()
{
  delete fMessengerPrimary;
}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
  const G4Track* aTrack)
{
  G4cout << "MyStackingAction::ClassifyNewTrack" << G4endl;
G4cout << "Process " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;

G4AnalysisManager *man = G4AnalysisManager::Instance();

if (fIonIsPrimary == 1)
{
G4cout << "save primary ion gamma daughter energy" << G4endl;
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
      // G4cout << "figlia di Cd-111" << G4endl;
      // particle is gamma
      if(aTrack->GetDefinition() == G4Gamma::Definition())
      {
        // G4cout << "gamma e figlia di Cd-111" << G4endl;
        // G4cout << "energy: " << aTrack->GetKineticEnergy()/keV << " keV" << G4endl;
        man->FillNtupleDColumn(0, 0, aTrack->GetKineticEnergy()/keV); // [keV]
      }
    }
}

if (fIonIsPrimary == 0)
{
	// gammas and primaries
	if(aTrack->GetDefinition() == G4Gamma::Definition() && aTrack->GetParentID() == 0)
	{
		G4cout << "save primary gamma energy" << G4endl;
		man->FillNtupleDColumn(0, 0, aTrack->GetKineticEnergy()/keV);
	}
}
    
  return fUrgent;
}
