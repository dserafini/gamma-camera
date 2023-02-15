#ifndef G4GammaCamera_HIT_H
#define G4GammaCamera_HIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class MyHit : public G4VHit
{
  public:

    // Constructors
    MyHit();
    MyHit(const MyHit&);
    // Destructor
    ~MyHit();

    // Operators
    const MyHit& operator=(const MyHit&);
    G4int operator==(const MyHit&) const;

    inline void * operator new(size_t);
    inline void   operator delete(void*);

    // User Defined Hit Interface

    void SetEdep(G4double edep){ fEdep = edep; };
    G4double GetEdep(){ return fEdep; };

  private:

    G4double fEdep;
};

using MyHitsCollection = G4THitsCollection<MyHit>;
extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;

inline void* MyHit::operator new(size_t)
{
  if(!MyHitAllocator) MyHitAllocator = new G4Allocator<MyHit>;

  void * hit;
  hit = (void*) MyHitAllocator->MallocSingle();

  return hit;
}

inline void MyHit::operator delete(void *hit)
{
  if(!MyHitAllocator) MyHitAllocator = new G4Allocator<MyHit>;

  MyHitAllocator->FreeSingle((MyHit*)hit);

  return ;
}

#endif
