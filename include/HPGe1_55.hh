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

//**************************************************************//
//	55%  HPGe Detector 1 @ HIGS Serial No. 41-TN31524A
//**************************************************************//

#ifndef HPGe1_55_h
#define HPGe1_55_h 1

#include "G4LogicalVolume.hh"

class HPGe1_55 {
  public:
	HPGe1_55(G4String Detector_Name);
	~HPGe1_55();

	G4LogicalVolume *Get_Logical() { return HPGe1_55_Logical; }

	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

  private:
	G4double Length;
	G4double Radius;

	G4LogicalVolume *HPGe1_55_Logical;
};

#endif
