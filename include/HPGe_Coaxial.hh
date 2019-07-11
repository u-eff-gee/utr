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

// Class for a generic coaxial High-purity Germanium (HPGe) detector

#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "HPGe_Coaxial_Properties.hh"

using std::vector;

class HPGe_Coaxial : public Detector{
	public:
		HPGe_Coaxial(G4LogicalVolume *World_Logical, G4String name) : Detector (World_Logical, name), use_filter_case(false), use_filter_case_ring(false), use_dewar(false){};
		~HPGe_Coaxial(){};

		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
			       G4double dist_from_center, G4double intrinsic_rotation_angle) const override;
		// If no intrinsic rotation angle is given, it defaults to zero
		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
			       G4double dist_from_center) const override;
		void setProperties(HPGe_Coaxial_Properties &prop){properties = prop;};
		void useFilterCase(){use_filter_case = true;};
		void useFilterCaseRing(){use_filter_case_ring = true;};
		void useDewar(){use_dewar = true;};

	private:
		HPGe_Coaxial_Properties properties;
		bool use_filter_case;
		bool use_filter_case_ring;
		bool use_dewar;
};
