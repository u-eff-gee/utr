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
Setup for the 150Sm Runs (Runs 563 (?) to 615) belonging to the 2014 campaign although conducted in February 2015.
Setup is based on the values given in section 3.3 of the B.Sc. thesis "Zerfallsverhalten der Scherenmode in der 0νββ-Tochter ¹⁵⁰Sm"  
(Decay Characteristics of the Scissors Mode in the 0νββ-Daughter ¹⁵⁰Sm )" of J. Kleemann at TU Darmstadt (2016) (and his private paper logbook),
as the ELOG hosted by the Goethe University Frankfurt apparently lost some entries on the setup at some point...
*/

#include "DetectorConstruction.hh"

#include "NamedColors.hh"

// Materials
#include "G4NistManager.hh"
#include "Units.hh"
#include "Materials.hh"

// Geometry
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "Collimator_Room.hh"
#include "Room.hh"
#include "Beampipe_Upstream.hh"
#include "First_UTR_Wall.hh"
#include "First_Setup.hh"
#include "Detectors_G3_150Sm.hh"
#include "Wheel.hh"
#include "G3_Table.hh"
#include "Table2.hh"
#include "ZeroDegree_Setup.hh"
#include "Sm150_Target.hh"

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
	 * FIRST_UTR_WALL
	 * FIRST_SETUP (first setup upstream of g3)
	 * G3_WALL (wall immediately in front of g3, not used here)
	 * DETECTORS_G3 (detectors in g3)
	 * WHEEL (g3 wheel)
	 * G3_TABLE
	 * TABLE_2 (the table on which the second setup is mounted)
	 * DETECTORS_2ND (detectors in second setup) - No information about the 2nd setup's geometry available for this experiment, so don't implement it at all
	 * ZERODEGREE_SETUP (zero-degree detector)
	 * G3_TARGET
	 * SECOND_TARGET (no second target used here!)
	 */

	
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
	G4double Floor_Level=-8.*inch*0.5 - 6.*inch - 41.*inch; //From G3_Wall_*.cc of 2018_2019 geometries
	G3_Target_To_2nd_Target = 62.*inch; // Estimated
	G4double ZeroDegree_To_2nd_Target = 980.*mm;

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	Collimator_Room collimator_Room(World_Logical, 0.5*1.*inch); // 1 inch collimator used
	Room room(World_Logical);
	Beampipe_Upstream beampipe_Upstream(World_Logical);
	First_UTR_Wall first_UTR_Wall(World_Logical);
	First_Setup first_Setup(World_Logical);
	Detectors_G3_150Sm detectors_G3(World_Logical);
	Wheel wheel(World_Logical);
	G3_Table g3_Table(World_Logical);
	Table2 table2(World_Logical);
	ZeroDegree_Setup zeroDegree_Setup(World_Logical);
	Sm150_Target g3_Target;

	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	/***************** COLLIMATOR_ROOM *****************/

	collimator_Room.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length()*0.5));
	Collimator_Entrance_To_G3_Target = Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()- First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length() - room.Get_Wall_Thickness() - collimator_Room.Get_Length();

	/***************** ROOM *****************/

	room.Construct(G4ThreeVector(), World_x, World_y, World_z,
		Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length(),
		Floor_Level);

	/***************** BEAMPIPE_UPSTREAM *****************/

	beampipe_Upstream.Construct(G4ThreeVector(0., 0., beampipe_Upstream.Get_Z_Axis_Offset_Z()), 1e-2);

	// Lead wrapping of beam pipe at g3 target position

	G4double Beampipe_Outer_Radius = 1.*inch;
	G4double Beampipe_G3_Pb_Wrap_Thickness = 3./32. * inch;
	G4double Beampipe_G3_Pb_Wrap_Width = 140. * mm; // Actually unknown

	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4Tubs *Beampipe_G3_Pb_Wrap_Solid =
	    new G4Tubs("Beampipe_G3_Pb_Wrap_Solid", Beampipe_Outer_Radius,
	               Beampipe_Outer_Radius + Beampipe_G3_Pb_Wrap_Thickness,
	               Beampipe_G3_Pb_Wrap_Width * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Beampipe_G3_Pb_Wrap_Logical =
	    new G4LogicalVolume(Beampipe_G3_Pb_Wrap_Solid, Pb, "Beampipe_G3_Pb_Wrap_Logical", 0, 0, 0);

	Beampipe_G3_Pb_Wrap_Logical->SetVisAttributes(new G4VisAttributes(green));

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), Beampipe_G3_Pb_Wrap_Logical,
	                  "Beampipe_G3_Pb_Wrap", World_Logical, false, 0);


	/***************** FIRST_UTR_WALL *****************/

	first_UTR_Wall.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length() - First_UTR_Wall_To_First_Setup - first_UTR_Wall.Get_Length()*0.5));

	/***************** FIRST_SETUP *****************/

	first_Setup.Construct(G4ThreeVector(0., 0., Wheel_To_Target - First_Setup_To_Wheel - first_Setup.Get_Length()*0.5));

	/***************** G3_WALL *****************/

	// Very likely that no G3_WALL (a wall just infront of the g3 setup) was used in this experiment

	/***************** DETECTORS_G3 *****************/

	detectors_G3.Construct(G4ThreeVector(0., 0., 0.));

	/***************** WHEEL *****************/

	wheel.Construct(G4ThreeVector(0., 0., Wheel_To_Target + wheel.Get_Length()*0.5));

	/***************** G3_TABLE *****************/

	g3_Table.Construct(G4ThreeVector(0., 0., Wheel_To_Target + wheel.Get_Length() + g3_Table.Get_Length()*0.5));

	/***************** TABLE_2 *****************/

	table2.Construct(G4ThreeVector(0., 0.,  Wheel_To_Target + wheel.Get_Length() + g3_Table.Get_Length() + table2.Get_Length()*0.5 + table2.Get_Z_Axis_Offset_Z()));

	/***************** DETECTORS_2ND *****************/

	//detectors_2nd.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target)); // No information about the 2nd setup's geometry available for this experiment, so don't implement it at all

	/***************** ZERODEGREE_SETUP *****************/

#ifdef USE_ZERODEGREE
	zeroDegree_Setup.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + ZeroDegree_To_2nd_Target));
#endif

	/***************** G3_TARGET *****************/
	#ifdef USE_TARGETS
		g3_Target.Set_Containing_Volume(beampipe_Upstream.Get_Beampipe_Vacuum());
		g3_Target.Construct(G4ThreeVector(0., 0., -beampipe_Upstream.Get_Z_Axis_Offset_Z()));
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

	/* // Distances of LaBrs unknown, so not really useable
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
	 */

	/*************** Second setup **************/

	/* // No information about the 2nd setup's geometry available for this experiment, so don't implement it at all
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
	*/
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
