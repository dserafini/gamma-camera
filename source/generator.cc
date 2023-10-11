#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	G4cout << "MyPrimaryGenerator::MyPrimaryGenerator" << G4endl;
	fParticleGPS = new G4GeneralParticleSource();

	// moby
	ROOT::EnableThreadSafety();
        TFile* rootfile = new TFile("../moby_20_act.root");
        //TFile* rootfile = new TFile("../moby_20_lsn_act.root");
        fHisto = (TH3F*)rootfile->Get("histo");
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
