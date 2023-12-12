#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // want to save energy deposition
  man->CreateNtuple("Scoring", "Scoring");
  man->CreateNtuple("Scintillator", "Scintillator");
  man->CreateNtuple("Optical", "Optical");

  // initial
  man->CreateNtupleDColumn(0, "fEini"); // [eV]

  // scintillator
  man->CreateNtupleDColumn(0, "fEdep"); // [eV]
  man->CreateNtupleDColumn(0, "fX"); // [mm]
  man->CreateNtupleDColumn(0, "fY"); // [mm]
  man->CreateNtupleDColumn(0, "fZ"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fEdep"); // [eV]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fZ"); // [mm]
  
  // SiPM detector
  // optical photons
  man->CreateNtupleIColumn(0, "pNumber"); // [1]
  man->CreateNtupleDColumn(0, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(0, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(0, "pSigmaR"); // [mm]
  man->CreateNtupleIColumn(Tuples::kOptical, "pNumber"); // [1]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaR"); // [mm]
  // pixels of SiPM
  man->CreateNtupleDColumn(Tsipm::kMeanX, "xMeanX"); // [mm]
  man->CreateNtupleDColumn(Tsipm::kMeanY, "xMeanY"); // [mm]
  man->CreateNtupleDColumn(Tsipm::kMostX, "xMostX"); // [mm]
  man->CreateNtupleDColumn(Tsipm::kMostY, "xMostY"); // [mm]

  // moby
  man->CreateNtupleDColumn(TGeneration::kVertexX, "mX"); // [mm]
  man->CreateNtupleDColumn(TGeneration::kVertexY, "mY"); // [mm]
  man->CreateNtupleDColumn(TGeneration::kVertexZ, "mZ"); // [mm]

  // finish tuple
  man->FinishNtuple(0);
  man->FinishNtuple(Tuples::kScintillator);
  man->FinishNtuple(Tuples::kOptical);
  man->FinishNtuple(Tuples::kSipm);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "MyRunAction::BeginOfRunAction" << G4endl;
  
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
  G4cout << "MyRunAction::EndOfRunAction" << G4endl;
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->Write();
  // it is very important to always write before closing otherwise the root file could take heavy damage
  man->CloseFile();
}
