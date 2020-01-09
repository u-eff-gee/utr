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

#include "DetectorConstruction.hh"

// Materials
#include "G4NistManager.hh"
#include "Units.hh"
#include "Materials.hh"
//Materials *materials = Materials::Instance();

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "Collimator_Room.hh"
#include "Room.hh"
//#include "Beampipe_Upstream.hh"
#include "Beampipe_Downstream.hh"
#include "First_UTR_Wall.hh"
#include "First_Setup.hh"

#include "G3_Wall_FGIC.hh"
#include "Detectors_G3_FGIC.hh"
#include "Wheel.hh"
#include "G3_Table.hh"
#include "Table2_FGIC.hh"
#include "Detectors_2nd_271_279.hh"
#include "ZeroDegree_Setup.hh"
#include "FGIC.hh"
#include "MWFGIC.hh"
#include "IC_ELI.hh"


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
	FGIC_Target_To_MWFGIC_Target = 65.*inch; // Estimated
	FGIC_Target_To_IC_ELI_Target = 85.*inch; // Estimated
	G4double ZeroDegree_To_2nd_Target = 980.*mm;

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	Collimator_Room collimator_Room(World_Logical, 0.5*0.75*inch);
	Room room(World_Logical);
//	Beampipe_Upstream beampipe_Upstream(World_Logical);
	First_UTR_Wall first_UTR_Wall(World_Logical);
	First_Setup first_Setup(World_Logical);
	G3_Wall_FGIC g3_Wall(World_Logical);
	Detectors_G3_FGIC detectors_G3(World_Logical);
	Wheel wheel(World_Logical);
	G3_Table g3_Table(World_Logical);
	Table2_FGIC table2(World_Logical);
	Beampipe_Downstream beampipe_Downstream(World_Logical);
	Detectors_2nd_271_279 detectors_2nd(World_Logical);	
	ZeroDegree_Setup zeroDegree_Setup(World_Logical);
	FGIC fgic(World_Logical);
	MWFGIC mwfgic(World_Logical);
	IC_ELI ic_eli(World_Logical);
//	Ni64_Target g3_Target;
//	Ni64_Sobotka_Target second_Target;

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

//	beampipe_Upstream.Construct(G4ThreeVector(0., 0., beampipe_Upstream.Get_Z_Axis_Offset_Z()), 1e-2);

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

	/***************** FGIC *****************/

	fgic.Construct(G4ThreeVector(0., 0., 0.));

	/***************** MW-FGIC *****************/

	mwfgic.Construct(G4ThreeVector(0., 0., FGIC_Target_To_MWFGIC_Target));

	/***************** IC_ELI *****************/


//	ic_eli.Construct(G4ThreeVector(0., 0., FGIC_Target_To_IC_ELI_Target));

	/***************** TABLE_2 *****************/

	table2.Construct(G4ThreeVector(0., 0.,  Wheel_To_Target + wheel.Get_Length() + g3_Table.Get_Length() + table2.Get_Length()*0.5 + table2.Get_Z_Axis_Offset_Z()));

	/***************** BEAMPIPE_DOWNSTREAM *****************/

//	beampipe_Downstream.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + beampipe_Downstream.Get_Z_Axis_Offset_Z()), 1e-2);

	/***************** DETECTORS_2ND *****************/

//	detectors_2nd.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target));

	/***************** ZERODEGREE_SETUP *****************/
/*
#ifdef USE_ZERODEGREE
	zeroDegree_Setup.Construct(G4ThreeVector(0., 0., G3_Target_To_2nd_Target + ZeroDegree_To_2nd_Target));
#endif
*/
#ifdef USE_TARGETS	
	/***************** G3_TARGET *****************/

//	g3_Target.Set_Containing_Volume(beampipe_Upstream.Get_Beampipe_Vacuum());
//	g3_Target.Construct(G4ThreeVector(0., 0., -beampipe_Upstream.Get_Z_Axis_Offset_Z()));

	/***************** SECOND_TARGET *****************/

