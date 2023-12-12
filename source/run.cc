#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // want to save energy deposition
  man->CreateNtuple("Generation", "Generation");
  man->CreateNtuple("Scintillator", "Scintillator");
  man->CreateNtuple("Optical", "Optical");
  man->CreateNtuple("Sipm", "Sipm");

  // moby
  man->CreateNtupleDColumn(Tuples::kGeneration, "fEini"); // [eV]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mZ"); // [mm]

  // scintillator
  man->CreateNtupleDColumn(Tuples::kScintillator, "fEdep"); // [eV]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fZ"); // [mm]
  
  // SiPM detector
  // optical photons
  man->CreateNtupleIColumn(Tuples::kOptical, "pNumber"); // [1]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaR"); // [mm]
  // pixels of SiPM
  man->CreateNtupleDColumn(Tuples::kSipm, "xMeanX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMeanY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMostX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMostY"); // [mm]

  // finish tuple
  man->FinishNtuple(Tuples::kGeneration);
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
