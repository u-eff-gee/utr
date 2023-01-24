/*
utr - Geant4 simulation of DHIPS at TU Darmstadt
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
 * Setup of the Sn112 NRF experiment from the 2015 DHIPS campaign
 * The purpose of this experiment was to measure the excitation strength
 * of the 2^+_1 state of 112Sn in NRF relative to the known strengths
 * of excited states in 27Al and 59Co. This was supposed to clarify the
 * experimental situation of the systematics of B(E2; 0^+_1 -> 2^+_1)
 * strengths in the tin isotopic chain.
 * 
 * An alternative would have been to determine two B(E2) strengths
 * of the tin isotopes 112Sn and 116Sn simultaneously. A sandwich
 * target which would be necessary for this alternative experiment
 * has also been implemented.
*/

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


// #include "G4UnitsTable.hh"
#include "utrConfig.h"
#include <iostream>
#include <array>



G4VPhysicalVolume *DetectorConstruction::Construct() {

	/***************** Define Lengths (along beam direction) ************/ 

	targetposition_z = 0*mm;
	target_length = 1*mm;
	const double target_radius = 10*mm;
	bremstarget_thickness1 = 0.5*mm;
	bremstarget_thickness2 = 2.5*mm;
	distance_bremstargets = 15*mm;  //guesstimated distance between bremstargets
	const double bremstarget_edge_length = 10*mm;
	collimator_to_bremstarget = 20*mm;
	collimator_to_target = 162*mm;
	const int N_long = 4;
	const int N_short = 10;
	const double blocklength_short = 100*mm;
	const double blocklength_long =120*mm;
	const double blocklength_last1 =20*mm;
	const double blocklength_last2 =98*mm;
	const double blocklength_last3 =2*mm;
	const double r_hole_short_max = 10.5 *mm;   //maximum hole radius for short blocks of the collimator. First hole has r=6mm, then each hole has 0.5mm more.
	const double r_hole_long = 12 *mm;        
	const double r_last1 = 12*mm;      //hole radii of last collimator block
	const double r_last2 = 17.5*mm;
	const double r_last3 = 24.75*mm;
	total_collimator_length = N_short*blocklength_short + N_long*blocklength_long;
	const double block_buffer_length = 40*mm;
	const double world_buffer_length = 10*mm;
	const double world_buffer_length_z = 40*mm;  //additional space for z axis.

	block_x = (r_last3 + block_buffer_length);  //Collimator edge length depending on target radius (in reality ~300mm).
	block_y = (r_last3 + block_buffer_length);
	
	World_z = (target_length + collimator_to_target + total_collimator_length + collimator_to_bremstarget + bremstarget_thickness1 + bremstarget_thickness2 + distance_bremstargets + world_buffer_length + world_buffer_length_z);
	World_x = block_x + world_buffer_length;
	World_y = block_y + world_buffer_length;

	 
	/***************** Define Materials ************/

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *vacuum  = nist->FindOrBuildMaterial("G4_Galactic");  	
	G4Material *gold    = nist->FindOrBuildMaterial("G4_Au");              	


	/******************** WORLD ******************/
	G4Box *World_solid = new G4Box("World_solid", World_x, World_y, World_z);
	World_logical = new G4LogicalVolume(World_solid, vacuum, "World_logical", 0, 0, 0);   

	//Visualisierung der Welt (Farbe)
	G4VisAttributes *world_vis = new G4VisAttributes(true, G4Color::Red());
	world_vis->SetForceWireframe(true);

	World_logical->SetVisAttributes(world_vis);
	G4VPhysicalVolume *World_physical = new G4PVPlacement(0, G4ThreeVector(), World_logical, "World", 0, false, 0);


	
	/******************** Bremsttarget 1 (1st target with 0.5mm) ******************/
	G4Box *Bremstarget_solid1 = new G4Box("Bremstarget_solid1", bremstarget_edge_length * 0.5, bremstarget_edge_length * 0.5, bremstarget_thickness1 * 0.5);
	G4LogicalVolume *Bremstarget_logical1 = new G4LogicalVolume(Bremstarget_solid1, gold, "Bremstarget_logical1", 0, 0, 0);

	//Visualisierung (Farbe)
	Bremstarget_logical1->SetVisAttributes(new G4VisAttributes(G4Color::Yellow()));
	new G4PVPlacement(0, G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - total_collimator_length - collimator_to_bremstarget - bremstarget_thickness2 - distance_bremstargets - bremstarget_thickness1/2),   Bremstarget_logical1, "Bremstarget", World_logical, false, 0);


	/******************** Bremsttarget 2 (2nd target with 2.5mm) ******************/
	G4Box *Bremstarget_solid2 = new G4Box("Bremstarget_solid2", bremstarget_edge_length * 0.5, bremstarget_edge_length * 0.5, bremstarget_thickness2 * 0.5);
	G4LogicalVolume *Bremstarget_logical2 = new G4LogicalVolume(Bremstarget_solid2, gold, "Bremstarget_logical2", 0, 0, 0);

	//Visualisierung (Farbe)
	Bremstarget_logical2->SetVisAttributes(new G4VisAttributes(G4Color::Yellow()));
	new G4PVPlacement(0, G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - total_collimator_length - collimator_to_bremstarget - bremstarget_thickness2/2), Bremstarget_logical2, "Bremstarget", World_logical, false, 0);	
	
	
	
	/******************** Detector ******************/
	G4Tubs *Detector_solid = new G4Tubs("Detector_solid", 0, target_radius, target_length * 0.5, 0, twopi);
	G4LogicalVolume *Detector_logical = new G4LogicalVolume(Detector_solid, vacuum, "Detector_logical", 0, 0, 0);

	//Visualisierung (Farbe)
	Detector_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
	new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Detector_logical, "Detector", World_logical, false, 0);
	
	
	
	/******************** Collimator ******************/
	//Collimator consists of 14 blocks in total. the first 10 blocks have a length of 100mm and an increasing hole radius from 6mm to 10.5mm in steps of +0.5mm. The next 3 blocks have a length of 100mm and an increasing hole radius from 10.5mm to 12mm in steps of +0.5mm. The last block has 3 different hole radii with different lengths: 1st with length 2mm and radius 12mm, 2nd with length ~98mm and radius 17.5mm, 3rd with length 20mm and radius 24.75mm.

	//Loop for 10 short Cu blocks with increasing hole radii from 6 to 10.5 mm in steps of +0.5 mm per block. 
	for (int i = 0; i < N_short; ++i){

		G4String name = ("Block_short" + std::to_string(i));
		G4double r_step = 0.5*mm;
		G4double holeradius = r_hole_short_max - r_step * i;
		G4ThreeVector local_coordinates = G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - N_long*blocklength_long - blocklength_short/2 - blocklength_short*i);    
		
		//Main method to construct collimator blocks.
		ConstructCollimatorBlocks(name, local_coordinates, blocklength_short, holeradius);	
	}

	//Loop for 3 long Cu blocks with increasing hole radii from 10.5 to 12 mm in steps of +0.5 mm per block. 
	for (int i = 0; i < 3; ++i){

		G4String name = ("Block_long" + std::to_string(i));
		G4double r_step = 0.5*mm;
		G4double holeradius = r_hole_long - r_step * i;
		G4ThreeVector local_coordinates = G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - 1*blocklength_long - blocklength_long/2 - blocklength_long*i);   
		
		//Main method to construct collimator blocks.
		ConstructCollimatorBlocks(name, local_coordinates, blocklength_long, holeradius);	
	}

	//Last block with 3 different radii.
	G4String last_block_name1 = std::string("LastBlock_part1");
	G4String last_block_name2 = std::string("LastBlock_part2");
	G4String last_block_name3 = std::string("LastBlock_part3");
	ConstructCollimatorBlocks(last_block_name1, G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - blocklength_last3 - blocklength_last2 - blocklength_last1/2), blocklength_last1, r_last1);
	ConstructCollimatorBlocks(last_block_name2, G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - blocklength_last3 - blocklength_last2/2), blocklength_last2, r_last2);
	ConstructCollimatorBlocks(last_block_name3, G4ThreeVector(0, 0, targetposition_z - target_length/2 - collimator_to_target - blocklength_last3/2), blocklength_last3, r_last3); //very last block in front of target
	
	
	print_info();
	return World_physical;
}

