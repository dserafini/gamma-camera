#ifndef SCINTILLATORHIT_HH
#define SCINTILLATORHIT_HH 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "tls.hh"
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class scintillatorHit : public G4VHit
{
  public:
    scintillatorHit() = default;
    scintillatorHit(const scintillatorHit&) = default;
    ~scintillatorHit() override = default;

    // operators
    scintillatorHit& operator=(const scintillatorHit&) = default;
    G4bool operator==(const scintillatorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Print() override;
    void Draw() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using scintillatorHitsCollection = G4THitsCollection<scintillatorHit>;

extern G4ThreadLocal G4Allocator<scintillatorHit>* scintillatorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* scintillatorHit::operator new(size_t)
{
  if(!scintillatorHitAllocator)
      scintillatorHitAllocator = new G4Allocator<scintillatorHit>;
  return (void *) scintillatorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void scintillatorHit::operator delete(void *hit)
{
  scintillatorHitAllocator->FreeSingle((scintillatorHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
