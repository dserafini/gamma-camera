#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	// he wants 1 primary vertex per event
	fParticleGun = new G4ParticleGun(1);

	// put the parameters of the gun here so that they can be overrun by macro files

	// what kind of particles we want to create?
	// the properties of the particle we want is store in the following table
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

	// for instance we want a proton
	// geantino is a place holder
	// we can replace it later in the macro files
	G4String particleName = "geantino";
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

	// where the particle should be created
	G4ThreeVector pos(0.,0.,0.);

	// same for momentum, that is not momentum itself but rather direction
	G4ThreeVector mom(0.,0.,1.);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(0.*GeV);
	fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	// if u put the parameters here then every primary generation they overrun the macro instructions

	G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

	// if we do not redefine the particle in the macro command then we define it here
	if(particle == G4Geantino::Geantino())
	{
		G4int Z = 27;
		G4int A = 60;
		// Co-60

		G4double charge = 0.*eplus;
		G4double energy = 0.*keV;

		G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);

		fParticleGun->SetParticleDefinition(ion);
		fParticleGun->SetParticleCharge(charge);
	}

	// tell Geant4 to generate primary vertex and hand over the variable anEvent
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
