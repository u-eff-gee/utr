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

#include "Blowfish_Frame.hh"
//#include "Materials.hh"
//Materials *materials = Materials::Instance();

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4Colour.hh"
#include "G4Transform3D.hh"

Blowfish_Frame::Blowfish_Frame(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Blowfish_Frame::Construct(G4ThreeVector global_coordinates){

	//------------------------------------------------------------------
	// Blowfish Frame
	//------------------------------------------------------------------
	// some materials
	G4NistManager* nist = G4NistManager::Instance();
	G4Element * elC  = nist->FindOrBuildElement("C");
	G4Element * elFe = nist->FindOrBuildElement("Fe");
 	G4double density;
  	G4int ncomponents;
	G4Material * Aluminum = nist->FindOrBuildMaterial("G4_Al");
	//G4Material * Air    = nist->FindOrBuildMaterial("G4_AIR");	
	//Steel (not stainless)
	G4Material* Steel = new G4Material("Steel", density=7.7*g/cm3, ncomponents=2);
	Steel->AddElement(elC, 0.04);
	Steel->AddElement(elFe, 0.96);

	//simple rotation matricies
	G4RotationMatrix * rot90y= new G4RotationMatrix();
 	rot90y->rotateY(90.0*deg);
	
	G4RotationMatrix * rot45y= new G4RotationMatrix();
 	rot45y->rotateY(45.0*deg);
	
	G4RotationMatrix * rot135y= new G4RotationMatrix();
 	rot135y->rotateY(135.0*deg);
	
	G4RotationMatrix * rot225y= new G4RotationMatrix();
 	rot225y->rotateY(225.0*deg);
	
	G4RotationMatrix * rot315y= new G4RotationMatrix();
 	rot315y->rotateY(315.0*deg);
	
	G4RotationMatrix * rot270y= new G4RotationMatrix();
 	rot270y->rotateY(270.0*deg);
	
	G4RotationMatrix * rot45z= new G4RotationMatrix();
 	rot45z->rotateZ(45.0*deg);
	
	G4RotationMatrix * rotm45z= new G4RotationMatrix();
 	rotm45z->rotateZ(-45.0*deg);
	
	
	//Much of Blowfish is designed to integral numbers of inches
	const G4double in = 2.54*cm;
	G4ThreeVector origin(0.,0.,0.);

	//Parameters from base_upper_frame_3.dwb

	//Thickness of the support frame peices: 3/8 of an inch
	const G4double support_material_thickness = 3.0/8.0*in;
	const G4double support_material_width = 4.0*in; //VERIFY THIS NUMER

	//Length of the support frame parallel and perpendicular to the array
	//as well as the vertical supports
	const G4double perp_base_length = 71.25*in;
	const G4double para_base_length = 84.0*in;
	const G4double vert_support_length = 82.0*in;

	//Parameters for angle supports.  
	//We must make an approximation to make this support fit easily
	//Approximate with square ends
	const G4double angle_support_length = (59.0 + 5.0/8.0)*in; 	//longest length
	const G4double angle_support_approx = 2.0*in + 4.0*in; //approximation: remove this amount from the length
	
	//The hub mount plate is 3/4x24x24 inches with a 10.5 in hole
	const G4double hub_mount_dimension = 24.0*in; 
	const G4double hub_mount_thickness = 3.0/4.0*in; 
	const G4double hub_hole_diam = 10.5*in;
	
	//The distance of the center of the cross bar from the top of the array
	const G4double cross_position_from_top = (32.0 + 1.0/8.0)*in;

	
	//The distance from the top of the vertical support to the hub in the Hub's medium position
	const G4double mediumHubDistFromTop = 16.125*in;

	//Hub Height: This is variable and can be changed from experiment to experiment
	//The hub height is measured from the bottom of the vertical support to the centre of the hub
	const G4double hubHeight = vert_support_length - mediumHubDistFromTop;
	

	//---------------------------------- Aluminum visual attributes 

	G4Colour yellowOffWhite = G4Colour(1.0,1.0,224.0/255.0);
	G4Colour grey = G4Colour(128.0/255.0,128.0/255.0,128.0/255.0);
	
	G4VisAttributes* frameAtt = new G4VisAttributes(yellowOffWhite); 
	
	//------------------------------ Base perpendicular to the beam
	
	//Support legend: 
	//First character:
	//	X means that the x-direction has the longest length
	//	Y means that the y-direction has the longest length
	//	Z means that the z-direction has the longest length
	//Second character:
	//	X means that the x-direction has length support_material_width
	//	Y means that the y-direction has length support_material_width
	//	Z means that the z-direction has length support_material_width
	//
	//example: baseSupportZX is the perpendicular base support with its
	//with in the x-direction
	//
	//We work by starting in the x,z>0 quarter and copy the objects into
	//the other three quarters
	
	const G4double baseSupportZX_x = support_material_width/2.0;
	const G4double baseSupportZX_y = support_material_thickness/2.0;
	const G4double baseSupportZX_z = perp_base_length/2.0;

	G4Box* baseSupportZXBox = new G4Box("BaseSupportZXBox",baseSupportZX_x,baseSupportZX_y,baseSupportZX_z);

	const G4double baseSupportZY_x = support_material_thickness/2.0;
	const G4double baseSupportZY_y = support_material_width/2.0 - baseSupportZX_y;
	const G4double baseSupportZY_z = perp_base_length/2.0;

	G4Box* baseSupportZYBox = new G4Box("BaseSupportZYBox",baseSupportZY_x,baseSupportZY_y,baseSupportZY_z);

	//The perpendicuarl part of the base's logical volume
	G4LogicalVolume * baseSupportZXLV = new G4LogicalVolume(baseSupportZXBox, Aluminum, "BaseSupportZXLV");
	G4LogicalVolume * baseSupportZYLV = new G4LogicalVolume(baseSupportZYBox, Aluminum, "BaseSupportZYLV");
	baseSupportZXLV->SetVisAttributes(frameAtt);
	baseSupportZYLV->SetVisAttributes(frameAtt);

	//Place the base supports in the array (placement relative to the origin, which may or may not be (0,0,0) )
	const G4double baseSupportZY_px = para_base_length/2.0 - baseSupportZY_x;
	const G4double baseSupportZY_py = -hubHeight + baseSupportZY_y;

	const G4double baseSupportZX_px = baseSupportZY_px + baseSupportZY_x - baseSupportZX_x;
	const G4double baseSupportZX_py = baseSupportZY_py - baseSupportZY_y - baseSupportZX_y;
	
	const G4ThreeVector baseSupportZXaPos = G4ThreeVector(baseSupportZX_px, baseSupportZX_py,0.0) + origin;
	const G4ThreeVector baseSupportZYaPos = G4ThreeVector(baseSupportZY_px, baseSupportZY_py,0.0) + origin;
	new G4PVPlacement(0,baseSupportZXaPos, baseSupportZXLV, "BaseSupportZXPhys", World_Logical,false,0);
	new G4PVPlacement(0,baseSupportZYaPos, baseSupportZYLV, "BaseSupportZYPhys", World_Logical,false,0);
	
	const G4ThreeVector baseSupportZXbPos = G4ThreeVector(-baseSupportZX_px, baseSupportZX_py,0.0) + origin;
	const G4ThreeVector baseSupportZYbPos = G4ThreeVector(-baseSupportZY_px, baseSupportZY_py,0.0) + origin;
	new G4PVPlacement(0,baseSupportZXbPos, baseSupportZXLV, "BaseSupportZXPhys", World_Logical,false,1);
	new G4PVPlacement(0,baseSupportZYbPos, baseSupportZYLV, "BaseSupportZYPhys", World_Logical,false,1);

	
	//------------------------------ Base parallel to the beam

	const G4double baseSupportXZ_x = para_base_length/2.0;
	const G4double baseSupportXZ_y = support_material_thickness/2.0;
	const G4double baseSupportXZ_z = support_material_width/2.0;

	G4Box* baseSupportXZBox = new G4Box("BaseSupportXZBox",baseSupportXZ_x,baseSupportXZ_y,baseSupportXZ_z);

	const G4double baseSupportXY_x = para_base_length/2.0;
	const G4double baseSupportXY_y = support_material_width/2.0 - baseSupportXZ_y;
	const G4double baseSupportXY_z = support_material_thickness/2.0;

	G4Box* baseSupportXYBox = new G4Box("BaseSupportXYBox",baseSupportXY_x,baseSupportXY_y,baseSupportXY_z);

	//The perpendicuarl part of the base's logical volume
	G4LogicalVolume * baseSupportXZLV = new G4LogicalVolume(baseSupportXZBox, Aluminum, "BaseSupportXZLV");
	G4LogicalVolume * baseSupportXYLV = new G4LogicalVolume(baseSupportXYBox, Aluminum, "BaseSupportXYLV");
	baseSupportXZLV->SetVisAttributes(frameAtt);
	baseSupportXYLV->SetVisAttributes(frameAtt);

	//Place the base supports in the array (placement relative to the origin, which may or may not be (0,0,0) )
	const G4double baseSupportXY_py = baseSupportZX_py - baseSupportZX_y + baseSupportXY_y;
	const G4double baseSupportXY_pz = perp_base_length/2.0 + baseSupportXY_z;

	const G4double baseSupportXZ_py = baseSupportXY_py - baseSupportXY_y - baseSupportXZ_y;
	const G4double baseSupportXZ_pz = baseSupportXY_pz + baseSupportXY_z - baseSupportXZ_z;
	
	const G4ThreeVector baseSupportXZaPos = G4ThreeVector(0.0, baseSupportXZ_py,baseSupportXZ_pz) + origin;
	const G4ThreeVector baseSupportXYaPos = G4ThreeVector(0.0, baseSupportXY_py,baseSupportXY_pz) + origin;
	new G4PVPlacement(0,baseSupportXZaPos, baseSupportXZLV, "BaseSupportXZPhys", World_Logical,false,0);
	new G4PVPlacement(0,baseSupportXYaPos, baseSupportXYLV, "BaseSupportXYPhys", World_Logical,false,0);
	
	const G4ThreeVector baseSupportXZbPos = G4ThreeVector(0.0, baseSupportXZ_py,-baseSupportXZ_pz) + origin;
	const G4ThreeVector baseSupportXYbPos = G4ThreeVector(0.0, baseSupportXY_py,-baseSupportXY_pz) + origin;
	new G4PVPlacement(0,baseSupportXZbPos, baseSupportXZLV, "BaseSupportXZPhys", World_Logical,false,1);
	new G4PVPlacement(0,baseSupportXYbPos, baseSupportXYLV, "BaseSupportXYPhys", World_Logical,false,1);


	//------------------------------ Base center supports

	//two pieces of support structure together to make the center supports
	const G4double centerSupportZX_x = 2.0*support_material_width/2.0;
	const G4double centerSupportZX_y = support_material_thickness/2.0;
	const G4double centerSupportZX_z = perp_base_length/2.0;

	G4Box * centerSupportZXBox = new G4Box("CenterSupportZXBox",centerSupportZX_x,centerSupportZX_y,centerSupportZX_z);
	
	const G4double centerSupportZY_x = 2.0*support_material_thickness/2.0;
	const G4double centerSupportZY_y = support_material_width/2.0 - centerSupportZX_y;
	const G4double centerSupportZY_z = perp_base_length/2.0;

	G4Box * centerSupportZYBox = new G4Box("CenterSupportZYBox",centerSupportZY_x,centerSupportZY_y,centerSupportZY_z);
	
	//The center support's logical volume
	G4LogicalVolume * centerSupportZXLV = new G4LogicalVolume(centerSupportZXBox,Aluminum,"CenterSupportZXLV");
	G4LogicalVolume * centerSupportZYLV = new G4LogicalVolume(centerSupportZYBox,Aluminum,"CenterSupportZYLV");
	centerSupportZXLV->SetVisAttributes(frameAtt);
	centerSupportZYLV->SetVisAttributes(frameAtt);

	//place the center support
	const G4double centerSupportZX_py = baseSupportXZ_py + baseSupportXZ_y + centerSupportZX_y;
	const G4double centerSupportZY_py = centerSupportZX_py + centerSupportZX_y + centerSupportZY_y;

	const G4ThreeVector centerSupportZXPos = G4ThreeVector(0.0,centerSupportZX_py,0.0) + origin;
	const G4ThreeVector centerSupportZYPos = G4ThreeVector(0.0,centerSupportZY_py,0.0) + origin;
	new G4PVPlacement(0,centerSupportZXPos,centerSupportZXLV,"CenterSupportZXPhys",World_Logical,false,0);
	new G4PVPlacement(0,centerSupportZYPos,centerSupportZYLV,"CenterSupportZYPhys",World_Logical,false,0);
	
	

	//------------------------------ Verical supports
	
	//from base_upper_frame_3.dwb
	//Build the L-style supports from two peices of aluminum
	
	const G4double vertSupportYZ_x = support_material_thickness/2.0;
	const G4double vertSupportYZ_y = vert_support_length/2.0; 
	const G4double vertSupportYZ_z = support_material_width/2.0;
	
	G4Box * vertSupportYZBox = new G4Box("VertSupport1Box",vertSupportYZ_x,vertSupportYZ_y,vertSupportYZ_z);

	const G4double vertSupportYX_x = support_material_width/2.0 - vertSupportYZ_x; //fits onto the larger peice
	const G4double vertSupportYX_y = vert_support_length/2.0; //same height
	const G4double vertSupportYX_z = support_material_thickness/2.0; //material thickness
	
	G4Box * vertSupportYXBox = new G4Box("VertSupport2Box",vertSupportYX_x,vertSupportYX_y,vertSupportYX_z);

	//the verical support's Logical Volumes
	G4LogicalVolume * vertSupportYZLV = new G4LogicalVolume(vertSupportYZBox,Aluminum,"VertSupportYZLV");
	G4LogicalVolume * vertSupportYXLV = new G4LogicalVolume(vertSupportYXBox,Aluminum,"VertSupportYXLV");
	vertSupportYZLV->SetVisAttributes(frameAtt);
	vertSupportYXLV->SetVisAttributes(frameAtt);

	//Place the logical volumes: there are 4 vertical supports
	const G4double vertSupportYZ_px = baseSupportZY_px - baseSupportZY_x - vertSupportYZ_x;
	const G4double vertSupportYZ_py = baseSupportZY_py - baseSupportZY_y + vertSupportYZ_y;
	const G4double vertSupportYZ_pz = hub_mount_dimension/2.0 - vertSupportYZ_z + 2.0*vertSupportYX_z;

	const G4double vertSupportYX_px = vertSupportYZ_px - vertSupportYZ_x - vertSupportYX_x;
	const G4double vertSupportYX_py = vertSupportYZ_py;
	const G4double vertSupportYX_pz = vertSupportYZ_pz + vertSupportYZ_z - vertSupportYX_z;

	const G4ThreeVector vertSupportYZaPos = G4ThreeVector(vertSupportYZ_px,vertSupportYZ_py,vertSupportYZ_pz) + origin;
	const G4ThreeVector vertSupportYXaPos = G4ThreeVector(vertSupportYX_px,vertSupportYX_py,vertSupportYX_pz) + origin;
	new G4PVPlacement(0,vertSupportYZaPos, vertSupportYZLV, "VertSupportYZPhys",World_Logical,false,0);
	new G4PVPlacement(0,vertSupportYXaPos, vertSupportYXLV, "VertSupportYXPhys",World_Logical,false,0);
	
	const G4ThreeVector vertSupportYZbPos = G4ThreeVector(vertSupportYZ_px,vertSupportYZ_py,-vertSupportYZ_pz) + origin;
	const G4ThreeVector vertSupportYXbPos = G4ThreeVector(vertSupportYX_px,vertSupportYX_py,-vertSupportYX_pz) + origin;
	new G4PVPlacement(0,vertSupportYZbPos, vertSupportYZLV, "VertSupportYZPhys",World_Logical,false,1);
	new G4PVPlacement(0,vertSupportYXbPos, vertSupportYXLV, "VertSupportYXPhys",World_Logical,false,1);

	const G4ThreeVector vertSupportYZcPos = G4ThreeVector(-vertSupportYZ_px,vertSupportYZ_py,vertSupportYZ_pz) + origin;
	const G4ThreeVector vertSupportYXcPos = G4ThreeVector(-vertSupportYX_px,vertSupportYX_py,vertSupportYX_pz) + origin;
	new G4PVPlacement(0,vertSupportYZcPos, vertSupportYZLV, "VertSupportYZPhys",World_Logical,false,2);
	new G4PVPlacement(0,vertSupportYXcPos, vertSupportYXLV, "VertSupportYXPhys",World_Logical,false,2);
	
	const G4ThreeVector vertSupportYZdPos = G4ThreeVector(-vertSupportYZ_px,vertSupportYZ_py,-vertSupportYZ_pz) + origin;
	const G4ThreeVector vertSupportYXdPos = G4ThreeVector(-vertSupportYX_px,vertSupportYX_py,-vertSupportYX_pz) + origin;
	new G4PVPlacement(0,vertSupportYZdPos, vertSupportYZLV, "VertSupportYZPhys",World_Logical,false,3);
	new G4PVPlacement(0,vertSupportYXdPos, vertSupportYXLV, "VertSupportYXPhys",World_Logical,false,3);

	
	//------------------------------ angle support between verical supports and center support
	
	//Approximate this structure with a rectangular box
	const G4double angleSupportXY_x = (angle_support_length - angle_support_approx)/2.0;
	const G4double angleSupportXY_y = support_material_width/2.0;
	const G4double angleSupportXY_z = support_material_thickness/2.0; 
	
	G4Box * angleSupportXYBox = new G4Box( "AngleSupportXYBox",angleSupportXY_x,angleSupportXY_y,angleSupportXY_z);

	const G4double angleSupportXZ_x = angleSupportXY_x;
	const G4double angleSupportXZ_y = support_material_thickness/2.0; 
	const G4double angleSupportXZ_z = support_material_width/2.0 - angleSupportXY_z;
	
	G4Box * angleSupportXZBox = new G4Box( "AngleSupportXZBox",angleSupportXZ_x,angleSupportXZ_y,angleSupportXZ_z);

	//The angle support's logical volume
	G4LogicalVolume * angleSupportXYLV = new G4LogicalVolume(angleSupportXYBox, Aluminum, "AngleSupportXYLV");
	G4LogicalVolume * angleSupportXZLV = new G4LogicalVolume(angleSupportXZBox, Aluminum, "AngleSupportXZLV");
	angleSupportXYLV->SetVisAttributes(frameAtt);
	angleSupportXZLV->SetVisAttributes(frameAtt);

	//Place the angle support in the array using the vertical support and center support as reference
	const G4double angleSupportXY_px = vertSupportYZ_px - vertSupportYZ_x - (angleSupportXY_x + angleSupportXY_y)*cos(45.0*deg);
	const G4double angleSupportXY_py = centerSupportZX_py + centerSupportZX_y + (angleSupportXY_x + angleSupportXY_y)*cos(45.0*deg);
	const G4double angleSupportXY_pz = hub_mount_dimension/2.0 - angleSupportXY_z;
	
	const G4double angleSupportXZ_px = vertSupportYZ_px - vertSupportYZ_x - (angleSupportXZ_x + angleSupportXZ_y)*cos(45.0*deg);
	const G4double angleSupportXZ_py = centerSupportZX_py + centerSupportZX_y + (angleSupportXZ_x + angleSupportXZ_y)*cos(45.0*deg);
	const G4double angleSupportXZ_pz = angleSupportXY_pz - angleSupportXY_z - angleSupportXZ_z;
	
	const G4ThreeVector angleSupportXYaPos = G4ThreeVector(angleSupportXY_px,angleSupportXY_py,angleSupportXY_pz) + origin;
	const G4ThreeVector angleSupportXZaPos = G4ThreeVector(angleSupportXZ_px,angleSupportXZ_py,angleSupportXZ_pz) + origin;
	new G4PVPlacement(rotm45z,angleSupportXYaPos, angleSupportXYLV,"AngleSupportXYPhys",World_Logical,false,0);
	new G4PVPlacement(rotm45z,angleSupportXZaPos, angleSupportXZLV,"AngleSupportXZPhys",World_Logical,false,0);
	
	const G4ThreeVector angleSupportXYbPos = G4ThreeVector(angleSupportXY_px,angleSupportXY_py,-angleSupportXY_pz) + origin;
	const G4ThreeVector angleSupportXZbPos = G4ThreeVector(angleSupportXZ_px,angleSupportXZ_py,-angleSupportXZ_pz) + origin;
	new G4PVPlacement(rotm45z,angleSupportXYbPos, angleSupportXYLV,"AngleSupportXYPhys",World_Logical,false,1);
	new G4PVPlacement(rotm45z,angleSupportXZbPos, angleSupportXZLV,"AngleSupportXZPhys",World_Logical,false,1);

	const G4ThreeVector angleSupportXYcPos = G4ThreeVector(-angleSupportXY_px,angleSupportXY_py,angleSupportXY_pz) + origin;
	const G4ThreeVector angleSupportXZcPos = G4ThreeVector(-angleSupportXZ_px,angleSupportXZ_py,angleSupportXZ_pz) + origin;
	new G4PVPlacement(rot45z,angleSupportXYcPos, angleSupportXYLV,"AngleSupportXYPhys",World_Logical,false,2);
	new G4PVPlacement(rot45z,angleSupportXZcPos, angleSupportXZLV,"AngleSupportXZPhys",World_Logical,false,2);

	const G4ThreeVector angleSupportXYdPos = G4ThreeVector(-angleSupportXY_px,angleSupportXY_py,-angleSupportXY_pz) + origin;
	const G4ThreeVector angleSupportXZdPos = G4ThreeVector(-angleSupportXZ_px,angleSupportXZ_py,-angleSupportXZ_pz) + origin;
	new G4PVPlacement(rot45z,angleSupportXYdPos, angleSupportXYLV,"AngleSupportXYPhys",World_Logical,false,3);
	new G4PVPlacement(rot45z,angleSupportXZdPos, angleSupportXZLV,"AngleSupportXZPhys",World_Logical,false,3);

	//------------------------------ cross bar between verical supports
	
	const G4double crossSupportZY_x = support_material_thickness/2.0;
	const G4double crossSupportZY_y = support_material_width/2.0;
	const G4double crossSupportZY_z = hub_mount_dimension/2.0;

	G4Box * crossSupportZYBox = new G4Box("CrossSupportZYBox",crossSupportZY_x,crossSupportZY_y,crossSupportZY_z);

	const G4double crossSupportZX_x = support_material_width/2.0 - crossSupportZY_x; 
	const G4double crossSupportZX_y = support_material_thickness/2.0;
	const G4double crossSupportZX_z = hub_mount_dimension/2.0;

	G4Box * crossSupportZXBox = new G4Box("CrossSupportZXBox",crossSupportZX_x,crossSupportZX_y,crossSupportZX_z);

	//the cross bar's logical volumes
	G4LogicalVolume * crossSupportZYLV = new G4LogicalVolume(crossSupportZYBox,Aluminum,"CrossSupportZYLV");
	G4LogicalVolume * crossSupportZXLV = new G4LogicalVolume(crossSupportZXBox,Aluminum,"CrossSupportZXLV");
	crossSupportZYLV->SetVisAttributes(frameAtt);
	crossSupportZXLV->SetVisAttributes(frameAtt);
	
	//Place the cross bar relative to the origin (which may not be (0,0,0) )
	const G4double crossSupportZY_px = vertSupportYZ_px + vertSupportYZ_x + crossSupportZY_x;
	const G4double crossSupportZY_py = vertSupportYZ_py + vertSupportYZ_y - cross_position_from_top;

	const G4double crossSupportZX_px = vertSupportYX_px;//an approximation to make the box fit without intersections
	const G4double crossSupportZX_py = crossSupportZY_py + crossSupportZY_y - crossSupportZX_y;
	
	const G4ThreeVector crossSupportZYaPos = G4ThreeVector(crossSupportZY_px,crossSupportZY_py,0.0) + origin;
	const G4ThreeVector crossSupportZXaPos = G4ThreeVector(crossSupportZX_px,crossSupportZX_py,0.0) + origin;
	new G4PVPlacement(0,crossSupportZYaPos, crossSupportZYLV, "CrossSupportZYPhys", World_Logical,false,0);
	new G4PVPlacement(0,crossSupportZXaPos, crossSupportZXLV, "CrossSupportZXPhys", World_Logical,false,0);
	
	const G4ThreeVector crossSupportZYbPos = G4ThreeVector(-crossSupportZY_px,crossSupportZY_py,0.0) + origin;
	const G4ThreeVector crossSupportZXbPos = G4ThreeVector(-crossSupportZX_px,crossSupportZX_py,0.0) + origin;
	new G4PVPlacement(0,crossSupportZYbPos, crossSupportZYLV, "CrossSupportZYPhys", World_Logical,false,1);
	new G4PVPlacement(0,crossSupportZXbPos, crossSupportZXLV, "CrossSupportZXPhys", World_Logical,false,1);

	
	//------------------------------ Hub mount plate
	const G4double hubMount_x = hub_mount_dimension/2.0; 
	const G4double hubMount_y = hub_mount_dimension/2.0;
	const G4double hubMount_z = hub_mount_thickness/2.0;	

	G4Box * hubMountBox = new G4Box("HubMountBox",hubMount_x,hubMount_y,hubMount_z);
	
	//------------------------------ Hole in the hub mount plate
	const G4double hubMntHole_Rmin = 0.0;
	const G4double hubMntHole_Rmax = hub_hole_diam/2.0;
	const G4double hubMntHole_z = hubMount_z + 1.*mm; //material thickness
	
	G4Tubs * hubMntHoleTubs = new G4Tubs("HubMntHoleTubs",hubMntHole_Rmin,hubMntHole_Rmax,hubMntHole_z,0.0,360.0*deg);
	G4RotationMatrix * rotp_unit = new G4RotationMatrix();
	G4SubtractionSolid * 
		hubWithHole = new G4SubtractionSolid("hubWithHole", hubMountBox, hubMntHoleTubs,
		rotp_unit, G4ThreeVector(0.,0.,0.));

	//the mount plate's logical volume
	G4LogicalVolume * hubMountLV = new G4LogicalVolume(hubWithHole, Aluminum,"HubMountLV");
	hubMountLV->SetVisAttributes(frameAtt);

	//place the mount plate at the end of the hub: upstream and down
	//We have to rotate the hub z->x.
	const G4double hubMount_px = vertSupportYZ_px - vertSupportYZ_x - hubMount_z;
	const G4ThreeVector downHubMountPos = G4ThreeVector(hubMount_px,0.0,0.0) + origin;
	new G4PVPlacement(rot90y, downHubMountPos, hubMountLV, "HubMountPhys",World_Logical,false,0); 
	
	const G4ThreeVector upHubMountPos = G4ThreeVector(-hubMount_px,0.0,0.0) + origin;
	new G4PVPlacement(rot90y, upHubMountPos, hubMountLV, "HubMountPhys",World_Logical,false,1); 
	

#if 0
	//------------------------------ Hole in the hub mount plate
	
	const G4double hubMntHole_Rmin = 0.0;
	const G4double hubMntHole_Rmax = hub_hole_diam/2.0;
	const G4double hubMntHole_z = hubMount_z + 1.*mm; //material thickness
	
	G4Tubs * hubMntHoleTubs = new G4Tubs("HubMntHoleTubs",hubMntHole_Rmin,hubMntHole_Rmax,hubMntHole_z,0.0,360.0*deg);
	G4SubtractionSolid * 
		hubWithHole = new G4SubtractionSolid("hubWithHole", hubMountBox, hubMntHoleTubs, 0, 0);
	G4LogicalVolume * ArmSegWithHoleLV = new G4LogicalVolume(ArmSegWithHole, Aluminum, "ArmSegWithHoleLV");

	//The hole's logical volume
	G4LogicalVolume * hubMntHoleLV = new G4LogicalVolume(hubMntHoleTubs,Air,"HubMntHoleLV");
	hubMntHoleLV->SetVisAttributes(frameHoleAtt);
	
	//the hole is right in the midle of the mount plate
	new G4PVPlacement(0, G4ThreeVector(0.0,0.0,0.0), hubMntHoleLV, "HubMntHolePhys",hubMountLV,false,0); 

#endif

	//------------------------------ Blowfish Hub
/*
	//build the hub
	BXHubBuilder theHubBuilder;
	theHubBuilder.Build();

	//place the two hubs in the world: upstream and downstream
	//Remember we rotated the hub mount z->x
	const G4double hubCont_px = hubMount_px - hubMount_z - theHubBuilder.GetLength()/2.0;
	
	const G4ThreeVector downHubContPos = G4ThreeVector(hubCont_px,0.0,0.0) + origin;
	new G4PVPlacement(rot90y, downHubContPos, theHubBuilder.GetLVPointer(), "HubContPhys",World_Logical,false,0); 

	const G4ThreeVector upHubContPos = G4ThreeVector(-hubCont_px,0.0,0.0) + origin;
	new G4PVPlacement(rot270y, upHubContPos, theHubBuilder.GetLVPointer(), "HubContPhys",World_Logical,false,1); 
*/
	
	//------------------------------------------------------------------
	// Blowfish Under Support
	//------------------------------------------------------------------

	//------------------------------ Parameters
	
	//taken from base_support_3.dwb
	
	//length of the alluminum supports for the under support portion of the frame
	//parallel and perpendicular to the beam
	const G4double para_under_length = 104.0*in;
	const G4double perp_under_length = 83.25*in;
	
	//the amount of length that the parallel supports stick out
	const G4double para_extend_length = 4.0*in; //CHECK THIS NUMBER: IT IS A GUESS

	//spacer used to support the corner supports
	const G4double under_spacer_length = 30.0*in;
	const G4double under_spacer_width = 3.0*in;
	
	//longest length of the corner support
	const G4double under_corner_length = 57.0*in;
	
	//------------------------------ Under supports: perpendicular to beam
	
	const G4double underSupportZX_x = support_material_width/2.0;
	const G4double underSupportZX_y = support_material_thickness/2.0;
	const G4double underSupportZX_z = perp_under_length/2.0;

	G4Box * underSupportZXBox = new G4Box("UnderSupportZXBox",underSupportZX_x,underSupportZX_y,underSupportZX_z);

	const G4double underSupportZY_x = support_material_thickness/2.0;
	const G4double underSupportZY_y = support_material_width/2.0 - underSupportZX_y;
	const G4double underSupportZY_z = perp_under_length/2.0;

	G4Box * underSupportZYBox = new G4Box("UnderSupportZYBox",underSupportZY_x,underSupportZY_y,underSupportZY_z);

	//Logical volumes for parallel under upports
	G4LogicalVolume * underSupportZXLV = new G4LogicalVolume(underSupportZXBox,Aluminum,"UnderSupportZXLV");
	G4LogicalVolume * underSupportZYLV = new G4LogicalVolume(underSupportZYBox,Aluminum,"UnderSupportZYLV");
	underSupportZXLV->SetVisAttributes(frameAtt);
	underSupportZYLV->SetVisAttributes(frameAtt);

	//Place the under support into the world: relative to the origin which may or may not be (0,0,0)
	//The y-direction is set by using base support XZ which is the most bottom base support 
	//we must also leave enough room for the corner peices
	const G4double underSupportZX_px = para_under_length/2.0 - para_extend_length - underSupportZX_x;
	const G4double underSupportZX_py = baseSupportXZ_py - baseSupportXZ_y - underSupportZX_y - support_material_thickness;

	const G4double underSupportZY_px = underSupportZX_px + underSupportZX_x - underSupportZY_x;
	const G4double underSupportZY_py = underSupportZX_py + underSupportZX_y + underSupportZY_y; 

	const G4ThreeVector underSupportZXaPos = G4ThreeVector(underSupportZX_px,underSupportZX_py,0.0) + origin;
	const G4ThreeVector underSupportZYaPos = G4ThreeVector(underSupportZY_px,underSupportZY_py,0.0) + origin;
	new G4PVPlacement(0,underSupportZXaPos, underSupportZXLV, "UnderSupportZXPhys", World_Logical,false,0);
	new G4PVPlacement(0,underSupportZYaPos, underSupportZYLV, "UnderSupportZYPhys", World_Logical,false,0);
	
	const G4ThreeVector underSupportZXbPos = G4ThreeVector(-underSupportZX_px,underSupportZX_py,0.0) + origin;
	const G4ThreeVector underSupportZYbPos = G4ThreeVector(-underSupportZY_px,underSupportZY_py,0.0) + origin;
	new G4PVPlacement(0,underSupportZXbPos, underSupportZXLV, "UnderSupportZXPhys", World_Logical,false,1);
	new G4PVPlacement(0,underSupportZYbPos, underSupportZYLV, "UnderSupportZYPhys", World_Logical,false,1);
	
	
	//------------------------------ Under supports: parallel to beam
	
	const G4double underSupportXZ_x = para_under_length/2.0;
	const G4double underSupportXZ_y = support_material_thickness/2.0;
	const G4double underSupportXZ_z = support_material_width/2.0;

	G4Box * underSupportXZBox = new G4Box("UnderSupportXZBox",underSupportXZ_x,underSupportXZ_y,underSupportXZ_z);

	const G4double underSupportXY_x = para_under_length/2.0;
	const G4double underSupportXY_y = support_material_width/2.0 - underSupportXZ_y;
	const G4double underSupportXY_z = support_material_thickness/2.0;

	G4Box * underSupportXYBox = new G4Box("UnderSupportXYBox",underSupportXY_x,underSupportXY_y,underSupportXY_z);

	//Logical volumes for parallel under upports
	G4LogicalVolume * underSupportXZLV = new G4LogicalVolume(underSupportXZBox,Aluminum,"UnderSupportXZLV");
	G4LogicalVolume * underSupportXYLV = new G4LogicalVolume(underSupportXYBox,Aluminum,"UnderSupportXYLV");
	underSupportXZLV->SetVisAttributes(frameAtt);
	underSupportXYLV->SetVisAttributes(frameAtt);

	//Place the under support into the world: relative to the origin which may or may not be (0,0,0)
	const G4double underSupportXZ_py = underSupportZX_py - underSupportZX_y - underSupportXZ_y; 
	const G4double underSupportXZ_pz = perp_under_length/2.0 - underSupportXZ_z + 2.0*underSupportXY_z;

	const G4double underSupportXY_py = underSupportXZ_py + underSupportXZ_y + underSupportXY_y; 
	const G4double underSupportXY_pz = underSupportXZ_pz + underSupportXZ_z - underSupportXY_z;

	const G4ThreeVector underSupportXZaPos = G4ThreeVector(0.0,underSupportXZ_py,underSupportXZ_pz) + origin;
	const G4ThreeVector underSupportXYaPos = G4ThreeVector(0.0,underSupportXY_py,underSupportXY_pz) + origin;
	new G4PVPlacement(0,underSupportXZaPos, underSupportXZLV, "UnderSupportXZPhys", World_Logical,false,0);
	new G4PVPlacement(0,underSupportXYaPos, underSupportXYLV, "UnderSupportXYPhys", World_Logical,false,0);
	
	const G4ThreeVector underSupportXZbPos = G4ThreeVector(0.0,underSupportXZ_py,-underSupportXZ_pz) + origin;
	const G4ThreeVector underSupportXYbPos = G4ThreeVector(0.0,underSupportXY_py,-underSupportXY_pz) + origin;
	new G4PVPlacement(0,underSupportXZbPos, underSupportXZLV, "UnderSupportXZPhys", World_Logical,false,1);
	new G4PVPlacement(0,underSupportXYbPos, underSupportXYLV, "UnderSupportXYPhys", World_Logical,false,1);
	
	
	//------------------------------ Under supports: spacer
	
	const G4double underSpacer_x = under_spacer_length/2.0;
	const G4double underSpacer_y = support_material_thickness/2.0;
	const G4double underSpacer_z = under_spacer_width/2.0;

	G4Box * underSpacerBox = new G4Box("UnderSpacerBox", underSpacer_x,underSpacer_y,underSpacer_z);
	G4LogicalVolume * underSpacerLV = new G4LogicalVolume(underSpacerBox,Aluminum,"UnderSpacerLV");
	underSpacerLV->SetVisAttributes(frameAtt);

	//place the spacer ontop of the parallel under spports
	const G4double underSpacer_py = underSupportXZ_py + underSupportXZ_y + underSpacer_y;
	const G4double underSpacer_pz = underSupportXZ_pz - underSupportXZ_z + underSpacer_z;

	const G4ThreeVector underSpaceraPos = G4ThreeVector(0.0,underSpacer_py,underSpacer_pz) + origin;
	const G4ThreeVector underSpacerbPos = G4ThreeVector(0.0,underSpacer_py,-underSpacer_pz) + origin;
	new G4PVPlacement(0,underSpaceraPos,underSpacerLV,"UnderSpaceraPhys",World_Logical,false,0);
	new G4PVPlacement(0,underSpacerbPos,underSpacerLV,"UnderSpacerbPhys",World_Logical,false,0);
	
	
	//------------------------------ Under supports: corner supports

	//Make the flat peice out of a trapezoid so that it has mitered corners that fit nicely
	//into the rest of the under support structure
	const G4double cornerSupportXZ_y = support_material_thickness/2.0;
	const G4double cornerSupportXZ_z = support_material_width/2.0;
	const G4double cornerSupportXZ_x1 = under_corner_length/2.0;
	const G4double cornerSupportXZ_x2 = (under_corner_length - 2.0*support_material_width)/2.0;

	G4Trd * cornerSupportXZTrd = new G4Trd( "CornerSupportXZTrd", cornerSupportXZ_x1, cornerSupportXZ_x2,
		cornerSupportXZ_y, cornerSupportXZ_y, cornerSupportXZ_z );

	//Make the vertical peice out of a box.
	//Make an approximation to make it fit in the geometry
	//We truncate its length so that it butts up against the frame but does not go underneath
	const G4double cornerSupportXY_x = under_corner_length/2.0 - 2.0*support_material_width/cos(45.0*deg)/2.0;
	const G4double cornerSupportXY_y = support_material_width/2.0;
	const G4double cornerSupportXY_z = support_material_thickness/2.0;
	
	G4Box * cornerSupportXYBox = new G4Box("CornerSupportXYBox", cornerSupportXY_x,cornerSupportXY_y,cornerSupportXY_z);

	//create the logical volumes
	G4LogicalVolume * cornerSupportXZLV = new G4LogicalVolume(cornerSupportXZTrd,Aluminum,"CornerSupportXZLV");
	G4LogicalVolume * cornerSupportXYLV = new G4LogicalVolume(cornerSupportXYBox,Aluminum,"CornerSupportXYLV");
	cornerSupportXZLV->SetVisAttributes(frameAtt);
	cornerSupportXYLV->SetVisAttributes(frameAtt);

	//place the corner support relative to the under supports
	//The mitered edgest must be butted up to the other parts of the under suport structure
	const G4double cornerSupportXZ_px = underSupportZY_px - underSupportZY_x - (cornerSupportXZ_z + cornerSupportXZ_x2)*cos(45.0*deg);
	const G4double cornerSupportXZ_py = underSpacer_py + underSpacer_y + cornerSupportXZ_y;
	const G4double cornerSupportXZ_pz = underSupportXY_pz - underSupportXY_z - (cornerSupportXZ_z + cornerSupportXZ_x2)*cos(45.0*deg);
	
	const G4double cornerSupportXY_px = cornerSupportXZ_px - (cornerSupportXZ_z - cornerSupportXY_z)*cos(45.0*deg);
	const G4double cornerSupportXY_py = cornerSupportXZ_py - cornerSupportXZ_y - cornerSupportXY_y;
	const G4double cornerSupportXY_pz = cornerSupportXZ_pz - (cornerSupportXZ_z - cornerSupportXY_z)*cos(45.0*deg);

	const G4ThreeVector cornerSupportXZaPos = G4ThreeVector( cornerSupportXZ_px,cornerSupportXZ_py, cornerSupportXZ_pz) + origin;
	const G4ThreeVector cornerSupportXZbPos = G4ThreeVector( cornerSupportXZ_px,cornerSupportXZ_py,-cornerSupportXZ_pz) + origin;
	const G4ThreeVector cornerSupportXZcPos = G4ThreeVector(-cornerSupportXZ_px,cornerSupportXZ_py, cornerSupportXZ_pz) + origin;
	const G4ThreeVector cornerSupportXZdPos = G4ThreeVector(-cornerSupportXZ_px,cornerSupportXZ_py,-cornerSupportXZ_pz) + origin;
	new G4PVPlacement(rot315y,cornerSupportXZaPos,cornerSupportXZLV,"CornerSupportXZPhys", World_Logical,false,0);
	new G4PVPlacement(rot225y,cornerSupportXZbPos,cornerSupportXZLV,"CornerSupportXZPhys", World_Logical,false,1);
	new G4PVPlacement(rot45y ,cornerSupportXZcPos,cornerSupportXZLV,"CornerSupportXZPhys", World_Logical,false,2);
	new G4PVPlacement(rot135y,cornerSupportXZdPos,cornerSupportXZLV,"CornerSupportXZPhys", World_Logical,false,3);

	const G4ThreeVector cornerSupportXYaPos = G4ThreeVector( cornerSupportXY_px,cornerSupportXY_py, cornerSupportXY_pz) + origin;
	const G4ThreeVector cornerSupportXYbPos = G4ThreeVector( cornerSupportXY_px,cornerSupportXY_py,-cornerSupportXY_pz) + origin;
	const G4ThreeVector cornerSupportXYcPos = G4ThreeVector(-cornerSupportXY_px,cornerSupportXY_py, cornerSupportXY_pz) + origin;
	const G4ThreeVector cornerSupportXYdPos = G4ThreeVector(-cornerSupportXY_px,cornerSupportXY_py,-cornerSupportXY_pz) + origin;
	new G4PVPlacement(rot315y,cornerSupportXYaPos,cornerSupportXYLV,"CornerSupportXYPhys", World_Logical,false,0);
	new G4PVPlacement(rot225y,cornerSupportXYbPos,cornerSupportXYLV,"CornerSupportXYPhys", World_Logical,false,1);
	new G4PVPlacement(rot45y ,cornerSupportXYcPos,cornerSupportXYLV,"CornerSupportXYPhys", World_Logical,false,2);
	new G4PVPlacement(rot135y,cornerSupportXYdPos,cornerSupportXYLV,"CornerSupportXYPhys", World_Logical,false,3);

	//------------------------------------------------------------------
	// Power supply
	//------------------------------------------------------------------

	//We build the power supply out of 1/16 inch steel
	//and place it on the upstream end of Blowfish.
	//We simulate the top, bottom, sides and front of the supply.
	//We do not simulate the back as this is where the modules go and
	//there is much open space.
	
	const G4double power_supply_height = 24.5*in;
	const G4double power_supply_length = 22.25*in;
	const G4double power_supply_width  = 16.0*in;
	const G4double power_supply_material_thickness = 1.0/16.0*in;

	//The power supply is supported by a 2x4 board (1.5 in x 3.5 in)
	//and a peiece of 1/2 in plywood that we do not simulate here.
	const G4double power_supply_height_from_frame = 4.0*in;

	//Build the iron box and an air box which will go inside
	const G4double power_supply_x = power_supply_length/2.0;
	const G4double power_supply_y = power_supply_height/2.0;
	const G4double power_supply_z = power_supply_width/2.0;
	G4Box * powerSupplyBox = new G4Box("PowerSupplyBox", power_supply_x, power_supply_y, power_supply_z);
	
	const G4double power_supply_air_x = (power_supply_length - 1.0*power_supply_material_thickness + 0.1*mm )/2.0; //do not include back
	const G4double power_supply_air_y = (power_supply_height - 2.0*power_supply_material_thickness )/2.0;
	const G4double power_supply_air_z = (power_supply_width  - 2.0*power_supply_material_thickness )/2.0;
	G4Box * powerSupplyAirBox = new G4Box("PowerSupplyAirBox", power_supply_air_x, power_supply_air_y, power_supply_air_z);

	G4SubtractionSolid * 
		powerSupplyWithHoleBox = new G4SubtractionSolid("powerSupplyWithHoleBox", powerSupplyBox, powerSupplyAirBox,
		rotp_unit, G4ThreeVector(power_supply_material_thickness,0.,0.));

	//create the logical volumes for these boxes
	G4LogicalVolume * powerSupplyLV    = new G4LogicalVolume(powerSupplyWithHoleBox, Steel, "PowerSupplyLV");

	//place the power supply box in the world volume
	const G4double powerSupply_px = -vertSupportYZ_px + vertSupportYZ_x + power_supply_x;
	const G4double powerSupply_py = baseSupportZX_py + baseSupportZX_y + power_supply_y + power_supply_height_from_frame;
	const G4double powerSupply_pz = 0.0; //centered in the array

	const G4ThreeVector powerSupplyPos = G4ThreeVector(powerSupply_px,powerSupply_py,powerSupply_pz) + origin;
	new G4PVPlacement(0,powerSupplyPos,powerSupplyLV, "PowerSupplyPhys", World_Logical, false, 0);
	
	//Set the visual attributes
	G4Colour powerSupplyBlue = G4Colour(102.0/255.0,153.0/255.0,1.0);
	G4VisAttributes* powerSupplyAtt = new G4VisAttributes(powerSupplyBlue);
	powerSupplyLV->SetVisAttributes(powerSupplyAtt);

}
