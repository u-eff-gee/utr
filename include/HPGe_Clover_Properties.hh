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

#pragma once

#include "G4String.hh"
#include "G4Types.hh"

struct HPGe_Clover_Properties{
	// Struct that contains the dimensions of a clover detector
	// It is assumed to be constructed from 4 formerly coaxial crystals,
	// which were flattened on all sides to fit into a rectangular arrangement.
	//
	// 'Front' = towards the target
	// 'Back' = away from the target
	
	// Detector crystals
	G4double crystal_radius; // Radius of the detector crystals if they were still round
	G4double crystal_length; // Length of the detector crystal
	G4double crystal_face_radius; // Rounding radius of the crystal face
	// The contact layers on the surface of the crystal are neglected at the moment, since
	// they usually have a thickness of less than a millimeter and are made of
	// very light materials
	G4double crystal_gap; // The four crystals do not touch each other, but there is some gap in between
	G4double end_cap_to_crystal_gap_front; // Vacuum-filled gap between the end cap and the detector
						//crystals at the front
	G4double vacuum_length; // Length of the evacuated area inside the end cap
	
	// End cap
	// Case with quadratic base area and smoothed edges which covers the whole mount cup.
	// It consists of the fron part that contains the detector crystals and a back part
	// that contains the electronics.
	// The back part is modelled as an skeleton, since the inner composition is unknown.
	G4double end_cap_front_side_length; // Side length of the front end cap with a quadratic (see next variable)
					    // base area 
	G4double end_cap_front_rounding_radius;  // Rounding radius of the edges
	G4double end_cap_front_length; // Length of the end cap around the crystal.
	G4double end_cap_front_thickness; // Thickness of the end cap at the front (But not at the part that faces
					// the target. This 'window thickness' (see below) may be thinner.)
	G4double end_cap_window_thickness; // Thickness of the window which faces the target

	/* The following parts are implemented mainly for aesthetic reasons and
	 * they are rough estimates for the actual structure of the end cap at
	 * the back, the dewar and the connecting piece.
	 */
	
	G4double end_cap_back_side_length; // Side length of the back end cap with a quadratic (see next variable)
					    // base area 
	G4double end_cap_back_rounding_radius;  // Rounding radius of the edges
	G4double end_cap_back_length; // Length of the end cap around the electronics.
	G4double end_cap_back_thickness; // Thickness of the end cap
	G4String end_cap_material;

	// Connection between dewar and mount cup / end cap
	// Contains the cold finger and electronics, but the latter are not implemented
	// here. Assumed to consist of solid aluminium.
	G4double connection_length; // Length of the connecting piece
	G4double connection_radius; // Radius of the connecting piece
	G4String connection_material;

	// Dewar
	// A cylindric tube with top and bottom
	G4double dewar_length; // Dewar length, including top and bottom
	G4double dewar_outer_radius; // Outer radius, can be measured most easily
	G4double dewar_wall_thickness; // Dewar wall thickness. Assumed to be the same for
	                               // top, bottom and side
	G4String dewar_material;

};
