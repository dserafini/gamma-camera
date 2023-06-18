#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
  // Decay Phisics
  RegisterPhysics(new G4DecayPhysics());
  // EM 3
  RegisterPhysics(new G4EmStandardPhysics_option3());
}

MyPhysicsList::~MyPhysicsList()
{}
