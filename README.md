# gamma-camera

Geant4 simulation of a gamma camera.

## Short description

The aim of this simulation is to optimize the main parameters of a gamma camera in order to perform imaging with Ag-111. This simulation is built in the framework of the [ISOLPHARM project](https://isolpharm.pd.infn.it/web/).

## References

This Monte Carlo simulation is written with [Geant4](https://geant4.web.cern.ch/). The starting point is the project G4GammaCamera made by Luca Morselli. An important reference is the [playlist of tutorials realized by Mustafa](https://www.youtube.com/watch?v=Lxb4WZyKeCE&list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL). One may have a look to the [Application Developer User Manual](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/index.html). Among the various macros, there is one called [tutorial.mac](https://github.com/dserafini/gamma-camera/blob/moby/macro/tutorial.mac) that is highly commented and is intended for users without previous Geant4 experience.

## Usage

In the following we will assume that Geant4 v10.7 is installed. Otherwise it can be installed from the [download page](https://geant4.web.cern.ch/download/10.7.4.html) of the official website following the [installation instructions](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html).

First step is cloning this repository locally. One possibility is to [install git](https://github.com/git-guides/install-git) and use the following command.
````bash
git clone https://github.com/dserafini/gamma-camera
````

Second step is creating a build folder inside the project directory and compile the source code.
````bash
cmake ..
make
````

Finally the executable can be executed opening the GUI
````bash
./sim
````
or with a macro file, for instance:
````bash
./sim macro/moby.mac
````
