/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

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
	void SetEventID(G4int id) { eventID = id; };
	void SetPosition(G4ThreeVector p) { pos = p; };
	void SetMomentum(G4ThreeVector momentum) { mom = momentum; };

	G4double GetKineticEnergy() { return ekin; };
	G4double GetEnergyDeposition() { return edep; };
	G4int GetParticleType() { return type; };
	G4int GetDetectorID() { return detectorID; };
	G4int GetEventID() { return eventID; };
	G4ThreeVector GetPosition() { return pos; };
	G4ThreeVector GetMomentum() { return mom; };

  private:
	G4double ekin;
	G4double edep;
	G4int type;
	G4int detectorID;
	G4int eventID;
	G4ThreeVector pos;
	G4ThreeVector mom;
};

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4ThreadLocal G4Allocator<TargetHit> *TargetHitAllocator;

inline void *TargetHit::operator new(size_t) {
	if (!TargetHitAllocator)
		TargetHitAllocator = new G4Allocator<TargetHit>;
	return (void *)TargetHitAllocator->MallocSingle();
}

inline void TargetHit::operator delete(void *hit) {
	TargetHitAllocator->FreeSingle((TargetHit *)hit);
}
