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
Setup for runs DHIPS Beamtime2019
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
#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "BGO.hh"

#include "RadiatorTarget.hh"
#include "BeamPipe_Upstream.hh"
#include "BeamPipe_Downstream.hh"
#include "LeadCastle.hh"
#include "Detectors.hh"
#include "B11_Target.hh"

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
#include "G4Sphere.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
#include <vector>
#include <iostream>

using std::stringstream;
using std::vector;
using std::cout;
using std::endl;

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct()
{

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

	/************************* World volume *****************/

	/***************** WORLD *****************/

	World_x = 6000. * mm;
	World_y = 6000. * mm;
	World_z = 6000. * mm;

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

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	BeamPipe_Upstream BeamPipe_Upstream(World_Logical);
	RadiatorTarget RadiatorTarget(World_Logical);
	BeamPipe_Downstream BeamPipe_Downstream(World_Logical);
	LeadCastle LeadCastle(World_Logical);
	Detectors Detectors(World_Logical);


	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	BeamPipe_Upstream.Construct(G4ThreeVector(0,0,-2500*mm),0.1);
	RadiatorTarget.Construct(G4ThreeVector(0,0,-2000*mm),"Radiator1","Au",3*mm,"Al",0.); 
	RadiatorTarget.Construct(G4ThreeVector(0,0,-1800*mm),"Radiator2","Au",3*mm,"Al",0.);
#ifdef DEUTERON_BREAK_UP
	BeamPipe_Downstream.Construct(G4ThreeVector(0,0,285*mm),0.1);//285*mm measured
#endif
	LeadCastle.Construct(G4ThreeVector());
	Detectors.Construct(G4ThreeVector());
	Detectors.ConstructDetectorFilter(G4ThreeVector(),"HPGe1",10*mm,10*mm);//Cu Filter Length, Pb Filter Length
	Detectors.ConstructDetectorFilter(G4ThreeVector(),"HPGe2",10*mm,10*mm);//Cu Filter Length, Pb Filter Length
	Detectors.ConstructDetectorFilter(G4ThreeVector(),"HPGePol",10*mm,10*mm);//Cu Filter Length, Pb Filter Length

#ifdef USE_TARGETS
	
#ifdef DEUTERON_BREAK_UP
	B11_Target B11_Target(BeamPipe_Downstream.Get_Beampipe_Vacuum());
	B11_Target.Construct(G4ThreeVector(0., 0., -BeamPipe_Downstream.Get_Z_Axis_Offset_Z()));
#endif
#ifndef DEUTERON_BREAK_UP
	B11_Target B11_Target(World_Logical);
	B11_Target.Construct(G4ThreeVector(0., 0., 0.));
#endif
	B11_Target.ConstructAbsorber(G4ThreeVector(0,0,-212*mm));
#endif
	
	print_info();
	return World_Physical;

}

void DetectorConstruction::ConstructSDandField() {

	EnergyDepositionSD *HPGe1SD = new EnergyDepositionSD("HPGe1", "HPGe1");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe1SD);
	HPGe1SD->SetDetectorID(HPGe1ID);
	SetSensitiveDetector("HPGe1", HPGe1SD, true);

	EnergyDepositionSD *HPGe2SD = new EnergyDepositionSD("HPGe2", "HPGe2");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2SD);
	HPGe2SD->SetDetectorID(HPGe2ID);
	SetSensitiveDetector("HPGe2", HPGe2SD, true);

	EnergyDepositionSD *HPGePolSD = new EnergyDepositionSD("HPGePol", "HPGePol");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGePolSD);
	HPGePolSD->SetDetectorID(HPGePolID);
	SetSensitiveDetector("HPGePol", HPGePolSD, true);
#ifdef DEUTERON_BREAK_UP
	EnergyDepositionSD *Si_Detector1SD = new EnergyDepositionSD("Si_Detector1", "Si_Detector1");
	G4SDManager::GetSDMpointer()->AddNewDetector(Si_Detector1SD);
	Si_Detector1SD->SetDetectorID(Si1ID);
	SetSensitiveDetector("Si_Detector", Si_Detector1SD, true);

	EnergyDepositionSD *Si_Detector2SD = new EnergyDepositionSD("Si_Detector2", "Si_Detector2");
	G4SDManager::GetSDMpointer()->AddNewDetector(Si_Detector2SD);
	Si_Detector2SD->SetDetectorID(Si2ID);
	SetSensitiveDetector("Si_Detector", Si_Detector2SD, true);

	EnergyDepositionSD *D_TargetSD = new EnergyDepositionSD("D_Target", "D_Target");
	G4SDManager::GetSDMpointer()->AddNewDetector(D_TargetSD);
	D_TargetSD->SetDetectorID(6);
	SetSensitiveDetector("D_Target_Logical", D_TargetSD, true);
#endif
}

void DetectorConstruction::print_info() const {
	printf("==============================================================\n");
	printf("  DetectorConstruction: Info (all dimensions in mm)\n");
	G4cout << G4endl;
	printf("Detector ID assignment:\n");
	printf("HPGe1:   %d \n",HPGe1ID);
	printf("HPGe2:   %d \n",HPGe2ID);
	printf("HPGePol: %d \n",HPGePolID);
#ifdef DEUTERON_BREAK_UP
	printf("Si1:     %d \n",Si1ID);
	printf("Si2:     %d \n",Si2ID);
#endif
	printf("> World dimensions             : ( %5.2f, %5.2f, %5.2f )\n", World_x, World_y, World_z);
	printf("==============================================================\n");
}
