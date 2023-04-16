#ifndef detectorHit_h
#define detectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class detectorHit : public G4VHit
{
  public:
    detectorHit() = default;
    detectorHit(const detectorHit&) = default;
    ~detectorHit() override = default;

    // operators
    detectorHit& operator=(const detectorHit&) = default;
    G4bool operator==(const detectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Print() override;

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPosMean      (G4ThreeVector xyz){ fPosMean = xyz; };
    void SetPosSigma      (G4ThreeVector xyz){ fPosSigma = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPosMean() const { return fPosMean; };
    G4ThreeVector GetPosSigma() const { return fPosSigma; };

  private:
    G4int         fTrackID = -1;
    G4double      fEdep = 0.;
    G4ThreeVector fPosMean;
    G4ThreeVector fPosSigma;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using detectorHitsCollection = G4THitsCollection<detectorHit>;

extern G4ThreadLocal G4Allocator<detectorHit>* detectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* detectorHit::operator new(size_t)
{
  if(!detectorHitAllocator)
      detectorHitAllocator = new G4Allocator<detectorHit>;
  return (void *) detectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void detectorHit::operator delete(void *hit)
{
  detectorHitAllocator->FreeSingle((detectorHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
