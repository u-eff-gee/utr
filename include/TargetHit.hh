#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

class TargetHit : public G4VHit {
  public:
	TargetHit();
	TargetHit(const TargetHit &);
	virtual ~TargetHit();

	const TargetHit &operator=(const TargetHit &);
	G4int operator==(const TargetHit &) const;

	inline void *operator new(size_t);
	inline void operator delete(void *);

	virtual void Draw();
	virtual void Print();

	void SetKineticEnergy(G4double ek) { ekin = ek; };
	void SetEnergyDeposition(G4double ed) { edep = ed; };
	void SetParticleType(G4int t) { type = t; };
	void SetDetectorID(G4int id) { detectorID = id; };
	void SetPosition(G4ThreeVector p) { pos = p; };
	void SetMomentum(G4ThreeVector momentum) { mom = momentum; };

	G4double GetKineticEnergy() { return ekin; };
	G4double GetEnergyDeposition() { return edep; };
	G4int GetParticleType() { return type; };
	G4int GetDetectorID() { return detectorID; };
	G4ThreeVector GetPosition() { return pos; };
	G4ThreeVector GetMomentum() { return mom; };

  private:
	G4double ekin;
	G4double edep;
	G4int type;
	G4int detectorID;
	G4ThreeVector pos;
	G4ThreeVector mom;
};

using TargetHitsCollection = G4THitsCollection<TargetHit>;

extern G4ThreadLocal G4Allocator<TargetHit> *TargetHitAllocator;

inline void *TargetHit::operator new(size_t) {
	if (!TargetHitAllocator)
		TargetHitAllocator = new G4Allocator<TargetHit>;
	return (void *)TargetHitAllocator->MallocSingle();
}

inline void TargetHit::operator delete(void *hit) {
	TargetHitAllocator->FreeSingle((TargetHit *)hit);
}

#endif
