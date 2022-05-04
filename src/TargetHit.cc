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

#include "TargetHit.hh"

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
