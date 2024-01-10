#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "infoPhantom.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"
#include "G4GenericMessenger.hh"
#include "g4root.hh"
#include "TFile.h"
#include "TH3F.h"
#include "TROOT.h"
#include "enums.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();

	virtual void GeneratePrimaries(G4Event*);

private:
	void SaveVertexPosition(G4ThreeVector);
	G4ThreeVector GenerateParticlePositionMOBY();
	G4GeneralParticleSource* fParticleGPS;
	G4ParticleGun* fParticleGun;
	G4GenericMessenger *fMessengerParticleSource;
	G4String sourceType;
	TH3F* fHisto;
};

#endif
