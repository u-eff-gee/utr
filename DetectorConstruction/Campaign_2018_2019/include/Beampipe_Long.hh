/* utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2019 the developing team (see README.md)

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

class Beampipe_Long{
public:
	Beampipe_Long(G4LogicalVolume *World_Log);
	~Beampipe_Long(){};

	void Construct(G4ThreeVector global_coordinates, G4double relative_density);

	G4double Get_Length(){ return Beampipe_Long_Length; };
	G4double Get_Z_Axis_Offset_Z(){ return Z_Axis_Offset_Z; };
	G4LogicalVolume* Get_Beampipe_Vacuum(){ return Vacuum_Logical; };

private:
	G4LogicalVolume *World_Logical;
	G4LogicalVolume *Vacuum_Logical;
	G4double Beampipe_Long_Upstream_Length;
	G4double Beampipe_Long_Downstream_Length;
	G4double Beampipe_Long_Length;
	G4double Z_Axis_Offset_Z;
};
