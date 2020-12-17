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

// Properties of available HPGe detectors
#pragma once

#include "G4SystemOfUnits.hh"

#include "HPGe_Coaxial_Properties.hh"
#include "HPGe_Clover_Properties.hh"
#include "Units.hh"

struct HPGe_Collection{
	HPGe_Coaxial_Properties HPGe_55_TUNL_21638;
	HPGe_Coaxial_Properties HPGe_55_TUNL_31524;
	HPGe_Coaxial_Properties HPGe_60_TUNL_21033;
	HPGe_Coaxial_Properties HPGe_60_TUNL_30986;
	HPGe_Coaxial_Properties HPGe_60_TUNL_31061;
	HPGe_Coaxial_Properties HPGe_60_TUNL_40663;
	HPGe_Coaxial_Properties HPGe_120_TUNL_40383;
	HPGe_Coaxial_Properties HPGe_80_TUD_90006;
	HPGe_Coaxial_Properties HPGe_100_TUD_72902;
	HPGe_Coaxial_Properties HPGe_100_TUD_72930;
	HPGe_Coaxial_Properties HPGe_100_TUD_73760;
	HPGe_Coaxial_Properties HPGe_100_Cologne_73954;
	HPGe_Coaxial_Properties HPGe_86_Stuttgart_31120;
	HPGe_Coaxial_Properties HPGe_ANL_31670;
	HPGe_Coaxial_Properties HPGe_ANL_41203;
	HPGe_Clover_Properties HPGe_Clover_Yale;

