#include "stacking.hh"

MyStackingAction::MyStackingAction()
{
	G4cout << "MyStackingAction::MyStackingAction" << G4endl;
}

MyStackingAction::~MyStackingAction() {}

G4ClassificationOfNewTrack MyStackingAction::ClassifyNewTrack(
	const G4Track* aTrack)
{
	// G4cout << "MyStackingAction::ClassifyNewTrack" << G4endl;
	G4cout << "Particle: " << aTrack->GetParticleDefinition()->GetParticleName() << ", ";
	if (aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition())
	{
		G4cout << G4endl;
		return fUrgent;
	}
	else
	{
		G4cout << "Kinetic energy: " << aTrack->GetKineticEnergy() / keV << " keV" << G4endl;
		
		const G4VProcess *aProcess = aTrack->GetCreatorProcess();
		G4String aProcessName = "none";
		if (aProcess)
			aProcessName = aProcess->GetProcessName();
		
		G4AnalysisManager *man = G4AnalysisManager::Instance();
		
		// particle is beta-
		if(aTrack->GetDefinition() == G4Electron::Definition() && aProcessName == "G4RadioactiveDecayBase")
			return fKill;
		
		// particle is anti_nu_e
		if(aTrack->GetDefinition() == G4AntiNeutrinoE::Definition())
			return fKill;
		
		// particle is gamma, primary or from de-excitation
		if(aTrack->GetDefinition() == G4Gamma::Definition() && (aProcessName == "G4RadioactiveDecayBase" || aProcessName == "none"))
		{
			// G4cout << "energy: " << aTrack->GetKineticEnergy()/keV << " keV" << G4endl;
			man->FillNtupleDColumn(0, 0, aTrack->GetKineticEnergy()/keV); // [keV]
		}
	}
	return fUrgent;
}
