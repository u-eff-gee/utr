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

/// \file HPGe3.hh
/// \brief Header file of HPGe3

//**************************************************************//
//	60%  HPGe detector 3 @ HIGS Serial No. 36-TN40663A
//**************************************************************//

#ifndef HPGe3_h
#define HPGe3_h 1

#include "G4LogicalVolume.hh"

class HPGe3 {
  public:
	HPGe3(G4String Detector_Name);
	~HPGe3();

	G4LogicalVolume *Get_Logical() { return HPGe3_Logical; }
	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

	G4double Length;
	G4double Radius;

  private:
	G4LogicalVolume *HPGe3_Logical;
};

#endif
