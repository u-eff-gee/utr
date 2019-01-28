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

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "Units.hh"
#include "Detectors_G3_FGIC.hh"
#include "HPGe_60_TUNL_30986.hh"
#include "HPGe_60_TUNL_31061.hh"
#include "HPGe_60_TUNL_40663.hh"
#include "HPGe_ANL.hh"
#include "LaBr_TUD.hh"
#include "FilterCase.hh"

Detectors_G3_FGIC::Detectors_G3_FGIC(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Detectors_G3_FGIC::Construct(G4ThreeVector global_coordinates){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");

	/************************** Detectors ***************************/
	//
	// Placement in spherical coordinate system
	//
	//                      y      z = beam direction
	//
	//                      |   /
	//                      |  /
	//                      | /
	//                      O------ x = polarization plane
	//
	//
	//			y
	//
	//			|   /Λ
	//			|  / |  phi
	//			| /  |
	//			|/___|__ x
	//
	//
	//			x-y-Plane
	//
	//			|   /Λ
	//			|  / |  theta
	//			| /  |
	//			|/___|__ z
	//

	/**************** HPGE1 *******************/
/*
	G4double HPGe1_rt = 250. * mm;
	G4double HPGe1_dy = 0. * mm;
	G4double HPGe1_dz = 0. * mm;
	G4double HPGe1_phi = 315. * deg;
	G4double HPGe1_theta = 135. * deg;

	G4double HPGe1_AngleX = 215.264 * deg;
	G4double HPGe1_AngleY = 150. * deg;

	G4double HPGe1_Cu_Radius = 45.*mm;
	G4double HPGe1_Cu_Thickness = 1.*1.15*mm;
	G4double HPGe1_Pb_Radius = 45.*mm;
	G4double HPGe1_Pb_Thickness = 2.*1.2*mm;
	G4double HPGe1_Pb_Wrap_Thickness = 1.2*mm;
	G4double HPGe1_Pb_Wrap_Length = 160.*mm;

	HPGe_60_TUNL_40663 *HPGe1_Instance = new HPGe_60_TUNL_40663("HPGe1");
	G4LogicalVolume *HPGe1_Logical = HPGe1_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe1 = new G4RotationMatrix();
	rotateHPGe1->rotateX(HPGe1_AngleX);
	rotateHPGe1->rotateY(HPGe1_AngleY);

	HPGe1_rt = HPGe1_rt + HPGe1_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe1,
	    global_coordinates + G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Logical, "HPGe1", World_Logical, false, 0);

	HPGe1_rt -= HPGe1_Instance->Get_Length() * 0.5;

	if(HPGe1_Pb_Wrap_Thickness != 0.){
		HPGe1_rt += HPGe1_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe1_Pb_Wrap_Solid = new G4Tubs("HPGe1_Pb_Wrap_Solid", HPGe1_Instance->Get_Front_Radius(), HPGe1_Instance->Get_Front_Radius() + HPGe1_Pb_Wrap_Thickness, HPGe1_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe1_Pb_Wrap_Logical = new G4LogicalVolume(HPGe1_Pb_Wrap_Solid, Pb, "HPGe1_Pb_Wrap_Logical");
		HPGe1_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1,
	    global_coordinates + G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Pb_Wrap_Logical, "HPGe1_Pb_Wrap", World_Logical, false, 0);

		HPGe1_rt -= HPGe1_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase1(HPGe1_Pb_Thickness + HPGe1_Cu_Thickness, false);
	HPGe1_rt -= filterCase1.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe1, 
	    global_coordinates + G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    filterCase1.Get_Logical(), "HPGe1_FilterCase", World_Logical, false, 0, false
	    );
	
	HPGe1_rt += filterCase1.Get_Offset_From_Detector();

	if(HPGe1_Cu_Thickness > 0.){
		HPGe1_rt -= HPGe1_Cu_Thickness * 0.5;

		G4Tubs* HPGe1_Cu_Solid = new G4Tubs("HPGe1_Cu_Solid", 0., HPGe1_Cu_Radius, HPGe1_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_Cu_Logical = new G4LogicalVolume(HPGe1_Cu_Solid, Cu, "HPGe1_Cu_Logical");
		HPGe1_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe1,
	    global_coordinates + G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Cu_Logical, "HPGe1_Cu", World_Logical, false, 0);
	}

	HPGe1_rt -= HPGe1_Cu_Thickness*0.5;

	if(HPGe1_Pb_Thickness > 0.){
		HPGe1_rt -= HPGe1_Pb_Thickness * 0.5;

		G4Tubs* HPGe1_Pb_Solid = new G4Tubs("HPGe1_Pb_Solid", 0., HPGe1_Pb_Radius, HPGe1_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_Pb_Logical = new G4LogicalVolume(HPGe1_Pb_Solid, Pb, "HPGe1_Pb_Logical");
		HPGe1_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1,
	    global_coordinates + G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Pb_Logical, "HPGe1_Pb", World_Logical, false, 0);
	}
*/
	/**************** HPGE2 *******************/
/*
	G4double HPGe2_rt = 250. * mm;
	G4double HPGe2_dy = 0. * mm;
	G4double HPGe2_dz = 0. * mm;
	G4double HPGe2_phi = 90. * deg;
	G4double HPGe2_theta = 90. * deg;

	G4double HPGe2_AngleX = 270. * deg;
	G4double HPGe2_AngleY = 0. * deg;
	G4double HPGe2_AngleZ = 180. * deg;

	G4double HPGe2_Cu_Radius = 45.*mm;
	G4double HPGe2_Cu_Thickness = 1.*1.15*mm;
	G4double HPGe2_Pb_Radius = 45.*mm;
	G4double HPGe2_Pb_Thickness = (1.*1.2 + 2.*2.)*mm;
	G4double HPGe2_Pb_Wrap_Thickness = 1.*1.2*mm;
	G4double HPGe2_Pb_Wrap_Length = 130.*mm;

	HPGe_60_TUNL_30986 *HPGe2_Instance = new HPGe_60_TUNL_30986("HPGe2");
	G4LogicalVolume *HPGe2_Logical = HPGe2_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe2 = new G4RotationMatrix();
	rotateHPGe2->rotateX(HPGe2_AngleX);
	rotateHPGe2->rotateY(HPGe2_AngleY);
	rotateHPGe2->rotateZ(HPGe2_AngleZ);

	HPGe2_rt = HPGe2_rt + HPGe2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe2,
	    global_coordinates + G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Logical, "HPGe2", World_Logical, false, 0);

	HPGe2_rt -= HPGe2_Instance->Get_Length() * 0.5;

	if(HPGe2_Pb_Wrap_Thickness != 0.){
		HPGe2_rt += HPGe2_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe2_Pb_Wrap_Solid = new G4Tubs("HPGe2_Pb_Wrap_Solid", HPGe2_Instance->Get_Front_Radius(), HPGe2_Instance->Get_Front_Radius() + HPGe2_Pb_Wrap_Thickness, HPGe2_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe2_Pb_Wrap_Logical = new G4LogicalVolume(HPGe2_Pb_Wrap_Solid, Pb, "HPGe2_Pb_Wrap_Logical");
		HPGe2_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe2,
	    global_coordinates + G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Pb_Wrap_Logical, "HPGe2_Pb_Wrap", World_Logical, false, 0);

		HPGe2_rt -= HPGe2_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase2(HPGe2_Pb_Thickness + HPGe2_Cu_Thickness, false);
	HPGe2_rt -= filterCase2.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe2, 
	    global_coordinates + G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    filterCase2.Get_Logical(), "HPGe2_FilterCase", World_Logical, false, 0, false
	    );
	
	HPGe2_rt += filterCase2.Get_Offset_From_Detector();

	if(HPGe2_Cu_Thickness > 0.){
		HPGe2_rt -= HPGe2_Cu_Thickness * 0.5;

		G4Tubs* HPGe2_Cu_Solid = new G4Tubs("HPGe2_Cu_Solid", 0., HPGe2_Cu_Radius, HPGe2_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe2_Cu_Logical = new G4LogicalVolume(HPGe2_Cu_Solid, Cu, "HPGe2_Cu_Logical");
		HPGe2_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe2,
	    global_coordinates + G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Cu_Logical, "HPGe2_Cu", World_Logical, false, 0);
	}

	HPGe2_rt -= HPGe2_Cu_Thickness*0.5;

	if(HPGe2_Pb_Thickness > 0.){
		HPGe2_rt -= HPGe2_Pb_Thickness * 0.5;

		G4Tubs* HPGe2_Pb_Solid = new G4Tubs("HPGe2_Pb_Solid", 0., HPGe2_Pb_Radius, HPGe2_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe2_Pb_Logical = new G4LogicalVolume(HPGe2_Pb_Solid, Pb, "HPGe2_Pb_Logical");
		HPGe2_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe2,
	    global_coordinates + G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Pb_Logical, "HPGe2_Pb", World_Logical, false, 0);
	}
*/
	/**************** HPGE3 *******************/
/*
	G4double HPGe3_rt = 250. * mm;
	G4double HPGe3_dy = 0. * mm; 
	G4double HPGe3_dz = 0. * mm;
	G4double HPGe3_phi = 45. * deg;
	G4double HPGe3_theta = 135. * deg;

	G4double HPGe3_AngleX = 144.736 * deg;
	G4double HPGe3_AngleY = 150. * deg;
	G4double HPGe3_AngleZ = 0. * deg;

	G4double HPGe3_Cu_Radius = 45.*mm;
	G4double HPGe3_Cu_Thickness = 1.*1.15*mm;
	G4double HPGe3_Pb_Radius = 45.*mm;
	G4double HPGe3_Pb_Thickness = 1.*1.2*mm;
	G4double HPGe3_Pb_Wrap_Thickness = 2.*1.2*mm;
	G4double HPGe3_Pb_Wrap_Length = 160.*mm;

	HPGe_60_TUNL_31061 *HPGe3_Instance = new HPGe_60_TUNL_31061("HPGe3");
	G4LogicalVolume *HPGe3_Logical = HPGe3_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe3 = new G4RotationMatrix();
	rotateHPGe3->rotateX(HPGe3_AngleX);
	rotateHPGe3->rotateY(HPGe3_AngleY);
	rotateHPGe3->rotateZ(HPGe3_AngleZ);

	HPGe3_rt = HPGe3_rt + HPGe3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe3,
	    global_coordinates + G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Logical, "HPGe3", World_Logical, false, 0);

	HPGe3_rt -= HPGe3_Instance->Get_Length() * 0.5;

	if(HPGe3_Pb_Wrap_Thickness != 0.){
		HPGe3_rt += HPGe3_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe3_Pb_Wrap_Solid = new G4Tubs("HPGe3_Pb_Wrap_Solid", HPGe3_Instance->Get_Front_Radius(), HPGe3_Instance->Get_Front_Radius() + HPGe3_Pb_Wrap_Thickness, HPGe3_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe3_Pb_Wrap_Logical = new G4LogicalVolume(HPGe3_Pb_Wrap_Solid, Pb, "HPGe3_Pb_Wrap_Logical");
		HPGe3_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe3,
	    global_coordinates + G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Pb_Wrap_Logical, "HPGe3_Pb_Wrap", World_Logical, false, 0);

		HPGe3_rt -= HPGe3_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase3(HPGe3_Pb_Thickness + HPGe3_Cu_Thickness, false);
	HPGe3_rt -= filterCase3.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe3, 
	    global_coordinates + G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    filterCase3.Get_Logical(), "HPGe3_FilterCase", World_Logical, false, 0, false
	    );
	
	HPGe3_rt += filterCase3.Get_Offset_From_Detector();

	if(HPGe3_Cu_Thickness > 0.){
		HPGe3_rt -= HPGe3_Cu_Thickness * 0.5;

		G4Tubs* HPGe3_Cu_Solid = new G4Tubs("HPGe3_Cu_Solid", 0., HPGe3_Cu_Radius, HPGe3_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe3_Cu_Logical = new G4LogicalVolume(HPGe3_Cu_Solid, Cu, "HPGe3_Cu_Logical");
		HPGe3_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe3,
	    global_coordinates + G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Cu_Logical, "HPGe3_Cu", World_Logical, false, 0);
	}

	HPGe3_rt -= HPGe3_Cu_Thickness*0.5;

	if(HPGe3_Pb_Thickness > 0.){
		HPGe3_rt -= HPGe3_Pb_Thickness * 0.5;

		G4Tubs* HPGe3_Pb_Solid = new G4Tubs("HPGe3_Pb_Solid", 0., HPGe3_Pb_Radius, HPGe3_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe3_Pb_Logical = new G4LogicalVolume(HPGe3_Pb_Solid, Pb, "HPGe3_Pb_Logical");
		HPGe3_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe3,
	    global_coordinates + G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Pb_Logical, "HPGe3_Pb", World_Logical, false, 0);
	}
*/
	/**************** HPGE4 *******************/
/*
	G4double HPGe4_rt = 250. * mm; 
	G4double HPGe4_dy = 0. * mm; 
	G4double HPGe4_dz = 0. * mm; 
	G4double HPGe4_phi = 180. * deg;
	G4double HPGe4_theta = 90. * deg;

	G4double HPGe4_AngleX = 0. * deg;
	G4double HPGe4_AngleY = 270. * deg;
	G4double HPGe4_AngleZ = 0. * deg;

	G4double HPGe4_Cu_Radius = 45.*mm; 
	G4double HPGe4_Cu_Thickness = 0.*mm; 
	G4double HPGe4_Pb_Radius = 45.*mm; 
	G4double HPGe4_Pb_Thickness = 5.*1.2*mm; 
	G4double HPGe4_Pb_Wrap_Thickness = 2.*1.2*mm; 
	G4double HPGe4_Pb_Wrap_Length = 150.*mm; 

	HPGe_ANL *HPGe4_Instance = new HPGe_ANL("HPGe4");
	G4LogicalVolume *HPGe4_Logical = HPGe4_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe4 = new G4RotationMatrix();
	rotateHPGe4->rotateX(HPGe4_AngleX);
	rotateHPGe4->rotateY(HPGe4_AngleY);
	rotateHPGe4->rotateZ(HPGe4_AngleZ);

	HPGe4_rt = HPGe4_rt + HPGe4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe4,
	    global_coordinates + G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Logical, "HPGe4", World_Logical, false, 0);

	HPGe4_rt -= HPGe4_Instance->Get_Length() * 0.5;

	if(HPGe4_Pb_Wrap_Thickness != 0.){
		HPGe4_rt += HPGe4_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe4_Pb_Wrap_Solid = new G4Tubs("HPGe4_Pb_Wrap_Solid", HPGe4_Instance->Get_Front_Radius(), HPGe4_Instance->Get_Front_Radius() + HPGe4_Pb_Wrap_Thickness, HPGe4_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe4_Pb_Wrap_Logical = new G4LogicalVolume(HPGe4_Pb_Wrap_Solid, Pb, "HPGe4_Pb_Wrap_Logical");
		HPGe4_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe4,
	    global_coordinates + G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Pb_Wrap_Logical, "HPGe4_Pb_Wrap", World_Logical, false, 0);

		HPGe4_rt -= HPGe4_Pb_Wrap_Length * 0.5;
	}

	if(HPGe4_Cu_Thickness > 0.){
		HPGe4_rt -= HPGe4_Cu_Thickness * 0.5;

		G4Tubs* HPGe4_Cu_Solid = new G4Tubs("HPGe4_Cu_Solid", 0., HPGe4_Cu_Radius, HPGe4_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe4_Cu_Logical = new G4LogicalVolume(HPGe4_Cu_Solid, Cu, "HPGe4_Cu_Logical");
		HPGe4_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe4,
	    global_coordinates + G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Cu_Logical, "HPGe4_Cu", World_Logical, false, 0);
	}

	HPGe4_rt -= HPGe4_Cu_Thickness*0.5;

	if(HPGe4_Pb_Thickness > 0.){
		HPGe4_rt -= HPGe4_Pb_Thickness * 0.5;

		G4Tubs* HPGe4_Pb_Solid = new G4Tubs("HPGe4_Pb_Solid", 0., HPGe4_Pb_Radius, HPGe4_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe4_Pb_Logical = new G4LogicalVolume(HPGe4_Pb_Solid, Pb, "HPGe4_Pb_Logical");
		HPGe4_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe4,
	    global_coordinates + G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Pb_Logical, "HPGe4_Pb", World_Logical, false, 0);
	}
*/
	/**************** LABR1 *******************/

	G4double LaBr1_rt = 350. * mm; 
	G4double LaBr1_dy = 0. * mm; 
	G4double LaBr1_dz = 0. * mm; 
	G4double LaBr1_phi = 0. * deg;
	G4double LaBr1_theta = 90. * deg;

	G4double LaBr1_AngleX = 0. * deg;
	G4double LaBr1_AngleY = 90. * deg;
	G4double LaBr1_AngleZ = 0. * deg;

	G4double LaBr1_Cu_Radius = 45.*mm; 
	G4double LaBr1_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr1_Pb_Radius = 45.*mm; 
	G4double LaBr1_Pb_Thickness = 0.*mm; 
	G4double LaBr1_Pb_Wrap_Thickness = 1.2*mm; 
	G4double LaBr1_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr1_Instance = new LaBr_TUD("LaBr1");
	G4LogicalVolume *LaBr1_Logical = LaBr1_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr1 = new G4RotationMatrix();
	rotateLaBr1->rotateX(LaBr1_AngleX);
	rotateLaBr1->rotateY(LaBr1_AngleY);
	rotateLaBr1->rotateZ(LaBr1_AngleZ);

	LaBr1_rt = LaBr1_rt + LaBr1_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr1,
	    global_coordinates + G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_Logical, "LaBr1", World_Logical, false, 0);

	LaBr1_rt -= LaBr1_Instance->Get_Length() * 0.5;

	if(LaBr1_Pb_Wrap_Thickness != 0.){
		LaBr1_rt += LaBr1_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr1_Pb_Wrap_Solid = new G4Tubs("LaBr1_Pb_Wrap_Solid", LaBr1_Instance->Get_Front_Radius(), LaBr1_Instance->Get_Front_Radius() + LaBr1_Pb_Wrap_Thickness, LaBr1_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr1_Pb_Wrap_Logical = new G4LogicalVolume(LaBr1_Pb_Wrap_Solid, Pb, "LaBr1_Pb_Wrap_Logical");
		LaBr1_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr1,
	    global_coordinates + G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_Pb_Wrap_Logical, "LaBr1_Pb_Wrap", World_Logical, false, 0);

		LaBr1_rt -= LaBr1_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL1(LaBr1_Pb_Thickness + LaBr1_Cu_Thickness, true);
	LaBr1_rt -= filterCaseL1.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr1, 
	    global_coordinates + G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    filterCaseL1.Get_Logical(), "LaBr1_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr1_rt += filterCaseL1.Get_Offset_From_Detector();
	LaBr1_rt -= filterCaseL1.Get_FilterCaseRing_Thickness();
*/
	if(LaBr1_Cu_Thickness > 0.){
		LaBr1_rt -= LaBr1_Cu_Thickness * 0.5;

		G4Tubs* LaBr1_Cu_Solid = new G4Tubs("LaBr1_Cu_Solid", 0., LaBr1_Cu_Radius, LaBr1_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr1_Cu_Logical = new G4LogicalVolume(LaBr1_Cu_Solid, Cu, "LaBr1_Cu_Logical");
		LaBr1_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr1,
	    global_coordinates + G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_Cu_Logical, "LaBr1_Cu", World_Logical, false, 0);
	}

	LaBr1_rt -= LaBr1_Cu_Thickness*0.5;

	if(LaBr1_Pb_Thickness > 0.){
		LaBr1_rt -= LaBr1_Pb_Thickness * 0.5;

		G4Tubs* LaBr1_Pb_Solid = new G4Tubs("LaBr1_Pb_Solid", 0., LaBr1_Pb_Radius, LaBr1_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr1_Pb_Logical = new G4LogicalVolume(LaBr1_Pb_Solid, Pb, "LaBr1_Pb_Logical");
		LaBr1_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr1,
	    global_coordinates + G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_Pb_Logical, "LaBr1_Pb", World_Logical, false, 0);
	}

	/**************** LABR2 *******************/

	G4double LaBr2_rt = 350. * mm; 
	G4double LaBr2_dy = 0. * mm; 
	G4double LaBr2_dz = 0. * mm; 
	G4double LaBr2_phi = 270. * deg;
	G4double LaBr2_theta = 90. * deg;

	G4double LaBr2_AngleX = 90. * deg;
	G4double LaBr2_AngleY = 0. * deg;
	G4double LaBr2_AngleZ = 0. * deg;

	G4double LaBr2_Cu_Radius = 45.*mm; 
	G4double LaBr2_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr2_Pb_Radius = 45.*mm; 
	G4double LaBr2_Pb_Thickness = 0.*mm; 
	G4double LaBr2_Pb_Wrap_Thickness = 1.*1.2*mm; 
	G4double LaBr2_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr2_Instance = new LaBr_TUD("LaBr2");
	G4LogicalVolume *LaBr2_Logical = LaBr2_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr2 = new G4RotationMatrix();
	rotateLaBr2->rotateX(LaBr2_AngleX);
	rotateLaBr2->rotateY(LaBr2_AngleY);
	rotateLaBr2->rotateZ(LaBr2_AngleZ);

	LaBr2_rt = LaBr2_rt + LaBr2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr2,
	    global_coordinates + G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Logical, "LaBr2", World_Logical, false, 0);

	LaBr2_rt -= LaBr2_Instance->Get_Length() * 0.5;

	if(LaBr2_Pb_Wrap_Thickness != 0.){
		LaBr2_rt += LaBr2_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr2_Pb_Wrap_Solid = new G4Tubs("LaBr2_Pb_Wrap_Solid", LaBr2_Instance->Get_Front_Radius(), LaBr2_Instance->Get_Front_Radius() + LaBr2_Pb_Wrap_Thickness, LaBr2_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr2_Pb_Wrap_Logical = new G4LogicalVolume(LaBr2_Pb_Wrap_Solid, Pb, "LaBr2_Pb_Wrap_Logical");
		LaBr2_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr2,
	    global_coordinates + G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Pb_Wrap_Logical, "LaBr2_Pb_Wrap", World_Logical, false, 0);

		LaBr2_rt -= LaBr2_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL2(LaBr2_Pb_Thickness + LaBr2_Cu_Thickness, false);
	LaBr2_rt -= filterCaseL2.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr2, 
	    global_coordinates + G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    filterCaseL2.Get_Logical(), "LaBr2_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr2_rt += filterCaseL2.Get_Offset_From_Detector();
*/
	if(LaBr2_Cu_Thickness > 0.){
		LaBr2_rt -= LaBr2_Cu_Thickness * 0.5;

		G4Tubs* LaBr2_Cu_Solid = new G4Tubs("LaBr2_Cu_Solid", 0., LaBr2_Cu_Radius, LaBr2_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr2_Cu_Logical = new G4LogicalVolume(LaBr2_Cu_Solid, Cu, "LaBr2_Cu_Logical");
		LaBr2_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr2,
	    global_coordinates + G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Cu_Logical, "LaBr2_Cu", World_Logical, false, 0);
	}

	LaBr2_rt -= LaBr2_Cu_Thickness*0.5;

	if(LaBr2_Pb_Thickness > 0.){
		LaBr2_rt -= LaBr2_Pb_Thickness * 0.5;

		G4Tubs* LaBr2_Pb_Solid = new G4Tubs("LaBr2_Pb_Solid", 0., LaBr2_Pb_Radius, LaBr2_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr2_Pb_Logical = new G4LogicalVolume(LaBr2_Pb_Solid, Pb, "LaBr2_Pb_Logical");
		LaBr2_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr2,
	    global_coordinates + G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Pb_Logical, "LaBr2_Pb", World_Logical, false, 0);
	}

	/**************** LABR3 *******************/

	G4double LaBr3_rt = 350. * mm; 
	G4double LaBr3_dy = 0. * mm; 
	G4double LaBr3_dz = 0. * mm; 
	G4double LaBr3_phi = 225. * deg;
	G4double LaBr3_theta = 135. * deg;

	G4double LaBr3_AngleX = 215.264 * deg;
	G4double LaBr3_AngleY = 210. * deg;
	G4double LaBr3_AngleZ = 0. * deg;

	G4double LaBr3_Cu_Radius = 45.*mm; 
	G4double LaBr3_Cu_Thickness = 1.*3.15*mm; 
	G4double LaBr3_Pb_Radius = 45.*mm; 
	G4double LaBr3_Pb_Thickness = 0.*mm; 
	G4double LaBr3_Pb_Wrap_Thickness = 2.*1.2*mm; 
	G4double LaBr3_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr3_Instance = new LaBr_TUD("LaBr3");
	G4LogicalVolume *LaBr3_Logical = LaBr3_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr3 = new G4RotationMatrix();
	rotateLaBr3->rotateX(LaBr3_AngleX);
	rotateLaBr3->rotateY(LaBr3_AngleY);
	rotateLaBr3->rotateZ(LaBr3_AngleZ);

	LaBr3_rt = LaBr3_rt + LaBr3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr3,
	    global_coordinates + G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_Logical, "LaBr3", World_Logical, false, 0);

	LaBr3_rt -= LaBr3_Instance->Get_Length() * 0.5;

	if(LaBr3_Pb_Wrap_Thickness != 0.){
		LaBr3_rt += LaBr3_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr3_Pb_Wrap_Solid = new G4Tubs("LaBr3_Pb_Wrap_Solid", LaBr3_Instance->Get_Front_Radius(), LaBr3_Instance->Get_Front_Radius() + LaBr3_Pb_Wrap_Thickness, LaBr3_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr3_Pb_Wrap_Logical = new G4LogicalVolume(LaBr3_Pb_Wrap_Solid, Pb, "LaBr3_Pb_Wrap_Logical");
		LaBr3_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr3,
	    global_coordinates + G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_Pb_Wrap_Logical, "LaBr3_Pb_Wrap", World_Logical, false, 0);

		LaBr3_rt -= LaBr3_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL3(LaBr3_Pb_Thickness + LaBr3_Cu_Thickness, false);
	LaBr3_rt -= filterCaseL3.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr3, 
	    global_coordinates + G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    filterCaseL3.Get_Logical(), "LaBr3_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr3_rt += filterCaseL3.Get_Offset_From_Detector();
*/
	if(LaBr3_Cu_Thickness > 0.){
		LaBr3_rt -= LaBr3_Cu_Thickness * 0.5;

		G4Tubs* LaBr3_Cu_Solid = new G4Tubs("LaBr3_Cu_Solid", 0., LaBr3_Cu_Radius, LaBr3_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr3_Cu_Logical = new G4LogicalVolume(LaBr3_Cu_Solid, Cu, "LaBr3_Cu_Logical");
		LaBr3_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr3,
	    global_coordinates + G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_Cu_Logical, "LaBr3_Cu", World_Logical, false, 0);
	}

	LaBr3_rt -= LaBr3_Cu_Thickness*0.5;

	if(LaBr3_Pb_Thickness > 0.){
		LaBr3_rt -= LaBr3_Pb_Thickness * 0.5;

		G4Tubs* LaBr3_Pb_Solid = new G4Tubs("LaBr3_Pb_Solid", 0., LaBr3_Pb_Radius, LaBr3_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr3_Pb_Logical = new G4LogicalVolume(LaBr3_Pb_Solid, Pb, "LaBr3_Pb_Logical");
		LaBr3_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr3,
	    global_coordinates + G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_Pb_Logical, "LaBr3_Pb", World_Logical, false, 0);
	}

	/**************** LABR4 *******************/

	G4double LaBr4_rt = 350. * mm;
	G4double LaBr4_dy = 0. * mm;
	G4double LaBr4_dz = 0. * mm;
	G4double LaBr4_phi = 135. * deg;
	G4double LaBr4_theta = 135. * deg;

	G4double LaBr4_AngleX = 144.736 * deg;
	G4double LaBr4_AngleY = 210. * deg;
	G4double LaBr4_AngleZ = 0. * deg;

	G4double LaBr4_Cu_Radius = 45.*mm; 
	G4double LaBr4_Cu_Thickness = 1.*3.15*mm;
	G4double LaBr4_Pb_Radius = 45.*mm;
	G4double LaBr4_Pb_Thickness = 0.*mm;
	G4double LaBr4_Pb_Wrap_Thickness = 2.*1.2*mm;
	G4double LaBr4_Pb_Wrap_Length = 65.*mm;

	LaBr_TUD *LaBr4_Instance = new LaBr_TUD("LaBr4");
	G4LogicalVolume *LaBr4_Logical = LaBr4_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr4 = new G4RotationMatrix();
	rotateLaBr4->rotateX(LaBr4_AngleX);
	rotateLaBr4->rotateY(LaBr4_AngleY);
	rotateLaBr4->rotateZ(LaBr4_AngleZ);

	LaBr4_rt = LaBr4_rt + LaBr4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr4,
	    global_coordinates + G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_Logical, "LaBr4", World_Logical, false, 0);

	LaBr4_rt -= LaBr4_Instance->Get_Length() * 0.5;

	if(LaBr4_Pb_Wrap_Thickness != 0.){
		LaBr4_rt += LaBr4_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr4_Pb_Wrap_Solid = new G4Tubs("LaBr4_Pb_Wrap_Solid", LaBr4_Instance->Get_Front_Radius(), LaBr4_Instance->Get_Front_Radius() + LaBr4_Pb_Wrap_Thickness, LaBr4_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr4_Pb_Wrap_Logical = new G4LogicalVolume(LaBr4_Pb_Wrap_Solid, Pb, "LaBr4_Pb_Wrap_Logical");
		LaBr4_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr4,
	    global_coordinates + G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_Pb_Wrap_Logical, "LaBr4_Pb_Wrap", World_Logical, false, 0);

		LaBr4_rt -= LaBr4_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL4(LaBr4_Pb_Thickness + LaBr4_Cu_Thickness, false);
	LaBr4_rt -= filterCaseL4.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr4, 
	    global_coordinates + G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    filterCaseL4.Get_Logical(), "LaBr4_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr4_rt += filterCaseL4.Get_Offset_From_Detector();
*/
	if(LaBr4_Cu_Thickness > 0.){
		LaBr4_rt -= LaBr4_Cu_Thickness * 0.5;

		G4Tubs* LaBr4_Cu_Solid = new G4Tubs("LaBr4_Cu_Solid", 0., LaBr4_Cu_Radius, LaBr4_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr4_Cu_Logical = new G4LogicalVolume(LaBr4_Cu_Solid, Cu, "LaBr4_Cu_Logical");
		LaBr4_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr4,
	    global_coordinates + G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_Cu_Logical, "LaBr4_Cu", World_Logical, false, 0);
	}

	LaBr4_rt -= LaBr4_Cu_Thickness*0.5;

	if(LaBr4_Pb_Thickness > 0.){
		LaBr4_rt -= LaBr4_Pb_Thickness * 0.5;

		G4Tubs* LaBr4_Pb_Solid = new G4Tubs("LaBr4_Pb_Solid", 0., LaBr4_Pb_Radius, LaBr4_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr4_Pb_Logical = new G4LogicalVolume(LaBr4_Pb_Solid, Pb, "LaBr4_Pb_Logical");
		LaBr4_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr4,
	    global_coordinates + G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_Pb_Logical, "LaBr4_Pb", World_Logical, false, 0);
	}

	/**************** LABR5 *******************/

	G4double LaBr5_rt = 350. * mm; 
	G4double LaBr5_dy = 0. * mm; 
	G4double LaBr5_dz = 0. * mm; 
	G4double LaBr5_phi = 315. * deg;
	G4double LaBr5_theta = 135. * deg;

	G4double LaBr5_AngleX = 215.264 * deg;
	G4double LaBr5_AngleY = 150. * deg;
	G4double LaBr5_AngleZ = 0. * deg;

	G4double LaBr5_Cu_Radius = 45.*mm; 
	G4double LaBr5_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr5_Pb_Radius = 45.*mm; 
	G4double LaBr5_Pb_Thickness = 0.*mm; 
	G4double LaBr5_Pb_Wrap_Thickness = 1.2*mm; 
	G4double LaBr5_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr5_Instance = new LaBr_TUD("LaBr5");
	G4LogicalVolume *LaBr5_Logical = LaBr5_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr5 = new G4RotationMatrix();
	rotateLaBr5->rotateX(LaBr5_AngleX);
	rotateLaBr5->rotateY(LaBr5_AngleY);
	rotateLaBr5->rotateZ(LaBr5_AngleZ);

	LaBr5_rt = LaBr5_rt + LaBr5_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr5,
	    global_coordinates + G4ThreeVector(LaBr5_rt * sin(LaBr5_theta) * cos(LaBr5_phi),
	                  LaBr5_rt * sin(LaBr5_theta) * sin(LaBr5_phi) + LaBr5_dy,
	                  LaBr5_rt * cos(LaBr5_theta) + LaBr5_dz),
	    LaBr5_Logical, "LaBr5", World_Logical, false, 0);

	LaBr5_rt -= LaBr5_Instance->Get_Length() * 0.5;

	if(LaBr5_Pb_Wrap_Thickness != 0.){
		LaBr5_rt += LaBr5_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr5_Pb_Wrap_Solid = new G4Tubs("LaBr5_Pb_Wrap_Solid", LaBr5_Instance->Get_Front_Radius(), LaBr5_Instance->Get_Front_Radius() + LaBr5_Pb_Wrap_Thickness, LaBr5_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr5_Pb_Wrap_Logical = new G4LogicalVolume(LaBr5_Pb_Wrap_Solid, Pb, "LaBr5_Pb_Wrap_Logical");
		LaBr5_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr5,
	    global_coordinates + G4ThreeVector(LaBr5_rt * sin(LaBr5_theta) * cos(LaBr5_phi),
	                  LaBr5_rt * sin(LaBr5_theta) * sin(LaBr5_phi) + LaBr5_dy,
	                  LaBr5_rt * cos(LaBr5_theta) + LaBr5_dz),
	    LaBr5_Pb_Wrap_Logical, "LaBr5_Pb_Wrap", World_Logical, false, 0);

		LaBr5_rt -= LaBr5_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL5(LaBr5_Pb_Thickness + LaBr5_Cu_Thickness, true);
	LaBr5_rt -= filterCaseL5.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr5, 
	    global_coordinates + G4ThreeVector(LaBr5_rt * sin(LaBr5_theta) * cos(LaBr5_phi),
	                  LaBr5_rt * sin(LaBr5_theta) * sin(LaBr5_phi) + LaBr5_dy,
	                  LaBr5_rt * cos(LaBr5_theta) + LaBr5_dz),
	    filterCaseL5.Get_Logical(), "LaBr5_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr5_rt += filterCaseL5.Get_Offset_From_Detector();
	LaBr5_rt -= filterCaseL5.Get_FilterCaseRing_Thickness();
*/
	if(LaBr5_Cu_Thickness > 0.){
		LaBr5_rt -= LaBr5_Cu_Thickness * 0.5;

		G4Tubs* LaBr5_Cu_Solid = new G4Tubs("LaBr5_Cu_Solid", 0., LaBr5_Cu_Radius, LaBr5_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr5_Cu_Logical = new G4LogicalVolume(LaBr5_Cu_Solid, Cu, "LaBr5_Cu_Logical");
		LaBr5_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr5,
	    global_coordinates + G4ThreeVector(LaBr5_rt * sin(LaBr5_theta) * cos(LaBr5_phi),
	                  LaBr5_rt * sin(LaBr5_theta) * sin(LaBr5_phi) + LaBr5_dy,
	                  LaBr5_rt * cos(LaBr5_theta) + LaBr5_dz),
	    LaBr5_Cu_Logical, "LaBr5_Cu", World_Logical, false, 0);
	}

	LaBr5_rt -= LaBr5_Cu_Thickness*0.5;

	if(LaBr5_Pb_Thickness > 0.){
		LaBr5_rt -= LaBr5_Pb_Thickness * 0.5;

		G4Tubs* LaBr5_Pb_Solid = new G4Tubs("LaBr5_Pb_Solid", 0., LaBr5_Pb_Radius, LaBr5_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr5_Pb_Logical = new G4LogicalVolume(LaBr5_Pb_Solid, Pb, "LaBr5_Pb_Logical");
		LaBr5_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr5,
	    global_coordinates + G4ThreeVector(LaBr5_rt * sin(LaBr5_theta) * cos(LaBr5_phi),
	                  LaBr5_rt * sin(LaBr5_theta) * sin(LaBr5_phi) + LaBr5_dy,
	                  LaBr5_rt * cos(LaBr5_theta) + LaBr5_dz),
	    LaBr5_Pb_Logical, "LaBr5_Pb", World_Logical, false, 0);
	}

	/**************** LABR6 *******************/

	G4double LaBr6_rt = 350. * mm; 
	G4double LaBr6_dy = 0. * mm; 
	G4double LaBr6_dz = 0. * mm; 
	G4double LaBr6_phi = 90. * deg;
	G4double LaBr6_theta = 90. * deg;

	G4double LaBr6_AngleX = 270. * deg;
	G4double LaBr6_AngleY = 0. * deg;
	G4double LaBr6_AngleZ = 180. * deg;

	G4double LaBr6_Cu_Radius = 45.*mm; 
	G4double LaBr6_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr6_Pb_Radius = 45.*mm; 
	G4double LaBr6_Pb_Thickness = 0.*mm; 
	G4double LaBr6_Pb_Wrap_Thickness = 1.2*mm; 
	G4double LaBr6_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr6_Instance = new LaBr_TUD("LaBr6");
	G4LogicalVolume *LaBr6_Logical = LaBr6_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr6 = new G4RotationMatrix();
	rotateLaBr6->rotateX(LaBr6_AngleX);
	rotateLaBr6->rotateY(LaBr6_AngleY);
	rotateLaBr6->rotateZ(LaBr6_AngleZ);

	LaBr6_rt = LaBr6_rt + LaBr6_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr6,
	    global_coordinates + G4ThreeVector(LaBr6_rt * sin(LaBr6_theta) * cos(LaBr6_phi),
	                  LaBr6_rt * sin(LaBr6_theta) * sin(LaBr6_phi) + LaBr6_dy,
	                  LaBr6_rt * cos(LaBr6_theta) + LaBr6_dz),
	    LaBr6_Logical, "LaBr6", World_Logical, false, 0);

	LaBr6_rt -= LaBr6_Instance->Get_Length() * 0.5;

	if(LaBr6_Pb_Wrap_Thickness != 0.){
		LaBr6_rt += LaBr6_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr6_Pb_Wrap_Solid = new G4Tubs("LaBr6_Pb_Wrap_Solid", LaBr6_Instance->Get_Front_Radius(), LaBr6_Instance->Get_Front_Radius() + LaBr6_Pb_Wrap_Thickness, LaBr6_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr6_Pb_Wrap_Logical = new G4LogicalVolume(LaBr6_Pb_Wrap_Solid, Pb, "LaBr6_Pb_Wrap_Logical");
		LaBr6_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr6,
	    global_coordinates + G4ThreeVector(LaBr6_rt * sin(LaBr6_theta) * cos(LaBr6_phi),
	                  LaBr6_rt * sin(LaBr6_theta) * sin(LaBr6_phi) + LaBr6_dy,
	                  LaBr6_rt * cos(LaBr6_theta) + LaBr6_dz),
	    LaBr6_Pb_Wrap_Logical, "LaBr6_Pb_Wrap", World_Logical, false, 0);

		LaBr6_rt -= LaBr6_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL6(LaBr6_Pb_Thickness + LaBr6_Cu_Thickness, true);
	LaBr6_rt -= filterCaseL6.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr6, 
	    global_coordinates + G4ThreeVector(LaBr6_rt * sin(LaBr6_theta) * cos(LaBr6_phi),
	                  LaBr6_rt * sin(LaBr6_theta) * sin(LaBr6_phi) + LaBr6_dy,
	                  LaBr6_rt * cos(LaBr6_theta) + LaBr6_dz),
	    filterCaseL6.Get_Logical(), "LaBr6_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr6_rt += filterCaseL6.Get_Offset_From_Detector();
	LaBr6_rt -= filterCaseL6.Get_FilterCaseRing_Thickness();
*/
	if(LaBr6_Cu_Thickness > 0.){
		LaBr6_rt -= LaBr6_Cu_Thickness * 0.5;

		G4Tubs* LaBr6_Cu_Solid = new G4Tubs("LaBr6_Cu_Solid", 0., LaBr6_Cu_Radius, LaBr6_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr6_Cu_Logical = new G4LogicalVolume(LaBr6_Cu_Solid, Cu, "LaBr6_Cu_Logical");
		LaBr6_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr6,
	    global_coordinates + G4ThreeVector(LaBr6_rt * sin(LaBr6_theta) * cos(LaBr6_phi),
	                  LaBr6_rt * sin(LaBr6_theta) * sin(LaBr6_phi) + LaBr6_dy,
	                  LaBr6_rt * cos(LaBr6_theta) + LaBr6_dz),
	    LaBr6_Cu_Logical, "LaBr6_Cu", World_Logical, false, 0);
	}

	LaBr6_rt -= LaBr6_Cu_Thickness*0.5;

	if(LaBr6_Pb_Thickness > 0.){
		LaBr6_rt -= LaBr6_Pb_Thickness * 0.5;

		G4Tubs* LaBr6_Pb_Solid = new G4Tubs("LaBr6_Pb_Solid", 0., LaBr6_Pb_Radius, LaBr6_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr6_Pb_Logical = new G4LogicalVolume(LaBr6_Pb_Solid, Pb, "LaBr6_Pb_Logical");
		LaBr6_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr6,
	    global_coordinates + G4ThreeVector(LaBr6_rt * sin(LaBr6_theta) * cos(LaBr6_phi),
	                  LaBr6_rt * sin(LaBr6_theta) * sin(LaBr6_phi) + LaBr6_dy,
	                  LaBr6_rt * cos(LaBr6_theta) + LaBr6_dz),
	    LaBr6_Pb_Logical, "LaBr6_Pb", World_Logical, false, 0);
	}

	/**************** LaBr7 *******************/

	G4double LaBr7_rt = 350. * mm; 
	G4double LaBr7_dy = 0. * mm; 
	G4double LaBr7_dz = 0. * mm; 
	G4double LaBr7_phi = 45. * deg;
	G4double LaBr7_theta = 135. * deg;

	G4double LaBr7_AngleX = 144.736 * deg;
	G4double LaBr7_AngleY = 150. * deg;
	G4double LaBr7_AngleZ = 0. * deg;

	G4double LaBr7_Cu_Radius = 45.*mm; 
	G4double LaBr7_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr7_Pb_Radius = 45.*mm; 
	G4double LaBr7_Pb_Thickness = 0.*mm; 
	G4double LaBr7_Pb_Wrap_Thickness = 1.2*mm; 
	G4double LaBr7_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr7_Instance = new LaBr_TUD("LaBr7");
	G4LogicalVolume *LaBr7_Logical = LaBr7_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr7 = new G4RotationMatrix();
	rotateLaBr7->rotateX(LaBr7_AngleX);
	rotateLaBr7->rotateY(LaBr7_AngleY);
	rotateLaBr7->rotateZ(LaBr7_AngleZ);

	LaBr7_rt = LaBr7_rt + LaBr7_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr7,
	    global_coordinates + G4ThreeVector(LaBr7_rt * sin(LaBr7_theta) * cos(LaBr7_phi),
	                  LaBr7_rt * sin(LaBr7_theta) * sin(LaBr7_phi) + LaBr7_dy,
	                  LaBr7_rt * cos(LaBr7_theta) + LaBr7_dz),
	    LaBr7_Logical, "LaBr7", World_Logical, false, 0);

	LaBr7_rt -= LaBr7_Instance->Get_Length() * 0.5;

	if(LaBr7_Pb_Wrap_Thickness != 0.){
		LaBr7_rt += LaBr7_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr7_Pb_Wrap_Solid = new G4Tubs("LaBr7_Pb_Wrap_Solid", LaBr7_Instance->Get_Front_Radius(), LaBr7_Instance->Get_Front_Radius() + LaBr7_Pb_Wrap_Thickness, LaBr7_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr7_Pb_Wrap_Logical = new G4LogicalVolume(LaBr7_Pb_Wrap_Solid, Pb, "LaBr7_Pb_Wrap_Logical");
		LaBr7_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr7,
	    global_coordinates + G4ThreeVector(LaBr7_rt * sin(LaBr7_theta) * cos(LaBr7_phi),
	                  LaBr7_rt * sin(LaBr7_theta) * sin(LaBr7_phi) + LaBr7_dy,
	                  LaBr7_rt * cos(LaBr7_theta) + LaBr7_dz),
	    LaBr7_Pb_Wrap_Logical, "LaBr7_Pb_Wrap", World_Logical, false, 0);

		LaBr7_rt -= LaBr7_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL7(LaBr7_Pb_Thickness + LaBr7_Cu_Thickness, true);
	LaBr7_rt -= filterCaseL7.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr7, 
	    global_coordinates + G4ThreeVector(LaBr7_rt * sin(LaBr7_theta) * cos(LaBr7_phi),
	                  LaBr7_rt * sin(LaBr7_theta) * sin(LaBr7_phi) + LaBr7_dy,
	                  LaBr7_rt * cos(LaBr7_theta) + LaBr7_dz),
	    filterCaseL7.Get_Logical(), "LaBr7_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr7_rt += filterCaseL7.Get_Offset_From_Detector();
	LaBr7_rt -= filterCaseL7.Get_FilterCaseRing_Thickness();
*/
	if(LaBr7_Cu_Thickness > 0.){
		LaBr7_rt -= LaBr7_Cu_Thickness * 0.5;

		G4Tubs* LaBr7_Cu_Solid = new G4Tubs("LaBr7_Cu_Solid", 0., LaBr7_Cu_Radius, LaBr7_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr7_Cu_Logical = new G4LogicalVolume(LaBr7_Cu_Solid, Cu, "LaBr7_Cu_Logical");
		LaBr7_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr7,
	    global_coordinates + G4ThreeVector(LaBr7_rt * sin(LaBr7_theta) * cos(LaBr7_phi),
	                  LaBr7_rt * sin(LaBr7_theta) * sin(LaBr7_phi) + LaBr7_dy,
	                  LaBr7_rt * cos(LaBr7_theta) + LaBr7_dz),
	    LaBr7_Cu_Logical, "LaBr7_Cu", World_Logical, false, 0);
	}

	LaBr7_rt -= LaBr7_Cu_Thickness*0.5;

	if(LaBr7_Pb_Thickness > 0.){
		LaBr7_rt -= LaBr7_Pb_Thickness * 0.5;

		G4Tubs* LaBr7_Pb_Solid = new G4Tubs("LaBr7_Pb_Solid", 0., LaBr7_Pb_Radius, LaBr7_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr7_Pb_Logical = new G4LogicalVolume(LaBr7_Pb_Solid, Pb, "LaBr7_Pb_Logical");
		LaBr7_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr7,
	    global_coordinates + G4ThreeVector(LaBr7_rt * sin(LaBr7_theta) * cos(LaBr7_phi),
	                  LaBr7_rt * sin(LaBr7_theta) * sin(LaBr7_phi) + LaBr7_dy,
	                  LaBr7_rt * cos(LaBr7_theta) + LaBr7_dz),
	    LaBr7_Pb_Logical, "LaBr7_Pb", World_Logical, false, 0);
	}

	/**************** LABR8 *******************/

	G4double LaBr8_rt = 350. * mm; 
	G4double LaBr8_dy = 0. * mm; 
	G4double LaBr8_dz = 0. * mm; 
	G4double LaBr8_phi = 180. * deg;
	G4double LaBr8_theta = 90. * deg;

	G4double LaBr8_AngleX = 0. * deg;
	G4double LaBr8_AngleY = 270. * deg;
	G4double LaBr8_AngleZ = 0. * deg;

	G4double LaBr8_Cu_Radius = 45.*mm; 
	G4double LaBr8_Cu_Thickness = 1.*1.15*mm; 
	G4double LaBr8_Pb_Radius = 45.*mm; 
	G4double LaBr8_Pb_Thickness = 0.*mm; 
	G4double LaBr8_Pb_Wrap_Thickness = 1.2*mm; 
	G4double LaBr8_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *LaBr8_Instance = new LaBr_TUD("LaBr8");
	G4LogicalVolume *LaBr8_Logical = LaBr8_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr8 = new G4RotationMatrix();
	rotateLaBr8->rotateX(LaBr8_AngleX);
	rotateLaBr8->rotateY(LaBr8_AngleY);
	rotateLaBr8->rotateZ(LaBr8_AngleZ);

	LaBr8_rt = LaBr8_rt + LaBr8_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr8,
	    global_coordinates + G4ThreeVector(LaBr8_rt * sin(LaBr8_theta) * cos(LaBr8_phi),
	                  LaBr8_rt * sin(LaBr8_theta) * sin(LaBr8_phi) + LaBr8_dy,
	                  LaBr8_rt * cos(LaBr8_theta) + LaBr8_dz),
	    LaBr8_Logical, "LaBr8", World_Logical, false, 0);

	LaBr8_rt -= LaBr8_Instance->Get_Length() * 0.5;

	if(LaBr8_Pb_Wrap_Thickness != 0.){
		LaBr8_rt += LaBr8_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr8_Pb_Wrap_Solid = new G4Tubs("LaBr8_Pb_Wrap_Solid", LaBr8_Instance->Get_Front_Radius(), LaBr8_Instance->Get_Front_Radius() + LaBr8_Pb_Wrap_Thickness, LaBr8_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr8_Pb_Wrap_Logical = new G4LogicalVolume(LaBr8_Pb_Wrap_Solid, Pb, "LaBr8_Pb_Wrap_Logical");
		LaBr8_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr8,
	    global_coordinates + G4ThreeVector(LaBr8_rt * sin(LaBr8_theta) * cos(LaBr8_phi),
	                  LaBr8_rt * sin(LaBr8_theta) * sin(LaBr8_phi) + LaBr8_dy,
	                  LaBr8_rt * cos(LaBr8_theta) + LaBr8_dz),
	    LaBr8_Pb_Wrap_Logical, "LaBr8_Pb_Wrap", World_Logical, false, 0);

		LaBr8_rt -= LaBr8_Pb_Wrap_Length * 0.5;
	}
/*
	FilterCase filterCaseL7(LaBr8_Pb_Thickness + LaBr8_Cu_Thickness, true);
	LaBr8_rt -= filterCaseL7.Get_Offset_From_Detector();

	new G4PVPlacement(rotateLaBr8, 
	    global_coordinates + G4ThreeVector(LaBr8_rt * sin(LaBr8_theta) * cos(LaBr8_phi),
	                  LaBr8_rt * sin(LaBr8_theta) * sin(LaBr8_phi) + LaBr8_dy,
	                  LaBr8_rt * cos(LaBr8_theta) + LaBr8_dz),
	    filterCaseL7.Get_Logical(), "LaBr8_FilterCase", World_Logical, false, 0, false
	    );
	
	LaBr8_rt += filterCaseL7.Get_Offset_From_Detector();
	LaBr8_rt -= filterCaseL7.Get_FilterCaseRing_Thickness();
*/
	if(LaBr8_Cu_Thickness > 0.){
		LaBr8_rt -= LaBr8_Cu_Thickness * 0.5;

		G4Tubs* LaBr8_Cu_Solid = new G4Tubs("LaBr8_Cu_Solid", 0., LaBr8_Cu_Radius, LaBr8_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr8_Cu_Logical = new G4LogicalVolume(LaBr8_Cu_Solid, Cu, "LaBr8_Cu_Logical");
		LaBr8_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr8,
	    global_coordinates + G4ThreeVector(LaBr8_rt * sin(LaBr8_theta) * cos(LaBr8_phi),
	                  LaBr8_rt * sin(LaBr8_theta) * sin(LaBr8_phi) + LaBr8_dy,
	                  LaBr8_rt * cos(LaBr8_theta) + LaBr8_dz),
	    LaBr8_Cu_Logical, "LaBr8_Cu", World_Logical, false, 0);
	}

	LaBr8_rt -= LaBr8_Cu_Thickness*0.5;

	if(LaBr8_Pb_Thickness > 0.){
		LaBr8_rt -= LaBr8_Pb_Thickness * 0.5;

		G4Tubs* LaBr8_Pb_Solid = new G4Tubs("LaBr8_Pb_Solid", 0., LaBr8_Pb_Radius, LaBr8_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr8_Pb_Logical = new G4LogicalVolume(LaBr8_Pb_Solid, Pb, "LaBr8_Pb_Logical");
		LaBr8_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr8,
	    global_coordinates + G4ThreeVector(LaBr8_rt * sin(LaBr8_theta) * cos(LaBr8_phi),
	                  LaBr8_rt * sin(LaBr8_theta) * sin(LaBr8_phi) + LaBr8_dy,
	                  LaBr8_rt * cos(LaBr8_theta) + LaBr8_dz),
	    LaBr8_Pb_Logical, "LaBr8_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector1 *******************/

	G4double NeutronDetector1_rt = 350. * mm; 
	G4double NeutronDetector1_dy = 0. * mm; 
	G4double NeutronDetector1_dz = 0. * mm; 
	G4double NeutronDetector1_phi = 45. * deg;
	G4double NeutronDetector1_theta = 90. * deg;

	G4double NeutronDetector1_AngleX = 315. * deg;
	G4double NeutronDetector1_AngleY = 90. * deg;
	G4double NeutronDetector1_AngleZ = 0. * deg;

	G4double NeutronDetector1_Cu_Radius = 45.*mm; 
	G4double NeutronDetector1_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector1_Pb_Radius = 45.*mm; 
	G4double NeutronDetector1_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector1_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector1_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector1_Instance = new LaBr_TUD("NeutronDetector1");
	G4LogicalVolume *NeutronDetector1_Logical = NeutronDetector1_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector1 = new G4RotationMatrix();
	rotateNeutronDetector1->rotateY(NeutronDetector1_AngleY);
	rotateNeutronDetector1->rotateX(NeutronDetector1_AngleX);
	rotateNeutronDetector1->rotateZ(NeutronDetector1_AngleZ);

	NeutronDetector1_rt = NeutronDetector1_rt + NeutronDetector1_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector1,
	    global_coordinates + G4ThreeVector(NeutronDetector1_rt * sin(NeutronDetector1_theta) * cos(NeutronDetector1_phi),
	                  NeutronDetector1_rt * sin(NeutronDetector1_theta) * sin(NeutronDetector1_phi) + NeutronDetector1_dy,
	                  NeutronDetector1_rt * cos(NeutronDetector1_theta) + NeutronDetector1_dz),
	    NeutronDetector1_Logical, "NeutronDetector1", World_Logical, false, 0);

	NeutronDetector1_rt -= NeutronDetector1_Instance->Get_Length() * 0.5;

	if(NeutronDetector1_Pb_Wrap_Thickness != 0.){
		NeutronDetector1_rt += NeutronDetector1_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector1_Pb_Wrap_Solid = new G4Tubs("NeutronDetector1_Pb_Wrap_Solid", NeutronDetector1_Instance->Get_Front_Radius(), NeutronDetector1_Instance->Get_Front_Radius() + NeutronDetector1_Pb_Wrap_Thickness, NeutronDetector1_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector1_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector1_Pb_Wrap_Solid, Pb, "NeutronDetector1_Pb_Wrap_Logical");
		NeutronDetector1_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector1,
	    global_coordinates + G4ThreeVector(NeutronDetector1_rt * sin(NeutronDetector1_theta) * cos(NeutronDetector1_phi),
	                  NeutronDetector1_rt * sin(NeutronDetector1_theta) * sin(NeutronDetector1_phi) + NeutronDetector1_dy,
	                  NeutronDetector1_rt * cos(NeutronDetector1_theta) + NeutronDetector1_dz),
	    NeutronDetector1_Pb_Wrap_Logical, "NeutronDetector1_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector1_rt -= NeutronDetector1_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector1_Cu_Thickness > 0.){
		NeutronDetector1_rt -= NeutronDetector1_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector1_Cu_Solid = new G4Tubs("NeutronDetector1_Cu_Solid", 0., NeutronDetector1_Cu_Radius, NeutronDetector1_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector1_Cu_Logical = new G4LogicalVolume(NeutronDetector1_Cu_Solid, Cu, "NeutronDetector1_Cu_Logical");
		NeutronDetector1_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector1,
	    global_coordinates + G4ThreeVector(NeutronDetector1_rt * sin(NeutronDetector1_theta) * cos(NeutronDetector1_phi),
	                  NeutronDetector1_rt * sin(NeutronDetector1_theta) * sin(NeutronDetector1_phi) + NeutronDetector1_dy,
	                  NeutronDetector1_rt * cos(NeutronDetector1_theta) + NeutronDetector1_dz),
	    NeutronDetector1_Cu_Logical, "NeutronDetector1_Cu", World_Logical, false, 0);
	}

	NeutronDetector1_rt -= NeutronDetector1_Cu_Thickness*0.5;

	if(NeutronDetector1_Pb_Thickness > 0.){
		NeutronDetector1_rt -= NeutronDetector1_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector1_Pb_Solid = new G4Tubs("NeutronDetector1_Pb_Solid", 0., NeutronDetector1_Pb_Radius, NeutronDetector1_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector1_Pb_Logical = new G4LogicalVolume(NeutronDetector1_Pb_Solid, Pb, "NeutronDetector1_Pb_Logical");
		NeutronDetector1_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector1,
	    global_coordinates + G4ThreeVector(NeutronDetector1_rt * sin(NeutronDetector1_theta) * cos(NeutronDetector1_phi),
	                  NeutronDetector1_rt * sin(NeutronDetector1_theta) * sin(NeutronDetector1_phi) + NeutronDetector1_dy,
	                  NeutronDetector1_rt * cos(NeutronDetector1_theta) + NeutronDetector1_dz),
	    NeutronDetector1_Pb_Logical, "NeutronDetector1_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector2 *******************/

	G4double NeutronDetector2_rt = 350. * mm; 
	G4double NeutronDetector2_dy = 0. * mm; 
	G4double NeutronDetector2_dz = 0. * mm; 
	G4double NeutronDetector2_phi = 135. * deg;
	G4double NeutronDetector2_theta = 90. * deg;

	G4double NeutronDetector2_AngleX = 315. * deg;
	G4double NeutronDetector2_AngleY = 270. * deg;
	G4double NeutronDetector2_AngleZ = 0. * deg;

	G4double NeutronDetector2_Cu_Radius = 45.*mm; 
	G4double NeutronDetector2_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector2_Pb_Radius = 45.*mm; 
	G4double NeutronDetector2_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector2_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector2_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector2_Instance = new LaBr_TUD("NeutronDetector2");
	G4LogicalVolume *NeutronDetector2_Logical = NeutronDetector2_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector2 = new G4RotationMatrix();
	rotateNeutronDetector2->rotateY(NeutronDetector2_AngleY);
	rotateNeutronDetector2->rotateX(NeutronDetector2_AngleX);
	rotateNeutronDetector2->rotateZ(NeutronDetector2_AngleZ);

	NeutronDetector2_rt = NeutronDetector2_rt + NeutronDetector2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector2,
	    global_coordinates + G4ThreeVector(NeutronDetector2_rt * sin(NeutronDetector2_theta) * cos(NeutronDetector2_phi),
	                  NeutronDetector2_rt * sin(NeutronDetector2_theta) * sin(NeutronDetector2_phi) + NeutronDetector2_dy,
	                  NeutronDetector2_rt * cos(NeutronDetector2_theta) + NeutronDetector2_dz),
	    NeutronDetector2_Logical, "NeutronDetector2", World_Logical, false, 0);

	NeutronDetector2_rt -= NeutronDetector2_Instance->Get_Length() * 0.5;

	if(NeutronDetector2_Pb_Wrap_Thickness != 0.){
		NeutronDetector2_rt += NeutronDetector2_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector2_Pb_Wrap_Solid = new G4Tubs("NeutronDetector2_Pb_Wrap_Solid", NeutronDetector2_Instance->Get_Front_Radius(), NeutronDetector2_Instance->Get_Front_Radius() + NeutronDetector2_Pb_Wrap_Thickness, NeutronDetector2_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector2_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector2_Pb_Wrap_Solid, Pb, "NeutronDetector2_Pb_Wrap_Logical");
		NeutronDetector2_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector2,
	    global_coordinates + G4ThreeVector(NeutronDetector2_rt * sin(NeutronDetector2_theta) * cos(NeutronDetector2_phi),
	                  NeutronDetector2_rt * sin(NeutronDetector2_theta) * sin(NeutronDetector2_phi) + NeutronDetector2_dy,
	                  NeutronDetector2_rt * cos(NeutronDetector2_theta) + NeutronDetector2_dz),
	    NeutronDetector2_Pb_Wrap_Logical, "NeutronDetector2_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector2_rt -= NeutronDetector2_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector2_Cu_Thickness > 0.){
		NeutronDetector2_rt -= NeutronDetector2_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector2_Cu_Solid = new G4Tubs("NeutronDetector2_Cu_Solid", 0., NeutronDetector2_Cu_Radius, NeutronDetector2_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector2_Cu_Logical = new G4LogicalVolume(NeutronDetector2_Cu_Solid, Cu, "NeutronDetector2_Cu_Logical");
		NeutronDetector2_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector2,
	    global_coordinates + G4ThreeVector(NeutronDetector2_rt * sin(NeutronDetector2_theta) * cos(NeutronDetector2_phi),
	                  NeutronDetector2_rt * sin(NeutronDetector2_theta) * sin(NeutronDetector2_phi) + NeutronDetector2_dy,
	                  NeutronDetector2_rt * cos(NeutronDetector2_theta) + NeutronDetector2_dz),
	    NeutronDetector2_Cu_Logical, "NeutronDetector2_Cu", World_Logical, false, 0);
	}

	NeutronDetector2_rt -= NeutronDetector2_Cu_Thickness*0.5;

	if(NeutronDetector2_Pb_Thickness > 0.){
		NeutronDetector2_rt -= NeutronDetector2_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector2_Pb_Solid = new G4Tubs("NeutronDetector2_Pb_Solid", 0., NeutronDetector2_Pb_Radius, NeutronDetector2_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector2_Pb_Logical = new G4LogicalVolume(NeutronDetector2_Pb_Solid, Pb, "NeutronDetector2_Pb_Logical");
		NeutronDetector2_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector2,
	    global_coordinates + G4ThreeVector(NeutronDetector2_rt * sin(NeutronDetector2_theta) * cos(NeutronDetector2_phi),
	                  NeutronDetector2_rt * sin(NeutronDetector2_theta) * sin(NeutronDetector2_phi) + NeutronDetector2_dy,
	                  NeutronDetector2_rt * cos(NeutronDetector2_theta) + NeutronDetector2_dz),
	    NeutronDetector2_Pb_Logical, "NeutronDetector2_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector3 *******************/

	G4double NeutronDetector3_rt = 350. * mm; 
	G4double NeutronDetector3_dy = 0. * mm; 
	G4double NeutronDetector3_dz = 0. * mm; 
	G4double NeutronDetector3_phi = 225. * deg;
	G4double NeutronDetector3_theta = 90. * deg;

	G4double NeutronDetector3_AngleX = 45. * deg;
	G4double NeutronDetector3_AngleY = 270. * deg;
	G4double NeutronDetector3_AngleZ = 0. * deg;

	G4double NeutronDetector3_Cu_Radius = 45.*mm; 
	G4double NeutronDetector3_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector3_Pb_Radius = 45.*mm; 
	G4double NeutronDetector3_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector3_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector3_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector3_Instance = new LaBr_TUD("NeutronDetector3");
	G4LogicalVolume *NeutronDetector3_Logical = NeutronDetector3_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector3 = new G4RotationMatrix();
	rotateNeutronDetector3->rotateY(NeutronDetector3_AngleY);
	rotateNeutronDetector3->rotateX(NeutronDetector3_AngleX);
	rotateNeutronDetector3->rotateZ(NeutronDetector3_AngleZ);

	NeutronDetector3_rt = NeutronDetector3_rt + NeutronDetector3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector3,
	    global_coordinates + G4ThreeVector(NeutronDetector3_rt * sin(NeutronDetector3_theta) * cos(NeutronDetector3_phi),
	                  NeutronDetector3_rt * sin(NeutronDetector3_theta) * sin(NeutronDetector3_phi) + NeutronDetector3_dy,
	                  NeutronDetector3_rt * cos(NeutronDetector3_theta) + NeutronDetector3_dz),
	    NeutronDetector3_Logical, "NeutronDetector3", World_Logical, false, 0);

	NeutronDetector3_rt -= NeutronDetector3_Instance->Get_Length() * 0.5;

	if(NeutronDetector3_Pb_Wrap_Thickness != 0.){
		NeutronDetector3_rt += NeutronDetector3_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector3_Pb_Wrap_Solid = new G4Tubs("NeutronDetector3_Pb_Wrap_Solid", NeutronDetector3_Instance->Get_Front_Radius(), NeutronDetector3_Instance->Get_Front_Radius() + NeutronDetector3_Pb_Wrap_Thickness, NeutronDetector3_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector3_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector3_Pb_Wrap_Solid, Pb, "NeutronDetector3_Pb_Wrap_Logical");
		NeutronDetector3_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector3,
	    global_coordinates + G4ThreeVector(NeutronDetector3_rt * sin(NeutronDetector3_theta) * cos(NeutronDetector3_phi),
	                  NeutronDetector3_rt * sin(NeutronDetector3_theta) * sin(NeutronDetector3_phi) + NeutronDetector3_dy,
	                  NeutronDetector3_rt * cos(NeutronDetector3_theta) + NeutronDetector3_dz),
	    NeutronDetector3_Pb_Wrap_Logical, "NeutronDetector3_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector3_rt -= NeutronDetector3_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector3_Cu_Thickness > 0.){
		NeutronDetector3_rt -= NeutronDetector3_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector3_Cu_Solid = new G4Tubs("NeutronDetector3_Cu_Solid", 0., NeutronDetector3_Cu_Radius, NeutronDetector3_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector3_Cu_Logical = new G4LogicalVolume(NeutronDetector3_Cu_Solid, Cu, "NeutronDetector3_Cu_Logical");
		NeutronDetector3_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector3,
	    global_coordinates + G4ThreeVector(NeutronDetector3_rt * sin(NeutronDetector3_theta) * cos(NeutronDetector3_phi),
	                  NeutronDetector3_rt * sin(NeutronDetector3_theta) * sin(NeutronDetector3_phi) + NeutronDetector3_dy,
	                  NeutronDetector3_rt * cos(NeutronDetector3_theta) + NeutronDetector3_dz),
	    NeutronDetector3_Cu_Logical, "NeutronDetector3_Cu", World_Logical, false, 0);
	}

	NeutronDetector3_rt -= NeutronDetector3_Cu_Thickness*0.5;

	if(NeutronDetector3_Pb_Thickness > 0.){
		NeutronDetector3_rt -= NeutronDetector3_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector3_Pb_Solid = new G4Tubs("NeutronDetector3_Pb_Solid", 0., NeutronDetector3_Pb_Radius, NeutronDetector3_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector3_Pb_Logical = new G4LogicalVolume(NeutronDetector3_Pb_Solid, Pb, "NeutronDetector3_Pb_Logical");
		NeutronDetector3_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector3,
	    global_coordinates + G4ThreeVector(NeutronDetector3_rt * sin(NeutronDetector3_theta) * cos(NeutronDetector3_phi),
	                  NeutronDetector3_rt * sin(NeutronDetector3_theta) * sin(NeutronDetector3_phi) + NeutronDetector3_dy,
	                  NeutronDetector3_rt * cos(NeutronDetector3_theta) + NeutronDetector3_dz),
	    NeutronDetector3_Pb_Logical, "NeutronDetector3_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector4 *******************/

	G4double NeutronDetector4_rt = 350. * mm; 
	G4double NeutronDetector4_dy = 0. * mm; 
	G4double NeutronDetector4_dz = 0. * mm; 
	G4double NeutronDetector4_phi = 315. * deg;
	G4double NeutronDetector4_theta = 90. * deg;

	G4double NeutronDetector4_AngleX = 45. * deg;
	G4double NeutronDetector4_AngleY = 90. * deg;
	G4double NeutronDetector4_AngleZ = 0. * deg;

	G4double NeutronDetector4_Cu_Radius = 45.*mm; 
	G4double NeutronDetector4_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector4_Pb_Radius = 45.*mm; 
	G4double NeutronDetector4_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector4_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector4_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector4_Instance = new LaBr_TUD("NeutronDetector4");
	G4LogicalVolume *NeutronDetector4_Logical = NeutronDetector4_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector4 = new G4RotationMatrix();
	rotateNeutronDetector4->rotateY(NeutronDetector4_AngleY);
	rotateNeutronDetector4->rotateX(NeutronDetector4_AngleX);
	rotateNeutronDetector4->rotateZ(NeutronDetector4_AngleZ);

	NeutronDetector4_rt = NeutronDetector4_rt + NeutronDetector4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector4,
	    global_coordinates + G4ThreeVector(NeutronDetector4_rt * sin(NeutronDetector4_theta) * cos(NeutronDetector4_phi),
	                  NeutronDetector4_rt * sin(NeutronDetector4_theta) * sin(NeutronDetector4_phi) + NeutronDetector4_dy,
	                  NeutronDetector4_rt * cos(NeutronDetector4_theta) + NeutronDetector4_dz),
	    NeutronDetector4_Logical, "NeutronDetector4", World_Logical, false, 0);

	NeutronDetector4_rt -= NeutronDetector4_Instance->Get_Length() * 0.5;

	if(NeutronDetector4_Pb_Wrap_Thickness != 0.){
		NeutronDetector4_rt += NeutronDetector4_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector4_Pb_Wrap_Solid = new G4Tubs("NeutronDetector4_Pb_Wrap_Solid", NeutronDetector4_Instance->Get_Front_Radius(), NeutronDetector4_Instance->Get_Front_Radius() + NeutronDetector4_Pb_Wrap_Thickness, NeutronDetector4_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector4_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector4_Pb_Wrap_Solid, Pb, "NeutronDetector4_Pb_Wrap_Logical");
		NeutronDetector4_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector4,
	    global_coordinates + G4ThreeVector(NeutronDetector4_rt * sin(NeutronDetector4_theta) * cos(NeutronDetector4_phi),
	                  NeutronDetector4_rt * sin(NeutronDetector4_theta) * sin(NeutronDetector4_phi) + NeutronDetector4_dy,
	                  NeutronDetector4_rt * cos(NeutronDetector4_theta) + NeutronDetector4_dz),
	    NeutronDetector4_Pb_Wrap_Logical, "NeutronDetector4_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector4_rt -= NeutronDetector4_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector4_Cu_Thickness > 0.){
		NeutronDetector4_rt -= NeutronDetector4_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector4_Cu_Solid = new G4Tubs("NeutronDetector4_Cu_Solid", 0., NeutronDetector4_Cu_Radius, NeutronDetector4_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector4_Cu_Logical = new G4LogicalVolume(NeutronDetector4_Cu_Solid, Cu, "NeutronDetector4_Cu_Logical");
		NeutronDetector4_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector4,
	    global_coordinates + G4ThreeVector(NeutronDetector4_rt * sin(NeutronDetector4_theta) * cos(NeutronDetector4_phi),
	                  NeutronDetector4_rt * sin(NeutronDetector4_theta) * sin(NeutronDetector4_phi) + NeutronDetector4_dy,
	                  NeutronDetector4_rt * cos(NeutronDetector4_theta) + NeutronDetector4_dz),
	    NeutronDetector4_Cu_Logical, "NeutronDetector4_Cu", World_Logical, false, 0);
	}

	NeutronDetector4_rt -= NeutronDetector4_Cu_Thickness*0.5;

	if(NeutronDetector4_Pb_Thickness > 0.){
		NeutronDetector4_rt -= NeutronDetector4_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector4_Pb_Solid = new G4Tubs("NeutronDetector4_Pb_Solid", 0., NeutronDetector4_Pb_Radius, NeutronDetector4_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector4_Pb_Logical = new G4LogicalVolume(NeutronDetector4_Pb_Solid, Pb, "NeutronDetector4_Pb_Logical");
		NeutronDetector4_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector4,
	    global_coordinates + G4ThreeVector(NeutronDetector4_rt * sin(NeutronDetector4_theta) * cos(NeutronDetector4_phi),
	                  NeutronDetector4_rt * sin(NeutronDetector4_theta) * sin(NeutronDetector4_phi) + NeutronDetector4_dy,
	                  NeutronDetector4_rt * cos(NeutronDetector4_theta) + NeutronDetector4_dz),
	    NeutronDetector4_Pb_Logical, "NeutronDetector4_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector5 *******************/

	G4double NeutronDetector5_rt = 350. * mm; 
	G4double NeutronDetector5_dy = 0. * mm; 
	G4double NeutronDetector5_dz = 0. * mm; 
	G4double NeutronDetector5_phi = 0. * deg;
	G4double NeutronDetector5_theta = 135. * deg;

	G4double NeutronDetector5_AngleX = 0. * deg;
	G4double NeutronDetector5_AngleY = 45. * deg;
	G4double NeutronDetector5_AngleZ = 0. * deg;

	G4double NeutronDetector5_Cu_Radius = 45.*mm; 
	G4double NeutronDetector5_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector5_Pb_Radius = 45.*mm; 
	G4double NeutronDetector5_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector5_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector5_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector5_Instance = new LaBr_TUD("NeutronDetector5");
	G4LogicalVolume *NeutronDetector5_Logical = NeutronDetector5_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector5 = new G4RotationMatrix();
	rotateNeutronDetector5->rotateX(NeutronDetector5_AngleX);
	rotateNeutronDetector5->rotateY(NeutronDetector5_AngleY);
	rotateNeutronDetector5->rotateZ(NeutronDetector5_AngleZ);

	NeutronDetector5_rt = NeutronDetector5_rt + NeutronDetector5_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector5,
	    global_coordinates + G4ThreeVector(NeutronDetector5_rt * sin(NeutronDetector5_theta) * cos(NeutronDetector5_phi),
	                  NeutronDetector5_rt * sin(NeutronDetector5_theta) * sin(NeutronDetector5_phi) + NeutronDetector5_dy,
	                  NeutronDetector5_rt * cos(NeutronDetector5_theta) + NeutronDetector5_dz),
	    NeutronDetector5_Logical, "NeutronDetector5", World_Logical, false, 0);

	NeutronDetector5_rt -= NeutronDetector5_Instance->Get_Length() * 0.5;

	if(NeutronDetector5_Pb_Wrap_Thickness != 0.){
		NeutronDetector5_rt += NeutronDetector5_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector5_Pb_Wrap_Solid = new G4Tubs("NeutronDetector5_Pb_Wrap_Solid", NeutronDetector5_Instance->Get_Front_Radius(), NeutronDetector5_Instance->Get_Front_Radius() + NeutronDetector5_Pb_Wrap_Thickness, NeutronDetector5_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector5_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector5_Pb_Wrap_Solid, Pb, "NeutronDetector5_Pb_Wrap_Logical");
		NeutronDetector5_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector5,
	    global_coordinates + G4ThreeVector(NeutronDetector5_rt * sin(NeutronDetector5_theta) * cos(NeutronDetector5_phi),
	                  NeutronDetector5_rt * sin(NeutronDetector5_theta) * sin(NeutronDetector5_phi) + NeutronDetector5_dy,
	                  NeutronDetector5_rt * cos(NeutronDetector5_theta) + NeutronDetector5_dz),
	    NeutronDetector5_Pb_Wrap_Logical, "NeutronDetector5_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector5_rt -= NeutronDetector5_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector5_Cu_Thickness > 0.){
		NeutronDetector5_rt -= NeutronDetector5_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector5_Cu_Solid = new G4Tubs("NeutronDetector5_Cu_Solid", 0., NeutronDetector5_Cu_Radius, NeutronDetector5_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector5_Cu_Logical = new G4LogicalVolume(NeutronDetector5_Cu_Solid, Cu, "NeutronDetector5_Cu_Logical");
		NeutronDetector5_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector5,
	    global_coordinates + G4ThreeVector(NeutronDetector5_rt * sin(NeutronDetector5_theta) * cos(NeutronDetector5_phi),
	                  NeutronDetector5_rt * sin(NeutronDetector5_theta) * sin(NeutronDetector5_phi) + NeutronDetector5_dy,
	                  NeutronDetector5_rt * cos(NeutronDetector5_theta) + NeutronDetector5_dz),
	    NeutronDetector5_Cu_Logical, "NeutronDetector5_Cu", World_Logical, false, 0);
	}

	NeutronDetector5_rt -= NeutronDetector5_Cu_Thickness*0.5;

	if(NeutronDetector5_Pb_Thickness > 0.){
		NeutronDetector5_rt -= NeutronDetector5_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector5_Pb_Solid = new G4Tubs("NeutronDetector5_Pb_Solid", 0., NeutronDetector5_Pb_Radius, NeutronDetector5_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector5_Pb_Logical = new G4LogicalVolume(NeutronDetector5_Pb_Solid, Pb, "NeutronDetector5_Pb_Logical");
		NeutronDetector5_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector5,
	    global_coordinates + G4ThreeVector(NeutronDetector5_rt * sin(NeutronDetector5_theta) * cos(NeutronDetector5_phi),
	                  NeutronDetector5_rt * sin(NeutronDetector5_theta) * sin(NeutronDetector5_phi) + NeutronDetector5_dy,
	                  NeutronDetector5_rt * cos(NeutronDetector5_theta) + NeutronDetector5_dz),
	    NeutronDetector5_Pb_Logical, "NeutronDetector5_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector6 *******************/

	G4double NeutronDetector6_rt = 350. * mm; 
	G4double NeutronDetector6_dy = 0. * mm; 
	G4double NeutronDetector6_dz = 0. * mm; 
	G4double NeutronDetector6_phi = 90. * deg;
	G4double NeutronDetector6_theta = 135. * deg;

	G4double NeutronDetector6_AngleX = 315. * deg;
	G4double NeutronDetector6_AngleY = 0. * deg;
	G4double NeutronDetector6_AngleZ = 0. * deg;

	G4double NeutronDetector6_Cu_Radius = 45.*mm; 
	G4double NeutronDetector6_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector6_Pb_Radius = 45.*mm; 
	G4double NeutronDetector6_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector6_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector6_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector6_Instance = new LaBr_TUD("NeutronDetector6");
	G4LogicalVolume *NeutronDetector6_Logical = NeutronDetector6_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector6 = new G4RotationMatrix();
	rotateNeutronDetector6->rotateX(NeutronDetector6_AngleX);
	rotateNeutronDetector6->rotateY(NeutronDetector6_AngleY);
	rotateNeutronDetector6->rotateZ(NeutronDetector6_AngleZ);

	NeutronDetector6_rt = NeutronDetector6_rt + NeutronDetector6_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector6,
	    global_coordinates + G4ThreeVector(NeutronDetector6_rt * sin(NeutronDetector6_theta) * cos(NeutronDetector6_phi),
	                  NeutronDetector6_rt * sin(NeutronDetector6_theta) * sin(NeutronDetector6_phi) + NeutronDetector6_dy,
	                  NeutronDetector6_rt * cos(NeutronDetector6_theta) + NeutronDetector6_dz),
	    NeutronDetector6_Logical, "NeutronDetector6", World_Logical, false, 0);

	NeutronDetector6_rt -= NeutronDetector6_Instance->Get_Length() * 0.5;

	if(NeutronDetector6_Pb_Wrap_Thickness != 0.){
		NeutronDetector6_rt += NeutronDetector6_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector6_Pb_Wrap_Solid = new G4Tubs("NeutronDetector6_Pb_Wrap_Solid", NeutronDetector6_Instance->Get_Front_Radius(), NeutronDetector6_Instance->Get_Front_Radius() + NeutronDetector6_Pb_Wrap_Thickness, NeutronDetector6_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector6_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector6_Pb_Wrap_Solid, Pb, "NeutronDetector6_Pb_Wrap_Logical");
		NeutronDetector6_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector6,
	    global_coordinates + G4ThreeVector(NeutronDetector6_rt * sin(NeutronDetector6_theta) * cos(NeutronDetector6_phi),
	                  NeutronDetector6_rt * sin(NeutronDetector6_theta) * sin(NeutronDetector6_phi) + NeutronDetector6_dy,
	                  NeutronDetector6_rt * cos(NeutronDetector6_theta) + NeutronDetector6_dz),
	    NeutronDetector6_Pb_Wrap_Logical, "NeutronDetector6_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector6_rt -= NeutronDetector6_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector6_Cu_Thickness > 0.){
		NeutronDetector6_rt -= NeutronDetector6_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector6_Cu_Solid = new G4Tubs("NeutronDetector6_Cu_Solid", 0., NeutronDetector6_Cu_Radius, NeutronDetector6_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector6_Cu_Logical = new G4LogicalVolume(NeutronDetector6_Cu_Solid, Cu, "NeutronDetector6_Cu_Logical");
		NeutronDetector6_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector6,
	    global_coordinates + G4ThreeVector(NeutronDetector6_rt * sin(NeutronDetector6_theta) * cos(NeutronDetector6_phi),
	                  NeutronDetector6_rt * sin(NeutronDetector6_theta) * sin(NeutronDetector6_phi) + NeutronDetector6_dy,
	                  NeutronDetector6_rt * cos(NeutronDetector6_theta) + NeutronDetector6_dz),
	    NeutronDetector6_Cu_Logical, "NeutronDetector6_Cu", World_Logical, false, 0);
	}

	NeutronDetector6_rt -= NeutronDetector6_Cu_Thickness*0.5;

	if(NeutronDetector6_Pb_Thickness > 0.){
		NeutronDetector6_rt -= NeutronDetector6_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector6_Pb_Solid = new G4Tubs("NeutronDetector6_Pb_Solid", 0., NeutronDetector6_Pb_Radius, NeutronDetector6_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector6_Pb_Logical = new G4LogicalVolume(NeutronDetector6_Pb_Solid, Pb, "NeutronDetector6_Pb_Logical");
		NeutronDetector6_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector6,
	    global_coordinates + G4ThreeVector(NeutronDetector6_rt * sin(NeutronDetector6_theta) * cos(NeutronDetector6_phi),
	                  NeutronDetector6_rt * sin(NeutronDetector6_theta) * sin(NeutronDetector6_phi) + NeutronDetector6_dy,
	                  NeutronDetector6_rt * cos(NeutronDetector6_theta) + NeutronDetector6_dz),
	    NeutronDetector6_Pb_Logical, "NeutronDetector6_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector7 *******************/

	G4double NeutronDetector7_rt = 350. * mm; 
	G4double NeutronDetector7_dy = 0. * mm; 
	G4double NeutronDetector7_dz = 0. * mm; 
	G4double NeutronDetector7_phi = 180. * deg;
	G4double NeutronDetector7_theta = 135. * deg;

	G4double NeutronDetector7_AngleX = 0. * deg;
	G4double NeutronDetector7_AngleY = 315. * deg;
	G4double NeutronDetector7_AngleZ = 0. * deg;

	G4double NeutronDetector7_Cu_Radius = 45.*mm; 
	G4double NeutronDetector7_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector7_Pb_Radius = 45.*mm; 
	G4double NeutronDetector7_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector7_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector7_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector7_Instance = new LaBr_TUD("NeutronDetector7");
	G4LogicalVolume *NeutronDetector7_Logical = NeutronDetector7_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector7 = new G4RotationMatrix();
	rotateNeutronDetector7->rotateX(NeutronDetector7_AngleX);
	rotateNeutronDetector7->rotateY(NeutronDetector7_AngleY);
	rotateNeutronDetector7->rotateZ(NeutronDetector7_AngleZ);

	NeutronDetector7_rt = NeutronDetector7_rt + NeutronDetector7_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector7,
	    global_coordinates + G4ThreeVector(NeutronDetector7_rt * sin(NeutronDetector7_theta) * cos(NeutronDetector7_phi),
	                  NeutronDetector7_rt * sin(NeutronDetector7_theta) * sin(NeutronDetector7_phi) + NeutronDetector7_dy,
	                  NeutronDetector7_rt * cos(NeutronDetector7_theta) + NeutronDetector7_dz),
	    NeutronDetector7_Logical, "NeutronDetector7", World_Logical, false, 0);

	NeutronDetector7_rt -= NeutronDetector7_Instance->Get_Length() * 0.5;

	if(NeutronDetector7_Pb_Wrap_Thickness != 0.){
		NeutronDetector7_rt += NeutronDetector7_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector7_Pb_Wrap_Solid = new G4Tubs("NeutronDetector7_Pb_Wrap_Solid", NeutronDetector7_Instance->Get_Front_Radius(), NeutronDetector7_Instance->Get_Front_Radius() + NeutronDetector7_Pb_Wrap_Thickness, NeutronDetector7_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector7_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector7_Pb_Wrap_Solid, Pb, "NeutronDetector7_Pb_Wrap_Logical");
		NeutronDetector7_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector7,
	    global_coordinates + G4ThreeVector(NeutronDetector7_rt * sin(NeutronDetector7_theta) * cos(NeutronDetector7_phi),
	                  NeutronDetector7_rt * sin(NeutronDetector7_theta) * sin(NeutronDetector7_phi) + NeutronDetector7_dy,
	                  NeutronDetector7_rt * cos(NeutronDetector7_theta) + NeutronDetector7_dz),
	    NeutronDetector7_Pb_Wrap_Logical, "NeutronDetector7_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector7_rt -= NeutronDetector7_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector7_Cu_Thickness > 0.){
		NeutronDetector7_rt -= NeutronDetector7_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector7_Cu_Solid = new G4Tubs("NeutronDetector7_Cu_Solid", 0., NeutronDetector7_Cu_Radius, NeutronDetector7_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector7_Cu_Logical = new G4LogicalVolume(NeutronDetector7_Cu_Solid, Cu, "NeutronDetector7_Cu_Logical");
		NeutronDetector7_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector7,
	    global_coordinates + G4ThreeVector(NeutronDetector7_rt * sin(NeutronDetector7_theta) * cos(NeutronDetector7_phi),
	                  NeutronDetector7_rt * sin(NeutronDetector7_theta) * sin(NeutronDetector7_phi) + NeutronDetector7_dy,
	                  NeutronDetector7_rt * cos(NeutronDetector7_theta) + NeutronDetector7_dz),
	    NeutronDetector7_Cu_Logical, "NeutronDetector7_Cu", World_Logical, false, 0);
	}

	NeutronDetector7_rt -= NeutronDetector7_Cu_Thickness*0.5;

	if(NeutronDetector7_Pb_Thickness > 0.){
		NeutronDetector7_rt -= NeutronDetector7_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector7_Pb_Solid = new G4Tubs("NeutronDetector7_Pb_Solid", 0., NeutronDetector7_Pb_Radius, NeutronDetector7_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector7_Pb_Logical = new G4LogicalVolume(NeutronDetector7_Pb_Solid, Pb, "NeutronDetector7_Pb_Logical");
		NeutronDetector7_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector7,
	    global_coordinates + G4ThreeVector(NeutronDetector7_rt * sin(NeutronDetector7_theta) * cos(NeutronDetector7_phi),
	                  NeutronDetector7_rt * sin(NeutronDetector7_theta) * sin(NeutronDetector7_phi) + NeutronDetector7_dy,
	                  NeutronDetector7_rt * cos(NeutronDetector7_theta) + NeutronDetector7_dz),
	    NeutronDetector7_Pb_Logical, "NeutronDetector7_Pb", World_Logical, false, 0);
	}

	/**************** NeutronDetector8 *******************/

	G4double NeutronDetector8_rt = 350. * mm; 
	G4double NeutronDetector8_dy = 0. * mm; 
	G4double NeutronDetector8_dz = 0. * mm; 
	G4double NeutronDetector8_phi = 270. * deg;
	G4double NeutronDetector8_theta = 135. * deg;

	G4double NeutronDetector8_AngleX = 45. * deg;
	G4double NeutronDetector8_AngleY = 0. * deg;
	G4double NeutronDetector8_AngleZ = 0. * deg;

	G4double NeutronDetector8_Cu_Radius = 45.*mm; 
	G4double NeutronDetector8_Cu_Thickness = 1.*1.15*mm; 
	G4double NeutronDetector8_Pb_Radius = 45.*mm; 
	G4double NeutronDetector8_Pb_Thickness = 0.*mm; 
	G4double NeutronDetector8_Pb_Wrap_Thickness = 1.2*mm; 
	G4double NeutronDetector8_Pb_Wrap_Length = 65.*mm; 

	LaBr_TUD *NeutronDetector8_Instance = new LaBr_TUD("NeutronDetector8");
	G4LogicalVolume *NeutronDetector8_Logical = NeutronDetector8_Instance->Get_Logical();

	G4RotationMatrix *rotateNeutronDetector8 = new G4RotationMatrix();
	rotateNeutronDetector8->rotateX(NeutronDetector8_AngleX);
	rotateNeutronDetector8->rotateY(NeutronDetector8_AngleY);
	rotateNeutronDetector8->rotateZ(NeutronDetector8_AngleZ);

	NeutronDetector8_rt = NeutronDetector8_rt + NeutronDetector8_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateNeutronDetector8,
	    global_coordinates + G4ThreeVector(NeutronDetector8_rt * sin(NeutronDetector8_theta) * cos(NeutronDetector8_phi),
	                  NeutronDetector8_rt * sin(NeutronDetector8_theta) * sin(NeutronDetector8_phi) + NeutronDetector8_dy,
	                  NeutronDetector8_rt * cos(NeutronDetector8_theta) + NeutronDetector8_dz),
	    NeutronDetector8_Logical, "NeutronDetector8", World_Logical, false, 0);

	NeutronDetector8_rt -= NeutronDetector8_Instance->Get_Length() * 0.5;

	if(NeutronDetector8_Pb_Wrap_Thickness != 0.){
		NeutronDetector8_rt += NeutronDetector8_Pb_Wrap_Length * 0.5;

		G4Tubs *NeutronDetector8_Pb_Wrap_Solid = new G4Tubs("NeutronDetector8_Pb_Wrap_Solid", NeutronDetector8_Instance->Get_Front_Radius(), NeutronDetector8_Instance->Get_Front_Radius() + NeutronDetector8_Pb_Wrap_Thickness, NeutronDetector8_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *NeutronDetector8_Pb_Wrap_Logical = new G4LogicalVolume(NeutronDetector8_Pb_Wrap_Solid, Pb, "NeutronDetector8_Pb_Wrap_Logical");
		NeutronDetector8_Pb_Wrap_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector8,
	    global_coordinates + G4ThreeVector(NeutronDetector8_rt * sin(NeutronDetector8_theta) * cos(NeutronDetector8_phi),
	                  NeutronDetector8_rt * sin(NeutronDetector8_theta) * sin(NeutronDetector8_phi) + NeutronDetector8_dy,
	                  NeutronDetector8_rt * cos(NeutronDetector8_theta) + NeutronDetector8_dz),
	    NeutronDetector8_Pb_Wrap_Logical, "NeutronDetector8_Pb_Wrap", World_Logical, false, 0);

		NeutronDetector8_rt -= NeutronDetector8_Pb_Wrap_Length * 0.5;
	}

	if(NeutronDetector8_Cu_Thickness > 0.){
		NeutronDetector8_rt -= NeutronDetector8_Cu_Thickness * 0.5;

		G4Tubs* NeutronDetector8_Cu_Solid = new G4Tubs("NeutronDetector8_Cu_Solid", 0., NeutronDetector8_Cu_Radius, NeutronDetector8_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector8_Cu_Logical = new G4LogicalVolume(NeutronDetector8_Cu_Solid, Cu, "NeutronDetector8_Cu_Logical");
		NeutronDetector8_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateNeutronDetector8,
	    global_coordinates + G4ThreeVector(NeutronDetector8_rt * sin(NeutronDetector8_theta) * cos(NeutronDetector8_phi),
	                  NeutronDetector8_rt * sin(NeutronDetector8_theta) * sin(NeutronDetector8_phi) + NeutronDetector8_dy,
	                  NeutronDetector8_rt * cos(NeutronDetector8_theta) + NeutronDetector8_dz),
	    NeutronDetector8_Cu_Logical, "NeutronDetector8_Cu", World_Logical, false, 0);
	}

	NeutronDetector8_rt -= NeutronDetector8_Cu_Thickness*0.5;

	if(NeutronDetector8_Pb_Thickness > 0.){
		NeutronDetector8_rt -= NeutronDetector8_Pb_Thickness * 0.5;

		G4Tubs* NeutronDetector8_Pb_Solid = new G4Tubs("NeutronDetector8_Pb_Solid", 0., NeutronDetector8_Pb_Radius, NeutronDetector8_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *NeutronDetector8_Pb_Logical = new G4LogicalVolume(NeutronDetector8_Pb_Solid, Pb, "NeutronDetector8_Pb_Logical");
		NeutronDetector8_Pb_Logical->SetVisAttributes(blue);

		new G4PVPlacement(rotateNeutronDetector8,
	    global_coordinates + G4ThreeVector(NeutronDetector8_rt * sin(NeutronDetector8_theta) * cos(NeutronDetector8_phi),
	                  NeutronDetector8_rt * sin(NeutronDetector8_theta) * sin(NeutronDetector8_phi) + NeutronDetector8_dy,
	                  NeutronDetector8_rt * cos(NeutronDetector8_theta) + NeutronDetector8_dz),
	    NeutronDetector8_Pb_Logical, "NeutronDetector8_Pb", World_Logical, false, 0);
	}
}
