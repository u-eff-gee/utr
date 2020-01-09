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
Setup for runs 252 - 270
*/

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "Collimator_Room.hh"
#include "Room.hh"
#include "Beampipe_Upstream.hh"
#include "Beampipe_Downstream.hh"
#include "First_UTR_Wall.hh"
#include "First_Setup.hh"
#include "G3_Wall_243_279.hh"
#include "Detectors_G3_242_270.hh"
#include "Wheel.hh"
#include "G3_Table.hh"
#include "Table2_243_279.hh"
#include "Detectors_2nd_252_270.hh"
#include "ZeroDegree_Setup.hh"
#include "Ni64_Target.hh"
#include "Ni64_Sobotka_Target.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

DetectorConstruction::DetectorConstruction() {}

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
	 * WHEEL (g3 wheel)
	 * G3_TABLE
	 * TABLE_2 (the table on which the second setup is mounted)
	 * DETECTORS_2ND (detectors in second setup)
	 * ZERODEGREE_SETUP (zero-degree detector)
	 * G3_TARGET
	 * SECOND_TARGET
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
	G4double First_Setup_To_G3_Wall = 3.5*inch;
	G3_Target_To_2nd_Target = 62.*inch; // Estimated
	G4double ZeroDegree_To_2nd_Target = 980.*mm;

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	Collimator_Room collimator_Room(World_Logical, 0.5*0.75*inch);
	Room room(World_Logical);
	Beampipe_Upstream beampipe_Upstream(World_Logical);
	First_UTR_Wall first_UTR_Wall(World_Logical);
	First_Setup first_Setup(World_Logical);
	G3_Wall_243_279 g3_Wall(World_Logical); // Was not there in these runs. However, it still defines the floor height, so it is needed here
	Detectors_G3_242_270 detectors_G3(World_Logical);
	Wheel wheel(World_Logical);
	G3_Table g3_Table(World_Logical);
	Table2_243_279 table2(World_Logical);
	Beampipe_Downstream beampipe_Downstream(World_Logical);
	Detectors_2nd_252_270 detectors_2nd(World_Logical);	
	ZeroDegree_Setup zeroDegree_Setup(World_Logical);
	Ni64_Target g3_Target;
	Ni64_Sobotka_Target second_Target;

	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	/***************** COLLIMATOR_ROOM *****************/

	collimator_Room.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length()*0.5));
	Collimator_Entrance_To_G3_Target = Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length();

	/***************** ROOM *****************/

	room.Construct(G4ThreeVector(), World_x, World_y, World_z,
            Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length(),
	g3_Wall.Get_Floor_Level());

	/***************** BEAMPIPE_UPSTREAM *****************/

	beampipe_Upstream.Construct(G4ThreeVector(0., 0., beampipe_Upstream.Get_Z_Axis_Offset_Z()), 1e-2);

	/***************** FIRST_UTR_WALL *****************/

	first_UTR_Wall.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length()*0.5));

	/***************** FIRST_SETUP *****************/

	first_Setup.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()*0.5));

	/***************** G3_WALL *****************/

	g3_Wall.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel + First_Setup_To_G3_Wall + g3_Wall.Get_Length()*0.5));

	/***************** DETECTORS_G3 *****************/

	detectors_G3.Construct(G4ThreeVector(0., 0., 0.));

	/***************** WHEEL *****************/

	wheel.Construct(G4ThreeVector(0., 0., Wheel_To_Target + wheel.Get_Length()*0.5));

	/***************** G3_TABLE *****************/

	g3_Table.Construct(G4ThreeVector(0., 0., Wheel_To_Target + wheel.Get_Length() + g3_Table.Get_Length()*0.5));

	/***************** TABLE_2 *****************/

	table2.Construct(G4ThreeVector(0., 0.,  Wheel_To_Target + wheel.Get_Length() + g3_Table.Get_Length() + table2.Get_Length()*0.5 + table2.Get_Z_Axis_Offset_Z()));

	/***************** BEAMPIPE_DOWNSTREAM *****************/

	beampipe_Downstream.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + beampipe_Downstream.Get_Z_Axis_Offset_Z()), 1e-2);

	/***************** DETECTORS_2ND *****************/

	detectors_2nd.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target));
	
	/***************** ZERODEGREE_SETUP *****************/

#ifdef USE_ZERODEGREE
	zeroDegree_Setup.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + ZeroDegree_To_2nd_Target));
#endif

#ifdef USE_TARGETS	
	/***************** G3_TARGET *****************/

	g3_Target.Set_Containing_Volume(beampipe_Upstream.Get_Beampipe_Vacuum());
	g3_Target.Construct(G4ThreeVector(0., 0., -beampipe_Upstream.Get_Z_Axis_Offset_Z()));

	/***************** SECOND_TARGET *****************/

	second_Target.Set_Containing_Volume(beampipe_Downstream.Get_Beampipe_Vacuum());
	second_Target.Construct(G4ThreeVector(0., 0., -beampipe_Downstream.Get_Z_Axis_Offset_Z()));
#endif

	print_info();

	return World_Physical;
}

void DetectorConstruction::ConstructSDandField() {

	/********* ZeroDegree detector *******/

#ifdef USE_ZERODEGREE
	EnergyDepositionSD *ZeroDegreeSD = new EnergyDepositionSD("ZeroDegree", "ZeroDegree");
	G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
	ZeroDegreeSD->SetDetectorID(0);
	SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);
#endif

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

	EnergyDepositionSD *LaBr1SD = new EnergyDepositionSD("LaBr1", "LaBr1");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr1SD);
	LaBr1SD->SetDetectorID(5);
	SetSensitiveDetector("LaBr1", LaBr1SD, true);

	EnergyDepositionSD *LaBr2SD = new EnergyDepositionSD("LaBr2", "LaBr2");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr2SD);
	LaBr2SD->SetDetectorID(6);
	SetSensitiveDetector("LaBr2", LaBr2SD, true);

	EnergyDepositionSD *LaBr3SD = new EnergyDepositionSD("LaBr3", "LaBr3");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr3SD);
	LaBr3SD->SetDetectorID(7);
	SetSensitiveDetector("LaBr3", LaBr3SD, true);

	EnergyDepositionSD *LaBr4SD = new EnergyDepositionSD("LaBr4", "LaBr4");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr4SD);
	LaBr4SD->SetDetectorID(8);
	SetSensitiveDetector("LaBr4", LaBr4SD, true);

	/*************** Second setup **************/

	//EnergyDepositionSD *HPGe9SD = new EnergyDepositionSD("HPGe9", "HPGe9");
	//G4SDManager::GetSDMpointer()->AddNewDetector(HPGe9SD);
	//HPGe9SD->SetDetectorID(9);
	//SetSensitiveDetector("HPGe9", HPGe9SD, true);

	EnergyDepositionSD *HPGe10SD = new EnergyDepositionSD("HPGe10", "HPGe10");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe10SD);
	HPGe10SD->SetDetectorID(10);
	SetSensitiveDetector("HPGe10", HPGe10SD, true);

	EnergyDepositionSD *HPGe11SD = new EnergyDepositionSD("HPGe11", "HPGe11");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe11SD);
	HPGe11SD->SetDetectorID(11);
	SetSensitiveDetector("HPGe11", HPGe11SD, true);

	EnergyDepositionSD *HPGe12SD = new EnergyDepositionSD("HPGe12", "HPGe12");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe12SD);
	HPGe12SD->SetDetectorID(12);
	SetSensitiveDetector("HPGe12", HPGe12SD, true);
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
