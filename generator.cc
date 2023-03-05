#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGPS = new G4GeneralParticleSource();
	fParticleGPS->SetParticleDefinition(G4Gamma::Definition());
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGPS;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
  // G4cout << "energy: " << fParticleGPS->GetParticleEnergy() << " MeV" << G4endl;
	fParticleGPS->GeneratePrimaryVertex(anEvent);
}
