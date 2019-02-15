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

#include "FGIC.hh"
#include "Materials.hh"
Materials *materials = Materials::Instance();

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

FGIC::FGIC(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void FGIC::Construct(G4ThreeVector global_coordinates){

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
	G4Material *peek = materials->Get_Peek();
	G4Material *P10 = materials->Get_P10();
	G4Material *PCB = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
	G4Material *PCB_coating = materials->Get_PCB_coating();
	G4Material *PA = nist->FindOrBuildMaterial("G4_NYLON-8062");
	G4Material *target_Pu240 = materials->Get_target_Pu240();
	G4Material *target_Pu239 = materials->Get_target_Pu239();
	G4Material *gold = nist->FindOrBuildMaterial("G4_Au");
	G4Material *kapton = nist->FindOrBuildMaterial("G4_KAPTON");
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
	auto detector_gas_100_solid = new G4Tubs("detector_gas_100_solid",0.*mm,94.*0.5*mm,180.*mm,0.,twopi);
	auto detector_gas_160_solid = new G4Tubs("detector_gas_160_solid",0.*mm,154.*0.5*mm,160.*mm*0.5,0.,twopi);
	auto detector_gas_250_solid = new G4Tubs("detector_gas_250_solid",0.*mm,240.*0.5*mm,312.0*0.5*mm,0.,twopi);
	auto detector_gas_250_solid_top = new G4Tubs("detector_gas_250_solid_top",0.*mm,240.*0.5*mm,50.0*0.5*mm,0.,twopi);

	auto detector_gas_pre3 = new G4UnionSolid("detector_gas_pre3",detector_gas_250_solid,detector_gas_250_solid_top,0,G4ThreeVector(0.,0.,-143.*mm));
	auto detector_gas_pre2 = new G4UnionSolid("detector_gas_pre2",detector_gas_pre3,detector_gas_100_solid,0,G4ThreeVector(0.,0.,-62.*mm));
	auto detector_gas_pre = new G4UnionSolid("detector_gas_pre",detector_gas_pre2,detector_gas_100_solid,rotateX_70,G4ThreeVector(0.,0.,0.));
	auto detector_gas = new G4UnionSolid("detector_gas",detector_gas_pre,detector_gas_160_solid,rotateY_90,G4ThreeVector(-160.*mm*0.5,0.,0.));

	auto detector_gas_logical = new G4LogicalVolume(detector_gas,P10,"detector_gas_logical");
	new G4PVPlacement(0,global_coordinates,detector_gas_logical,"detector_gas_phys",World_Logical,false,0,false);
	detector_gas_logical->SetVisAttributes(blue);

//***********target***********
	auto target_solid = new G4Tubs("target_solid",0.*mm,15.*mm,8.E-6*cm*0.5,0.,twopi);	//assumed 8.E-6*cm*0.5
//	auto target_logical = new G4LogicalVolume(target_solid,target_Pu240,"target_logical");
	auto target_logical = new G4LogicalVolume(target_solid,target_Pu239,"target_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),target_logical,"target_phys",detector_gas_logical,false,0,false);
	target_logical->SetVisAttributes(red);

	auto target_Au_solid = new G4Tubs("target_Au_solid",0.*mm,20.*mm,2.59E-6*cm*0.5,0.,twopi);	//calculated assuming 50 ug/cm2 and density of 19.32 g/cm3
	auto target_Au_logical = new G4LogicalVolume(target_Au_solid,gold,"target_Au_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,8.E-6*cm*0.5+2.59E-6*cm*0.5),target_Au_logical,"target_Au_phys",detector_gas_logical,false,0,false);
	target_Au_logical->SetVisAttributes(yellow);

	auto target_kapton_solid = new G4Tubs("target_kapton_solid",0.*mm,25.*mm,24.64E-6*cm*0.5,0.,twopi);	//calculated assuming 35 ug/cm2 and density of 1.42 g/cm3
	auto target_kapton_logical = new G4LogicalVolume(target_kapton_solid,kapton,"target_kapton_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,8.E-6*cm*0.5+2.59E-6*cm+24.64E-6*cm*0.5),target_kapton_logical,"target_kapton_phys",detector_gas_logical,false,0,false);
	target_kapton_logical->SetVisAttributes(white);

	auto target_sphere = new G4Sphere("target_sphere",29.999*mm,30.*mm,0.,twopi,0.,twopi);
	auto target_sphere_logical = new G4LogicalVolume(target_sphere,vacuum,"target_sphere_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),target_sphere_logical,"target_sphere_phys",detector_gas_logical,false,0,false);
	target_sphere_logical->SetVisAttributes(red);
	

//***********electrodes*******
	auto cathode_solid = new G4Tubs("cathode_solid",23.*mm,90.*mm,2.0*0.5*mm,0.,twopi);
	auto cathode_logical = new G4LogicalVolume(cathode_solid,stainlessSteel,"cathode_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),cathode_logical,"cathode_phys",detector_gas_logical,false,0,false);
	cathode_logical->SetVisAttributes(grey);

	auto FG_solid = new G4Tubs("FG_solid",65.*mm,90.*mm,3.2*0.5*mm,0.,twopi);
	auto FG_logical = new G4LogicalVolume(FG_solid,PCB,"FG_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,35.*mm),FG_logical,"FG_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-35.*mm),FG_logical,"FG_phys",detector_gas_logical,false,0,false);
	FG_logical->SetVisAttributes(light_orange);

	auto FG_gold = new G4Tubs("FG_gold",65.*mm,72.*mm,40.E-03*mm,0.,twopi);
	auto FG_gold_logical = new G4LogicalVolume(FG_gold,PCB_coating,"FG_gold_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,35.*mm+3.2*0.5*mm),FG_gold_logical,"FG_gold_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-35.*mm-3.2*0.5*mm),FG_gold_logical,"FG_gold_phys",detector_gas_logical,false,0,false);
	FG_gold_logical->SetVisAttributes(yellow);

	auto AG_plate = new G4Tubs("AG_plate",0.*mm,90.*mm,1.6*0.5*mm,0.,twopi);
	auto AG_box = new G4Box("AG_box",50.*mm,50.*mm,50.*mm);
	auto AG_solid = new G4SubtractionSolid("AG_solid",AG_plate,AG_box);
	auto AG_logical = new G4LogicalVolume(AG_solid,PCB,"AG_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,40.*mm),AG_logical,"AG_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-40.*mm),AG_logical,"AG_phys",detector_gas_logical,false,0,false);
	AG_logical->SetVisAttributes(green);

	auto AS_solid = new G4Tubs("AS_solid",0.*mm,90.*mm,1.6*0.5*mm,0.,twopi);
	auto AS_logical = new G4LogicalVolume(AS_solid,PCB,"AS_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,45.*mm),AS_logical,"AS_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-45.*mm),AS_logical,"AS_phys",detector_gas_logical,false,0,false);
	AS_logical->SetVisAttributes(green);

	auto AS_gold = new G4Box("AS_gold",50.*mm,50.*mm,40.E-03*mm);
	auto AS_gold_logical = new G4LogicalVolume(AS_gold,PCB_coating,"AS_gold_logical");
	new G4PVPlacement(0,G4ThreeVector(0.,0.,45.*mm-1.6*0.5*mm),AS_gold_logical,"AS_gold_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,-45.*mm+1.6*0.5*mm),AS_gold_logical,"AS_gold_phys",detector_gas_logical,false,0,false);
	AS_gold_logical->SetVisAttributes(yellow);

	auto PEEK_solid = new G4Tubs("PEEK_solid",0.*mm,5.*0.5*mm,270.*0.5*mm,0.,twopi);
	auto PEEK_logical = new G4LogicalVolume(PEEK_solid,peek,"PEEK_logical");
	new G4PVPlacement(0,G4ThreeVector(80.*mm,0.,-21.*mm),PEEK_logical,"PEEK_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(-80.*mm,0.,-21.*mm),PEEK_logical,"PEEK_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,80.*mm,-21.*mm),PEEK_logical,"PEEK_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,-80.*mm,-21.*mm),PEEK_logical,"PEEK_phys",detector_gas_logical,false,0,false);
	PEEK_logical->SetVisAttributes(brown);

	auto PA_solid = new G4Tubs("PA_solid",5.*0.5*mm,8.*0.5*mm,250.*0.5*mm,0.,twopi);
	auto PA_logical = new G4LogicalVolume(PA_solid,PA,"PA_logical");
	new G4PVPlacement(0,G4ThreeVector(80.*mm,0.,-31.*mm),PA_logical,"PA_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(-80.*mm,0.,-31.*mm),PA_logical,"PA_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,80.*mm,-31.*mm),PA_logical,"PA_phys",detector_gas_logical,false,0,false);
	new G4PVPlacement(0,G4ThreeVector(0.,-80.*mm,-31.*mm),PA_logical,"PA_phys",detector_gas_logical,false,0,false);
	PA_logical->SetVisAttributes(white);

//***********chamber pot*******

	auto negative_100 = new G4Tubs("negative_100",0.*mm,94.*0.5*mm,450.0*0.5*mm,0.,twopi);
	auto negative_160 = new G4Tubs("negative_160",0.*mm,154.*0.5*mm,450.0*0.5*mm,0.,twopi);
	auto negative_250 = new G4Tubs("negative_250",0.*mm,240.*0.5*mm,450.0*0.5*mm,0.,twopi);

	auto chamber_wall_solid_pre2 = new G4Tubs("chamber_wall_solid_pre2",120.*mm,125.*mm,312.0*0.5*mm,0.,twopi);
	auto chamber_wall_solid_pre = new G4SubtractionSolid("chamber_wall_solid_pre",chamber_wall_solid_pre2,negative_100,rotateX_70,G4ThreeVector(0.,0.,0.));
	auto chamber_wall_solid = new G4SubtractionSolid("chamber_wall_solid",chamber_wall_solid_pre,negative_160,rotateY_90,G4ThreeVector(-300.*mm,0.,0.));
	auto chamber_wall_logical = new G4LogicalVolume(chamber_wall_solid,aluminum,"chamber_wall_logical");
	new G4PVPlacement(0,global_coordinates,chamber_wall_logical,"chamber_wall_phys",World_Logical,false,0,false);
	chamber_wall_logical->SetVisAttributes(grey);

	auto chamber_bottom_solid = new G4Tubs("chamber_bottom_solid",0.*mm,125.*mm,10.*0.5*mm,0.,twopi);
	auto chamber_bottom_logical = new G4LogicalVolume(chamber_bottom_solid,aluminum,"chamber_bottom_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,312.0*0.5*mm + 10.*0.5*mm),chamber_bottom_logical,"chamber_bottom_phys",World_Logical,false,0,false);
	chamber_bottom_logical->SetVisAttributes(grey);

	auto chamber_top_solid = new G4Tubs("chamber_top_solid",120.*mm,290.*0.5*mm,12.*0.5*mm,0.,twopi);
	auto chamber_top_logical = new G4LogicalVolume(chamber_top_solid,aluminum,"chamber_top_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 12.*0.5*mm),chamber_top_logical,"chamber_top_phys",World_Logical,false,0,false);
	chamber_top_logical->SetVisAttributes(grey);

	auto chamber_side_tube_160_solid_pre = new G4Tubs("chamber_side_tube_160_solid_pre",154.*0.5*mm,160.*0.5*mm,(160.*mm-12.*mm)*0.5,0.,twopi);
	auto chamber_side_tube_160_solid = new G4SubtractionSolid("chamber_side_tube_160_solid",chamber_side_tube_160_solid_pre,negative_250,rotate160,G4ThreeVector(0.,0.,-74.*mm));
	auto chamber_side_tube_160_logical = new G4LogicalVolume(chamber_side_tube_160_solid,aluminum,"chamber_side_tube_160_logical");
	new G4PVPlacement(rotateY_90,global_coordinates + G4ThreeVector(-(160.*mm-12.*mm)*0.5,0.,0.),chamber_side_tube_160_logical,"chamber_side_tube_160_phys",World_Logical,false,0,false);
	chamber_side_tube_160_logical->SetVisAttributes(grey);

	auto chamber_side_160_solid = new G4Tubs("chamber_side_160_solid",154.*0.5*mm,180.*0.5*mm,12.0*0.5*mm,0.,twopi);
	auto chamber_side_160_logical = new G4LogicalVolume(chamber_side_160_solid,aluminum,"chamber_side_160_logical");
	new G4PVPlacement(rotateY_90,global_coordinates + G4ThreeVector(-160.*mm + 12.0*0.5*mm,0.,0.),chamber_side_160_logical,"chamber_side_160_phys",World_Logical,false,0,false);
	chamber_side_160_logical->SetVisAttributes(grey);

	auto chamber_side_tube_100_solid_pre = new G4Tubs("chamber_side_tube_100_solid_pre",94.*0.5*mm,100.*0.5*mm,180.*mm-12.*mm,0.,twopi);
	auto chamber_side_tube_100_solid = new G4SubtractionSolid("chamber_side_tube_100_solid",chamber_side_tube_100_solid_pre,negative_250,rotate100,G4ThreeVector(0.,0.,0.));
	auto chamber_side_tube_100_logical = new G4LogicalVolume(chamber_side_tube_100_solid,aluminum,"chamber_side_tube_100_logical");
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,0.,0.),chamber_side_tube_100_logical,"chamber_side_tube_100_phys",World_Logical,false,0,false);
	chamber_side_tube_100_logical->SetVisAttributes(grey);

	auto chamber_side_100_solid = new G4Tubs("chamber_side_100_solid",94.*0.5*mm,130.*0.5*mm,12.*0.5*mm,0.,twopi);
	auto chamber_side_100_logical = new G4LogicalVolume(chamber_side_100_solid,aluminum,"chamber_side_100_logical");
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(180.*mm - 12.*0.5*mm)*sin(70.*deg),(180.*mm - 12.*0.5*mm)*cos(70.*deg)),chamber_side_100_logical,"chamber_side_100_phys",World_Logical,false,0,false);
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(-180.*mm + 12.*0.5*mm)*sin(70.*deg),(-180.*mm + 12.*0.5*mm)*cos(70.*deg)),chamber_side_100_logical,"chamber_side_100_phys",World_Logical,false,0,false);
	chamber_side_100_logical->SetVisAttributes(grey);

