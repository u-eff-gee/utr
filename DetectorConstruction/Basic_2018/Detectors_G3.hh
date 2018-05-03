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

#ifndef DETECTORS_G3_HH
#define DETECTORS_G3_HH 1

#include "G4LogicalVolume.hh"

class Detectors_G3{
public:
	Detectors_G3();
	~Detectors_G3(){};

	G4LogicalVolume *Get_Logical(){ return Detectors_G3_Logical; }
	
	G4double Get_Length(){ return Detectors_G3_Length; };

private:
	G4double Detectors_G3_Length;

	G4LogicalVolume *Detectors_G3_Logical;
};

#endif