	HPGe_Collection(){
		// 55% coaxial HPGe with ORTEC serial number 4-TN21638A
		// Property of TUNL
		HPGe_55_TUNL_21638.detector_radius = 0.5*63.2*mm; // (A) in ORTEC data sheet
		HPGe_55_TUNL_21638.detector_length = 84.8*mm; // (B) in ORTEC data sheet
		HPGe_55_TUNL_21638.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_55_TUNL_21638.hole_radius = 0.5*9.2*mm; // (C) in ORTEC data sheet
		HPGe_55_TUNL_21638.hole_depth = 73.1*mm; // (D) in ORTEC data sheet
		HPGe_55_TUNL_21638.mount_cup_thickness = 0.8*mm; // (K) in ORTEC data sheet
		HPGe_55_TUNL_21638.mount_cup_base_thickness = 3.*mm; // ORTEC data sheet
		HPGe_55_TUNL_21638.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_outer_radius = 0.5*80.*mm; // Estimated
		HPGe_55_TUNL_21638.end_cap_to_crystal_gap_front = 4.*mm; // (G) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_thickness = 1.*mm; // (L) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_window_thickness = 0.5*mm; // (I) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_length = 6.5*inch; // Measured at the detector
		HPGe_55_TUNL_21638.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_55_TUNL_21638.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_55_TUNL_21638.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_55_TUNL_21638.connection_length = 1.5*inch; // Measured
		HPGe_55_TUNL_21638.connection_radius = 0.75*inch; // Estimated
		HPGe_55_TUNL_21638.dewar_offset = 0.*inch; // Measured
		HPGe_55_TUNL_21638.connection_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_21638.dewar_length = 12.5*inch; // Measured
		HPGe_55_TUNL_21638.dewar_outer_radius = 4.5*inch; // Measured
		HPGe_55_TUNL_21638.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_55_TUNL_21638.dewar_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_21638.hole_face_radius = HPGe_55_TUNL_21638.hole_radius; // Estimated to be the same as hole radius
		HPGe_55_TUNL_21638.cold_finger_penetration_depth = HPGe_55_TUNL_21638.hole_depth - 5.*mm; // Estimated
		HPGe_55_TUNL_21638.end_cap_to_crystal_gap_side = 
			HPGe_55_TUNL_21638.end_cap_outer_radius- 
			HPGe_55_TUNL_21638.end_cap_thickness- 
			HPGe_55_TUNL_21638.mount_cup_thickness-
			HPGe_55_TUNL_21638.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_55_TUNL_21638.mount_cup_length = 
			HPGe_55_TUNL_21638.end_cap_length-
			HPGe_55_TUNL_21638.end_cap_window_thickness-
			HPGe_55_TUNL_21638.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (F) in ORTEC data sheet, because it shows only the part which contains the detector crystal.

		// 55% coaxial HPGe with ORTEC serial number 41-TN31524A
		// Property of TUNL
		HPGe_55_TUNL_31524.detector_radius = 0.5*67.1*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.detector_length = 63.1*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_55_TUNL_31524.hole_radius = 0.5*8.7*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.hole_depth = 54.3*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_55_TUNL_31524.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_55_TUNL_31524.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_outer_radius = 0.5*80.*mm; // Estimated
		HPGe_55_TUNL_31524.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_length = 6.5*inch; // Measured at the detector
		HPGe_55_TUNL_31524.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_55_TUNL_31524.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_55_TUNL_31524.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_55_TUNL_31524.connection_length = 1.5*inch; // Measured
		HPGe_55_TUNL_31524.connection_radius = 0.75*inch; // Estimated
		HPGe_55_TUNL_31524.dewar_offset = 0.*inch; // Measured
		HPGe_55_TUNL_31524.connection_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_31524.dewar_length = 12.5*inch; // Measured
		HPGe_55_TUNL_31524.dewar_outer_radius = 4.5*inch; // Measured
		HPGe_55_TUNL_31524.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_55_TUNL_31524.dewar_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_31524.hole_face_radius = HPGe_55_TUNL_31524.hole_radius; // Estimated to be the same as hole radius
		HPGe_55_TUNL_31524.cold_finger_penetration_depth = HPGe_55_TUNL_31524.hole_depth - 5.*mm; // Estimated
		HPGe_55_TUNL_31524.mount_cup_length = 
			HPGe_55_TUNL_31524.end_cap_length-
			HPGe_55_TUNL_31524.end_cap_window_thickness-
			HPGe_55_TUNL_31524.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_55_TUNL_31524.end_cap_to_crystal_gap_side = 
			HPGe_55_TUNL_31524.end_cap_outer_radius- 
			HPGe_55_TUNL_31524.end_cap_thickness- 
			HPGe_55_TUNL_31524.mount_cup_thickness-
			HPGe_55_TUNL_31524.detector_radius; // Calculated from outer radius and other given dimensions

		// 60% coaxial HPGe with ORTEC serial number 36-TN21033A
		// Property of TUNL
		HPGe_60_TUNL_21033.detector_radius = 0.5*68.*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.detector_length = 80.5*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_21033.hole_radius = 0.5*9.3*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.hole_depth = 73.1*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_21033.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_21033.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_outer_radius = 0.5*85.*mm; // Estimated
		HPGe_60_TUNL_21033.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_length = 10.5*inch; // Measured at the detector
		HPGe_60_TUNL_21033.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_21033.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_21033.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_21033.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_21033.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_21033.dewar_offset = 1.*inch; // Measured
		HPGe_60_TUNL_21033.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_21033.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_21033.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_21033.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_21033.dewar_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_21033.hole_face_radius = HPGe_60_TUNL_21033.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_21033.cold_finger_penetration_depth = HPGe_60_TUNL_21033.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_21033.mount_cup_length = 
			HPGe_60_TUNL_21033.end_cap_length-
			HPGe_60_TUNL_21033.end_cap_window_thickness-
			HPGe_60_TUNL_21033.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (F) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_21033.end_cap_to_crystal_gap_side = 
			HPGe_60_TUNL_21033.end_cap_outer_radius- 
			HPGe_60_TUNL_21033.end_cap_thickness- 
			HPGe_60_TUNL_21033.mount_cup_thickness-
			HPGe_60_TUNL_21033.detector_radius; // Calculated from outer radius and other given dimensions

		// 60% coaxial HPGe with ORTEC serial number 36-TN30986A
		// Property of TUNL
		HPGe_60_TUNL_30986.detector_radius = 0.5*68.2*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.detector_length = 77.5*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_30986.hole_radius = 0.5*11.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.hole_depth = 70.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_30986.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_30986.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_outer_radius = 0.5*85.*mm; // Estimated
		HPGe_60_TUNL_30986.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_length = 10.5*inch; // Measured at the detector
		HPGe_60_TUNL_30986.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_30986.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_30986.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_30986.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_30986.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_30986.dewar_offset = 1.*inch; // Measured
		HPGe_60_TUNL_30986.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_30986.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_30986.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_30986.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_30986.dewar_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_30986.hole_face_radius = HPGe_60_TUNL_30986.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_30986.cold_finger_penetration_depth = HPGe_60_TUNL_30986.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_30986.end_cap_to_crystal_gap_side = 
			HPGe_60_TUNL_30986.end_cap_outer_radius- 
			HPGe_60_TUNL_30986.end_cap_thickness- 
			HPGe_60_TUNL_30986.mount_cup_thickness-
			HPGe_60_TUNL_30986.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_60_TUNL_30986.mount_cup_length = 
			HPGe_60_TUNL_30986.end_cap_length-
			HPGe_60_TUNL_30986.end_cap_window_thickness-
			HPGe_60_TUNL_30986.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.

		// 60% coaxial HPGe with ORTEC serial number 36-TN31061A
		// Property of TUNL
		//HPGe_60_TUNL_31061.detector_radius = 0.5*64.6*mm; // ORTEC data sheet
		//HPGe_60_TUNL_31061.detector_length = 80.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.detector_radius = 0.5*64.5*mm; // According to B. Fallin, this detector was reconditioned, so the crystal length and radius are smaller than in the data sheet
		HPGe_60_TUNL_31061.detector_length = 79.4*mm; // see above
		HPGe_60_TUNL_31061.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_31061.hole_radius = 0.5*11.2*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.hole_depth = 72.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_31061.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_31061.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_outer_radius = 0.5*85.*mm; // Estimated
		HPGe_60_TUNL_31061.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_length = 10.5*inch; // Measured at the detector
		HPGe_60_TUNL_31061.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_31061.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_31061.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_31061.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_31061.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_31061.dewar_offset = 0.*inch; // Measured
		HPGe_60_TUNL_31061.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_31061.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_31061.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_31061.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_31061.dewar_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_31061.hole_face_radius = HPGe_60_TUNL_31061.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_31061.cold_finger_penetration_depth = HPGe_60_TUNL_31061.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_31061.mount_cup_length = 
			HPGe_60_TUNL_31061.end_cap_length-
			HPGe_60_TUNL_31061.end_cap_window_thickness-
			HPGe_60_TUNL_31061.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_31061.end_cap_to_crystal_gap_side = 
			HPGe_60_TUNL_31061.end_cap_outer_radius- 
			HPGe_60_TUNL_31061.end_cap_thickness- 
			HPGe_60_TUNL_31061.mount_cup_thickness-
			HPGe_60_TUNL_31061.detector_radius; // Calculated from outer radius and other given dimensions

		// 60% coaxial HPGe with ORTEC serial number 36-TN40663A
		// Property of TUNL
		HPGe_60_TUNL_40663.detector_radius = 0.5*69.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.detector_length = 68.3*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_40663.hole_radius = 0.5*9.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.hole_depth = 60.*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_40663.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_40663.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_outer_radius = 0.5*85.*mm; // Estimated
		HPGe_60_TUNL_40663.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_to_crystal_gap_side = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_length = 10.5*inch; // Measured at the detector
		HPGe_60_TUNL_40663.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_40663.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_40663.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_40663.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_40663.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_40663.dewar_offset = 1.*inch; // Measured
		HPGe_60_TUNL_40663.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_40663.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_40663.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_40663.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_40663.dewar_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_40663.hole_face_radius = HPGe_60_TUNL_40663.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_40663.cold_finger_penetration_depth = HPGe_60_TUNL_40663.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_40663.end_cap_to_crystal_gap_side = 
			HPGe_60_TUNL_40663.end_cap_outer_radius- 
			HPGe_60_TUNL_40663.end_cap_thickness- 
			HPGe_60_TUNL_40663.mount_cup_thickness-
			HPGe_60_TUNL_40663.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_60_TUNL_40663.mount_cup_length = 
			HPGe_60_TUNL_40663.end_cap_length-
			HPGe_60_TUNL_40663.end_cap_window_thickness-
			HPGe_60_TUNL_40663.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.

		// 120% coaxial HPGe with ORTEC serial number 33-P40383A
		// 'zero-degree detector'
		// Note that this is a detector with a tabletop dewar, which is definitely
		// not coaxial. Do not set the 'use_dewar' flag to 'true' when constructing
		// this detector!
		// Property of TUNL
		HPGe_120_TUNL_40383.detector_radius = 0.5*82.*mm; // ORTEC data sheet
		HPGe_120_TUNL_40383.detector_length = 108.7*mm; // ORTEC data sheet
		HPGe_120_TUNL_40383.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_120_TUNL_40383.hole_radius = 0.5*10.9*mm; // ORTEC data sheet
		HPGe_120_TUNL_40383.hole_depth = 96.1*mm; // ORTEC data sheet
		HPGe_120_TUNL_40383.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_120_TUNL_40383.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_120_TUNL_40383.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_outer_radius = 0.5*90.*mm; // Estimated
		HPGe_120_TUNL_40383.end_cap_to_crystal_gap_front = 4.*mm; // (B) in ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_to_crystal_gap_side = 4.*mm; // (B) in ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_thickness = 1.5*mm; // (I) in ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_window_thickness = 1.5*mm; // (D) in ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_length = 5.5*inch; // Estimated
		HPGe_120_TUNL_40383.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_120_TUNL_40383.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_120_TUNL_40383.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_120_TUNL_40383.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_120_TUNL_40383.connection_length = 2.*inch; // Estimated and irrelevant
		HPGe_120_TUNL_40383.connection_radius = 0.75*inch; // Estimated and irrelevant
		HPGe_120_TUNL_40383.dewar_offset = 0.*inch; // Estimated and irrelevant
		HPGe_120_TUNL_40383.connection_material = "G4_Al"; // Estimated and irrelevant
		HPGe_120_TUNL_40383.dewar_length = 9.*inch; // Estimated and irrelevant
		HPGe_120_TUNL_40383.dewar_outer_radius = 3.*inch; // Estimated and irrelevant
		HPGe_120_TUNL_40383.dewar_wall_thickness = 5.*mm; // Estimated and irrelevant
		HPGe_120_TUNL_40383.dewar_material = "G4_Al"; // Estimated and irrelevant
		HPGe_120_TUNL_40383.hole_face_radius = HPGe_120_TUNL_40383.hole_radius; // Estimated to be the same as hole radius
		HPGe_120_TUNL_40383.cold_finger_penetration_depth = HPGe_120_TUNL_40383.hole_depth - 5.*mm; // Estimated
		HPGe_120_TUNL_40383.mount_cup_length = 
			HPGe_120_TUNL_40383.end_cap_length-
			HPGe_120_TUNL_40383.end_cap_window_thickness-
			HPGe_120_TUNL_40383.end_cap_to_crystal_gap_front; // Calculated from end cap length. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_120_TUNL_40383.end_cap_to_crystal_gap_side = 
			HPGe_120_TUNL_40383.end_cap_outer_radius- 
			HPGe_120_TUNL_40383.end_cap_thickness- 
			HPGe_120_TUNL_40383.mount_cup_thickness-
			HPGe_120_TUNL_40383.detector_radius; // Calculated from outer radius and other given dimensions

		// 80% (68.2% according to data sheet) coaxial HPGe with
		// CANBERRA serial number b90006
		// Informally known as '80%-detector'
		// Property of IKP TU Darmstadt. Found in a storage room and repaired by
		// CANBERRA in 2019
		// Reconstructed from CANBERRA data sheet that was given to the Darmstadt
		// group after the repair
		HPGe_80_TUD_90006.detector_radius = 0.5*73.2*mm; // CANBERRA data sheet
		HPGe_80_TUD_90006.detector_length = 69.4*mm; // CANBERRA data sheet
		HPGe_80_TUD_90006.detector_face_radius = 2.*mm; // Estimated
		HPGe_80_TUD_90006.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_80_TUD_90006.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_80_TUD_90006.mount_cup_base_thickness = 3.*mm; // Estimated
		HPGe_80_TUD_90006.mount_cup_material = "G4_Al"; // Estimated
		HPGe_80_TUD_90006.end_cap_outer_radius = 0.5*95.*mm; // Measured
		HPGe_80_TUD_90006.end_cap_to_crystal_gap_front = 6.*mm - HPGe_80_TUD_90006.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_80_TUD_90006.end_cap_thickness = 1.*mm; // Estimated
		HPGe_80_TUD_90006.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_80_TUD_90006.end_cap_length = 172.*mm; // Measured at the detector
		HPGe_80_TUD_90006.end_cap_material = "G4_Al"; // Estimated
		HPGe_80_TUD_90006.end_cap_window_material = "G4_Al"; // Estimated, definitely the same as the rest of the end cap
		HPGe_80_TUD_90006.cold_finger_radius = 0.5*6.*mm; // Estimated
		HPGe_80_TUD_90006.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_80_TUD_90006.connection_length = 214.*mm; // Measured
		HPGe_80_TUD_90006.connection_radius = 0.5*25.*mm; // Measured
		HPGe_80_TUD_90006.dewar_offset = 0.*inch; // Estimated
		HPGe_80_TUD_90006.connection_material = "G4_Al"; // Estimated
		HPGe_80_TUD_90006.dewar_length = 343.*mm; // Measured
		HPGe_80_TUD_90006.dewar_outer_radius = 110.*mm; // Measured
		HPGe_80_TUD_90006.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_80_TUD_90006.dewar_material = "G4_Al"; // Estimated
		HPGe_80_TUD_90006.hole_depth = 0.8*HPGe_80_TUD_90006.detector_length; // Estimated
		HPGe_80_TUD_90006.hole_face_radius = HPGe_80_TUD_90006.hole_radius; // Estimated to be the same as hole radius
		HPGe_80_TUD_90006.cold_finger_penetration_depth = HPGe_80_TUD_90006.hole_depth - 2.*mm; // Estimated
		HPGe_80_TUD_90006.end_cap_to_crystal_gap_side = 
			HPGe_80_TUD_90006.end_cap_outer_radius- 
			HPGe_80_TUD_90006.end_cap_thickness- 
			HPGe_80_TUD_90006.mount_cup_thickness-
			HPGe_80_TUD_90006.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_80_TUD_90006.mount_cup_length = 
			HPGe_80_TUD_90006.end_cap_length-
			HPGe_80_TUD_90006.end_cap_window_thickness-
			HPGe_80_TUD_90006.end_cap_to_crystal_gap_front; // Calculated from end cap length

		// 100% (102% according to data sheet) coaxial HPGe with
		// CANBERRA serial number 73760
		// Informally known as 'Detector 1'
		// Property of IKP TU Darmstadt, under repair at the time these dimensions were
		// implemented
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		HPGe_100_TUD_73760.detector_radius = 0.5*78.9*mm; // CANBERRA data sheet
		HPGe_100_TUD_73760.detector_length = 79.*mm; // CANBERRA data sheet
		HPGe_100_TUD_73760.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_73760.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_73760.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_73760.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_73760.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_73760.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_73760.end_cap_outer_radius = 0.5*100.*mm; // Estimated
		HPGe_100_TUD_73760.end_cap_thickness = 1.*mm; // Estimated
		HPGe_100_TUD_73760.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_73760.end_cap_length = 150.*mm; // Measured at the detector
		HPGe_100_TUD_73760.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_73760.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_73760.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_73760.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_73760.connection_length = 2.*inch; // Estimated
		HPGe_100_TUD_73760.connection_radius = 0.75*inch; // Estimated
		HPGe_100_TUD_73760.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_73760.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_73760.dewar_length = 9.*inch; // Estimated
		HPGe_100_TUD_73760.dewar_outer_radius = 3.*inch; // Estimated
		HPGe_100_TUD_73760.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_73760.dewar_material = "G4_Al"; // Estimated
		HPGe_100_TUD_73760.hole_face_radius = HPGe_100_TUD_73760.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_73760.end_cap_to_crystal_gap_front = 3.5*mm - HPGe_100_TUD_73760.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_73760.end_cap_to_crystal_gap_side = 
			HPGe_100_TUD_73760.end_cap_outer_radius- 
			HPGe_100_TUD_73760.end_cap_thickness- 
			HPGe_100_TUD_73760.mount_cup_thickness-
			HPGe_100_TUD_73760.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_100_TUD_73760.mount_cup_length = 
			HPGe_100_TUD_73760.end_cap_length-
			HPGe_100_TUD_73760.end_cap_window_thickness-
			HPGe_100_TUD_73760.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_100_TUD_73760.cold_finger_penetration_depth = HPGe_100_TUD_73760.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.

		// 100% (98% according to data sheet) coaxial HPGe with
		// CANBERRA serial number 72902
		// Informally known as 'Detector 2'
		// Property of IKP TU Darmstadt
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		HPGe_100_TUD_72902.detector_radius = 0.5*77.*mm; // CANBERRA data sheet
		HPGe_100_TUD_72902.detector_length = 80.*mm; // CANBERRA data sheet
		HPGe_100_TUD_72902.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_72902.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_72902.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_72902.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72902.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_72902.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72902.end_cap_outer_radius = 0.5*100.*mm; // Measured
		HPGe_100_TUD_72902.end_cap_thickness = 1.*mm; // Estimated
		HPGe_100_TUD_72902.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72902.end_cap_length = 258.*mm; // Measured at the detector
		HPGe_100_TUD_72902.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_72902.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_72902.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_72902.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_72902.connection_length = 60.*mm; // Measured
		HPGe_100_TUD_72902.connection_radius = 25.*mm; // Measured
		HPGe_100_TUD_72902.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_72902.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72902.dewar_length = 415.*mm; // Measured
		HPGe_100_TUD_72902.dewar_outer_radius = 110.*mm; // Measured
		HPGe_100_TUD_72902.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_72902.dewar_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72902.hole_face_radius = HPGe_100_TUD_72902.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_72902.end_cap_to_crystal_gap_front = 5.0*mm - HPGe_100_TUD_72902.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_72902.end_cap_to_crystal_gap_side = 
			HPGe_100_TUD_72902.end_cap_outer_radius- 
			HPGe_100_TUD_72902.end_cap_thickness- 
			HPGe_100_TUD_72902.mount_cup_thickness-
			HPGe_100_TUD_72902.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_100_TUD_72902.mount_cup_length = 
			HPGe_100_TUD_72902.end_cap_length-
			HPGe_100_TUD_72902.end_cap_window_thickness-
			HPGe_100_TUD_72902.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_100_TUD_72902.cold_finger_penetration_depth = HPGe_100_TUD_72902.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.

		// 100% (99% according to data sheet from 2019, 100% according to data sheet
		// from 1997) segmented coaxial HPGe with
		// CANBERRA serial number 72930
		// Informally known as 'Polarimeter'
		// Property of IKP TU Darmstadt
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		HPGe_100_TUD_72930.detector_radius = 0.5*78.*mm; // Eurisys data sheet
		HPGe_100_TUD_72930.detector_length = 78.*mm; // Eurisys data sheet
		HPGe_100_TUD_72930.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_72930.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_72930.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_72930.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72930.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_72930.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72930.end_cap_outer_radius = 0.5*99.*mm; // Measured
		HPGe_100_TUD_72930.end_cap_thickness = 1.*mm; // Estimated
		HPGe_100_TUD_72930.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72930.end_cap_length = 350.*mm; // Measured at the detector
		HPGe_100_TUD_72930.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_72930.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_72930.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_72930.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_72930.connection_length = 50.*mm; // Measured
		HPGe_100_TUD_72930.connection_radius = 25.*mm; // Measured
		HPGe_100_TUD_72930.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_72930.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72930.dewar_length = 270.*mm; // Measured
		HPGe_100_TUD_72930.dewar_outer_radius = 110.*mm; // Measured
		HPGe_100_TUD_72930.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_72930.dewar_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72930.hole_face_radius = HPGe_100_TUD_72930.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_72930.end_cap_to_crystal_gap_front = 5.0*mm - HPGe_100_TUD_72930.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_72930.end_cap_to_crystal_gap_side = 
			HPGe_100_TUD_72930.end_cap_outer_radius- 
			HPGe_100_TUD_72930.end_cap_thickness- 
			HPGe_100_TUD_72930.mount_cup_thickness-
			HPGe_100_TUD_72930.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_100_TUD_72930.mount_cup_length = 
			HPGe_100_TUD_72930.end_cap_length-
			HPGe_100_TUD_72930.end_cap_window_thickness-
			HPGe_100_TUD_72930.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_100_TUD_72930.cold_finger_penetration_depth = HPGe_100_TUD_72930.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.

		// 100% coaxial HPGe, Ortec serial number 73954
		// Dimensions from g4horus Geant4 implementation of the
		// HORUS setup in Cologne (https://gitlab.ikp.uni-koeln.de/jmayer/g4horus/)
		// if not indicated otherwise
		// Published under the MIT license
		// Property of IKP Universitaet zu Koeln
		HPGe_100_Cologne_73954.detector_radius = 0.5*74*mm; 
		HPGe_100_Cologne_73954.detector_length = 70.*mm;
		HPGe_100_Cologne_73954.detector_face_radius = 2.*mm; // Estimated
		HPGe_100_Cologne_73954.hole_radius = 5.8*mm;
		HPGe_100_Cologne_73954.hole_depth = 52.*mm;
		HPGe_100_Cologne_73954.mount_cup_thickness = 1.*mm;
		HPGe_100_Cologne_73954.mount_cup_base_thickness = 5.*mm;
		HPGe_100_Cologne_73954.mount_cup_material = "G4_Al";
		HPGe_100_Cologne_73954.end_cap_outer_radius = 0.5*80.*mm;
		HPGe_100_Cologne_73954.end_cap_to_crystal_gap_front = 3.*mm;
		HPGe_100_Cologne_73954.end_cap_to_crystal_gap_side = 3.*mm;
		HPGe_100_Cologne_73954.end_cap_thickness = 1.*mm;
		HPGe_100_Cologne_73954.end_cap_window_thickness = 1.*mm;
		HPGe_100_Cologne_73954.end_cap_length = 10.5*inch; // Measured at the detector
		HPGe_100_Cologne_73954.end_cap_material = "G4_Al";
		HPGe_100_Cologne_73954.end_cap_window_material = "G4_Al";
		HPGe_100_Cologne_73954.cold_finger_radius = 0.5*4.*mm; // Estimated
		HPGe_100_Cologne_73954.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_Cologne_73954.connection_length = 3.5*inch; // Measured
		HPGe_100_Cologne_73954.connection_radius = 0.75*inch; // Measured
		HPGe_100_Cologne_73954.dewar_offset = 0.*inch; // Measured
		HPGe_100_Cologne_73954.connection_material = "G4_Al"; // Estimated
		HPGe_100_Cologne_73954.dewar_length = 10.5*inch; // Measured
		HPGe_100_Cologne_73954.dewar_outer_radius = 5.*inch; // Measured
		HPGe_100_Cologne_73954.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_Cologne_73954.dewar_material = "G4_Al"; // Estimated
		HPGe_100_Cologne_73954.hole_face_radius = HPGe_100_Cologne_73954.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_Cologne_73954.end_cap_to_crystal_gap_side = 
			HPGe_100_Cologne_73954.end_cap_outer_radius- 
			HPGe_100_Cologne_73954.end_cap_thickness- 
			HPGe_100_Cologne_73954.mount_cup_thickness-
			HPGe_100_Cologne_73954.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_100_Cologne_73954.mount_cup_length = 
			HPGe_100_Cologne_73954.end_cap_length-
			HPGe_100_Cologne_73954.end_cap_window_thickness-
			HPGe_100_Cologne_73954.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_100_Cologne_73954.cold_finger_penetration_depth = HPGe_100_Cologne_73954.hole_depth - 5.*mm; // Estimated

		// 86.2% coaxial HPGe, CANBERRA serial number 37-N31120A
		// Dimensions from CANBERRA data sheet and CT scan by C. Fransen
		// if not indicated otherwise
		// Property of IKP Universitaet zu Koeln
		// For the material of the end cap, it was noted that it actually does not look
		// like aluminium on the outside, but maybe this is due to the advanced age
		// of the detector
		HPGe_86_Stuttgart_31120.detector_radius = 0.5*75*mm; // CANBERRA data sheet
		HPGe_86_Stuttgart_31120.detector_length = 92.*mm; // CANBERRA data sheet
		HPGe_86_Stuttgart_31120.detector_face_radius = 12.*mm; // CT scan 
		HPGe_86_Stuttgart_31120.hole_radius = 0.5*10.3*mm; // CT scan
		HPGe_86_Stuttgart_31120.mount_cup_length = 178.*mm; // CT scan
		HPGe_86_Stuttgart_31120.mount_cup_thickness = 0.76*mm; // Estimated
		HPGe_86_Stuttgart_31120.mount_cup_base_thickness = 6.*mm; // CT scan
		HPGe_86_Stuttgart_31120.mount_cup_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_to_crystal_gap_front = 5.*mm; // CANBERRA data sheet
		HPGe_86_Stuttgart_31120.end_cap_outer_radius = 0.5*90.*mm; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_thickness = 1.*mm; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_length = 178.*mm; // Measured at the detector
		HPGe_86_Stuttgart_31120.end_cap_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_window_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.cold_finger_radius = 0.5*8.*mm; // CT scan
		HPGe_86_Stuttgart_31120.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_86_Stuttgart_31120.connection_length = 13.1*inch; // Measured
		HPGe_86_Stuttgart_31120.connection_radius = 16.*mm; // Measured
		HPGe_86_Stuttgart_31120.dewar_offset = 0.*inch; // Measured
		HPGe_86_Stuttgart_31120.connection_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.dewar_length = 31.5*inch; // Measured
		HPGe_86_Stuttgart_31120.dewar_outer_radius = 6.*inch; // Measured
		HPGe_86_Stuttgart_31120.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_86_Stuttgart_31120.dewar_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.hole_face_radius = HPGe_86_Stuttgart_31120.hole_radius; // Estimated to be the same as hole radius
		HPGe_86_Stuttgart_31120.hole_depth = 0.8*HPGe_86_Stuttgart_31120.detector_length; // Estimated by C. Fransen
		HPGe_86_Stuttgart_31120.end_cap_to_crystal_gap_side = 
			HPGe_86_Stuttgart_31120.end_cap_outer_radius- 
			HPGe_86_Stuttgart_31120.end_cap_thickness- 
			HPGe_86_Stuttgart_31120.mount_cup_thickness-
			HPGe_86_Stuttgart_31120.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_86_Stuttgart_31120.mount_cup_length = 
			HPGe_86_Stuttgart_31120.end_cap_length-
			HPGe_86_Stuttgart_31120.end_cap_window_thickness-
			HPGe_86_Stuttgart_31120.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_86_Stuttgart_31120.cold_finger_penetration_depth = HPGe_86_Stuttgart_31120.hole_depth - 5.*mm; // Estimated

		// 100% (103.5% according to data sheet) coaxial HPGe 
		// with ORTEC serial number 42-TP41203A
		// Property of R.V.F. Janssens group, Argonne National Lab
		HPGe_ANL_41203.detector_radius = 0.5*89.9*mm; // Data sheet
		HPGe_ANL_41203.detector_length = 98.3*mm; // Data sheet
		HPGe_ANL_41203.detector_face_radius = 2.*mm; // Estimated
		HPGe_ANL_41203.hole_radius = 0.5*10.*mm; // Estimated from 60% ORTEC models
		HPGe_ANL_41203.mount_cup_thickness = 0.8*mm; // Estimated
		HPGe_ANL_41203.mount_cup_material = "G4_Al"; // Estimated
		HPGe_ANL_41203.end_cap_outer_radius = 0.5*4.25*inch; // Data sheet
		HPGe_ANL_41203.end_cap_to_crystal_gap_front = 6.5*mm; // Data sheet
		HPGe_ANL_41203.end_cap_thickness = 3.5*mm; // 'Absorbing layer' in data sheet
		HPGe_ANL_41203.end_cap_window_thickness = 3.5*mm; // 'Absorbing layer' in data sheet
		HPGe_ANL_41203.end_cap_length = 8.*inch; // Data sheet
		HPGe_ANL_41203.end_cap_material = "G4_Al"; // Data sheet
		HPGe_ANL_41203.end_cap_window_material = "G4_Al"; // Data sheet
		HPGe_ANL_41203.cold_finger_radius = 0.5*4.*mm; // Estimated from 60% ORTEC models
		HPGe_ANL_41203.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_ANL_41203.connection_length = 4.5*inch; // Data sheet
		HPGe_ANL_41203.connection_radius = 0.5*4.5*inch; // Data sheet
		HPGe_ANL_41203.dewar_offset = 0.*inch;
		HPGe_ANL_41203.connection_material = "G4_Al"; // Estimated
		HPGe_ANL_41203.dewar_length = 16.5*inch; // Data sheet
		HPGe_ANL_41203.dewar_outer_radius = 0.5*9.*inch; // Data sheet
		HPGe_ANL_41203.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_ANL_41203.dewar_material = "G4_Al"; // Estimated
		HPGe_ANL_41203.hole_depth = 0.8*HPGe_ANL_41203.detector_length; // Estimated
		HPGe_ANL_41203.hole_face_radius = HPGe_ANL_41203.hole_radius; // Estimated
		HPGe_ANL_41203.mount_cup_base_thickness = HPGe_ANL_41203.mount_cup_thickness; // Estimated
		HPGe_ANL_41203.end_cap_to_crystal_gap_side = 
			HPGe_ANL_41203.end_cap_outer_radius- 
			HPGe_ANL_41203.end_cap_thickness- 
			HPGe_ANL_41203.mount_cup_thickness-
			HPGe_ANL_41203.detector_radius; // Calculated from outer radius and other given dimensions
		HPGe_ANL_41203.mount_cup_length = 
			HPGe_ANL_41203.end_cap_length-
			HPGe_ANL_41203.end_cap_window_thickness-
			HPGe_ANL_41203.end_cap_to_crystal_gap_front; // Calculated from end cap length
		HPGe_ANL_41203.cold_finger_penetration_depth = HPGe_ANL_41203.hole_depth - 5.*mm; // Estimated

		// 100% coaxial HPGe 
		// with ORTEC serial number 43-TP31670A
		// Property of R.V.F. Janssens group, Argonne National Lab
		HPGe_ANL_31670.detector_radius = 0.5*88.6*mm;
		HPGe_ANL_31670.detector_length = 84.1*mm;
		HPGe_ANL_31670.detector_face_radius = HPGe_ANL_41203.detector_face_radius;
		HPGe_ANL_31670.hole_radius = HPGe_ANL_41203.hole_radius;
		HPGe_ANL_31670.hole_depth = HPGe_ANL_41203.hole_depth;
		HPGe_ANL_31670.hole_face_radius = HPGe_ANL_41203.hole_face_radius;
		HPGe_ANL_31670.mount_cup_thickness = HPGe_ANL_41203.mount_cup_thickness;
		HPGe_ANL_31670.mount_cup_base_thickness = HPGe_ANL_41203.mount_cup_base_thickness;
		HPGe_ANL_31670.mount_cup_material = HPGe_ANL_41203.mount_cup_material;
		HPGe_ANL_31670.end_cap_outer_radius = HPGe_ANL_41203.end_cap_outer_radius;
		HPGe_ANL_31670.end_cap_to_crystal_gap_front = HPGe_ANL_41203.end_cap_to_crystal_gap_front;
		HPGe_ANL_31670.end_cap_thickness = HPGe_ANL_41203.end_cap_thickness;
		HPGe_ANL_31670.end_cap_to_crystal_gap_side = HPGe_ANL_41203.end_cap_to_crystal_gap_side;
		HPGe_ANL_31670.end_cap_window_thickness = HPGe_ANL_41203.end_cap_window_thickness;
		HPGe_ANL_31670.end_cap_length = HPGe_ANL_41203.end_cap_length;
		HPGe_ANL_31670.mount_cup_length = HPGe_ANL_41203.mount_cup_length;
		HPGe_ANL_31670.end_cap_material = HPGe_ANL_41203.end_cap_material;
		HPGe_ANL_31670.end_cap_window_material = HPGe_ANL_41203.end_cap_window_material;
		HPGe_ANL_31670.cold_finger_radius = HPGe_ANL_41203.cold_finger_radius;
		HPGe_ANL_31670.cold_finger_penetration_depth = HPGe_ANL_41203.cold_finger_penetration_depth;
		HPGe_ANL_31670.cold_finger_material = HPGe_ANL_41203.cold_finger_material;
		HPGe_ANL_31670.connection_length = HPGe_ANL_41203.connection_length;
		HPGe_ANL_31670.connection_radius = HPGe_ANL_41203.connection_radius;
		HPGe_ANL_31670.dewar_offset = HPGe_ANL_41203.dewar_offset;
		HPGe_ANL_31670.connection_material = HPGe_ANL_41203.connection_material;
		HPGe_ANL_31670.dewar_length = HPGe_ANL_41203.dewar_length;
		HPGe_ANL_31670.dewar_outer_radius = HPGe_ANL_41203.dewar_outer_radius;
		HPGe_ANL_31670.dewar_wall_thickness = HPGe_ANL_41203.dewar_wall_thickness;
		HPGe_ANL_31670.dewar_material = HPGe_ANL_41203.dewar_material;

		// Clover detector, Eurisys Mesures serial number OC107395
		// Property of Yale University. Also used in the CAGRA campaign.
		HPGe_Clover_Yale.crystal_radius = 0.5*50.*mm;
		HPGe_Clover_Yale.crystal_length = 80.*mm;
		HPGe_Clover_Yale.crystal_face_radius = 2.*mm;
		HPGe_Clover_Yale.crystal_gap = 2.*mm;
		HPGe_Clover_Yale.end_cap_to_crystal_gap_front = 10.*mm;
		HPGe_Clover_Yale.vacuum_length = 125.*mm;
		HPGe_Clover_Yale.end_cap_front_side_length = 101.*mm;
		HPGe_Clover_Yale.end_cap_front_rounding_radius = 15.5*mm;
		HPGe_Clover_Yale.end_cap_front_length = 260.*mm;
		HPGe_Clover_Yale.end_cap_front_thickness = 1.*mm;
		HPGe_Clover_Yale.end_cap_window_thickness = 0.5*mm;
		HPGe_Clover_Yale.end_cap_back_length = 220.*mm;
		HPGe_Clover_Yale.end_cap_back_side_length = 130.*mm;
		HPGe_Clover_Yale.end_cap_back_rounding_radius = 30.*mm;
		HPGe_Clover_Yale.end_cap_back_thickness = 0.5*29.*mm;
		HPGe_Clover_Yale.end_cap_material = "G4_Al";
		HPGe_Clover_Yale.connection_length = 50.*mm;
		HPGe_Clover_Yale.connection_radius = 0.5*70.*mm; // Estimated
		HPGe_Clover_Yale.connection_material = "G4_Al";
		HPGe_Clover_Yale.dewar_length = 268.*mm;
		HPGe_Clover_Yale.dewar_outer_radius = 111.*mm;
		HPGe_Clover_Yale.dewar_wall_thickness = 5.*mm;
		HPGe_Clover_Yale.dewar_material = "G4_Al";
	}
};
