#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH 1

#include "globals.hh"
#include "g4root.hh"
#include "G4UserStackingAction.hh"
#include "generator.hh"
#include "G4Track.hh"
#include "G4Electron.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"

class MyStackingAction : public G4UserStackingAction
{
 public:
  MyStackingAction(MyPrimaryGenerator* aGenerator);
  ~MyStackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
  // void NewStage() override;
  // void PrepareNewEvent() override;
};

#endif
