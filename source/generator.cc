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
	
	// fParticleGPS->SetParticlePosition(GenerateParticlePositionMOBY());
	// fParticleGPS->SetParticlePosition(G4ThreeVector(1,1,1));

	// G4cout << "energy: " << fParticleGPS->GetParticleEnergy() << G4endl;
	G4cout << "myPos: " << fParticleGPS->GetParticlePosition() << G4endl;
	
	fParticleGPS->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector MyPrimaryGenerator::GenerateParticlePositionMOBY()
{
	G4double x, y, z;
	fHisto->GetRandom3(x,y,z);
	
	G4double my_x = (x + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsX() * 2*HalfVoxelSize) / 2.0;
	G4double my_y = (y + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsY() * 2*HalfVoxelSize) / 2.0;
	G4double my_z = (z + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsZ() * 2*HalfVoxelSize) / 2.0 + HalfPhantomDepth;
	
	G4ThreeVector myPos = G4ThreeVector(my_x, my_y, my_z);
	
	/*G4double my_x = (x-fHisto->GetNbinsX()*0.5+G4UniformRand())*2*HalfVoxelSize;
	G4double my_y = (y-fHisto->GetNbinsY()*0.5+G4UniformRand())*2*HalfVoxelSize;
	G4double my_z = (z-fHisto->GetNbinsZ()*0.5+G4UniformRand())*2*HalfVoxelSize;*/
	
	/*std::cout << "Random index: (" << x << ", " << y << ", " << z << ")\n";
	std::cout << "Random position: (" << my_x << ", " << my_y << ", " << my_z << ")\n\n";*/
	
	return myPos;
}
