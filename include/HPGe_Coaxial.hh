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

// Class for a generic coaxial High-purity Germanium (HPGe) detector +
// a struct that contains the properties of a specific detector

#ifndef HPGE_COAXIAL_h
#define HPGE_COAXIAL_h 1

#include "G4LogicalVolume.hh"

#include "HPGe_Coaxial_Properties.hh"

class HPGe_Coaxial{
	public:
		HPGe_Coaxial(HPGe_Coaxial_Properties prop, G4LogicalVolume *World_Logical, G4String name);
		~HPGe_Coaxial(){};

		void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center);
	private:
		G4LogicalVolume *world_Logical;	
		HPGe_Coaxial_Properties properties;
		G4String detector_name;
};

#endif
