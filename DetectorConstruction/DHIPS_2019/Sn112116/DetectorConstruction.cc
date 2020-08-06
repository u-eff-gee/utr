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
 * Setup of the Sn112 + Sn116 NRF experiment from the 2020 spring DHIPS
 * campaign. The purpose of this experiment was to measure the relative
 * excitation strength of the 2^+_1 state of 112Sn and 116Sn in NRF.
 * This was supposed to clarify the experimental situation of the 
 * systematics of B(E2; 0^+_1 -> 2^+_1) strengths in the Sn chain.
 */

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
Materials *materials = Materials::Instance();

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

//Detectors
#include "RadiatorTarget.hh"
#include "BeamPipe_Upstream.hh"
#include "BeamPipe_Downstream.hh"
#include "LeadCastle.hh"
#include "Detectors.hh"
#include "Sn112116_Target.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4TwoVector.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	/************************* World volume *****************/

	/***************** WORLD *****************/

	World_x = 6000. * mm;
	World_y = 6000. * mm;
	World_z = 6000. * mm;

	G4Box *World_dim =
	    new G4Box("World_Solid", World_x * 0.5, World_y * 0.5, World_z * 0.5);

	G4LogicalVolume *World_Logical =
	    new G4LogicalVolume(World_dim, air, "World_Logical", 0, 0, 0);

    	G4VisAttributes* world_vis = new G4VisAttributes(true, G4Color::Red());
    	world_vis->SetForceWireframe(true);

	World_Logical->SetVisAttributes(world_vis);

	G4VPhysicalVolume *World_Physical =
	    new G4PVPlacement(0, G4ThreeVector(), World_Logical, "World", 0, false, 0);

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	BeamPipe_Upstream BeamPipe_Upstream(World_Logical);
	RadiatorTarget RadiatorTarget(World_Logical);
	LeadCastle LeadCastle(World_Logical);
	Detectors Detectors(World_Logical);


	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	BeamPipe_Upstream.Construct(G4ThreeVector(0,0,-1500*mm),0.1);  // Position guesstimated
	RadiatorTarget.Construct(G4ThreeVector(0,0,-1190*mm),"Radiator1","Au",2.5*mm,"Al",0.);  // Position guesstimated
	RadiatorTarget.Construct(G4ThreeVector(0,0,-1160*mm),"Radiator2","Au",0.,"Al",0.);  // Position guesstimated

	auto BGO1 = BGO(World_Logical, "BGO1");
	auto BGO2 = BGO(World_Logical, "BGO2");
	auto BGOPol = BGO(World_Logical, "BGOPol");
	auto HPGe80 = DetectorInfo::get(DetectorDHIPS::HPGe80, PositionDHIPS::Right90);
	auto HPGe2 = DetectorInfo::get(DetectorDHIPS::HPGe2, PositionDHIPS::Left130, false);
	auto HPGePol = DetectorInfo::get(DetectorDHIPS::HPGePol, PositionDHIPS::Left90);

	Detectors.ConstructDetectorBGO(G4ThreeVector(), HPGe80, BGO1);
	Detectors.ConstructDetectorBGO(G4ThreeVector(), HPGe2, BGO2);
	Detectors.ConstructDetectorBGO(G4ThreeVector(), HPGePol, BGOPol);
	LeadCastle.Construct(G4ThreeVector(), &BGO1, &BGO2, &BGOPol);
	Detectors.ConstructDetectorFilter(G4ThreeVector(), HPGe80, 10. * mm, 0.);
	Detectors.ConstructDetectorFilter(G4ThreeVector(), HPGe2, 10. * mm, 0.);
	Detectors.ConstructDetectorFilter(G4ThreeVector(), HPGePol, 10. * mm, 0.);

#ifdef USE_TARGETS
	auto rotation = new G4RotationMatrix();
#ifdef ROTATE_TARGET
	rotation->rotateY(180.*deg);
#endif
	Sn112116_Target Sn112116_Target(World_Logical);
	Sn112116_Target.Construct(rotation, G4ThreeVector(0., 0., 0.));
#endif
	
	print_info();
	return World_Physical;

}

void DetectorConstruction::ConstructSDandField() {

	EnergyDepositionSD *HPGe80SD = new EnergyDepositionSD("HPGe80", "HPGe80");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe80SD);
	HPGe80SD->SetDetectorID(1);
	SetSensitiveDetector("HPGe80", HPGe80SD, true);

	EnergyDepositionSD *HPGe2SD = new EnergyDepositionSD("HPGe2", "HPGe2");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2SD);
	HPGe2SD->SetDetectorID(2);
	SetSensitiveDetector("HPGe2", HPGe2SD, true);

	EnergyDepositionSD *HPGePolSD = new EnergyDepositionSD("HPGePol", "HPGePol");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGePolSD);
	HPGePolSD->SetDetectorID(3);
	SetSensitiveDetector("HPGePol", HPGePolSD, true);
}

void DetectorConstruction::print_info() const {
	printf("==============================================================\n");
	printf("  DetectorConstruction: Info (all dimensions in mm)\n");
	G4cout << G4endl;
	printf("> World dimensions             : ( %5.2f, %5.2f, %5.2f )\n", World_x, World_y, World_z);
	printf("==============================================================\n");
}
