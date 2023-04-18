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
  void AddPrePosition(G4ThreeVector position, G4double edep) { fPrePosition += position*edep; }
  void AddPostPosition(G4ThreeVector position, G4double edep) { fPostPosition += position*edep; }
  G4int GetCross() { return fCross; }
  void SetCross(G4int cross) { fCross = cross; }
  void SetCopyNumber(G4int copy) { fCopyNumber = copy; }
  G4int GetCopyNumber() { return fCopyNumber; }
  // accumulate number of optical photons
  void AddNum(G4int num) { pNum += num; }
  void AddPhotonPosition(G4ThreeVector position, G4int num) { pPosition += position*num; }

private:
  // for gammas
  G4double fEdep;
  G4ThreeVector fPrePosition;
  G4ThreeVector fPostPosition;
  G4int fCross;
  G4int fCopyNumber;
  
  // for optical photons
  G4int pNum;
  G4ThreeVector pPosition;
  G4ThreeVector pSigma;
};

#endif
