#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // want to save energy deposition
  man->CreateNtuple("Events", "Events");
  man->CreateNtuple("Generation", "Generation");
  man->CreateNtuple("Scintillator", "Scintillator");
  man->CreateNtuple("Optical", "Optical");
  man->CreateNtuple("Sipm", "Sipm");
  man->CreateNtuple("AllOptical", "AllOptical");
  man->CreateNtuple("Channels", "Channels");

  // number of generated events
  man->CreateNtupleIColumn(Tuples::kEvents, "gEvents"); // [1]

  // moby
  man->CreateNtupleDColumn(Tuples::kGeneration, "fEini"); // [eV]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kGeneration, "mZ"); // [mm]
  man->CreateNtupleIColumn(Tuples::kGeneration, "eventID"); // [1]

  // scintillator
  man->CreateNtupleDColumn(Tuples::kScintillator, "fEdep"); // [eV]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kScintillator, "fZ"); // [mm]
  
  // scintillation optical photons
  man->CreateNtupleIColumn(Tuples::kOptical, "pNumber"); // [1]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pMeanZ"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kOptical, "pSigmaR"); // [mm]
  
  // SiPM detector
  // pixels of SiPM
  man->CreateNtupleIColumn(Tuples::kSipm, "xNumber"); // [1]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMeanX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMeanY"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMostX"); // [mm]
  man->CreateNtupleDColumn(Tuples::kSipm, "xMostY"); // [mm]

  // all optical photons info, not average
  man->CreateNtupleIColumn(Tuples::kAllOptical, "eventID"); // [1]
  man->CreateNtupleDColumn(Tuples::kAllOptical, "xSipm"); // [mm]
  man->CreateNtupleDColumn(Tuples::kAllOptical, "ySipm"); // [mm]
  
  // real pixels of SiPM
  man->CreateNtupleIColumn(Tuples::kChannels, "eventID"); // [1]
  man->CreateNtupleIColumn(Tuples::kChannels, "dNumber"); // [1]
  man->CreateNtupleIColumn(Tuples::kChannels, "dIndexX"); // [1]
  man->CreateNtupleIColumn(Tuples::kChannels, "dIndexY"); // [1]

  // deactivate all optical ntuple by default
  man->SetActivation(true);
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/analysis/ntuple/setActivation " + std::to_string(Tuples::kAllOptical) + " False");
  UImanager->ApplyCommand("/analysis/ntuple/setActivation " + std::to_string(Tuples::kOptical) + " False");
  UImanager->ApplyCommand("/analysis/ntuple/setActivation " + std::to_string(Tuples::kSipm) + " False");

  // finish tuple
  man->FinishNtuple(Tuples::kEvents);
  man->FinishNtuple(Tuples::kGeneration);
  man->FinishNtuple(Tuples::kScintillator);
  man->FinishNtuple(Tuples::kOptical);
  man->FinishNtuple(Tuples::kSipm);
  man->FinishNtuple(Tuples::kAllOptical);
  man->FinishNtuple(Tuples::kChannels);
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

  // retrieve number of events in the run
  man->FillNtupleIColumn(Tuples::kEvents, TEvents::kEvents, run->GetNumberOfEventToBeProcessed());
  man->AddNtupleRow(Tuples::kEvents);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
  G4cout << "MyRunAction::EndOfRunAction" << G4endl;
  
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->Write();
  // it is very important to always write before closing otherwise the root file could take heavy damage
  man->CloseFile();
}
