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
	fParticleGPS->GeneratePrimaryVertex(anEvent);
}
