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
//	LaBr Crystal (Saint Gobain BrilLanCe 380) 3x3" (TUD)
//**************************************************************//
#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

#include "Detector.hh"

using std::vector;

class LaBr_3x3 : public Detector{
  public:
	LaBr_3x3(G4LogicalVolume *World_Logical, G4String name) : Detector (World_Logical, name), use_filter_case(false), use_filter_case_ring(false), use_housing(false){};
	~LaBr_3x3(){};

	void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center) const override;
	// When an intrinsic rotation angle is given, it will simply be ignored and
	// the Construct() method above is called
	void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center, G4double intrinsic_rotation_angle) const override;
	void useFilterCase(){use_filter_case = true;};
	void useFilterCaseRing(){use_filter_case_ring = true;};
	void useHousing(){use_housing = true;};

  private:
	bool use_filter_case;
	bool use_filter_case_ring;
	bool use_housing;
};
