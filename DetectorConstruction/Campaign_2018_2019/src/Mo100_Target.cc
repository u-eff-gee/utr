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

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Mo100_Target.hh"
#include "Units.hh"

Mo100_Target::Mo100_Target():
World_Logical(nullptr)
{}

Mo100_Target::Mo100_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Mo100_Target::Construct(G4ThreeVector global_coordinates){

	G4NistManager *nist = G4NistManager::Instance();

	/*************************************************/
	// Enriched 100Mo target from 2018 NRF experiments
	// Property of Technische Universität Darmstadt
	// All information about the enriched material from the
	// data sheet if not indicated otherwise
	// All information about target container from technical drawing
	/*************************************************/

	// Target material
	// Atomic masses from Atomic Mass Evaluation
	G4Isotope *Mo92  = new G4Isotope("92Mo",  42,  92, 91.906807155*g/mole);
	G4Isotope *Mo94  = new G4Isotope("94Mo",  42,  94, 93.905083592*g/mole);
	G4Isotope *Mo95  = new G4Isotope("95Mo",  42,  95, 94.905837442*g/mole);
	G4Isotope *Mo96  = new G4Isotope("96Mo",  42,  96, 95.904674774*g/mole);
	G4Isotope *Mo97  = new G4Isotope("97Mo",  42,  97, 96.906016903*g/mole);
	G4Isotope *Mo98  = new G4Isotope("98Mo",  42,  98, 97.905403608*g/mole);
	G4Isotope *Mo100 = new G4Isotope("100Mo", 42, 100, 99.907467976*g/mole);
	G4Element *nat_O = nist->FindOrBuildElement("O");

	G4Element *enriched_Mo100 =
	    new G4Element("enriched Mo100", "Mo_100e", 7);

	enriched_Mo100->AddIsotope(  Mo92,  0.09 * perCent);
	enriched_Mo100->AddIsotope(  Mo94,  0.06 * perCent);
	enriched_Mo100->AddIsotope(  Mo95,  0.10 * perCent);
	enriched_Mo100->AddIsotope(  Mo96,  0.11 * perCent);
	enriched_Mo100->AddIsotope(  Mo97,  0.08 * perCent);
	enriched_Mo100->AddIsotope(  Mo98,  0.55 * perCent);
	enriched_Mo100->AddIsotope( Mo100, 99.01 * perCent);

	G4double molybdenum_oxide_mass = 2951.0 * mg; // Measured at HIGS

	G4double molybdenum_oxide_density = 4.69*g/cm3; // Density of natural MoO_3 from Wikipedia

	G4Material* target_Mo100 = new G4Material("target_Mo100", molybdenum_oxide_density, 2);
	target_Mo100->AddElement(enriched_Mo100, 1);
	target_Mo100->AddElement(nat_O, 3);

	// Target dimensions
	G4double container_inner_radius = 10.*mm;
	G4double container_outer_radius = 11.*mm;
	G4double container_length = 20.*mm;
	G4double container_lid_thickness = 2.*mm;
	G4double molybdenum_oxide_length = molybdenum_oxide_mass/molybdenum_oxide_density/(pi*container_inner_radius*container_inner_radius);

	// Target container
	// Container wall
	G4Tubs *container_wall_solid = new G4Tubs("Mo100_container_wall_solid", container_inner_radius, container_outer_radius, container_length*0.5, 0., twopi);
	G4LogicalVolume *container_wall_logical = new G4LogicalVolume(container_wall_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "Mo100_container_wall_logical");
	container_wall_logical->SetVisAttributes(G4Color::Grey());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., 0.5*container_length - container_lid_thickness - molybdenum_oxide_length*0.5), container_wall_logical, "Mo100_container_wall", World_Logical, false, 0);

	// Container bottom and top
	G4Tubs *container_lid_solid = new G4Tubs("Mo100_container_lid_solid", 0., container_inner_radius, container_lid_thickness*0.5, 0., twopi);

	G4LogicalVolume *container_bottom_logical = new G4LogicalVolume(container_lid_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "Mo100_container_bottom_logical");
	container_bottom_logical->SetVisAttributes(G4Color::Grey());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., -0.5*container_lid_thickness - 0.5*molybdenum_oxide_length), container_bottom_logical, "Mo100_container_bottom", World_Logical, false, 0);

	G4LogicalVolume *container_top_logical = new G4LogicalVolume(container_lid_solid, nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"), "Mo100_container_top_logical");
	container_top_logical->SetVisAttributes(G4Color::Grey());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., 0.5*container_lid_thickness + 0.5*molybdenum_oxide_length), container_top_logical, "Mo100_container_top", World_Logical, false, 0);

	// Target Material
	G4Tubs *Mo100_solid = new G4Tubs("Mo100_container_bottom_solid", 0., container_inner_radius, molybdenum_oxide_length*0.5, 0., twopi);
	G4LogicalVolume *Mo100_logical = new G4LogicalVolume(Mo100_solid, target_Mo100, "Mo100_container_bottom_logical");
	Mo100_logical->SetVisAttributes(G4Color::Yellow());
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(), Mo100_logical, "Mo100_container_bottom", World_Logical, false, 0);
};
