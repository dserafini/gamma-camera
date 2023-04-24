#ifndef COPYNUMBER_HH
#define COPYNUMBER_HH

#include "globals.hh"

class MyCopyNumber
{
public:
	MyCopyNumber();
	~MyCopyNumber();

  void SetMaxX(G4double valueX) { fMaxX = valueX; };
  void SetMaxY(G4double valueY) { fMaxY = valueY; };
  void SetMaxNoX(G4int valueX) { fMaxNoX = valueX; };
  void SetMaxNoY(G4int valueY) { fMaxNoY = valueY; };
  G4int GetCopyNo(G4double, G4double);
  G4int GetCopyNoX() { return fCopyNoX; };
  G4int GetCopyNoY() { return fCopyNoY; };

private:
  void SetCopyNoX(G4double valueX);
  void SetCopyNoY(G4double valueY);
  G4int GetCopyNoForSquareSide(G4double, G4double, G4int);
  void UpdateCopyNo();
  G4int fMaxNoX;
  G4int fMaxNoY;
  G4double fMaxX;
  G4double fMaxY;
  G4int fCopyNoX;
  G4int fCopyNoY;
  G4int fCopyNo;
};

#endif