//Method for creating single collimator blocks with a name, a position at local_coordinates, block length block_z and a hole radius holeradius. Tube (hole) is placed in the center of the block.
void DetectorConstruction::ConstructCollimatorBlocks(G4String &name, G4ThreeVector local_coordinates, G4double block_z, G4double holeradius) {

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *Cu  = nist->FindOrBuildMaterial("G4_Cu");
	G4Colour light_orange = G4Colour(1.0, 0.82, 0.36);

	auto *collimatorSolidSubstractionBox = new G4Box(name, block_x * 0.5, block_y * 0.5, block_z * 0.5);
	auto *collimatorSolidSubstractionHole = new G4Tubs(name, 0., holeradius, block_z * 0.51, 0., twopi);
	auto *collimatorSolid = new G4SubtractionSolid("collimatorSolid", collimatorSolidSubstractionBox, collimatorSolidSubstractionHole);
	auto *collimatorLogical = new G4LogicalVolume(collimatorSolid, Cu, "collimatorLogical");
	new G4PVPlacement(0, local_coordinates, collimatorLogical, "collimator", World_logical, false, 0);
	collimatorLogical->SetVisAttributes(light_orange);
}


// Definiere das Detektorvolumen als Detektor/sensitives Volumen in Geant4
void DetectorConstruction::ConstructSDandField() {

	// Use ParticleSD instead of EnergyDepositionSD, as ParticleSD records the hits of each particle within a event individually regardless whether the particle actually deposited energy in the detector or not.
	// An EnergyDepositionSD however only records a single particle per event and only if it actually left some energy in the detector
	ParticleSD *DetectorSD = new ParticleSD("Detector_logical", "Detector_logical");
	G4SDManager::GetSDMpointer()->AddNewDetector(DetectorSD);
	DetectorSD->SetDetectorID(0);
	SetSensitiveDetector("Detector_logical", DetectorSD, true);
}

void DetectorConstruction::print_info() const {
	printf("==============================================================\n");
	printf("  DetectorConstruction: Info (all dimensions in mm)\n");
	G4cout << G4endl;
	printf("> World dimensions:           ( %5.2f, %5.2f, %5.2f )\n", World_x*2, World_y*2, World_z*2);
	printf("> Position z Target:          ( %5.2f)               \n", targetposition_z);
	printf("> Position z Collimator end:  ( %5.2f)               \n", targetposition_z - target_length/2 - collimator_to_target);
	printf("> Position z Collimator start:( %5.2f)               \n", targetposition_z - target_length/2 - collimator_to_target - total_collimator_length);
	printf("> Position z Bremstarget1:    ( %5.2f)               \n", targetposition_z - target_length/2 - collimator_to_target - total_collimator_length - collimator_to_bremstarget - bremstarget_thickness2 - distance_bremstargets - bremstarget_thickness1/2);
	printf("> Position z Bremstarget2:    ( %5.2f)               \n", targetposition_z - target_length/2 - collimator_to_target - total_collimator_length - collimator_to_bremstarget - bremstarget_thickness2/2);
	printf("==============================================================\n");
}
