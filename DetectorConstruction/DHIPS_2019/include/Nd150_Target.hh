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

#include "G4LogicalVolume.hh"

class Nd150_Target{
public:
	Nd150_Target();
	Nd150_Target(G4LogicalVolume *World_Log);
	~Nd150_Target(){};

	void Construct(G4ThreeVector global_coordinates);
	void Set_Containing_Volume(G4LogicalVolume *World_Log){ World_Logical = World_Log; };

private:
	G4LogicalVolume *World_Logical;
};
