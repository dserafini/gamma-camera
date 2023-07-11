#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	G4cout << "MyPrimaryGenerator::MyPrimaryGenerator" << G4endl;
	fParticleGPS = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGPS;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	// G4cout << "energy: " << fParticleGPS->GetParticleEnergy() / eV << " eV" << G4endl;
	fParticleGPS->GeneratePrimaryVertex(anEvent);
}
