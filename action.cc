#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
}

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);

	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
	// we are telling Geant4 that we do not use any default run action
	// instead we want to use our own run action
	// same for all the other user defined actions

	MyEventAction *eventAction = new MyEventAction(runAction);
	SetUserAction(eventAction);

	// MySteppingAction *steppingAction = new MySteppingAction(eventAction);
	// SetUserAction(steppingAction);

	MyStackingAction *stackingAction = new MyStackingAction();
	SetUserAction(stackingAction);
}
