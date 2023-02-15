#include "hit.hh"

G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator = 0;

MyHit::MyHit() : G4VHit()
{
  fEdep = 0.0;
}

MyHit::~MyHit(){}

MyHit::MyHit(const MyHit& right) : G4VHit()
{
	fEdep = right.fEdep;
}

const MyHit& MyHit::operator=(const MyHit& right)
{
	fEdep = right.fEdep;
	return *this;
}

G4int MyHit::operator==(const MyHit& right) const
{
	return (this == &right ) ? 1 : 0;
}
