#ifndef STACKINGACTION_HH
#define STACKINGACTION_HH 1

#include "globals.hh"
#include "g4root.hh"
#include "G4UserStackingAction.hh"
#include "G4Track.hh"
#include "G4Electron.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "enums.hh"

class MyStackingAction : public G4UserStackingAction
{
 public:
  MyStackingAction();
  ~MyStackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
  // void NewStage() override;
  // void PrepareNewEvent() override;
};

#endif
