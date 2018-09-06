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
Setup for runs 3 - 9
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

#include "Collimator_Room.hh"
#include "Room.hh"
#include "First_UTR_Wall.hh"
#include "First_Setup.hh"
#include "Detectors_G3_3_9.hh"
#include "ZeroDegree_Setup.hh"
#include "Ar40_Target.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

DetectorConstruction::DetectorConstruction():
G3_Target_To_2nd_Target(0.),
Collimator_Entrance_To_G3_Target(0.)
{}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

	/*
	 * Fast-forward to specific parts of the geometry by searching for
	 * COLLIMATOR_ROOM (Collimator, paddle and shielding in collimator room)
	 * ROOM (UTR walls and floor)
	 * WORLD (world volume)
	 * BEAMPIPE_UPSTREAM 
	 * BEAMPIPE_DOWNSTREAM
	 * FIRST_UTR_WALL
	 * FIRST_SETUP (first setup upstream of g3)
	 * G3_WALL (wall immediately in front of g3)
	 * DETECTORS_G3 (detectors in g3)
	 * ZERODEGREE_SETUP (zero-degree detector)
	 * G3_TARGET
	 */

	G4Colour white(1.0, 1.0, 1.0);
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour black(0.0, 0.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	G4Colour cyan(0.0, 1.0, 1.0);
	G4Colour magenta(1.0, 0.0, 1.0);
	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour light_orange(1.0, 0.82, 0.36);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	/***************** WORLD *****************/

	World_x = 3000. * mm;
	World_y = 3150. * mm;
	World_z = 8000. * mm;

	G4Box *World_dim =
	    new G4Box("World_Solid", World_x * 0.5, World_y * 0.5, World_z * 0.5);

	G4LogicalVolume *World_Logical =
	    new G4LogicalVolume(World_dim, air, "World_Logical", 0, 0, 0);

	//World_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
    	G4VisAttributes* world_vis = new G4VisAttributes(true, red);
    	world_vis->SetForceWireframe(true);

	World_Logical->SetVisAttributes(world_vis);

	G4VPhysicalVolume *World_Physical =
	    new G4PVPlacement(0, G4ThreeVector(), World_Logical, "World", 0, false, 0);

	/***************** GENERAL DIMENSIONS *****************/

	G4double Wheel_To_Target = 3.*inch;
	G4double First_Setup_To_Wheel = 34.*inch;
	G4double First_UTR_Wall_To_First_Setup = 4.2*inch;
	G3_Target_To_2nd_Target = 62.*inch; // Estimated
	G4double ZeroDegree_To_2nd_Target = 980.*mm;

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	Collimator_Room collimator_Room(World_Logical, 0.5*0.75*inch);
	Room room(World_Logical);
	First_UTR_Wall first_UTR_Wall(World_Logical);
	First_Setup first_Setup(World_Logical);
	Detectors_G3_3_9 detectors_G3(World_Logical);
	ZeroDegree_Setup zeroDegree_Setup(World_Logical);
	Ar40_Target g3_Target(World_Logical);

	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	/***************** COLLIMATOR_ROOM *****************/

	collimator_Room.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length()*0.5));
	Collimator_Entrance_To_G3_Target = Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length();

	/***************** ROOM *****************/

	room.Construct(G4ThreeVector(), World_x, World_y, World_z,
            Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length(),
	-51.*inch); // Hard-coded floor level because no part of the geometry is touching the floor.

	/***************** FIRST_UTR_WALL *****************/

	first_UTR_Wall.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length()*0.5));

	/***************** FIRST_SETUP *****************/

	first_Setup.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()*0.5));

	/***************** DETECTORS_G3 *****************/

	detectors_G3.Construct(G4ThreeVector(0., 0., 0.));
	
	/***************** ZERODEGREE_SETUP *****************/

	zeroDegree_Setup.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + ZeroDegree_To_2nd_Target));

#ifdef USE_TARGETS	
	/***************** G3_TARGET *****************/

	g3_Target.Construct(G4ThreeVector(0., 0., 0.));
#endif

	print_info();

	return World_Physical;
}

void DetectorConstruction::ConstructSDandField() {

	/********* ZeroDegree detector *******/

	EnergyDepositionSD *ZeroDegreeSD = new EnergyDepositionSD("ZeroDegree", "ZeroDegree");
	G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
	ZeroDegreeSD->SetDetectorID(0);
	SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);

	/*************** Gamma3 **************/

	EnergyDepositionSD *HPGe1SD = new EnergyDepositionSD("HPGe1", "HPGe1");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe1SD);
	HPGe1SD->SetDetectorID(1);
	SetSensitiveDetector("HPGe1", HPGe1SD, true);

	EnergyDepositionSD *HPGe2SD = new EnergyDepositionSD("HPGe2", "HPGe2");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2SD);
	HPGe2SD->SetDetectorID(2);
	SetSensitiveDetector("HPGe2", HPGe2SD, true);

	EnergyDepositionSD *HPGe3SD = new EnergyDepositionSD("HPGe3", "HPGe3");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe3SD);
	HPGe3SD->SetDetectorID(3);
	SetSensitiveDetector("HPGe3", HPGe3SD, true);

	EnergyDepositionSD *HPGe4SD = new EnergyDepositionSD("HPGe4", "HPGe4");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe4SD);
	HPGe4SD->SetDetectorID(4);
	SetSensitiveDetector("HPGe4", HPGe4SD, true);
}

void DetectorConstruction::print_info() const {
	printf("==============================================================\n");
	printf("  DetectorConstruction: Info (all dimensions in mm)\n");
	G4cout << G4endl;
	printf("> Collimator entrance position : ( %5.2f, %5.2f, %5.2f )\n", 0., 0., Collimator_Entrance_To_G3_Target);
	printf("> Ideal position of G3 target  : ( %5.2f, %5.2f, %5.2f )\n", 0., 0., 0.);
	printf("> Ideal position of 2nd target : ( %5.2f, %5.2f, %5.2f )\n", 0., 0., G3_Target_To_2nd_Target);
	printf("> World dimensions             : ( %5.2f, %5.2f, %5.2f )\n", World_x, World_y, World_z);
	printf("==============================================================\n");
}
