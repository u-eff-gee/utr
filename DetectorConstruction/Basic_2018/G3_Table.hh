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

#ifndef G3_TABLE_HH
#define G3_TABLE_HH 1

#include "G4LogicalVolume.hh"

class G3_Table{
public:
	G3_Table();
	~G3_Table(){};

	G4LogicalVolume *Get_Logical(){ return G3_Table_Logical; }
	
	G4double Get_Length(){ return G3_Table_Length; };
	G4double Get_Z_Axis_Offset_Y(){ return Z_Axis_Offset_Y; };

private:
	G4double G3_Table_Length;
	G4double Z_Axis_Offset_Y;

	G4LogicalVolume *G3_Table_Logical;
};

#endif
