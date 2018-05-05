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

#ifndef ZERODEGREE_SETUP_HH
#define ZERODEGREE_SETUP_HH 1

#include "G4LogicalVolume.hh"

class ZeroDegree_Setup{
public:
	ZeroDegree_Setup();
	~ZeroDegree_Setup(){};

	G4LogicalVolume *Get_Logical(){ return ZeroDegree_Setup_Logical; }
	
	G4double Get_Length(){ return ZeroDegree_Setup_Length; };

private:
	G4double ZeroDegree_Setup_Length;

	G4LogicalVolume *ZeroDegree_Setup_Logical;
};

#endif
