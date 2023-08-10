#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
  // Decay Phisics
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  // EM 3
  RegisterPhysics(new G4EmStandardPhysics_option3());
  //
  G4OpticalPhysics * opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics(opticalPhysics);
}

MyPhysicsList::~MyPhysicsList()
{}
