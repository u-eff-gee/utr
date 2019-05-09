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

/*
 * Setup 10 for G3 was used between run 409 and 489.
 * In between, for runs 425 to 464, the 2nd setup was
 * modified to Setup 11, but the G3 setup was not changed.
 */

#ifndef DETECTORS_G3_SETUP_10_HH
#define DETECTORS_G3_SETUP_10_HH 1

#include "G4LogicalVolume.hh"

class Detectors_G3_Setup_10{
public:
	Detectors_G3_Setup_10(G4LogicalVolume *World_Log);
	~Detectors_G3_Setup_10(){};

	void Construct(G4ThreeVector global_coordinates);

private:
	G4LogicalVolume *World_Logical;
};

#endif
