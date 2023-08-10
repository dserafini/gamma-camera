#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4OpticalPhoton.hh"

#include "g4root.hh"
// #include "g4root_defs.hh"

#include "construction.hh"
#include "event.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
  MySteppingAction(MyEventAction*);
  ~MySteppingAction();

  virtual void UserSteppingAction(const G4Step*);

private:
  MyEventAction *fEventAction;
};

#endif
