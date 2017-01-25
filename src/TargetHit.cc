#include "TargetHit.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TargetHit> *TargetHitAllocator = 0;

TargetHit::TargetHit() : G4VHit() {}

TargetHit::~TargetHit() {}

TargetHit::TargetHit(const TargetHit &right) : G4VHit() {
	edep = right.edep;
	ekin = right.ekin;
	type = right.type;
	detectorID = right.detectorID;
	pos = right.pos;
	mom = right.mom;
}

const TargetHit &TargetHit::operator=(const TargetHit &right) {

	edep = right.edep;
	ekin = right.ekin;
	type = right.type;
	detectorID = right.detectorID;
	pos = right.pos;
	mom = right.mom;

	return *this;
}

G4int TargetHit::operator==(const TargetHit &right) const {
	return (this == &right) ? 1 : 0;
}

void TargetHit::Draw() {}

void TargetHit::Print() {}
