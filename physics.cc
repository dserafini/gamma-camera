#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	// the aim here is to produce Cerenkov effect
	// so we need two interactions
	// he advises to use only the physics that is required for your application
	// the more physics you implement the longer it takes for calculations
	RegisterPhysics (new G4EmStandardPhysics());
	RegisterPhysics (new G4OpticalPhysics());

	// for any type of particle decay
	RegisterPhysics (new G4DecayPhysics());
	// for decay of radioactive ions
	RegisterPhysics (new G4RadioactiveDecayPhysics());
}

MyPhysicsList::~MyPhysicsList()
{}
