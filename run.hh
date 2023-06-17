#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
// include standalone root functions in Geant4 with g4root.hh
// that is to avoid to include ROOT dependencies
// it includes the class G4AnalysisManager class
#include "g4root.hh" // v10.7
// #include "g4root_defs.hh" // v11.0
// #include "G4AnalysisManager.hh" // v11.0
#include "G4Run.hh"
#include "generator.hh"
#include "stacking.hh"


class MyRunAction : public G4UserRunAction
{
public:
	MyRunAction(MyPrimaryGenerator*, MyStackingAction*);
	~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
	MyGenerator *fGenerator;
	MyStackingAction *fStackingAction;
};

#endif
