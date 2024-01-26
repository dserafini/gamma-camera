#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "g4root.hh" // v10.7
// #include "g4root_defs.hh" // v11.1
// #include "G4AnalysisManager.hh" // v11.1
#include "G4SystemOfUnits.hh"

#include "run.hh"
#include "enums.hh"
#include "detectorSD.hh"
#include "G4SDManager.hh"

class MyEventAction : public G4UserEventAction
{
public:
  MyEventAction(MyRunAction*);
  ~MyEventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

private:
  G4int idH3;
};

#endif
