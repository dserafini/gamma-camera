#ifndef COPYNUMBER_HH
#define COPYNUMBER_HH

class MyCopyNumber
{
public:
	MyCopyNumber();
	~MyCopyNumber();

  void SetMaxX(G4double valueX) { fMaxX = valueX; };
  void SetMaxY(G4double valueY) { fMaxY = valueY; };
  void SetMaxNoX(G4int valueX) { fMaxNoX = valueX; };
  void SetMaxNoY(G4int valueY) { fMaxNoY = valueY; };
  void SetCopyNoX(G4double);
  void SetCopyNoY(G4double);
  void UpdateCopyNo();
  G4int GetCopyNoX() { return fCopyNoX; };
  G4int GetCopyNoY() { return fCopyNoY; };
  G4int GetCopyNo() { return fCopyNo; };

private:
  G4int fMaxNoX;
  G4int fMaxNoY;
  G4int fCopyNoX;
  G4int fCopyNoY;
  G4int fCopyNo;
  G4double fMaxX;
  G4double fMaxY;
};

#endif
