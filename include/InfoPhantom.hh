#ifndef INFOPHANTOM_HH
#define INFOPHANTOM_HH

const G4int nGroups = 4;
extern G4ThreeVector groupOrigin[nGroups];
extern G4double groupRadius[nGroups];
extern G4double PocketSection[nGroups];
extern G4double groupArea[nGroups];
extern G4double cumulativeGroupArea[nGroups];
extern G4double totalArea;
extern G4double PocketDepth;

// Variables needed for the stepping action
extern G4double HalfPhantomDepth; 
extern G4double HalfFilmDepth; 
extern G4double HalfSupportDepth; 
extern G4double HalfScintillatorDepth;
extern G4double GapScint; 

// Variables needed for run action
extern G4double HalfPhantomSide;
extern G4double HalfDetectorSide;

// Variables needed for Pd simulation
const G4int nPd = 3;
extern G4double HalfSide[nPd];
extern G4double HalfPdDepth;
extern G4double HalfPdWidth;
extern G4ThreeVector position[nPd];

extern G4double HalfVoxelSize;



#endif
