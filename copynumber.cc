#include "copynumber.hh"

MyCopyNumber::MyCopyNumber()
{
  fMaxNoX = 0;
  fMaxNoY = 0;
  fMaxX = 0.;
  fMaxY = 0.;
  fCopyNoX = 0;
  fCopyNoY = 0;
  fCopyNo = 0;
}

MyCopyNumber::~MyCopyNumber()
{}

G4int MyCopyNumber::GetCopyNoForSquareSide(G4double value, G4double maxValue, G4int maxNo)
{
  G4int copyNo = G4int((value + maxValue/2.) / maxValue * maxNo);
  return copyNo;
}

void MyCopyNumber::UpdateCopyNo()
{
  fCopyNo = fCopyNoY * fMaxY + fCopyNoX;
}

G4int MyCopyNumber::GetCopyNo(G4double valueX, G4double valueY)
{
  MyCopyNumber::SetCopyNoX(G4double valueX);
  MyCopyNumber::SetCopyNoY(G4double valueY);
  MyCopyNumber::UpdateCopyNo();
  return fCopyNo;
}
