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

// Struct that contains the properties of a specific 
// coaxial High-purity Germanium (HPGe) detector

#ifndef HPGE_CLOVER_PROPERTIES_h
#define HPGE_CLOVER_PROPERTIES_h 1

#include "G4String.hh"
#include "G4Types.hh"

struct HPGe_Clover_Properties{
	// Struct that contains the dimensions of a clover detector
	// It is assumed to be constructed from 4 formerly coaxial crystals,
	// which were flattened on all sides to fit into a rectangular arrangement.
	//
	// 'Face' = towards the target
	// 'Base' = away from the target
	
	// Detector crystals
	G4double crystal_radius; // Radius of the detector crystals if they were still round
	G4double crystal_length; // Length of the detector crystal
	G4double crystal_face_radius; // Rounding radius of the crystal face
	// The contact layers on the surface of the crystal are neglected at the moment, since
	// they usually have a thickness of less than a millimeter and are made of
	// very light materials
	
	// End cap
	// Rectangular case with smoothed edges which covers the whole mount cup.
	G4double end_cap_outer_side_length; // Side length of the end cap with a quadratic base area 
	                                    // Measured from the outside
	G4double end_cap_length; // Length of the end cap
	G4double end_cap_radius; // Rounding radius of the end cap
	G4double end_cap_thickness; // Thickness of the end cap
	G4double end_cap_window_thickness; // Thickness of the window which faces the target
	G4String end_cap_material;
	G4String end_cap_window_material;

};

#endif
