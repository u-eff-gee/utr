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

#include <sstream>

#include "G4Color.hh"
#include "G4Cons.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "LaBr_3x3.hh"
#include "Filter_Case.hh"
#include "Units.hh"

using std::stringstream;

void LaBr_3x3::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center) const{

	G4NistManager *nist = G4NistManager::Instance();

	G4ThreeVector symmetry_axis(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); // Symmetry axis along which the single elements of the detector are constructed

	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(-phi);
	rotation->rotateY(-theta);

	// Dimensions from 
	// 1) A previous implementation by B. Loeher and J. Isaak (BI) (crystal, vacuum and crystal housing)
	// 2) A measurement in 2018
	// if not indicated otherwise.
	
	// Only the outer dimensions of the housing, but no details about the inside, are known. This 
	// is why all the back parts are simply constructed as hollow aluminium. The housing is 
	// only implemented for aesthetic reasons and can be switched off via the 'use_housing' flag.

	G4double crystal_length = 3.*inch; // BI
	G4double crystal_radius = 3.*inch * 0.5; // BI
	G4double crystal_housing_thickness = 0.5*mm; // BI
	G4double crystal_housing_outer_radius = 3.25*inch*0.5; // BI
	G4double vacuum_thickness_front = 2.*mm; // BI
	G4double vacuum_thickness_back = 2.*mm; //BI

	G4double circuit_housing_thickness = 2.*mm; // Estimated, determines the thickness of 
						// all circuit_housing elements
	G4double circuit_housing_1_length = 2.25*inch; // Measured
	G4double circuit_housing_1_radius = 3.5*inch *0.5; // Measured

	G4double circuit_housing_2_length = 3.*cm; // Measured
	G4double circuit_housing_2_rmin = 2.5*inch * 0.5; // Measured
	G4double circuit_housing_2_rmax = 3.5*inch * 0.5; // Measured

	
	G4double circuit_housing_3_length = 3.*cm; // Measured
	G4double circuit_housing_3_radius = 2.5*inch *0.5; // Measured

	G4double pmt_housing_length = 7.*cm; // Measured
	G4double pmt_housing_radius = 2.5*inch * 0.5; // Measured

	/************** Crystal housing *************/

	G4Tubs *crystal_housing_face_solid = new G4Tubs(detector_name + "_crystal_housing_face_solid",
			0., crystal_housing_outer_radius, crystal_housing_thickness*0.5, 0., twopi);
	G4LogicalVolume *crystal_housing_face_logical = new G4LogicalVolume(crystal_housing_face_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_face_logical");
	crystal_housing_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 0.5*crystal_housing_thickness)*symmetry_axis, crystal_housing_face_logical, detector_name + "_crystal_housing_face", world_Logical, 0, 0, false);

	G4double crystal_housing_case_length = vacuum_thickness_back + vacuum_thickness_front + crystal_length + 2.*crystal_housing_thickness;
	G4Tubs *crystal_housing_case_solid = new G4Tubs(detector_name + "_crystal_housing_case_solid",
			crystal_housing_outer_radius - crystal_housing_thickness, crystal_housing_outer_radius, crystal_housing_case_length*0.5, 0., twopi);
	G4LogicalVolume *crystal_housing_case_logical = new G4LogicalVolume(crystal_housing_case_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_case_logical");
	crystal_housing_case_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length*0.5)*symmetry_axis, crystal_housing_case_logical, detector_name + "_crystal_housing_case", world_Logical, 0, 0, false);

	if(!use_housing){
		// If the construction of the dummy housing is not desired, close the housing with
		// a bottom
		G4LogicalVolume *crystal_housing_bottom_logical = new G4LogicalVolume(crystal_housing_face_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_bottom_logical");
		crystal_housing_bottom_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));
		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 1.5*crystal_housing_thickness + crystal_housing_case_length)*symmetry_axis, crystal_housing_bottom_logical, detector_name + "_crystal_housing_bottom", world_Logical, 0, 0, false);
	}

	/************** Vacuum around crystal *************/

	G4Tubs *vacuum_solid = new G4Tubs(detector_name + "_vacuum_solid", 0.,
			crystal_housing_outer_radius - crystal_housing_thickness, crystal_housing_case_length*0.5, 0., twopi);
	G4LogicalVolume *vacuum_logical = new G4LogicalVolume(vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"), detector_name + "_vacuum_logical");
	vacuum_logical->SetVisAttributes(new G4VisAttributes(G4Color::Cyan()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length*0.5)*symmetry_axis, vacuum_logical, detector_name + "_vacuum", world_Logical, 0, 0, false);

	/************** Detector crystal *************/

	G4Tubs *crystal_solid = new G4Tubs(detector_name + "_crystal_solid",
			0., crystal_radius, crystal_length*0.5, 0., twopi);
	G4LogicalVolume *crystal_logical = new G4LogicalVolume(crystal_solid, nist->FindOrBuildMaterial("Brillance380"), detector_name);
	crystal_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

	new G4PVPlacement(0, G4ThreeVector(0., 0., vacuum_thickness_front + 0.5*(crystal_length -crystal_housing_case_length)), crystal_logical, detector_name + "_crystal", vacuum_logical, 0, 0, false);

	if(use_housing){

		/************** Circuit housing 1 *************/
		
		G4Tubs *circuit_housing_1_solid = new G4Tubs(detector_name + "_circuit_housing_1_solid",
				crystal_housing_outer_radius, circuit_housing_1_radius, circuit_housing_1_length*0.5, 0., twopi);
		G4LogicalVolume *circuit_housing_1_logical = new G4LogicalVolume(circuit_housing_1_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_1_logical");
		circuit_housing_1_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length*0.5)*symmetry_axis, circuit_housing_1_logical, detector_name + "_circuit_housing_1", world_Logical, 0, 0, false);

		/************** Circuit housing 2 *************/

		G4Cons *circuit_housing_2_solid =
		    new G4Cons("circuit_housing_2_solid", circuit_housing_2_rmax - circuit_housing_thickness, circuit_housing_2_rmax, circuit_housing_2_rmin - circuit_housing_thickness, circuit_housing_2_rmin, circuit_housing_2_length*0.5, 0., twopi);
		G4LogicalVolume *circuit_housing_2_logical = new G4LogicalVolume(circuit_housing_2_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_2_logical");
		circuit_housing_2_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length + circuit_housing_2_length*0.5)*symmetry_axis, circuit_housing_2_logical, detector_name + "_circuit_housing_2", world_Logical, 0, 0, false);

		/************** Circuit housing 3 *************/
		
		G4Tubs *circuit_housing_3_solid = new G4Tubs(detector_name + "_circuit_housing_3_solid",
				circuit_housing_3_radius - circuit_housing_thickness, circuit_housing_3_radius, circuit_housing_3_length*0.5, 0., twopi);
		G4LogicalVolume *circuit_housing_3_logical = new G4LogicalVolume(circuit_housing_3_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_circuit_housing_3_logical");
		circuit_housing_3_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length  + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length*0.5)*symmetry_axis, circuit_housing_3_logical, detector_name + "_circuit_housing_3", world_Logical, 0, 0, false);

		/************** Circuit housing 4 *************/
		
		G4Tubs *pmt_housing_solid = new G4Tubs(detector_name + "_pmt_housing_solid",
				pmt_housing_radius - circuit_housing_thickness, pmt_housing_radius, pmt_housing_length*0.5, 0., twopi);
		G4LogicalVolume *pmt_housing_logical = new G4LogicalVolume(pmt_housing_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_pmt_housing_logical");
		pmt_housing_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length  + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length + pmt_housing_length*0.5)*symmetry_axis, pmt_housing_logical, detector_name + "_pmt_housing", world_Logical, 0, 0, false);

		G4Tubs *pmt_housing_bottom_solid = new G4Tubs(detector_name + "_pmt_housing_solid",
				0., pmt_housing_radius - circuit_housing_thickness, circuit_housing_thickness*0.5, 0., twopi);
		G4LogicalVolume *pmt_housing_bottom_logical = new G4LogicalVolume(pmt_housing_bottom_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_pmt_housing_bottom_logical");
		pmt_housing_bottom_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

		new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_housing_thickness + crystal_housing_case_length + circuit_housing_1_length + circuit_housing_2_length + circuit_housing_3_length + pmt_housing_length - circuit_housing_thickness*0.5)*symmetry_axis, pmt_housing_bottom_logical, detector_name + "_pmt_housing_bottom", world_Logical, 0, 0, false);
	}

	// Filter case
	Filter_Case filter_case(world_Logical, detector_name);
	if(use_filter_case_ring){
		filter_case.Construct_Ring(global_coordinates, theta, phi,
				dist_from_center - 
				filter_case.get_filter_case_ring_thickness()*0.5);
	}

	// Filters
	G4double filter_position_z = 0.; // Will be gradually increased to be able to place
					// filters on top of each other
	if(filter_materials.size()){
		if(use_filter_case_ring)
			filter_position_z = filter_position_z + filter_case.get_filter_case_ring_thickness();
		G4Tubs *filter_solid = nullptr;
		G4LogicalVolume *filter_logical = nullptr;
		stringstream filter_solid_name, filter_logical_name, filter_name;
		for(unsigned int i = 0; i < filter_materials.size(); ++i){
			filter_solid_name << "filter_" << detector_name << "_" << i << "_solid";
			filter_solid = new G4Tubs(filter_solid_name.str(), 0., filter_radii[i],
					filter_thicknesses[i]*0.5, 0., twopi);
			filter_solid_name.clear();

			filter_logical_name << "filter_" << detector_name << "_" << i << "_logical";
			filter_logical = new G4LogicalVolume(filter_solid,
					nist->FindOrBuildMaterial(filter_materials[i]),
					filter_logical_name.str());
			filter_logical_name.clear();
			if(i % 2 == 0){
				filter_logical->SetVisAttributes(new G4VisAttributes(G4Color::Red()));
			} else{
				filter_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));
			}

			filter_name << "filter_" << detector_name << "_" << i;
			new G4PVPlacement(rotation, global_coordinates + (dist_from_center - filter_position_z - filter_thicknesses[i]*0.5)*symmetry_axis, filter_logical, filter_name.str(), world_Logical, 0, 0, false);
			filter_name.clear();
			filter_position_z = filter_position_z + filter_thicknesses[i];
		}
	}

	if(use_filter_case){
		filter_case.Construct_Case(global_coordinates, theta, phi,
				dist_from_center - filter_case.get_filter_case_bottom_thickness()*0.5-
				filter_position_z);
	}
	
	// Wraps
	if(wrap_materials.size()){
		G4double wrap_radius = crystal_housing_outer_radius; // Will be gradually increased to be able to place
					// wraps on top of each other
		G4Tubs *wrap_solid = nullptr;
		G4LogicalVolume *wrap_logical = nullptr;
		stringstream wrap_solid_name, wrap_logical_name, wrap_name;
		for(unsigned int i = 0; i < wrap_materials.size(); ++i){
			wrap_solid_name << "wrap_" << detector_name << "_" << i << "_solid";
			wrap_solid = new G4Tubs(wrap_solid_name.str(), wrap_radius, wrap_radius + wrap_thicknesses[i], (crystal_housing_case_length + crystal_housing_thickness)*0.5, 0., twopi);
			wrap_solid_name.clear();

			wrap_logical_name << "wrap_" << detector_name << "_" << i << "_logical";
			wrap_logical = new G4LogicalVolume(wrap_solid,
					nist->FindOrBuildMaterial(wrap_materials[i]),
					wrap_logical_name.str());
			wrap_logical_name.clear();
			wrap_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

			wrap_name << "wrap_" << detector_name << "_" << i;
			new G4PVPlacement(rotation, global_coordinates + (dist_from_center + (crystal_housing_thickness + crystal_housing_case_length)*0.5)*symmetry_axis, wrap_logical, wrap_name.str(), world_Logical, 0, 0, false);
			wrap_name.clear();
			wrap_radius = wrap_radius + wrap_thicknesses[i];
		}
	}
}

void LaBr_3x3::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center, G4double intrinsic_rotation_angle) const {
	G4cout << "Warning: Parameter 'intrinsic_rotation_angle=" << intrinsic_rotation_angle << "' given to completely symmetric LaBr_3x3 class was ignored" << G4endl;
	Construct(global_coordinates, theta, phi, dist_from_center);
}
