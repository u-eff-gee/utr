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
//	3 inch x 3 inch LaBr Detector of the Galatea Array 
//	at the Institut für Kernphysik, TU Darmstadt
//**************************************************************//

#include <sstream>

#include "G4Color.hh"
#include "G4Cons.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "LaBr_Galatea.hh"
#include "Filter_Case.hh"
#include "Units.hh"

using std::stringstream;

void LaBr_Galatea::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center) const{

	G4NistManager *nist = G4NistManager::Instance();

	G4ThreeVector symmetry_axis(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); // Symmetry axis along which the single elements of the detector are constructed

	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(-phi);
	rotation->rotateY(-theta);

	// Dimensions from a Geant4 simulation of the Galatea array by M. Baumann and D. Symochko
	// The implementation here uses the same dimensions and nomenclature, although they were found to produce volume overlaps and an inconsistent naming.
	// Changes w.r.t. the original implementation are indicated by comments.
	// In the original implementation, everything was implemented in a world filled with air, therefore there is no vacuum layer around the crystal here.

	G4double crystal_length = 3.*inch;
	G4double crystal_radius = 3.*inch * 0.5;
	G4double crystal_housing_inner_radius = crystal_radius + 1.4*mm + 0.5*mm;
	G4double crystal_housing_thickness = 7.*mm;
	G4double crystal_housing_case_length = crystal_length - 19.*mm;

	G4double light_guide_radius = crystal_housing_inner_radius;
	G4double light_guide_thickness = 5.*mm;

	G4double housing_inner_radius = crystal_radius + 1.4*mm;
	G4double housing_thickness = 0.5*mm;
	G4double housing_length = crystal_length+7.3*mm;

	G4double flange_inner_radius = crystal_housing_inner_radius;
	G4double flange_thickness = 25.*mm;
	G4double flange_length = 49.*mm;

	G4double pmt_housing_inner_radius = crystal_housing_inner_radius;
	G4double pmt_housing_thickness = 7.*mm;
	G4double pmt_housing_length = 175.*mm;

	G4double lid_outer_radius = crystal_housing_inner_radius;
	G4double lid_thickness = 4.*mm;

	/************** Crystal housing *************/

	G4Tubs *crystal_housing_case_solid = new G4Tubs(detector_name + "_crystal_housing_case_solid",
			crystal_housing_inner_radius, crystal_housing_inner_radius + crystal_housing_thickness, crystal_housing_case_length*0.5, 0., twopi);
	G4LogicalVolume *crystal_housing_case_logical = new G4LogicalVolume(crystal_housing_case_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_crystal_housing_case_logical");
	crystal_housing_case_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 0.5*crystal_housing_case_length - 7.*mm)*symmetry_axis, crystal_housing_case_logical, detector_name + "_crystal_housing_case", world_Logical, 0, 0, false);

	/************** Housing *************/

	G4Tubs *housing_front_solid = new G4Tubs(detector_name + "_housing_front_solid",
			0., housing_inner_radius, housing_thickness*0.5, 0., twopi);
	G4LogicalVolume *housing_front_logical = new G4LogicalVolume(housing_front_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_housing_front_logical");
	housing_front_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 1.*mm)*symmetry_axis, housing_front_logical, detector_name + "_housing_front", world_Logical, 0, 0, false);

	G4Tubs *housing_solid = new G4Tubs(detector_name + "_housing_solid",
			housing_inner_radius, housing_inner_radius + housing_thickness, housing_length*0.5, 0., twopi);
	G4LogicalVolume *housing_logical = new G4LogicalVolume(housing_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_housing_logical");
	housing_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 0.5*crystal_length + 2.5*mm)*symmetry_axis, housing_logical, detector_name + "_housing", world_Logical, 0, 0, false);

	/************** Flange *************/

	G4Tubs *flange_solid = new G4Tubs(detector_name + "_flange_solid",
			flange_inner_radius, flange_inner_radius + flange_thickness, flange_length*0.5, 0., twopi);
	G4LogicalVolume *flange_logical = new G4LogicalVolume(flange_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_flange_logical");
	flange_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_length - 19.*mm - 7.*mm + 0.5*flange_length)*symmetry_axis, flange_logical, detector_name + "_flange", world_Logical, 0, 0, false);

	/************** PMT Housing *************/

	G4Tubs *pmt_housing_solid = new G4Tubs(detector_name + "_pmt_housing_solid",
			pmt_housing_inner_radius, pmt_housing_inner_radius + pmt_housing_thickness, pmt_housing_length*0.5, 0., twopi);
	G4LogicalVolume *pmt_housing_logical = new G4LogicalVolume(pmt_housing_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_pmt_housing_logical");
	pmt_housing_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_length - 19.*mm - 7.*mm + flange_length + 0.5*pmt_housing_length)*symmetry_axis, pmt_housing_logical, detector_name + "_pmt_housing", world_Logical, 0, 0, false);

	/************** Lid *************/

	G4Tubs *lid_solid = new G4Tubs(detector_name + "_lid_solid",
			0., lid_outer_radius, lid_thickness*0.5, 0., twopi);
	G4LogicalVolume *lid_logical = new G4LogicalVolume(lid_solid, nist->FindOrBuildMaterial("G4_Al"), detector_name + "_lid_logical");
	lid_logical->SetVisAttributes(new G4VisAttributes(G4Color::Grey()));

	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + crystal_length - 19.*mm - 7.*mm + flange_length + pmt_housing_length - 0.5*lid_thickness)*symmetry_axis, lid_logical, detector_name + "_lid", world_Logical, 0, 0, false);

	/************** Detector crystal *************/

	G4Element *nat_La = nist->FindOrBuildElement("La");
	G4Element *nat_Br = nist->FindOrBuildElement("Br");
	G4double labr_density = 5.08 * g / cm3;

	G4Material *LaBr3 = new G4Material("LaBr3", labr_density, 2);
	LaBr3->AddElement(nat_La, 1);
	LaBr3->AddElement(nat_Br, 3);

	G4Tubs *crystal_solid = new G4Tubs(detector_name + "_crystal_solid",
			0., crystal_radius, crystal_length*0.5, 0., twopi);
	G4LogicalVolume *crystal_logical = new G4LogicalVolume(crystal_solid, LaBr3, detector_name);
	crystal_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));

	new G4PVPlacement(0, global_coordinates + (dist_from_center + 0.5*crystal_length + 5.*mm)*symmetry_axis, crystal_logical, detector_name + "_crystal", world_Logical, 0, 0, false);

	/************** Glass light guide *************/

	G4Material *glass = nist->FindOrBuildMaterial("G4_GLASS_PLATE"); // Use Geant4's implementation of glass. The original simulation had SiO_2 with a density of 2.5*g/m3.

	G4Tubs *light_guide_solid = new G4Tubs(detector_name + "_light_guide_solid", 0., light_guide_radius, 0.5*light_guide_thickness, 0., twopi);
	G4LogicalVolume *light_guide_logical = new G4LogicalVolume(light_guide_solid, glass, detector_name + "_light_guide_logical");
	light_guide_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));

	new G4PVPlacement(0, global_coordinates + (dist_from_center + crystal_length + 0.5*light_guide_thickness + 5.2*mm)*symmetry_axis, light_guide_logical, detector_name + "_light_guide", world_Logical, 0, 0, false);

	// Filters
	G4double filter_position_z = 0.; // Will be gradually increased to be able to place
					// filters on top of each other
	if(filter_materials.size()){
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
	
	// Wraps
	if(wrap_materials.size()){
		G4double wrap_radius = crystal_housing_inner_radius + crystal_housing_thickness; // Will be gradually increased to be able to place
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

void LaBr_Galatea::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
		       G4double dist_from_center, G4double intrinsic_rotation_angle) const {
	G4cout << "Warning: Parameter 'intrinsic_rotation_angle=" << intrinsic_rotation_angle << "' given to completely symmetric LaBr_Galatea class was ignored" << G4endl;
	Construct(global_coordinates, theta, phi, dist_from_center);
}
