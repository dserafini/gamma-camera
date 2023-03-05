#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGPS = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGPS;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  // G4cout << "energy: " << fParticleGPS->GetParticleEnergy() << " MeV" << G4endl;
	fParticleGPS->SetParticleDefinition(G4Gamma::Definition());
	fParticleGPS->GeneratePrimaryVertex(anEvent);
}
