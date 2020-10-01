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

/*
 * 112Sn target from 2015 DHIPS experiment [1]
 * The target consists of three different quasi-monoisotopic materials
 * and 5 distinct parts, ordered from upstream to downstream:
 *
 * I    59Co
 * II   27Al
 * III 112Sn
 * IV   27Al
 * V    59Co
 *
 * The materials were filled into a target container made of polyvinyl-chloride.
 *
 * [1] M. Schilling, MSc thesis, TU Darmstadt (2016)
 */

#include <sstream>
#include <vector>

#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Sn112_Target.hh"

using std::stringstream;
using std::vector;

Sn112_Target::Sn112_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Sn112_Target::Construct(G4ThreeVector global_coordinates){

	// Target data
	container_inner_radius = 10.*mm;
	G4double container_outer_radius = 11.*mm;
	G4double container_lid_thickness = 2.*mm;

	G4double al27_I_density = 2.70*g/cm3; // Density of natural aluminium from Wikipedia
	G4double al27_I_mass = 1.7055*g; // +- 0.0003 g
	al27_I_thickness = al27_I_mass/(al27_I_density*pi*container_inner_radius*container_inner_radius);

	G4double co59_II_density = 8.90*g/cm3; // Density of natural cobalt from Wikipedia
	G4double co59_II_mass = 0.7689*g; // +- 0.0003 g
	co59_II_thickness = co59_II_mass/(co59_II_density*pi*container_inner_radius*container_inner_radius);

	G4double sn112_III_density = 7.265*g/cm3; // Density of natural beta-tin from Wikipedia.
	G4double sn112_III_mass = 4.4285*g; // +- 0.0003 g
	sn112_III_thickness = sn112_III_mass/(sn112_III_density*pi*container_inner_radius*container_inner_radius);

	G4double co59_IV_density = 8.90*g/cm3; // Density of natural cobalt from Wikipedia
	G4double co59_IV_mass = 0.7511*g; // +- 0.0003 g
	co59_IV_thickness = co59_IV_mass/(co59_IV_density*pi*container_inner_radius*container_inner_radius);

	G4double al27_V_density = 2.70*g/cm3; // Density of natural aluminium from Wikipedia
	G4double al27_V_mass = 1.7067*g; // +- 0.0003 g
	al27_V_thickness = al27_V_mass/(al27_V_density*pi*container_inner_radius*container_inner_radius);

	G4double container_inner_length = (
		al27_I_thickness +
		co59_II_thickness +
		sn112_III_thickness +
		co59_IV_thickness +
		al27_V_thickness);
	
	// Create / load materials
	G4NistManager *nist = G4NistManager::Instance();

	// Natural cobalt
	G4Material *nat_Co = nist->FindOrBuildMaterial("G4_Co");
	
	// Natural aluminium
	G4Material *nat_Al = nist->FindOrBuildMaterial("G4_Al");

	// Enriched 112Sn
	G4Isotope *sn112 = new G4Isotope("Sn112", 50, 62, 111.904824877*g/mole); // Enrichment was actually 99.994%, but assume here that the target was monoisotopic.
	G4Element *sn112_element = new G4Element("Sn112_element", "Sn", 1);
	sn112_element->AddIsotope(sn112, 100.*perCent);
	G4Material *sn112_material = new G4Material("Sn112_material", sn112_III_density, 1);
	sn112_material->AddElement(sn112_element, 1);

	// Construct target container
	// Container wall
	G4Tubs *container_wall_solid = new G4Tubs("container_wall_solid", container_inner_radius, container_outer_radius, container_inner_length*0.5, 0., twopi);
	G4LogicalVolume *container_wall_logical = new G4LogicalVolume(container_wall_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "container_wall_logical");
	container_wall_logical->SetVisAttributes(G4Color::White());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(), container_wall_logical, "container_wall", World_Logical, false, 0);

	// Container bottom and top
	G4Tubs *container_lid_solid = new G4Tubs("container_lid_solid", 0., container_outer_radius, container_lid_thickness*0.5, 0., twopi);

	G4LogicalVolume *container_bottom_logical = new G4LogicalVolume(container_lid_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "container_bottom_logical");
	container_bottom_logical->SetVisAttributes(G4Color::Grey());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., -0.5*container_lid_thickness - 0.5*container_inner_length), container_bottom_logical, "container_bottom", World_Logical, false, 0);

	G4LogicalVolume *container_top_logical = new G4LogicalVolume(container_lid_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "container_top_logical");
	container_top_logical->SetVisAttributes(G4Color::Grey());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., 0.5*container_lid_thickness + 0.5*container_inner_length), container_top_logical, "container_top", World_Logical, false, 0);

	// Construct targets in a loop
	vector<G4String> target_names = {"Al27_I", "Co59_II", "Sn112_III", "Co59_IV", "Al27_V"};
	vector<G4Material*> target_materials = {nat_Al, nat_Co, sn112_material, nat_Co, nat_Al};
	vector<G4Tubs*> target_solids;
	vector<G4LogicalVolume*> target_logicals;
	vector<G4Color> target_colors = {
		G4Color::Cyan(),
		G4Color::Green(),
		G4Color::Yellow(),
		G4Color::Green(),
		G4Color::Cyan(),
	};
	vector<double> target_thicknesses = {
		al27_I_thickness,
		co59_II_thickness,
		sn112_III_thickness,
		co59_IV_thickness,
		al27_V_thickness,
	};

	stringstream name;
	G4double next_target_z = 0.;
	G4ThreeVector pos;

	printf("==============================================================\n");
	printf("  Sn112 Target: Info (all dimensions in mm)\n");
	for(unsigned int i = 0; i < target_materials.size(); ++i){
		name << target_names[i] << "solid";
		target_solids.push_back(new G4Tubs(name.str(), 0., container_inner_radius, target_thicknesses[i]*0.5, 0., twopi));
		name.str("");

		name << target_names[i] << "logical";
		target_logicals.push_back(new G4LogicalVolume(target_solids[i], target_materials[i], name.str()));
		target_logicals[i]->SetVisAttributes(target_colors[i]);
		name.str("");

		name << target_names[i];
		pos = G4ThreeVector(0., 0., -0.5*container_inner_length + next_target_z + 0.5*target_thicknesses[i]);
		new G4PVPlacement(0, global_coordinates + pos, target_logicals[i], name.str(), World_Logical, false, 0);
		printf("> %s position   (source)  : ( %8.5f, %8.5f, %8.5f )\n", name.str().c_str(), pos.getX(), pos.getY(), pos.getZ());
		printf("> %s dimensions (sourceD) : ( %8.5f, %8.5f, %8.5f )\n", name.str().c_str(), 2.*container_inner_radius, 2.*container_inner_radius, target_thicknesses[i]);
		name.str("");
		next_target_z += target_thicknesses[i];
	}
	printf("==============================================================\n");
}
