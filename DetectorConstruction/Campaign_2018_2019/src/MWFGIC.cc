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

#include "MWFGIC.hh"
#include "Materials.hh"
Materials *materials2 = Materials::Instance();

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

MWFGIC::MWFGIC(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void MWFGIC::Construct(G4ThreeVector global_coordinates){

	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour green(0., 0.5, 0.);
	G4Colour light_orange(1.0, 0.82, 0.36);
	G4Colour white(1.0, 1.0, 1.0);
	G4Colour black(0.0, 0.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	G4Colour cyan(0.0, 1.0, 1.0);
	G4Colour magenta(1.0, 0.0, 1.0);
	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour brown(0.45,0.25,0.0);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *stainlessSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	G4Material *aluminum = nist->FindOrBuildMaterial("G4_Al");
	G4Material *P10 = materials2->Get_P10();
	G4Material *target_Th232 = materials2->Get_target_Th232();
	G4Material *target_U238 = materials2->Get_target_U238();
	G4Material *gold = nist->FindOrBuildMaterial("G4_Au");
	G4Material *kapton = nist->FindOrBuildMaterial("G4_KAPTON");
	G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

//***********rotation matrices**
	G4RotationMatrix *rotateY_90 = new G4RotationMatrix();
	rotateY_90->rotateY(90.*deg);
	G4RotationMatrix *rotate160 = new G4RotationMatrix();
	rotate160->rotateY(90.*deg);
	G4RotationMatrix *rotateX_70 = new G4RotationMatrix();
	rotateX_70->rotateX(70.*deg);
	G4RotationMatrix *rotate100 = new G4RotationMatrix();
	rotate100->rotateX(110.*deg);

//***********detector gas*******
	auto detector_gas2 = new G4Tubs("detector_gas2",0.*mm,176.*0.5*mm,274.0*0.5*mm,0.,twopi);
	auto detector_gas2_logical = new G4LogicalVolume(detector_gas2,P10,"detector_gas2_logical");
	new G4PVPlacement(0,global_coordinates,detector_gas2_logical,"detector_gas2_phys",World_Logical,false,0,false);
	detector_gas2_logical->SetVisAttributes(blue);

//***********targets***********
	auto target2_solid = new G4Tubs("target2_solid",0.*mm,15.*mm,7.25E-6*cm*0.5,0.,twopi);	//calculated assuming 85 ug/cm2 and density of 11.72 g/cm3
	auto target2_logical = new G4LogicalVolume(target2_solid,target_Th232,"target2_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-76.5*mm),target2_logical,"target2_phys",detector_gas2_logical,false,0,false);
	target2_logical->SetVisAttributes(red);

	auto target3_solid = new G4Tubs("target3_solid",0.*mm,15.*mm,6.74E-6*cm*0.5,0.,twopi);	//calculated assuming 130.297 ug/cm2 and density of 18.95 g/cm3
	auto target3_logical = new G4LogicalVolume(target3_solid,target_U238,"target3_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),target3_logical,"target3_phys",detector_gas2_logical,false,0,false);
	target3_logical->SetVisAttributes(red);

	auto target4_solid = new G4Tubs("target4_solid",0.*mm,15.*mm,7.25E-6*cm*0.5,0.,twopi);	//calculated assuming 85 ug/cm2 and density of 11.72 g/cm3
	auto target4_logical = new G4LogicalVolume(target4_solid,target_Th232,"target4_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,+76.5*mm),target4_logical,"target4_phys",detector_gas2_logical,false,0,false);
	target4_logical->SetVisAttributes(red);

	auto target2_Au_solid = new G4Tubs("target2_Au_solid",0.*mm,20.*mm,2.59E-6*cm*0.5,0.,twopi);	//calculated assuming 50 ug/cm2 and density of 19.32 g/cm3
	auto target2_Au_logical = new G4LogicalVolume(target2_Au_solid,gold,"target2_Au_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-76.5*mm+7.25E-6*cm*0.5+2.59E-6*cm*0.5),target2_Au_logical,"target2_Au_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,6.74E-6*cm*0.5+2.59E-6*cm*0.5),target2_Au_logical,"target2_Au_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,76.5*mm+7.25E-6*cm*0.5+2.59E-6*cm*0.5),target2_Au_logical,"target2_Au_phys",detector_gas2_logical,false,0,false);
	target2_Au_logical->SetVisAttributes(yellow);

	auto target2_kapton_solid = new G4Tubs("target2_kapton_solid",0.*mm,30.*mm,24.64E-6*cm*0.5,0.,twopi);	//calculated assuming 35 ug/cm2 and density of 1.42 g/cm3
	auto target2_kapton_logical = new G4LogicalVolume(target2_kapton_solid,kapton,"target2_kapton_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-76.5*mm+7.25E-6*cm*0.5+2.59E-6*cm+24.64E-6*cm*0.5),target2_kapton_logical,"target2_kapton_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,6.74E-6*cm*0.5+2.59E-6*cm+24.64E-6*cm*0.5),target2_kapton_logical,"target2_kapton_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,76.5*mm+7.25E-6*cm*0.5+2.59E-6*cm+24.64E-6*cm*0.5),target2_kapton_logical,"target2_kapton_phys",detector_gas2_logical,false,0,false);
	target2_kapton_logical->SetVisAttributes(white);

	auto target2_sphere = new G4Sphere("target2_sphere",29.999,30.*mm,0.,twopi,0.,twopi);
	auto target2_sphere_logical = new G4LogicalVolume(target2_sphere,vacuum,"target2_sphere_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-76.5*mm),target2_sphere_logical,"target2_sphere_phys",detector_gas2_logical,false,0,false);
	target2_sphere_logical->SetVisAttributes(red);

	auto target3_sphere = new G4Sphere("target3_sphere",29.999,30.*mm,0.,twopi,0.,twopi);
	auto target3_sphere_logical = new G4LogicalVolume(target3_sphere,vacuum,"target3_sphere_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),target3_sphere_logical,"target3_sphere_phys",detector_gas2_logical,false,0,false);
	target3_sphere_logical->SetVisAttributes(red);

	auto target4_sphere = new G4Sphere("target4_sphere",29.999,30.*mm,0.,twopi,0.,twopi);
	auto target4_sphere_logical = new G4LogicalVolume(target4_sphere,vacuum,"target4_sphere_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,+76.5*mm),target4_sphere_logical,"target4_sphere_phys",detector_gas2_logical,false,0,false);
	target4_sphere_logical->SetVisAttributes(red);

//***********electrodes*******
	auto cathode2_solid = new G4Tubs("cathode2_solid",24.*mm,60.*mm,2.0*0.5*mm,0.,twopi);
	auto cathode2_logical = new G4LogicalVolume(cathode2_solid,stainlessSteel,"cathode2_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),cathode2_logical,"cathode2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-76.5*mm),cathode2_logical,"cathode2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,76.5*mm),cathode2_logical,"cathode2_phys",detector_gas2_logical,false,0,false);
	cathode2_logical->SetVisAttributes(grey);

	auto FG2_solid = new G4Tubs("FG2_solid",45.*mm,60.*mm,1.5*0.5*mm,0.,twopi);
	auto FG2_logical = new G4LogicalVolume(FG2_solid,stainlessSteel,"FG2_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-108.6*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-44.4*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-32.1*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,32.1*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,44.4*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,108.6*mm),FG2_logical,"FG2_phys",detector_gas2_logical,false,0,false);
	FG2_logical->SetVisAttributes(grey);

	auto anode2_solid = new G4Tubs("anode2_solid",0.*mm,60.*mm,0.5*0.5*mm,0.,twopi);
	auto anode2_logical = new G4LogicalVolume(anode2_solid,aluminum,"anode2_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-114.75*mm),anode2_logical,"anode2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-38.25*mm),anode2_logical,"anode2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,38.25*mm),anode2_logical,"anode2_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,114.75*mm),anode2_logical,"anode2_phys",detector_gas2_logical,false,0,false);
	anode2_logical->SetVisAttributes(grey);

	auto PVC_plate_solid = new G4Box("PVC_plate_solid",10.0*mm*0.5,120.0*mm*0.5,260.0*mm*0.5);
	auto PVC_plate_logical = new G4LogicalVolume(PVC_plate_solid,PVC,"PVC_plate_logical");
	new G4PVPlacement(0,G4ThreeVector(40.*mm,0.,-14.*mm*0.5),PVC_plate_logical,"PVC_plate_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(-40.*mm,0.,-14.*mm*0.5),PVC_plate_logical,"PVC_plate_phys",detector_gas2_logical,false,0,false);
	PVC_plate_logical->SetVisAttributes(grey);

	auto PVC_tube_solid = new G4Tubs("PVC_tube_solid",0.*mm,9.*0.5*mm,80.0*0.5*mm,0.,twopi);
	auto PVC_tube_logical = new G4LogicalVolume(PVC_tube_solid,PVC,"PVC_tube_logical");
	new G4PVPlacement(rotateY_90,G4ThreeVector(0.,50.*mm,65.*mm),PVC_tube_logical,"PVC_tube_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(rotateY_90,G4ThreeVector(0.,50.*mm,-65.*mm),PVC_tube_logical,"PVC_tube_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(rotateY_90,G4ThreeVector(0.,-50.*mm,65.*mm),PVC_tube_logical,"PVC_tube_phys",detector_gas2_logical,false,0,false);
	new G4PVPlacement(rotateY_90,G4ThreeVector(0.,-50.*mm,-65.*mm),PVC_tube_logical,"PVC_tube_phys",detector_gas2_logical,false,0,false);
	PVC_tube_logical->SetVisAttributes(grey);

//***********chamber pot*******
	auto chamber_wall_solid = new G4Tubs("chamber_wall_solid",176.*0.5*mm,180.*0.5*mm,264.*0.5*mm,0.,twopi);
	auto chamber_wall_logical = new G4LogicalVolume(chamber_wall_solid,aluminum,"chamber_wall_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,5.*mm),chamber_wall_logical,"chamber_wall_phys",World_Logical,false,0,false);
	chamber_wall_logical->SetVisAttributes(grey);

	auto chamber_bottom_solid = new G4Tubs("chamber_bottom_solid",0.*mm,180.*0.5*mm,6.*0.5*mm,0.,twopi);
	auto chamber_bottom_logical = new G4LogicalVolume(chamber_bottom_solid,aluminum,"chamber_bottom_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,137.*mm + 6.*0.5*mm),chamber_bottom_logical,"chamber_bottom_phys",World_Logical,false,0,false);
	chamber_bottom_logical->SetVisAttributes(grey);

	auto chamber_top_solid = new G4Tubs("chamber_top_solid",176.*0.5*mm,195.*0.5*mm,10.*0.5*mm,0.,twopi);
	auto chamber_top_logical = new G4LogicalVolume(chamber_top_solid,aluminum,"chamber_top_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-127.0*mm - 10.*0.5*mm),chamber_top_logical,"chamber_top_phys",World_Logical,false,0,false);
	chamber_top_logical->SetVisAttributes(grey);

	auto chamber_window_solid = new G4Tubs("chamber_window_solid",0.*mm,195.*0.5*mm,1.*0.5*mm,0.,twopi);
	auto chamber_window_logical = new G4LogicalVolume(chamber_window_solid,aluminum,"chamber_window_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-127.0*mm - 10.*mm - 1.*0.5*mm),chamber_window_logical,"chamber_window_phys",World_Logical,false,0,false);
	chamber_window_logical->SetVisAttributes(grey);

	auto chamber_top2_solid = new G4Tubs("chamber_top2_solid",160.*0.5*mm,195.*0.5*mm,10.*0.5*mm,0.,twopi);
	auto chamber_top2_logical = new G4LogicalVolume(chamber_top2_solid,stainlessSteel,"chamber_top2_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-127.0*mm - 10.*mm - 1.*mm - 10.*0.5*mm),chamber_top2_logical,"chamber_top2_phys",World_Logical,false,0,false);
	chamber_top2_logical->SetVisAttributes(grey);
}
