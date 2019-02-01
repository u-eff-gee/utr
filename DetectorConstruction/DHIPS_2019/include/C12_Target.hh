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

#ifndef C12_TARGET_HH
#define C12_TARGET_HH 1

#include "G4LogicalVolume.hh"

class C12_Target{
public:
	C12_Target();
	C12_Target(G4LogicalVolume *World_Log);
	~C12_Target(){};

	void Construct(G4ThreeVector global_coordinates);
	void Set_Containing_Volume(G4LogicalVolume *World_Log){ World_Logical = World_Log; };

private:
	G4LogicalVolume *World_Logical;
};

#endif