//***********side flanges*******
	auto side_160_solid = new G4Tubs("side_160_solid",0.*mm,180.*0.5*mm,12.0*0.5*mm,0.,twopi);
	auto side_160_logical = new G4LogicalVolume(side_160_solid,aluminum,"side_160_logical");
	new G4PVPlacement(rotateY_90,global_coordinates + G4ThreeVector(-160.*mm - 12.0*mm + 12.0*0.5*mm,0.,0.),side_160_logical,"side_160_phys",World_Logical,false,0,false);
	side_160_logical->SetVisAttributes(grey);

	auto side_100_solid = new G4Tubs("side_100_solid",94.*0.5*mm,130.*0.5*mm,12.*0.5*mm,0.,twopi);
	auto side_100_logical = new G4LogicalVolume(side_100_solid,aluminum,"side_100_logical");
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(180.*mm + 12.*0.5*mm)*sin(70.*deg),(180.*mm + 12.*0.5*mm)*cos(70.*deg)),side_100_logical,"side_100_phys",World_Logical,false,0,false);
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(-180.*mm - 12.*0.5*mm)*sin(70.*deg),(-180.*mm - 12.*0.5*mm)*cos(70.*deg)),side_100_logical,"side_100_phys",World_Logical,false,0,false);
	side_100_logical->SetVisAttributes(grey);

	auto window_solid = new G4Tubs("window_solid",0.*mm,130.*0.5*mm,0.5*0.5*mm,0.,twopi);
	auto window_logical = new G4LogicalVolume(window_solid,aluminum,"window_logical");
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(180.*mm)*sin(70.*deg),(180.*mm)*cos(70.*deg)),window_logical,"window_phys",World_Logical,false,0,false);
	new G4PVPlacement(rotateX_70,global_coordinates + G4ThreeVector(0.,(-180.*mm)*sin(70.*deg),(-180.*mm)*cos(70.*deg)),window_logical,"window_phys",World_Logical,false,0,false);
	window_logical->SetVisAttributes(orange);

