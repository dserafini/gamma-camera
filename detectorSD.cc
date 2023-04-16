#include "detectorSD.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
  G4cout << "MySensitiveDetector::MySensitiveDetector" << G4endl;
  quEff = new G4PhysicsOrderedFreeVector();

  // read the data files
  std::ifstream datafile;
  datafile.open("eff.dat");

  // put the read data files into the quEff vector
  while(1)
  {
    G4double wlen, queff;

    datafile >> wlen >> queff;

    if(datafile.eof())
      break;

    // make sure everything work fine by printing
    // G4cout << wlen << " " << queff << G4endl;

    quEff->InsertValues(wlen, queff/100.);
  }

  // close data file
  datafile.close();

  // quEff->SetSpline(false);
  // in standard configuration Spline is set to on
  // it means that interpolates distances between data points with splines
  // he said it sometimes creates problem if u use splines
  // he suggests to have measurements with very small distances in wavelength
  // and then using linear interpolation in between the points
  // this will lead to less problems
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *, G4TouchableHistory *)
{

  return true;
}
