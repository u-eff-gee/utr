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
#include "Detectors_2nd_250_251.hh"
#include "HPGe_Stuttgart.hh"
#include "HPGe_Cologne.hh"
#include "HPGe_55_TUNL_31524.hh"

Detectors_2nd_250_251::Detectors_2nd_250_251(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Detectors_2nd_250_251::Construct(G4ThreeVector global_coordinates){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");

	/************************** Detectors ***************************/
	//
	// 1) HPGE10 (HPGe_Cologne)
	// 2) HPGE11 (HPGe_Stuttgart)
	// 3) HPGE12 (TUNL HPGe1_55)
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

	/**************** HPGE10 HPGe_Cologne *******************/

	G4double HPGe_Cologne_rt = 94.4 * mm; 
	G4double HPGe_Cologne_dy = 0. * mm; 
	G4double HPGe_Cologne_dz = 0. * mm; 
	G4double HPGe_Cologne_phi = 270. * deg;
	G4double HPGe_Cologne_theta = 90. * deg;

	G4double HPGe_Cologne_AngleX = 90. * deg;
	G4double HPGe_Cologne_AngleY = 0. * deg;

	G4double HPGe_Cologne_Cu_Radius = 50.*mm; 
	G4double HPGe_Cologne_Cu_Thickness = 1.*1.15*mm; 
	G4double HPGe_Cologne_Pb_Radius = 50.*mm; 
	G4double HPGe_Cologne_Pb_Thickness = (2.*1.2 + 1.*2.)*mm; 
	G4double HPGe_Cologne_Pb_Wrap_Thickness = 2.*1.2*mm; 
	G4double HPGe_Cologne_Pb_Wrap_Length = 200.*mm; 

	HPGe_Cologne *HPGe_Cologne_Instance = new HPGe_Cologne("HPGe_Cologne");
	G4LogicalVolume *HPGe_Cologne_Logical = HPGe_Cologne_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe_Cologne = new G4RotationMatrix();
	rotateHPGe_Cologne->rotateX(HPGe_Cologne_AngleX);
	rotateHPGe_Cologne->rotateY(HPGe_Cologne_AngleY);

	HPGe_Cologne_rt = HPGe_Cologne_rt + HPGe_Cologne_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe_Cologne,
	    global_coordinates + G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Logical, "HPGe_Cologne", World_Logical, false, 0);

	HPGe_Cologne_rt -= HPGe_Cologne_Instance->Get_Length() * 0.5;

	if(HPGe_Cologne_Pb_Wrap_Thickness != 0.){
		HPGe_Cologne_rt += HPGe_Cologne_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe_Cologne_Pb_Wrap_Solid = new G4Tubs("HPGe_Cologne_Pb_Wrap_Solid", HPGe_Cologne_Instance->Get_Front_Radius(), HPGe_Cologne_Instance->Get_Front_Radius() + HPGe_Cologne_Pb_Wrap_Thickness, HPGe_Cologne_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe_Cologne_Pb_Wrap_Logical = new G4LogicalVolume(HPGe_Cologne_Pb_Wrap_Solid, Pb, "HPGe_Cologne_Pb_Wrap_Logical");
		HPGe_Cologne_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Cologne,
	    global_coordinates + G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Pb_Wrap_Logical, "HPGe_Cologne_Pb_Wrap", World_Logical, false, 0);

		HPGe_Cologne_rt -= HPGe_Cologne_Pb_Wrap_Length * 0.5;
	}

	if(HPGe_Cologne_Cu_Thickness > 0.){
		HPGe_Cologne_rt -= HPGe_Cologne_Cu_Thickness * 0.5;

		G4Tubs* HPGe_Cologne_Cu_Solid = new G4Tubs("HPGe_Cologne_Cu_Solid", 0., HPGe_Cologne_Cu_Radius, HPGe_Cologne_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Cologne_Cu_Logical = new G4LogicalVolume(HPGe_Cologne_Cu_Solid, Cu, "HPGe_Cologne_Cu_Logical");
		HPGe_Cologne_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe_Cologne,
	    global_coordinates + G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Cu_Logical, "HPGe_Cologne_Cu", World_Logical, false, 0);
	}

	HPGe_Cologne_rt -= HPGe_Cologne_Cu_Thickness*0.5;

	if(HPGe_Cologne_Pb_Thickness > 0.){
		HPGe_Cologne_rt -= HPGe_Cologne_Pb_Thickness * 0.5;

		G4Tubs* HPGe_Cologne_Pb_Solid = new G4Tubs("HPGe_Cologne_Pb_Solid", 0., HPGe_Cologne_Pb_Radius, HPGe_Cologne_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Cologne_Pb_Logical = new G4LogicalVolume(HPGe_Cologne_Pb_Solid, Pb, "HPGe_Cologne_Pb_Logical");
		HPGe_Cologne_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Cologne,
	    global_coordinates + G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Pb_Logical, "HPGe_Cologne_Pb", World_Logical, false, 0);
	}

	/**************** HPGE11 HPGe_Stuttgart *******************/

	G4double HPGe_Stuttgart_rt = 123.4 * mm; 
	G4double HPGe_Stuttgart_dy = 0. * mm; 
	G4double HPGe_Stuttgart_dz = 0. * mm; 
	G4double HPGe_Stuttgart_phi = 0. * deg;
	G4double HPGe_Stuttgart_theta = 90. * deg;

	G4double HPGe_Stuttgart_AngleX = 180. * deg;
	G4double HPGe_Stuttgart_AngleY = 90. * deg;

	G4double HPGe_Stuttgart_Cu_Radius = 50.*mm; 
	G4double HPGe_Stuttgart_Cu_Thickness = 1.*1.15*mm; 
	G4double HPGe_Stuttgart_Pb_Radius = 50.*mm; 
	G4double HPGe_Stuttgart_Pb_Thickness = (3.*1.2 + 1.*2.)*mm; 
	G4double HPGe_Stuttgart_Pb_Wrap_Thickness = 1.*1.2*mm; 
	G4double HPGe_Stuttgart_Pb_Wrap_Length = 250.*mm; 

	HPGe_Stuttgart *HPGe_Stuttgart_Instance = new HPGe_Stuttgart("HPGe_Stuttgart");
	G4LogicalVolume *HPGe_Stuttgart_Logical = HPGe_Stuttgart_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe_Stuttgart = new G4RotationMatrix();
	rotateHPGe_Stuttgart->rotateX(HPGe_Stuttgart_AngleX);
	rotateHPGe_Stuttgart->rotateY(HPGe_Stuttgart_AngleY);

	HPGe_Stuttgart_rt = HPGe_Stuttgart_rt + HPGe_Stuttgart_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe_Stuttgart,
	    global_coordinates + G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Logical, "HPGe_Stuttgart", World_Logical, false, 0);

	HPGe_Stuttgart_rt -= HPGe_Stuttgart_Instance->Get_Length() * 0.5;

	if(HPGe_Stuttgart_Pb_Wrap_Thickness != 0.){
		HPGe_Stuttgart_rt += HPGe_Stuttgart_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe_Stuttgart_Pb_Wrap_Solid = new G4Tubs("HPGe_Stuttgart_Pb_Wrap_Solid", HPGe_Stuttgart_Instance->Get_Front_Radius(), HPGe_Stuttgart_Instance->Get_Front_Radius() + HPGe_Stuttgart_Pb_Wrap_Thickness, HPGe_Stuttgart_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe_Stuttgart_Pb_Wrap_Logical = new G4LogicalVolume(HPGe_Stuttgart_Pb_Wrap_Solid, Pb, "HPGe_Stuttgart_Pb_Wrap_Logical");
		HPGe_Stuttgart_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    global_coordinates + G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Pb_Wrap_Logical, "HPGe_Stuttgart_Pb_Wrap", World_Logical, false, 0);

		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Pb_Wrap_Length * 0.5;
	}

	if(HPGe_Stuttgart_Cu_Thickness > 0.){
		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Cu_Thickness * 0.5;

		G4Tubs* HPGe_Stuttgart_Cu_Solid = new G4Tubs("HPGe_Stuttgart_Cu_Solid", 0., HPGe_Stuttgart_Cu_Radius, HPGe_Stuttgart_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Stuttgart_Cu_Logical = new G4LogicalVolume(HPGe_Stuttgart_Cu_Solid, Cu, "HPGe_Stuttgart_Cu_Logical");
		HPGe_Stuttgart_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    global_coordinates + G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Cu_Logical, "HPGe_Stuttgart_Cu", World_Logical, false, 0);
	}

	HPGe_Stuttgart_rt -= HPGe_Stuttgart_Cu_Thickness*0.5;

	if(HPGe_Stuttgart_Pb_Thickness > 0.){
		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Pb_Thickness * 0.5;

		G4Tubs* HPGe_Stuttgart_Pb_Solid = new G4Tubs("HPGe_Stuttgart_Pb_Solid", 0., HPGe_Stuttgart_Pb_Radius, HPGe_Stuttgart_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Stuttgart_Pb_Logical = new G4LogicalVolume(HPGe_Stuttgart_Pb_Solid, Pb, "HPGe_Stuttgart_Pb_Logical");
		HPGe_Stuttgart_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    global_coordinates + G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Pb_Logical, "HPGe_Stuttgart_Pb", World_Logical, false, 0);
	}

	/**************** HPGE12 HPGe_55_TUNL_31524 *******************/

	G4double HPGe12_rt = 89.4 * mm; 
	G4double HPGe12_dy = 0. * mm; 
	G4double HPGe12_dz = 0. * mm; 
	G4double HPGe12_phi = 90. * deg;
	G4double HPGe12_theta = 90. * deg;

	G4double HPGe12_AngleX = 270. * deg;
	G4double HPGe12_AngleY = 0. * deg;
	
	G4double HPGe12_Cu_Radius = 50.*mm; 
	G4double HPGe12_Cu_Thickness = 1.*1.15*mm; 
	G4double HPGe12_Pb_Radius = 50.*mm; 
	G4double HPGe12_Pb_Thickness = (2.*1.2 + 1.*2.)*mm; 
	G4double HPGe12_Pb_Wrap_Thickness = 2.*1.2*mm; 
	G4double HPGe12_Pb_Wrap_Length = 200.*mm; 

	HPGe_55_TUNL_31524 *HPGe12_Instance = new HPGe_55_TUNL_31524("HPGe12");
	G4LogicalVolume *HPGe12_Logical = HPGe12_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe12 = new G4RotationMatrix();
	rotateHPGe12->rotateX(HPGe12_AngleX);
	rotateHPGe12->rotateY(HPGe12_AngleY);

	HPGe12_rt = HPGe12_rt + HPGe12_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe12,
	    global_coordinates + G4ThreeVector(HPGe12_rt * sin(HPGe12_theta) * cos(HPGe12_phi),
	                  HPGe12_rt * sin(HPGe12_theta) * sin(HPGe12_phi) + HPGe12_dy,
	                  HPGe12_rt * cos(HPGe12_theta) + HPGe12_dz),
	    HPGe12_Logical, "HPGe12", World_Logical, false, 0);

	HPGe12_rt -= HPGe12_Instance->Get_Length() * 0.5;

	if(HPGe12_Pb_Wrap_Thickness != 0.){
		HPGe12_rt += HPGe12_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe12_Pb_Wrap_Solid = new G4Tubs("HPGe12_Pb_Wrap_Solid", HPGe12_Instance->Get_Front_Radius(), HPGe12_Instance->Get_Front_Radius() + HPGe12_Pb_Wrap_Thickness, HPGe12_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe12_Pb_Wrap_Logical = new G4LogicalVolume(HPGe12_Pb_Wrap_Solid, Pb, "HPGe12_Pb_Wrap_Logical");
		HPGe12_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe12,
	    global_coordinates + G4ThreeVector(HPGe12_rt * sin(HPGe12_theta) * cos(HPGe12_phi),
	                  HPGe12_rt * sin(HPGe12_theta) * sin(HPGe12_phi) + HPGe12_dy,
	                  HPGe12_rt * cos(HPGe12_theta) + HPGe12_dz),
	    HPGe12_Pb_Wrap_Logical, "HPGe12_Pb_Wrap", World_Logical, false, 0);

		HPGe12_rt -= HPGe12_Pb_Wrap_Length * 0.5;
	}

	if(HPGe12_Cu_Thickness > 0.){
		HPGe12_rt -= HPGe12_Cu_Thickness * 0.5;

		G4Tubs* HPGe12_Cu_Solid = new G4Tubs("HPGe12_Cu_Solid", 0., HPGe12_Cu_Radius, HPGe12_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe12_Cu_Logical = new G4LogicalVolume(HPGe12_Cu_Solid, Cu, "HPGe12_Cu_Logical");
		HPGe12_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe12,
	    global_coordinates + G4ThreeVector(HPGe12_rt * sin(HPGe12_theta) * cos(HPGe12_phi),
	                  HPGe12_rt * sin(HPGe12_theta) * sin(HPGe12_phi) + HPGe12_dy,
	                  HPGe12_rt * cos(HPGe12_theta) + HPGe12_dz),
	    HPGe12_Cu_Logical, "HPGe12_Cu", World_Logical, false, 0);
	}

	HPGe12_rt -= HPGe12_Cu_Thickness*0.5;

	if(HPGe12_Pb_Thickness > 0.){
		HPGe12_rt -= HPGe12_Pb_Thickness * 0.5;

		G4Tubs* HPGe12_Pb_Solid = new G4Tubs("HPGe12_Pb_Solid", 0., HPGe12_Pb_Radius, HPGe12_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe12_Pb_Logical = new G4LogicalVolume(HPGe12_Pb_Solid, Pb, "HPGe12_Pb_Logical");
		HPGe12_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe12,
	    global_coordinates + G4ThreeVector(HPGe12_rt * sin(HPGe12_theta) * cos(HPGe12_phi),
	                  HPGe12_rt * sin(HPGe12_theta) * sin(HPGe12_phi) + HPGe12_dy,
	                  HPGe12_rt * cos(HPGe12_theta) + HPGe12_dz),
	    HPGe12_Pb_Logical, "HPGe12_Pb", World_Logical, false, 0);
	}
}