//***********top flanges*******
	auto top_250_solid = new G4Tubs("top_250_solid",94.*0.5*mm,290.*0.5*mm,12.*0.5*mm,0.,twopi);
	auto top_250_logical = new G4LogicalVolume(top_250_solid,aluminum,"top_250_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 12.*mm - 12.*0.5*mm),top_250_logical,"top_250_phys",World_Logical,false,0,false);
	top_250_logical->SetVisAttributes(grey);

	auto top_tube_solid = new G4Tubs("top_tube_solid",94.*0.5*mm,100.*0.5*mm,50.*0.5*mm,0.,twopi);
	auto top_tube_logical = new G4LogicalVolume(top_tube_solid,aluminum,"top_tube_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 2*12.*mm - 50.*0.5*mm),top_tube_logical,"top_tube_phys",World_Logical,false,0,false);
	top_tube_logical->SetVisAttributes(grey);

	auto top_100_solid = new G4Tubs("top_100_solid",94.*0.5*mm,130.*0.5*mm,12.*0.5*mm,0.,twopi);
	auto top_100_logical = new G4LogicalVolume(top_100_solid,aluminum,"top_100_logical");
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 2*12.*mm - 50.*mm - 12.*0.5*mm),top_100_logical,"top_100_phys",World_Logical,false,0,false);
	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 3*12.*mm - 50.*mm - 12.*0.5*mm),top_100_logical,"top_100_phys",World_Logical,false,0,false);
	top_100_logical->SetVisAttributes(grey);

	new G4PVPlacement(0,global_coordinates + G4ThreeVector(0.,0.,-312.0*0.5*mm - 3*12.*mm - 50.*mm),window_logical,"window_phys",World_Logical,false,0,false);
}
