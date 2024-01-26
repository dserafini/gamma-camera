#ifndef channelHit_h
#define channelHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class channelHit : public G4VHit
{
  public:
    channelHit() = default;
    channelHit(const channelHit&) = default;
    ~channelHit() override = default;

    // operators
    channelHit& operator=(const channelHit&) = default;
    G4bool operator==(const channelHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Print() override;
    void Draw() override;

    // Set methods
    void SetNopti     (G4int de)      { fNopti = de; };
    void SetPixelPos      (G4ThreeVector xyz){ fPixelPos = xyz; };

    // Get methods
    G4double GetNopti() const     { return fNopti; };
    G4ThreeVector GetPixelPos() const { return fPixelPos; };

  private:
    G4int      fNopti = 0;
    G4ThreeVector fPixelPos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using channelHitCollection = G4THitsCollection<channelHit>;

extern G4ThreadLocal G4Allocator<channelHit>* channelHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* channelHit::operator new(size_t)
{
  if(!channelHitAllocator)
      channelHitAllocator = new G4Allocator<channelHit>;
  return (void *) channelHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void channelHit::operator delete(void *hit)
{
  channelHitAllocator->FreeSingle((channelHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
