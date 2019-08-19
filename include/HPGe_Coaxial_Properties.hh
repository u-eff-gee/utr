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

struct HPGe_Coaxial_Properties{
	// Struct that contains the dimensions of a coaxial detector
	// The naming is partially inspired by typical Ortec data sheets
	//
	// 'Face' = towards the target
	// 'Base' = away from the target
	
	// Detector crystal
	G4double detector_radius; // Radius of the detector crystal
	G4double detector_length; // Length of the detector crystal, including the rounded part
	G4double detector_face_radius; // Rounding radius of the crystal face
	G4double hole_radius; // Radius of the hole in the crystal which is penetrated by 
			      // the cold finger
	G4double hole_depth; // Depth of the hole, measured from the base of the crystal,
			     // including the rounded part
	G4double hole_face_radius; // Rounding radius of the tip of the hole. Most probably the
	                           // same as the hole radius
	// The contact layers on the surface of the crystal are neglected at the moment, since
	// they usually have a thickness of less than a millimeter and are made of
	// very light materials
	
	// Mount cup 
	// Cylindrical tube with top and bottom inside which the crystal is mounted.
	// (Top == Face in this case)
	// It is separated from the outer cup by a layer of vacuum.
	G4double mount_cup_length; // Including top and bottom 
	G4double mount_cup_thickness; // Thickness of the mount cup wall and top
	G4double mount_cup_base_thickness; // Thickness of the mount cup base, which may be
	                                   // different from the thickness of top and side
	G4String mount_cup_material;
	
	// End cap
	// Cylindrical cube which covers the whole mount cup.
	// The end cap length is assumed to be the same as the mount cup length plus
	// the vacuum layer
	G4double end_cap_to_crystal_gap_front; // Distance from the front of the mount cup
	                                 // to the inside of the end cap window, i.e. thickness
					 // of the vacuum layer between both
	G4double end_cap_to_crystal_gap_side; // Distance from the side of the mount cup
	                                 // to the inner side of the end cap, i.e. thickness
					 // of the vacuum layer between both
	G4double end_cap_thickness; // Thickness of the end cap side
	G4double end_cap_length; // Length of the end cap, including end cap window thickness
	G4double end_cap_outer_radius; // Outer radius of the end cap. Not used in the actual
		                       // construction, but can be used for placements or
				       // calculation of the size of the vacuum gap
	G4double end_cap_window_thickness; // Thickness of the window which faces the target
	G4String end_cap_material;
	G4String end_cap_window_material;

	// Cold finger
	// Conducts heat from the dewar to the detector crystal. It penetrates the hole in the
	// crystal and is attached to the connection. Therefore, its total length depends
	// on other dimensions.
	G4double cold_finger_radius; // Radius of the cold finger, which is equal to the 
	                             // rounding radius
	G4double cold_finger_penetration_depth; // Determines how far the cold finger, including
						// the rounded tip, reaches into the crystal
	G4String cold_finger_material;

	/* The following parts are implemented mainly for aesthetic reasons and
	 * they are rough estimates for the actual structure of the dewar and the
	 * connecting piece.
	 */
	
	// Connection between dewar and mount cup / end cap
	// Contains the cold finger and electronics, but the latter are not implemented
	// here. Assumed to consist of solid aluminium.
	G4double connection_length; // Length of the connecting piece
	G4double connection_radius; // Radius of the connecting piece
	G4double dewar_offset; // Offset of the symmetry axis of dewar and connection
	                       // from the symmetry axis of the front part.
	G4String connection_material;

	// Dewar
	// A cylindric tube with top and bottom
	G4double dewar_length; // Dewar length, including top and bottom
	G4double dewar_outer_radius; // Outer radius, can be measured most easily
	G4double dewar_wall_thickness; // Dewar wall thickness. Assumed to be the same for
	                               // top, bottom and side
	G4String dewar_material;

};