//	second_Target.Set_Containing_Volume(beampipe_Downstream.Get_Beampipe_Vacuum());
//	second_Target.Construct(G4ThreeVector(0., 0., -beampipe_Downstream.Get_Z_Axis_Offset_Z()));
#endif

	print_info();

	return World_Physical;
}

void DetectorConstruction::ConstructSDandField() {

	/********* ZeroDegree detector *******/
/*
#ifdef USE_ZERODEGREE
	EnergyDepositionSD *ZeroDegreeSD = new EnergyDepositionSD("ZeroDegree", "ZeroDegree");
	G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
	ZeroDegreeSD->SetDetectorID(0);
	SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);
#endif

	/*************** Gamma3 **************/

	EnergyDepositionSD *LaBr1SD = new EnergyDepositionSD("LaBr1", "LaBr1");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr1SD);
	LaBr1SD->SetDetectorID(1);
	SetSensitiveDetector("LaBr1", LaBr1SD, true);

	EnergyDepositionSD *LaBr2SD = new EnergyDepositionSD("LaBr2", "LaBr2");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr2SD);
	LaBr2SD->SetDetectorID(2);
	SetSensitiveDetector("LaBr2", LaBr2SD, true);

	EnergyDepositionSD *LaBr3SD = new EnergyDepositionSD("LaBr3", "LaBr3");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr3SD);
	LaBr3SD->SetDetectorID(3);
	SetSensitiveDetector("LaBr3", LaBr3SD, true);

	EnergyDepositionSD *LaBr4SD = new EnergyDepositionSD("LaBr4", "LaBr4");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr4SD);
	LaBr4SD->SetDetectorID(4);
	SetSensitiveDetector("LaBr4", LaBr4SD, true);

	EnergyDepositionSD *LaBr5SD = new EnergyDepositionSD("LaBr5", "LaBr5");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr5SD);
	LaBr5SD->SetDetectorID(5);
	SetSensitiveDetector("LaBr5", LaBr5SD, true);

	EnergyDepositionSD *LaBr6SD = new EnergyDepositionSD("LaBr6", "LaBr6");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr6SD);
	LaBr6SD->SetDetectorID(6);
	SetSensitiveDetector("LaBr6", LaBr6SD, true);

	EnergyDepositionSD *LaBr7SD = new EnergyDepositionSD("LaBr7", "LaBr7");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr7SD);
	LaBr7SD->SetDetectorID(7);
	SetSensitiveDetector("LaBr7", LaBr7SD, true);

	EnergyDepositionSD *LaBr8SD = new EnergyDepositionSD("LaBr8", "LaBr8");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr8SD);
	LaBr8SD->SetDetectorID(8);
	SetSensitiveDetector("LaBr8", LaBr8SD, true);

	/*************** Gamma3 - neutron detectors **************/

	EnergyDepositionSD *NeutronDetector1SD = new EnergyDepositionSD("NeutronDetector1", "NeutronDetector1");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector1SD);
	NeutronDetector1SD->SetDetectorID(11);
	SetSensitiveDetector("NeutronDetector1", NeutronDetector1SD, true);

	EnergyDepositionSD *NeutronDetector2SD = new EnergyDepositionSD("NeutronDetector2", "NeutronDetector2");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector2SD);
	NeutronDetector2SD->SetDetectorID(12);
	SetSensitiveDetector("NeutronDetector2", NeutronDetector2SD, true);

	EnergyDepositionSD *NeutronDetector3SD = new EnergyDepositionSD("NeutronDetector3", "NeutronDetector3");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector3SD);
	NeutronDetector3SD->SetDetectorID(13);
	SetSensitiveDetector("NeutronDetector3", NeutronDetector3SD, true);

	EnergyDepositionSD *NeutronDetector4SD = new EnergyDepositionSD("NeutronDetector4", "NeutronDetector4");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector4SD);
	NeutronDetector4SD->SetDetectorID(14);
	SetSensitiveDetector("NeutronDetector4", NeutronDetector4SD, true);

	EnergyDepositionSD *NeutronDetector5SD = new EnergyDepositionSD("NeutronDetector5", "NeutronDetector5");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector5SD);
	NeutronDetector5SD->SetDetectorID(15);
	SetSensitiveDetector("NeutronDetector5", NeutronDetector5SD, true);

	EnergyDepositionSD *NeutronDetector6SD = new EnergyDepositionSD("NeutronDetector6", "NeutronDetector6");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector6SD);
	NeutronDetector6SD->SetDetectorID(16);
	SetSensitiveDetector("NeutronDetector6", NeutronDetector6SD, true);

	EnergyDepositionSD *NeutronDetector7SD = new EnergyDepositionSD("NeutronDetector7", "NeutronDetector7");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector7SD);
	NeutronDetector7SD->SetDetectorID(17);
	SetSensitiveDetector("NeutronDetector7", NeutronDetector7SD, true);

	EnergyDepositionSD *NeutronDetector8SD = new EnergyDepositionSD("NeutronDetector8", "NeutronDetector8");
	G4SDManager::GetSDMpointer()->AddNewDetector(NeutronDetector8SD);
	NeutronDetector8SD->SetDetectorID(18);
	SetSensitiveDetector("NeutronDetector8", NeutronDetector8SD, true);

	/*************** First setup **************/

	ParticleSD *Pu240_targetSD = new ParticleSD("Pu240_targetSD", "Pu240_targetSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Pu240_targetSD);
	Pu240_targetSD->SetDetectorID(20);
	SetSensitiveDetector("target_logical", Pu240_targetSD, true);

	ParticleSD *Pu240_sphereSD = new ParticleSD("Pu240_sphereSD", "Pu240_sphereSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Pu240_sphereSD);
	Pu240_sphereSD->SetDetectorID(21);
	SetSensitiveDetector("target_sphere_logical", Pu240_sphereSD, true);

	/*************** Second setup **************/

	ParticleSD *Th232_1_targetSD = new ParticleSD("Th232_1_targetSD", "Th232_1_targetSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Th232_1_targetSD);
	Th232_1_targetSD->SetDetectorID(22);
	SetSensitiveDetector("target2_logical", Th232_1_targetSD, true);

	ParticleSD *Th232_1_sphereSD = new ParticleSD("Th232_1_sphereSD", "Th232_1_sphereSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Th232_1_sphereSD);
	Th232_1_sphereSD->SetDetectorID(23);
	SetSensitiveDetector("target2_sphere_logical", Th232_1_sphereSD, true);

	ParticleSD *U238_targetSD = new ParticleSD("U238_targetSD", "U238_targetSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(U238_targetSD);
	U238_targetSD->SetDetectorID(24);
	SetSensitiveDetector("target3_logical", U238_targetSD, true);

	ParticleSD *U238_sphereSD = new ParticleSD("U238_sphereSD", "U238_sphereSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(U238_sphereSD);
	U238_sphereSD->SetDetectorID(25);
	SetSensitiveDetector("target3_sphere_logical", U238_sphereSD, true);

	ParticleSD *Th232_2_targetSD = new ParticleSD("Th232_2_targetSD", "Th232_2_targetSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Th232_2_targetSD);
	Th232_2_targetSD->SetDetectorID(26);
	SetSensitiveDetector("target3_logical", Th232_2_targetSD, true);

	ParticleSD *Th232_2_sphereSD = new ParticleSD("Th232_2_sphereSD", "Th232_2_sphereSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Th232_2_sphereSD);
	Th232_2_sphereSD->SetDetectorID(27);
	SetSensitiveDetector("target3_sphere_logical", Th232_2_sphereSD, true);
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
