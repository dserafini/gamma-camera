// c++ inclusions
#include <iostream>

// Geant4 inclusions
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

// my inclusions
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "generator.hh"

// looks like:
// argc is the number of parameters
// argv is the array containing these parameters as strings
// dim(argv) = argc
// https://stackoverflow.com/questions/3024197/what-does-int-argc-char-argv-mean
int main(int argc, char** argv)
{
	// for multithreading
	#ifdef G4MULTITHREADED
		G4MTRunManager *runManager = new G4MTRunManager();
	  // Setting up the number of threads
	  G4int nThreads = 1;
	  runManager->SetNumberOfThreads(nThreads);
		std::cout << "multithreaded" << std::endl;
	#else
		// the hearth of Geant is the G4RunManager, it takes care of everything
		G4RunManager *runManager = new G4RunManager();
		std::cout << "singlethreaded" << std::endl;
	#endif
	// u can use the multithreaded version and only one thread
	// but still there are problems if your code is not thread safe
	// so it is better to make the code compatible with both modalities

	// everything that we want to add later we have to add to this runmanager
	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new MyActionInitialization());

	// runManager->Initialize();
	// but you need detector, physics, track and step information
	// runManager initilization here is not good for multithreading
	// we move it to macro files

	// we need for user interface
	G4UIExecutive *ui = 0;

	// the argument is 1 if there is only the name of the executable, sim
	// if there are other parameters in the line command then it is argc > 1
	if(argc == 1)
	{
		ui = new G4UIExecutive(argc, argv);
	}

	// we need visualization manager
	G4VisManager *visManager = new G4VisExecutive();

	// initialize the visualization manager
	visManager->Initialize();

	G4UImanager *UImanager = G4UImanager::GetUIpointer();

	if(ui)
	{
	// tell Geant4 to run vis.mac
	UImanager->ApplyCommand("/control/execute vis.mac");

	// start the session from G4UIExecutive
	ui->SessionStart();
	}
	else
	{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}

	return 0;
}
