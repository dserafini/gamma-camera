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
  fCopyNo = fCopyNoY * fMaxNoY + fCopyNoX;
}

G4int MyCopyNumber::GetCopyNo(G4double valueX, G4double valueY)
{
  SetCopyNoX(valueX);
  MyCopyNumber::SetCopyNoY(valueY);
  MyCopyNumber::UpdateCopyNo();
  
  if (valueX < -fMaxX/2. || valueX > fMaxX/2. || valueY < -fMaxY/2. || valueY > fMaxY/2.)
    return -1;
  else
    return fCopyNo;
}

void MyCopyNumber::SetCopyNoX(G4double valueX)
{
  if (valueX >= -fMaxX/2. && valueX <= fMaxX/2.)
    fCopyNoX = GetCopyNoForSquareSide(valueX, fMaxX, fMaxNoX);
  else
    G4cout << "x point outside volume" << G4endl;
}

void MyCopyNumber::SetCopyNoY(G4double valueY)
{
  if (valueY >= -fMaxY/2. && valueY <= fMaxY/2.)
    fCopyNoY = GetCopyNoForSquareSide(valueY, fMaxY, fMaxNoY);
  else
    G4cout << "y point outside volume" << G4endl;
}
