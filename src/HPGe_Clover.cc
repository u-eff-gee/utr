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
 * General topology of a clover detector was taken from an
 * Eurysis manual for clover detectors.
 */

#include "G4Box.hh"
#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "HPGe_Clover.hh"

HPGe_Clover::HPGe_Clover(HPGe_Clover_Properties prop, G4LogicalVolume *World_Logical, G4String name):
	world_Logical(World_Logical),
	properties(prop),
	detector_name(name)
{}

void HPGe_Clover::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center){

	G4NistManager *nist = G4NistManager::Instance();
	G4ThreeVector symmetry_axis(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); // Symmetry axis along which the single elements of the detector are constructed

	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(-phi);
	rotation->rotateY(-theta);

	/******** End cap *********/

	G4Box *end_cap_vacuum_solid = new G4Box("end_cap_vacuum_solid", 50.*mm, 50.*mm, 50.*mm);
	G4LogicalVolume* end_cap_vacuum_logical = new G4LogicalVolume(end_cap_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"), "end_cap_vacuum_logical");
	end_cap_vacuum_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
	new G4PVPlacement(rotation, global_coordinates + (dist_from_center + 50.*mm)*symmetry_axis, end_cap_vacuum_logical, "end_cap_vacuum", world_Logical, 0, 0, false);

	/******** Crystals ********/

	G4Tubs *crystal_original = new G4Tubs("crystal_original", 0., properties.crystal_radius, properties.crystal_length*0.5, 0., twopi);
	G4Box *subtraction_solid = new G4Box("subtraction_solid", properties.crystal_radius, properties.crystal_radius, properties.crystal_length);
	G4SubtractionSolid *crystal_step1_solid = new G4SubtractionSolid("crystal_step1_solid", crystal_original, subtraction_solid, 0, G4ThreeVector(properties.crystal_radius + 23.*mm, 0., 0.));
	G4SubtractionSolid *crystal_step2_solid = new G4SubtractionSolid("crystal_step2_solid", crystal_step1_solid, subtraction_solid, 0, G4ThreeVector(-properties.crystal_radius - 22.*mm, 0., 0.));
	G4SubtractionSolid *crystal_step3_solid = new G4SubtractionSolid("crystal_step3_solid", crystal_step2_solid, subtraction_solid, 0, G4ThreeVector(0., -properties.crystal_radius - 22.*mm, 0.));
	G4SubtractionSolid *crystal_step4_solid = new G4SubtractionSolid("crystal_step4_solid", crystal_step3_solid, subtraction_solid, 0, G4ThreeVector(0., properties.crystal_radius + 23.*mm, 0.));

	G4LogicalVolume *crystal1_logical = new G4LogicalVolume(crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"), "crystal1_logical");
	crystal1_logical->SetVisAttributes(new G4VisAttributes(G4Color::Green()));
	new G4PVPlacement(0, G4ThreeVector(22.*mm, 22.*mm, 0.), crystal1_logical, "crystal1", end_cap_vacuum_logical, 0, 0, false);

	G4RotationMatrix *rotate2 = new G4RotationMatrix();
	rotate2->rotateZ(-90.*deg);
	G4LogicalVolume *crystal2_logical = new G4LogicalVolume(crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"), "crystal2_logical");
	crystal2_logical->SetVisAttributes(new G4VisAttributes(G4Color::Red()));
	new G4PVPlacement(rotate2, G4ThreeVector(-22.*mm, 22.*mm, 0.), crystal2_logical, "crystal2", end_cap_vacuum_logical, 0, 0, false);

	G4RotationMatrix *rotate3 = new G4RotationMatrix();
	rotate3->rotateZ(-270.*deg);
	G4LogicalVolume *crystal3_logical = new G4LogicalVolume(crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"), "crystal3_logical");
	crystal3_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
	new G4PVPlacement(rotate3, G4ThreeVector(22.*mm, -22.*mm, 0.), crystal3_logical, "crystal3", end_cap_vacuum_logical, 0, 0, false);

	G4RotationMatrix *rotate4 = new G4RotationMatrix();
	rotate4->rotateZ(-180.*deg);
	G4LogicalVolume *crystal4_logical = new G4LogicalVolume(crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"), "crystal4_logical");
	crystal4_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
	new G4PVPlacement(rotate4, G4ThreeVector(-22.*mm, -22.*mm, 0.), crystal4_logical, "crystal4", end_cap_vacuum_logical, 0, 0, false);
}
