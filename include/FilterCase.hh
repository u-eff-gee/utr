/*
 * This geometry is deprecated and not safe against volume overlaps.
 * It will be removed in a future version, and a much safer alternative
 * exists in Filter_Case.hh
 */

/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.)
utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILTERCASE_H
#define FILTERCASE_H 1

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class FilterCase {
  public:

	FilterCase(G4double Filter_Thickness, G4bool Use_Ring);
	~FilterCase(){};

	G4LogicalVolume* Get_Logical(){ return FilterCase_Logical; };

	G4double Get_Length(){ return Length; };
	G4double Get_Radius(){ return Radius; };
	G4double Get_Offset_From_Detector(){ return Radial_Offset; }; // The radial offset determines how far away from the detector nose the filter case needs to be positioned in order to hold all the filters.
	G4double Get_FilterCaseRing_Thickness(){ return FilterCaseRing_Thickness; };

  private:
	G4LogicalVolume *FilterCase_Logical;

	G4double Length;
	G4double Radius;
	G4double Radial_Offset;
	G4double FilterCaseRing_Thickness;

};

#endif
