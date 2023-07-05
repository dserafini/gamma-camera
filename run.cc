#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // want to save energy deposition
  man->CreateNtuple("Scoring", "Scoring");
  man->CreateNtupleDColumn(0, "fEini"); // [eV]
  man->CreateNtupleDColumn(0, "fEdep"); // [eV]
  man->CreateNtupleDColumn(0, "fX"); // [mm]
  man->CreateNtupleDColumn(0, "fY"); // [mm]
  man->CreateNtupleDColumn(0, "fZ"); // [mm]
  // and optical photon positions
  man->CreateNtupleIColumn(0, "pNumber"); // [1]
  man->CreateNtupleDColumn(0, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaR"); // [mm]
  man->FinishNtuple(0);

  // save SiPM pixel information not event by event
  man->CreateNtuple("Sipm", "Sipm");
  man->CreateNtupleDColumn(1, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(1, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(1, "pMostX"); // [mm]
  man->CreateNtupleDColumn(1, "pMostY"); // [mm]
  man->FinishNtuple(1);
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
