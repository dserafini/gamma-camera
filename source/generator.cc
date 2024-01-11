#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	G4cout << "MyPrimaryGenerator::MyPrimaryGenerator" << G4endl;
	fParticleGPS = new G4GeneralParticleSource();
	fParticleGun = new G4ParticleGun();

	// moby
	ROOT::EnableThreadSafety();
        // TFile* rootfile = new TFile("../moby_background_prova_act.root");
	TFile* rootfile = new TFile("../prova_ds_all_act.root");
        //TFile* rootfile = new TFile("../moby_20_lsn_act.root");
	if (rootfile)
        	fHisto = (TH3F*)rootfile->Get("histo");
	else
		G4cout << "No activity root file found!!" << G4endl;

	fMessengerParticleSource = new G4GenericMessenger(this, "/particleSource/", "Particle source generator");
	fMessengerParticleSource->DeclareProperty("source", gunorgps, "gun or gps");
	gunorgps = "gun";
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGPS;
	delete fParticleGun;
	delete fMessengerParticleSource;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4cout << "energy: " << fParticleGPS->GetParticleEnergy() / eV << " eV" << G4endl;
	if (gunorgps == "gun")
	{
		fParticleGun->SetParticlePosition(GenerateParticlePositionMOBY());
		fParticleGun->GeneratePrimaryVertex(anEvent);
		SaveVertexPosition(fParticleGun->GetParticlePosition());
		// G4cout << "myPos: " << fParticleGun->GetParticlePosition() << G4endl;
	}
	else
	{
		if (gunorgps == "gps")
		{
			fParticleGPS->SetParticlePosition(G4ThreeVector(0*mm,15*mm,0.*mm));
			fParticleGPS->GeneratePrimaryVertex(anEvent);
			fParticleGPS->SetParticlePosition(G4ThreeVector(15*mm,15*mm,0.*mm));
			fParticleGPS->SetParticlePosition(G4ThreeVector(15*mm,15*mm,0.*mm));
			fParticleGPS->GeneratePrimaryVertex(anEvent);
			SaveVertexPosition(fParticleGPS->GetParticlePosition());
			G4cout << "inenergy: " << fParticleGPS->GetParticleEnergy() << G4endl;
		}
		else
			G4cout << "Error with the particle generator!!!" << G4endl;
	}
}

G4ThreeVector MyPrimaryGenerator::GenerateParticlePositionMOBY()
{
	G4double x, y, z;
	fHisto->GetRandom3(x,y,z);
	// fHisto x[0,550], y[0,200], z[0,200]
	// halfVoxelSize è in mm
	
	G4double my_x = (x + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsX() * 2*HalfVoxelSize) / 2.0;
	G4double my_y = (y + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsY() * 2*HalfVoxelSize) / 2.0;
	G4double my_z = (z + 0.5) * 2*HalfVoxelSize - (fHisto->GetNbinsZ() * 2*HalfVoxelSize) / 2.0 - HalfPhantomDepth - mouseCollimatorDistance;
	
	G4ThreeVector myPos = G4ThreeVector(my_x, my_y, my_z);
	
	/*G4double my_x = (x-fHisto->GetNbinsX()*0.5+G4UniformRand())*2*HalfVoxelSize;
	G4double my_y = (y-fHisto->GetNbinsY()*0.5+G4UniformRand())*2*HalfVoxelSize;
	G4double my_z = (z-fHisto->GetNbinsZ()*0.5+G4UniformRand())*2*HalfVoxelSize;*/
	
	/*std::cout << "Random index: (" << x << ", " << y << ", " << z << ")\n";
	std::cout << "Random position: (" << my_x << ", " << my_y << ", " << my_z << ")\n\n";*/
	
	return myPos;
}

void MyPrimaryGenerator::SaveVertexPosition(G4ThreeVector vertexPos)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->FillNtupleDColumn(Tuples::kGeneration, TGeneration::kVertexX, vertexPos.getX());
	man->FillNtupleDColumn(Tuples::kGeneration, TGeneration::kVertexY, vertexPos.getY());
	man->FillNtupleDColumn(Tuples::kGeneration, TGeneration::kVertexZ, vertexPos.getZ());
}
