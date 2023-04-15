#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "g4root.hh" // v10.7
// #include "g4root_defs.hh" // v11.1
// #include "G4AnalysisManager.hh" // v11.1

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
public:
  MyEventAction(MyRunAction*);
  ~MyEventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  // accumulate energy deposited
  void AddEdep(G4double edep) { fEdep += edep; }
  void AddPosition(G4ThreeVector position, G4double edep) { fPosition += position*edep; }

private:
  // for gammas
  G4double fEdep;
  G4ThreeVector fPosition;
  
  // for optical photons
  G4int pNumber;
  G4ThreeVector pPosition;
  G4ThreeVector pSigma;
};

#endif
