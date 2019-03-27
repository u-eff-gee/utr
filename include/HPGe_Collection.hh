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

// Properties of available coaxial HPGe detectors

#include "G4SystemOfUnits.hh"

#include "HPGe_Coaxial_Properties.hh"
#include "HPGe_Clover_Properties.hh"
#include "Units.hh"

struct HPGe_Collection{
	HPGe_Coaxial_Properties HPGe_60_TUNL_21033;
	HPGe_Clover_Properties HPGe_Clover_Yale;

	HPGe_Collection(){
		HPGe_60_TUNL_21033.detector_radius = 0.5*68.*mm;
		HPGe_60_TUNL_21033.detector_length = 80.*mm;
		HPGe_60_TUNL_21033.detector_face_radius = 8.*mm;
		HPGe_60_TUNL_21033.hole_radius = 0.5*9.3*mm;
		HPGe_60_TUNL_21033.hole_depth = 73.1*mm;
		HPGe_60_TUNL_21033.hole_face_radius = HPGe_60_TUNL_21033.hole_radius;
		HPGe_60_TUNL_21033.mount_cup_length = 10.5*inch;
		HPGe_60_TUNL_21033.mount_cup_thickness = 0.76*mm;
		HPGe_60_TUNL_21033.mount_cup_base_thickness = 3.2*mm;
		HPGe_60_TUNL_21033.mount_cup_material = "G4_Al";
		HPGe_60_TUNL_21033.end_cap_to_crystal_gap = 4.*mm;
		HPGe_60_TUNL_21033.end_cap_thickness = 1.*mm;
		HPGe_60_TUNL_21033.end_cap_window_thickness = 0.5*mm;
		HPGe_60_TUNL_21033.end_cap_material = "G4_Al";
		HPGe_60_TUNL_21033.end_cap_window_material = "G4_Be";
		HPGe_60_TUNL_21033.cold_finger_radius = 0.5*4.*mm;
		HPGe_60_TUNL_21033.cold_finger_penetration_depth = HPGe_60_TUNL_21033.hole_depth - 5.*mm;
		HPGe_60_TUNL_21033.cold_finger_material = "G4_Cu";
		HPGe_60_TUNL_21033.connection_length = 2.*inch;
		HPGe_60_TUNL_21033.connection_radius = 0.75*inch;
		HPGe_60_TUNL_21033.dewar_offset = 0.;
		HPGe_60_TUNL_21033.connection_material = "G4_Al";
		HPGe_60_TUNL_21033.dewar_length = 9.*inch;
		HPGe_60_TUNL_21033.dewar_outer_radius = 3.*inch;
		HPGe_60_TUNL_21033.dewar_wall_thickness = 5.*mm;
		HPGe_60_TUNL_21033.dewar_material = "G4_Al";

		HPGe_Clover_Yale.crystal_radius = 0.5*50.*mm;
		HPGe_Clover_Yale.crystal_length = 80.*mm;
		HPGe_Clover_Yale.crystal_face_radius = 2.*mm;
		HPGe_Clover_Yale.end_cap_outer_side_length = 101.*mm;
		HPGe_Clover_Yale.end_cap_radius = 15.5*mm;
		HPGe_Clover_Yale.end_cap_thickness = 1.*mm;
		HPGe_Clover_Yale.end_cap_window_thickness = 0.5*mm;
		HPGe_Clover_Yale.end_cap_material = "G4_Al";
		HPGe_Clover_Yale.end_cap_window_material = "G4_Al";
	}
};
