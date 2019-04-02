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

#include "G4SystemOfUnits.hh"

#include "HPGe_Coaxial_Properties.hh"
#include "HPGe_Clover_Properties.hh"
#include "Units.hh"

struct HPGe_Collection{
	HPGe_Coaxial_Properties HPGe_60_TUNL_21033;
	HPGe_Clover_Properties HPGe_Clover_Yale;

	HPGe_Collection(){
		// 55% coaxial HPGe with ORTEC serial number 4-TN21638A
		// Property of TUNL
		HPGe_55_TUNL_21638.detector_radius = 0.5*63.2*mm; // (A) in ORTEC data sheet
		HPGe_55_TUNL_21638.detector_length = 84.8*mm; // (B) in ORTEC data sheet
		HPGe_55_TUNL_21638.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_55_TUNL_21638.hole_radius = 0.5*9.2*mm; // (C) in ORTEC data sheet
		HPGe_55_TUNL_21638.hole_depth = 73.1*mm; // (D) in ORTEC data sheet
		HPGe_55_TUNL_21638.hole_face_radius = HPGe_55_TUNL_21638.hole_radius; // Estimated to be the same as hole radius
		HPGe_55_TUNL_21638.mount_cup_length = 6.5*inch; // Measured at the detector. Different from (F) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_55_TUNL_21638.mount_cup_thickness = 0.8*mm; // (K) in ORTEC data sheet
		HPGe_55_TUNL_21638.mount_cup_base_thickness = 3.*mm; // ORTEC data sheet
		HPGe_55_TUNL_21638.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_to_crystal_gap = 4.*mm; // (G) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_thickness = 1.*mm; // (L) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_window_thickness = 0.5*mm; // (I) in ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_21638.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_55_TUNL_21638.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_55_TUNL_21638.cold_finger_penetration_depth = HPGe_55_TUNL_21638.hole_depth - 5.*mm; // Estimated
		HPGe_55_TUNL_21638.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_55_TUNL_21638.connection_length = 1.5*inch; // Measured
		HPGe_55_TUNL_21638.connection_radius = 0.75*inch; // Estimated
		HPGe_55_TUNL_21638.dewar_offset = 0.*inch; // Measured
		HPGe_55_TUNL_21638.connection_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_21638.dewar_length = 12.5*inch; // Measured
		HPGe_55_TUNL_21638.dewar_outer_radius = 4.5*inch; // Measured
		HPGe_55_TUNL_21638.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_55_TUNL_21638.dewar_material = "G4_Al"; // Estimated
		
		// 55% coaxial HPGe with ORTEC serial number 41-TN31524A
		// Property of TUNL
		HPGe_55_TUNL_31524.detector_radius = 0.5*67.1*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.detector_length = 63.1*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_55_TUNL_31524.hole_radius = 0.5*8.7*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.hole_depth = 54.3*mm; // ORTEC data sheet
		HPGe_55_TUNL_31524.hole_face_radius = HPGe_55_TUNL_31524.hole_radius; // Estimated to be the same as hole radius
		HPGe_55_TUNL_31524.mount_cup_length = 6.5*inch; // Measured at the detector. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_55_TUNL_31524.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_55_TUNL_31524.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_55_TUNL_31524.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_to_crystal_gap = 4.*mm; // (B) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_55_TUNL_31524.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_55_TUNL_31524.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_55_TUNL_31524.cold_finger_penetration_depth = HPGe_55_TUNL_31524.hole_depth - 5.*mm; // Estimated
		HPGe_55_TUNL_31524.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_55_TUNL_31524.connection_length = 1.5*inch; // Measured
		HPGe_55_TUNL_31524.connection_radius = 0.75*inch; // Estimated
		HPGe_55_TUNL_31524.dewar_offset = 0.*inch; // Measured
		HPGe_55_TUNL_31524.connection_material = "G4_Al"; // Estimated
		HPGe_55_TUNL_31524.dewar_length = 12.5*inch; // Measured
		HPGe_55_TUNL_31524.dewar_outer_radius = 4.5*inch; // Measured
		HPGe_55_TUNL_31524.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_55_TUNL_31524.dewar_material = "G4_Al"; // Estimated
	
		// 60% coaxial HPGe with ORTEC serial number 36-TN21033A
		// Property of TUNL
		HPGe_60_TUNL_21033.detector_radius = 0.5*68.*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.detector_length = 80.5*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_21033.hole_radius = 0.5*9.3*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.hole_depth = 73.1*mm; // ORTEC data sheet
		HPGe_60_TUNL_21033.hole_face_radius = HPGe_60_TUNL_21033.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_21033.mount_cup_length = 10.5*inch; // Measured at the detector. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_21033.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_21033.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_21033.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_to_crystal_gap = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_21033.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_21033.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_21033.cold_finger_penetration_depth = HPGe_60_TUNL_21033.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_21033.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_21033.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_21033.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_21033.dewar_offset = 1.*inch; // Measured
		HPGe_60_TUNL_21033.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_21033.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_21033.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_21033.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_21033.dewar_material = "G4_Al"; // Estimated

		// 60% coaxial HPGe with ORTEC serial number 36-TN30986A
		// Property of TUNL
		HPGe_60_TUNL_30986.detector_radius = 0.5*68.2*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.detector_length = 77.5*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_30986.hole_radius = 0.5*11.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.hole_depth = 70.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_30986.hole_face_radius = HPGe_60_TUNL_30986.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_30986.mount_cup_length = 10.5*inch; // Measured at the detector. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_30986.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_30986.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_30986.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_to_crystal_gap = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_30986.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_30986.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_30986.cold_finger_penetration_depth = HPGe_60_TUNL_30986.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_30986.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_30986.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_30986.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_30986.dewar_offset = 0.*inch; // Measured
		HPGe_60_TUNL_30986.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_30986.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_30986.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_30986.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_30986.dewar_material = "G4_Al"; // Estimated

		// 60% coaxial HPGe with ORTEC serial number 36-TN31061A
		// Property of TUNL
		//HPGe_60_TUNL_31061.detector_radius = 0.5*64.6*mm; // ORTEC data sheet
		//HPGe_60_TUNL_31061.detector_length = 80.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.detector_radius = 0.5*64.5*mm; // According to B. Fallin, this detector was reconditioned, so the crystal length and radius are smaller than in the data sheet
		HPGe_60_TUNL_31061.detector_length = 79.4*mm; // see above
		HPGe_60_TUNL_31061.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_31061.hole_radius = 0.5*11.2*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.hole_depth = 72.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_31061.hole_face_radius = HPGe_60_TUNL_31061.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_31061.mount_cup_length = 10.5*inch; // Measured at the detector. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_31061.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_31061.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_31061.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_to_crystal_gap = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_31061.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_31061.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_31061.cold_finger_penetration_depth = HPGe_60_TUNL_31061.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_31061.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_31061.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_31061.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_31061.dewar_offset = 0.*inch; // Measured
		HPGe_60_TUNL_31061.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_31061.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_31061.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_31061.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_31061.dewar_material = "G4_Al"; // Estimated
		
		// 60% coaxial HPGe with ORTEC serial number 36-TN40663A
		// Property of TUNL
		HPGe_60_TUNL_40663.detector_radius = 0.5*69.7*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.detector_length = 68.3*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.detector_face_radius = 8.*mm; // (J) in ORTEC data sheet
		HPGe_60_TUNL_40663.hole_radius = 0.5*9.9*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.hole_depth = 60.*mm; // ORTEC data sheet
		HPGe_60_TUNL_40663.hole_face_radius = HPGe_60_TUNL_40663.hole_radius; // Estimated to be the same as hole radius
		HPGe_60_TUNL_40663.mount_cup_length = 10.5*inch; // Measured at the detector. Different from (A) in ORTEC data sheet, because it shows only the part which contains the detector crystal.
		HPGe_60_TUNL_40663.mount_cup_thickness = 0.76*mm; // (H) in ORTEC data sheet
		HPGe_60_TUNL_40663.mount_cup_base_thickness = 3.2*mm; // (C) in ORTEC data sheet
		HPGe_60_TUNL_40663.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_to_crystal_gap = 4.*mm; // (B) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_thickness = 1.*mm; // (I) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_window_thickness = 0.5*mm; // (D) in ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_material = "G4_Al"; // ORTEC data sheet
		HPGe_60_TUNL_40663.end_cap_window_material = "G4_Be"; // ORTEC data sheet
		HPGe_60_TUNL_40663.cold_finger_radius = 0.5*4.*mm; // Suggestion by B. Fallin, Duke University. Not in ORTEC data sheet
		HPGe_60_TUNL_40663.cold_finger_penetration_depth = HPGe_60_TUNL_40663.hole_depth - 5.*mm; // Estimated
		HPGe_60_TUNL_40663.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_60_TUNL_40663.connection_length = 2.*inch; // Measured
		HPGe_60_TUNL_40663.connection_radius = 0.75*inch; // Estimated
		HPGe_60_TUNL_40663.dewar_offset = 0.*inch; // Measured
		HPGe_60_TUNL_40663.connection_material = "G4_Al"; // Estimated
		HPGe_60_TUNL_40663.dewar_length = 9.*inch; // Measured
		HPGe_60_TUNL_40663.dewar_outer_radius = 3.*inch; // Measured
		HPGe_60_TUNL_40663.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_60_TUNL_40663.dewar_material = "G4_Al"; // Estimated
		
		// 100% (102% according to data sheet) coaxial HPGe with
		// CANBERRA serial number 73760
		// Informally known as 'Detector 1'
		// Property of IKP TU Darmstadt, under repair at the time these dimensions were
		// implemented
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		// The outer radius of the detector as given in that simulation (50.0 mm) is improbable,
		// given the radius of the crystal (39.45 mm) and its distance from the
		// cap (3.5 mm) in the data sheet. Assuming that the distance to the cap includes
		// the thickness of the mount cup and the vacuum layer, this would mean that the
		// end cap has a thickness of 6.05 mm, which is very large.
		// Preferred an estimated end cap thickness.
		HPGe_100_TUD_73760.detector_radius = 0.5*78.9*mm; // CANBERRA data sheet
		HPGe_100_TUD_73760.detector_length = 79.*mm; // CANBERRA data sheet
		HPGe_100_TUD_73760.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_73760.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_73760.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_73760.hole_face_radius = HPGe_100_TUD_73760.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_73760.mount_cup_length = 150.*mm; // Not included in original simulation
		HPGe_100_TUD_73760.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_73760.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_73760.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_73760.end_cap_to_crystal_gap = 3.5*mm - HPGe_100_TUD_73760.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_73760.end_cap_thickness = 1.*mm; // In the original simulation, the outer radius of the end cap was given as 50 mm. See also the general comments on this detector.
		HPGe_100_TUD_73760.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_73760.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_73760.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_73760.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_73760.cold_finger_penetration_depth = HPGe_100_TUD_73760.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.
		HPGe_100_TUD_73760.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_73760.connection_length = 2.*inch; // Estimated
		HPGe_100_TUD_73760.connection_radius = 0.75*inch; // Estimated
		HPGe_100_TUD_73760.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_73760.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_73760.dewar_length = 9.*inch; // Estimated
		HPGe_100_TUD_73760.dewar_outer_radius = 3.*inch; // Estimated
		HPGe_100_TUD_73760.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_73760.dewar_material = "G4_Al"; // Estimated
	
		// 100% (98% according to data sheet) coaxial HPGe with
		// CANBERRA serial number 72902
		// Informally known as 'Detector 2'
		// Property of IKP TU Darmstadt
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		// The outer radius of the detector as given in that simulation (50.0 mm) is improbable,
		// given the radius of the crystal (38.5 mm) and its distance from the
		// cap (5.0 mm) in the data sheet. Assuming that the distance to the cap includes
		// the thickness of the mount cup and the vacuum layer, this would mean that the
		// end cap has a thickness of 6.5 mm, which is very large.
		// Preferred an estimated end cap thickness.
		HPGe_100_TUD_72902.detector_radius = 0.5*77.*mm; // CANBERRA data sheet
		HPGe_100_TUD_72902.detector_length = 80.*mm; // CANBERRA data sheet
		HPGe_100_TUD_72902.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_72902.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_72902.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_72902.hole_face_radius = HPGe_100_TUD_72902.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_72902.mount_cup_length = 258.*mm; // Not included in original simulation
		HPGe_100_TUD_72902.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72902.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_72902.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72902.end_cap_to_crystal_gap = 5.0*mm - HPGe_100_TUD_72902.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_72902.end_cap_thickness = 1.*mm; // In the original simulation, the outer radius of the end cap was given as 50 mm. see also the general comments on this detector.
		HPGe_100_TUD_72902.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72902.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_72902.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_72902.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_72902.cold_finger_penetration_depth = HPGe_100_TUD_72902.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.
		HPGe_100_TUD_72902.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_72902.connection_length = 60.*mm; // Measured
		HPGe_100_TUD_72902.connection_radius = 25.*mm; // Measured
		HPGe_100_TUD_72902.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_72902.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72902.dewar_length = 415.*mm; // Measured
		HPGe_100_TUD_72902.dewar_outer_radius = 110.*mm; // Measured
		HPGe_100_TUD_72902.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_72902.dewar_material = "G4_Al"; // Estimated

		// 100% (99% according to data sheet from 2019, 100% according to data sheet
		// from 1997) segmented coaxial HPGe with
		// CANBERRA serial number 72930
		// Informally known as 'Polarimeter'
		// Property of IKP TU Darmstadt
		// Reconstructed from old Geant4 simulation by M. Fritzsche, C. Romig et al.
		// The detector was repaired in 2018, but the dimension of the crystal stayed the same
		// The outer radius of the detector as given in that simulation (49.5 mm) is improbable,
		// given the radius of the crystal (39.0 mm) and its distance from the
		// cap (5.0 mm) in the data sheet. Assuming that the distance to the cap includes
		// the thickness of the mount cup and the vacuum layer, this would mean that the
		// end cap has a thickness of 5.5 mm, which is very large.
		// Preferred an estimated end cap thickness.
		HPGe_100_TUD_72930.detector_radius = 0.5*78.*mm; // Eurisys data sheet
		HPGe_100_TUD_72930.detector_length = 78.*mm; // Eurisys data sheet
		HPGe_100_TUD_72930.detector_face_radius = 2.*mm; // Not included in original simulation
		HPGe_100_TUD_72930.hole_radius = 0.5*8.*mm; // Estimated
		HPGe_100_TUD_72930.hole_depth = 48.*mm; // Estimated. In the original simulation, the cold finger was simply placed inside the Germanium crystal. Constructed the hole such that it is 2mm wider than the cold finger
		HPGe_100_TUD_72930.hole_face_radius = HPGe_100_TUD_72930.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_TUD_72930.mount_cup_length = 350.*mm; // Not included in original simulation
		HPGe_100_TUD_72930.mount_cup_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72930.mount_cup_base_thickness = 3.*mm; // Not included in original simulation
		HPGe_100_TUD_72930.mount_cup_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72930.end_cap_to_crystal_gap = 5.0*mm - HPGe_100_TUD_72930.mount_cup_thickness; // Estimated from the distance of the crystal to the end cap in the CANBERRA data sheet
		HPGe_100_TUD_72930.end_cap_thickness = 1.*mm; // In the original simulation, the outer radius of the end cap was given as 50 mm. Constructed the thickness of the (newly added) mount cup, vacuum layer and this value here to end up with a radius of 50 mm again.
		HPGe_100_TUD_72930.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_100_TUD_72930.end_cap_material = "G4_Al"; // From original simulation
		HPGe_100_TUD_72930.end_cap_window_material = "G4_Be"; // Estimated
		HPGe_100_TUD_72930.cold_finger_radius = 0.5*6.*mm; // Estimated. The original simulation has a radius of 6 mm in it, which seemed extremely large. Assumed that the diameter and radius were confused.
		HPGe_100_TUD_72930.cold_finger_penetration_depth = HPGe_100_TUD_72930.hole_depth - 2.*mm; // Estimated. See also the comment to the hole depth.
		HPGe_100_TUD_72930.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_TUD_72930.connection_length = 50.*mm; // Measured
		HPGe_100_TUD_72930.connection_radius = 25.*mm; // Measured
		HPGe_100_TUD_72930.dewar_offset = 0.*inch; // Estimated
		HPGe_100_TUD_72930.connection_material = "G4_Al"; // Estimated
		HPGe_100_TUD_72930.dewar_length = 270.*mm; // Measured
		HPGe_100_TUD_72930.dewar_outer_radius = 110.*mm; // Measured
		HPGe_100_TUD_72930.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_TUD_72930.dewar_material = "G4_Al"; // Estimated

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
		HPGe_100_Cologne_73954.hole_face_radius = HPGe_100_Cologne_73954.hole_radius; // Estimated to be the same as hole radius
		HPGe_100_Cologne_73954.mount_cup_length = 10.5*inch; // Measured at the detector. Different from g4horus, because it implements only the part which contains the detector crystal.
		HPGe_100_Cologne_73954.mount_cup_thickness = 1.*mm;
		HPGe_100_Cologne_73954.mount_cup_base_thickness = 5.*mm;
		HPGe_100_Cologne_73954.mount_cup_material = "G4_Al"; // ORTEC data sheet
		HPGe_100_Cologne_73954.end_cap_to_crystal_gap = 3.*mm;
		HPGe_100_Cologne_73954.end_cap_thickness = 1.*mm;
		HPGe_100_Cologne_73954.end_cap_window_thickness = 1.*mm;
		HPGe_100_Cologne_73954.end_cap_material = "G4_Al";
		HPGe_100_Cologne_73954.end_cap_window_material = "G4_Al";
		HPGe_100_Cologne_73954.cold_finger_radius = 0.5*4.*mm; // Estimated
		HPGe_100_Cologne_73954.cold_finger_penetration_depth = HPGe_100_Cologne_73954.hole_depth - 5.*mm; // Estimated
		HPGe_100_Cologne_73954.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_100_Cologne_73954.connection_length = 3.5*inch; // Measured
		HPGe_100_Cologne_73954.connection_radius = 0.75*inch; // Measured
		HPGe_100_Cologne_73954.dewar_offset = 0.*inch; // Measured
		HPGe_100_Cologne_73954.connection_material = "G4_Al"; // Estimated
		HPGe_100_Cologne_73954.dewar_length = 10.5*inch; // Measured
		HPGe_100_Cologne_73954.dewar_outer_radius = 5.*inch; // Measured
		HPGe_100_Cologne_73954.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_100_Cologne_73954.dewar_material = "G4_Al"; // Estimated
		
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
		HPGe_86_Stuttgart_31120.hole_depth = 0.8*detector_length; // Estimated by C. Fransen
		HPGe_86_Stuttgart_31120.hole_face_radius = HPGe_86_Stuttgart_31120.hole_radius; // Estimated to be the same as hole radius
		HPGe_86_Stuttgart_31120.mount_cup_length = 178.*mm; // CT scan
		HPGe_86_Stuttgart_31120.mount_cup_thickness = 0.76*mm; // Estimated
		HPGe_86_Stuttgart_31120.mount_cup_base_thickness = 6.*mm; // CT scan
		HPGe_86_Stuttgart_31120.mount_cup_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_to_crystal_gap = 5.*mm; // CANBERRA data sheet
		HPGe_86_Stuttgart_31120.end_cap_thickness = 1.*mm; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_window_thickness = 0.5*mm; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.end_cap_window_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.cold_finger_radius = 0.5*8.*mm; // CT scan
		HPGe_86_Stuttgart_31120.cold_finger_penetration_depth = HPGe_86_Stuttgart_31120.hole_depth - 5.*mm; // Estimated
		HPGe_86_Stuttgart_31120.cold_finger_material = "G4_Cu"; // Estimated
		HPGe_86_Stuttgart_31120.connection_length = 13.1*inch; // Measured
		HPGe_86_Stuttgart_31120.connection_radius = 16.*mm; // Measured
		HPGe_86_Stuttgart_31120.dewar_offset = 0.*inch; // Measured
		HPGe_86_Stuttgart_31120.connection_material = "G4_Al"; // Estimated
		HPGe_86_Stuttgart_31120.dewar_length = 31.5*inch; // Measured
		HPGe_86_Stuttgart_31120.dewar_outer_radius = 6.*inch; // Measured
		HPGe_86_Stuttgart_31120.dewar_wall_thickness = 5.*mm; // Estimated
		HPGe_86_Stuttgart_31120.dewar_material = "G4_Al"; // Estimated

		// 100% (?) coaxial HPGe of R.V.F. Janssens group
		// No datasheet available, all dimensions from Cologne 73954 detector,
		// since this one was the most similar (however, it is still a very
		// crude approximation!)
		// Property of Argonne National Lab
		HPGe_ANL.detector_radius = 0.5*74*mm; 
		HPGe_ANL.detector_length = 70.*mm;
		HPGe_ANL.detector_face_radius = 2.*mm;
		HPGe_ANL.hole_radius = 5.8*mm;
		HPGe_ANL.hole_depth = 52.*mm;
		HPGe_ANL.hole_face_radius = HPGe_ANL.hole_radius;
		HPGe_ANL.mount_cup_length = 10.5*inch;
		HPGe_ANL.mount_cup_thickness = 1.*mm;
		HPGe_ANL.mount_cup_base_thickness = 5.*mm;
		HPGe_ANL.mount_cup_material = "G4_Al";
		HPGe_ANL.end_cap_to_crystal_gap = 3.*mm;
		HPGe_ANL.end_cap_thickness = 1.*mm;
		HPGe_ANL.end_cap_window_thickness = 1.*mm;
		HPGe_ANL.end_cap_material = "G4_Al";
		HPGe_ANL.end_cap_window_material = "G4_Al";
		HPGe_ANL.cold_finger_radius = 0.5*4.*mm;
		HPGe_ANL.cold_finger_penetration_depth = HPGe_ANL.hole_depth - 5.*mm;
		HPGe_ANL.cold_finger_material = "G4_Cu";
		HPGe_ANL.connection_length = 3.5*inch; 
		HPGe_ANL.connection_radius = 0.75*inch;
		HPGe_ANL.dewar_offset = 0.*inch;
		HPGe_ANL.connection_material = "G4_Al";
		HPGe_ANL.dewar_length = 10.5*inch;
		HPGe_ANL.dewar_outer_radius = 5.*inch; 
		HPGe_ANL.dewar_wall_thickness = 5.*mm; 
		HPGe_ANL.dewar_material = "G4_Al";

		// Clover detector, Eurisys Mesures serial number OC107395
		// Property of Yale University. Also used in the CAGRA campaign.
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
