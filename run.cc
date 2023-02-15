#include "run.hh"

MyRunAction::MyRunAction()
{
  // u can create a single output file here for all the runs

  // u can create the tuple here just one time
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // create tuples
  man->CreateNtuple("Photons", "Photons");
  man->CreateNtupleIColumn(0, "fEvent"); // event number

  // here he saves data that can be later compared with the experiment data
  man->CreateNtupleDColumn(0, "fX"); // x coordinate of the detector [mm]
  man->CreateNtupleDColumn(0, "fY"); // y
  man->CreateNtupleDColumn(0, "fZ"); // z
  man->CreateNtupleDColumn(0, "fWlen"); // wavelength [nm]
  man->CreateNtupleDColumn(0, "fT"); // time information [s]
  man->FinishNtuple(0);
  // per vari motivi poi il tempo su ogni tile non assomiglia troppo ad una gaussiana

  // here he saves data that are only available in the simulation while not in the experiment
  // he calls these type of data Monte Carlo truth
  // for instance the exact position of the photon when it hits the photosensor
  // or the wavelength of the photon
  // so we create a new Ntuple that contains pure Monte Carlo data and not experimental data
  man->CreateNtuple("Hits", "Hits");
  man->CreateNtupleIColumn(1, "fEvent"); // event number
  man->CreateNtupleDColumn(1, "fX"); // x coordinate of the detector [mm]
  man->CreateNtupleDColumn(1, "fY"); // y
  man->CreateNtupleDColumn(1, "fZ"); // z
  man->FinishNtuple(1);

  // in root I can see the circle on xy plane with
  // Hits->Draw("fX:fY", "", "colz")

  // want to save energy deposition
  man->CreateNtuple("Scoring", "Scoring");
  man->CreateNtupleDColumn(2, "fEdep"); // [eV]
  man->FinishNtuple(2);
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

  man->OpenFile("output" + strRunID.str() + ".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->Write();
  // it is very important to always write before closing otherwise the root file could take heavy damage
  man->CloseFile();
}
