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
	// G4cout << "Particle: " << aTrack->GetParticleDefinition()->GetParticleName() << ", ";
	if (aTrack->GetParticleDefinition() == G4OpticalPhoton::Definition())
	{
		const G4VProcess *aProcess = aTrack->GetCreatorProcess();
		G4String aProcessName = "none";
		if (aProcess)
			aProcessName = aProcess->GetProcessName();
		G4double enePhoton = aTrack->GetKineticEnergy();
  		G4double wlen = (1.239841939*eV/enePhoton)*1e3;
		G4cout << wlen;
		// G4cout << ",\t" << aProcessName;
		G4cout << G4endl;
		return fUrgent;
	}
	else
	{
		// G4cout << "Kinetic energy: " << aTrack->GetKineticEnergy() / keV << " keV" << ", ";
		
		const G4VProcess *aProcess = aTrack->GetCreatorProcess();
		G4String aProcessName = "none";
		if (aProcess)
			aProcessName = aProcess->GetProcessName();
		// G4cout << "from: " << aProcessName << G4endl;
		
		//G4cout << aTrack->GetDefinition()->GetParticleName() << " of " << aTrack->GetKineticEnergy()/keV << " keV from " << aProcessName << G4endl;
		G4AnalysisManager *man = G4AnalysisManager::Instance();
		
		// particle is beta-
		if(aTrack->GetDefinition() == G4Electron::Definition() && aProcessName == "Radioactivation")
			return fKill;
		
		// particle is anti_nu_e
		if(aTrack->GetDefinition() == G4AntiNeutrinoE::Definition())
			return fKill;
		
		// particle is gamma, primary or from de-excitation
		if(aTrack->GetDefinition() == G4Gamma::Definition() && (aProcessName == "RadioactiveDecayBase" || aProcessName == "none"))
		{
			// if ( aTrack->GetKineticEnergy() != 245*keV)
			// {
			// 	G4cout << "Kinetic energy: " << aTrack->GetKineticEnergy() / keV << " keV" << ", ";
			// 	G4cout << "parent: " << aTrack->GetParentID() << ", ";
			// 	G4cout << "vertex: " << aTrack->GetVertexPosition() << ", ";
			// 	G4cout << "from: " << aProcessName << G4endl;
			// }
		}
	}
	return fUrgent;
}
