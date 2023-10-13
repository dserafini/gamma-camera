#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // histogram to test moby activity distribution
  idH3 = man->CreateH3("moby_act", "moby_act",
               550, -550*0.18/2., +550*0.18/2.,
               200, -200*0.18/2., +200*0.18/2.,
               200, 0., -200*0.18); // [mm]

  // want to save energy deposition
  man->CreateNtuple("Scoring", "Scoring");

  // initial
  man->CreateNtupleDColumn(0, "fEini"); // [eV]

  // scintillator
  man->CreateNtupleDColumn(0, "fEdep"); // [eV]
  man->CreateNtupleDColumn(0, "fX"); // [mm]
  man->CreateNtupleDColumn(0, "fY"); // [mm]
  man->CreateNtupleDColumn(0, "fZ"); // [mm]
  
  // SiPM detector
  // optical photons
  man->CreateNtupleIColumn(0, "pNumber"); // [1]
  man->CreateNtupleDColumn(0, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaR"); // [mm]
  // pixels of SiPM
  man->CreateNtupleDColumn(0, "xMeanX"); // [mm]
  man->CreateNtupleDColumn(0, "xMeanY"); // [mm]
  man->CreateNtupleDColumn(0, "xMostX"); // [mm]
  man->CreateNtupleDColumn(0, "xMostY"); // [mm]

  // finish tuple
  man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
  // u can create a new output file here for every run
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // create different output files for different runs
  G4int runID = run->GetRunID();

  // convert integer RunID into a string
  std::stringstream strRunID;
  strRunID << runID;

  //man->OpenFile("output" + strRunID.str() + ".root");
  // I prefer to give the file name from macro
  man->OpenFile();
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->Write();
  // it is very important to always write before closing otherwise the root file could take heavy damage
  man->CloseFile();
}
