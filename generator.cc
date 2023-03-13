#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGPS = new G4GeneralParticleSource();
	fMessenger = new G4GenericMessenger(this, "/source/", "Source definition");
	fMessenger->DeclarePropertyWithUnit("sourceDistance", "mm", sourceDistance, "Distance source-collimator");
	sourceDistance = 30.*mm;
	
	G4UImanager * uiManager = G4UImanager::GetUIpointer();
	uiManager->ApplyCommand("/gps/position 0 0 -5 cm");
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
