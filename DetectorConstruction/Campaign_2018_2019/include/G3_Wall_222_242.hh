/* utr - Geant4 simulation of the UTR at HIGS
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

#ifndef G3_WALL_222_242_HH
#define G3_WALL_222_242_HH 1

#include "G4LogicalVolume.hh"

class G3_Wall_222_242{
public:
	G3_Wall_222_242(G4LogicalVolume *World_Log);
	~G3_Wall_222_242(){};

	void Construct(G4ThreeVector global_coordinates);

	G4double Get_Floor_Level(){ return Floor_Level; };
	G4double Get_Length(){ return G3_Wall_Length; };

private:
	G4LogicalVolume *World_Logical;

	G4double Lead_Wall_Tunnel_Y;
	G4double Lead_Wall_Base_Y;
	G4double Concrete_Base_Hor_Y;
	G4double Floor_Level;
	G4double Concrete_Base_Hor_Z;
	G4double Concrete_Base_Ver_Z;
	G4double G3_Wall_Length;
};

#endif
