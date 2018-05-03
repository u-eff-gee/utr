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
#include "Detectors_2nd.hh"
#include "HPGe_Stuttgart.hh"
#include "HPGe_Cologne.hh"
#include "HPGe1_55.hh"

Detectors_2nd::Detectors_2nd(){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");

	G4double Detectors_2nd_X = 70.*inch; // Dimension of mother volume, arbitrary
	G4double Detectors_2nd_Y = 70.*inch; // Dimension of mother volume, arbitrary
	Detectors_2nd_Length = 70.*inch; // Dimension of mother volume, arbitrary
	
	// Mother volume
	G4Box *Detectors_2nd_Solid = new G4Box("Detectors_2nd_Solid", Detectors_2nd_X*0.5, Detectors_2nd_Y*0.5, Detectors_2nd_Length*0.5);
	Detectors_2nd_Logical = new G4LogicalVolume(Detectors_2nd_Solid, air, "Detectors_2nd_Logical");

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

	G4double HPGe_Cologne_rt = 100. * mm; // Estimated
	G4double HPGe_Cologne_dy = 0. * mm; // Estimated
	G4double HPGe_Cologne_dz = 0. * mm; // Estimated
	G4double HPGe_Cologne_phi = 270. * deg;
	G4double HPGe_Cologne_theta = 90. * deg;

	G4double HPGe_Cologne_AngleX = 90. * deg;
	G4double HPGe_Cologne_AngleY = 0. * deg;

	G4double HPGe_Cologne_Cu_Radius = 50.*mm; // Estimated
	G4double HPGe_Cologne_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe_Cologne_Pb_Radius = 50.*mm; // Estimated
	G4double HPGe_Cologne_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe_Cologne_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe_Cologne_Pb_Wrap_Length = 150.*mm; // Estimated

	HPGe_Cologne *HPGe_Cologne_Instance = new HPGe_Cologne("HPGe_Cologne");
	G4LogicalVolume *HPGe_Cologne_Logical = HPGe_Cologne_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe_Cologne = new G4RotationMatrix();
	rotateHPGe_Cologne->rotateX(HPGe_Cologne_AngleX);
	rotateHPGe_Cologne->rotateY(HPGe_Cologne_AngleY);

	HPGe_Cologne_rt = HPGe_Cologne_rt + HPGe_Cologne_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe_Cologne,
	    G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Logical, "HPGe_Cologne", Detectors_2nd_Logical, false, 0);

	HPGe_Cologne_rt -= HPGe_Cologne_Instance->Get_Length() * 0.5;

	if(HPGe_Cologne_Pb_Wrap_Thickness != 0.){
		HPGe_Cologne_rt += HPGe_Cologne_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe_Cologne_Pb_Wrap_Solid = new G4Tubs("HPGe_Cologne_Pb_Wrap_Solid", HPGe_Cologne_Instance->Get_Front_Radius(), HPGe_Cologne_Instance->Get_Front_Radius() + HPGe_Cologne_Pb_Wrap_Thickness, HPGe_Cologne_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe_Cologne_Pb_Wrap_Logical = new G4LogicalVolume(HPGe_Cologne_Pb_Wrap_Solid, Pb, "HPGe_Cologne_Pb_Wrap_Logical");
		HPGe_Cologne_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Cologne,
	    G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Pb_Wrap_Logical, "HPGe_Cologne_Pb_Wrap", Detectors_2nd_Logical, false, 0);

		HPGe_Cologne_rt -= HPGe_Cologne_Pb_Wrap_Length * 0.5;
	}

	if(HPGe_Cologne_Cu_Thickness > 0.){
		HPGe_Cologne_rt -= HPGe_Cologne_Cu_Thickness * 0.5;

		G4Tubs* HPGe_Cologne_Cu_Solid = new G4Tubs("HPGe_Cologne_Cu_Solid", 0., HPGe_Cologne_Cu_Radius, HPGe_Cologne_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Cologne_Cu_Logical = new G4LogicalVolume(HPGe_Cologne_Cu_Solid, Cu, "HPGe_Cologne_Cu_Logical");
		HPGe_Cologne_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe_Cologne,
	    G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Cu_Logical, "HPGe_Cologne_Cu", Detectors_2nd_Logical, false, 0);
	}

	HPGe_Cologne_rt -= HPGe_Cologne_Cu_Thickness*0.5;

	if(HPGe_Cologne_Pb_Thickness > 0.){
		HPGe_Cologne_rt -= HPGe_Cologne_Pb_Thickness * 0.5;

		G4Tubs* HPGe_Cologne_Pb_Solid = new G4Tubs("HPGe_Cologne_Pb_Solid", 0., HPGe_Cologne_Pb_Radius, HPGe_Cologne_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Cologne_Pb_Logical = new G4LogicalVolume(HPGe_Cologne_Pb_Solid, Pb, "HPGe_Cologne_Pb_Logical");
		HPGe_Cologne_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Cologne,
	    G4ThreeVector(HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * cos(HPGe_Cologne_phi),
	                  HPGe_Cologne_rt * sin(HPGe_Cologne_theta) * sin(HPGe_Cologne_phi) + HPGe_Cologne_dy,
	                  HPGe_Cologne_rt * cos(HPGe_Cologne_theta) + HPGe_Cologne_dz),
	    HPGe_Cologne_Pb_Logical, "HPGe_Cologne_Pb", Detectors_2nd_Logical, false, 0);
	}

	/**************** HPGE11 HPGe_Stuttgart *******************/

	G4double HPGe_Stuttgart_rt = 100. * mm; // Estimated
	G4double HPGe_Stuttgart_dy = 0. * mm; // Estimated
	G4double HPGe_Stuttgart_dz = 0. * mm; // Estimated
	G4double HPGe_Stuttgart_phi = 0. * deg;
	G4double HPGe_Stuttgart_theta = 90. * deg;

	G4double HPGe_Stuttgart_AngleX = 180. * deg;
	G4double HPGe_Stuttgart_AngleY = 90. * deg;

	G4double HPGe_Stuttgart_Cu_Radius = 50.*mm; // Estimated
	G4double HPGe_Stuttgart_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe_Stuttgart_Pb_Radius = 50.*mm; // Estimated
	G4double HPGe_Stuttgart_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe_Stuttgart_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe_Stuttgart_Pb_Wrap_Length = 150.*mm; // Estimated

	HPGe_Stuttgart *HPGe_Stuttgart_Instance = new HPGe_Stuttgart("HPGe_Stuttgart");
	G4LogicalVolume *HPGe_Stuttgart_Logical = HPGe_Stuttgart_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe_Stuttgart = new G4RotationMatrix();
	rotateHPGe_Stuttgart->rotateX(HPGe_Stuttgart_AngleX);
	rotateHPGe_Stuttgart->rotateY(HPGe_Stuttgart_AngleY);

	HPGe_Stuttgart_rt = HPGe_Stuttgart_rt + HPGe_Stuttgart_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe_Stuttgart,
	    G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Logical, "HPGe_Stuttgart", Detectors_2nd_Logical, false, 0);

	HPGe_Stuttgart_rt -= HPGe_Stuttgart_Instance->Get_Length() * 0.5;

	if(HPGe_Stuttgart_Pb_Wrap_Thickness != 0.){
		HPGe_Stuttgart_rt += HPGe_Stuttgart_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe_Stuttgart_Pb_Wrap_Solid = new G4Tubs("HPGe_Stuttgart_Pb_Wrap_Solid", HPGe_Stuttgart_Instance->Get_Front_Radius(), HPGe_Stuttgart_Instance->Get_Front_Radius() + HPGe_Stuttgart_Pb_Wrap_Thickness, HPGe_Stuttgart_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe_Stuttgart_Pb_Wrap_Logical = new G4LogicalVolume(HPGe_Stuttgart_Pb_Wrap_Solid, Pb, "HPGe_Stuttgart_Pb_Wrap_Logical");
		HPGe_Stuttgart_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Pb_Wrap_Logical, "HPGe_Stuttgart_Pb_Wrap", Detectors_2nd_Logical, false, 0);

		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Pb_Wrap_Length * 0.5;
	}

	if(HPGe_Stuttgart_Cu_Thickness > 0.){
		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Cu_Thickness * 0.5;

		G4Tubs* HPGe_Stuttgart_Cu_Solid = new G4Tubs("HPGe_Stuttgart_Cu_Solid", 0., HPGe_Stuttgart_Cu_Radius, HPGe_Stuttgart_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Stuttgart_Cu_Logical = new G4LogicalVolume(HPGe_Stuttgart_Cu_Solid, Cu, "HPGe_Stuttgart_Cu_Logical");
		HPGe_Stuttgart_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Cu_Logical, "HPGe_Stuttgart_Cu", Detectors_2nd_Logical, false, 0);
	}

	HPGe_Stuttgart_rt -= HPGe_Stuttgart_Cu_Thickness*0.5;

	if(HPGe_Stuttgart_Pb_Thickness > 0.){
		HPGe_Stuttgart_rt -= HPGe_Stuttgart_Pb_Thickness * 0.5;

		G4Tubs* HPGe_Stuttgart_Pb_Solid = new G4Tubs("HPGe_Stuttgart_Pb_Solid", 0., HPGe_Stuttgart_Pb_Radius, HPGe_Stuttgart_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe_Stuttgart_Pb_Logical = new G4LogicalVolume(HPGe_Stuttgart_Pb_Solid, Pb, "HPGe_Stuttgart_Pb_Logical");
		HPGe_Stuttgart_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe_Stuttgart,
	    G4ThreeVector(HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * cos(HPGe_Stuttgart_phi),
	                  HPGe_Stuttgart_rt * sin(HPGe_Stuttgart_theta) * sin(HPGe_Stuttgart_phi) + HPGe_Stuttgart_dy,
	                  HPGe_Stuttgart_rt * cos(HPGe_Stuttgart_theta) + HPGe_Stuttgart_dz),
	    HPGe_Stuttgart_Pb_Logical, "HPGe_Stuttgart_Pb", Detectors_2nd_Logical, false, 0);
	}

	/**************** HPGE12 HPGe1_55 *******************/

	G4double HPGe1_55_rt = 100. * mm; // Estimated
	G4double HPGe1_55_dy = 0. * mm; // Estimated
	G4double HPGe1_55_dz = 0. * mm; // Estimated
	G4double HPGe1_55_phi = 90. * deg;
	G4double HPGe1_55_theta = 90. * deg;

	G4double HPGe1_55_AngleX = 270. * deg;
	G4double HPGe1_55_AngleY = 0. * deg;
	
	G4double HPGe1_55_Cu_Radius = 50.*mm; // Estimated
	G4double HPGe1_55_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe1_55_Pb_Radius = 50.*mm; // Estimated
	G4double HPGe1_55_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe1_55_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe1_55_Pb_Wrap_Length = 150.*mm; // Estimated

	HPGe1_55 *HPGe1_55_Instance = new HPGe1_55("HPGe1_55");
	G4LogicalVolume *HPGe1_55_Logical = HPGe1_55_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe1_55 = new G4RotationMatrix();
	rotateHPGe1_55->rotateX(HPGe1_55_AngleX);
	rotateHPGe1_55->rotateY(HPGe1_55_AngleY);

	HPGe1_55_rt = HPGe1_55_rt + HPGe1_55_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	                  HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz),
	    HPGe1_55_Logical, "HPGe1_55", Detectors_2nd_Logical, false, 0);

	HPGe1_55_rt -= HPGe1_55_Instance->Get_Length() * 0.5;

	if(HPGe1_55_Pb_Wrap_Thickness != 0.){
		HPGe1_55_rt += HPGe1_55_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe1_55_Pb_Wrap_Solid = new G4Tubs("HPGe1_55_Pb_Wrap_Solid", HPGe1_55_Instance->Get_Front_Radius(), HPGe1_55_Instance->Get_Front_Radius() + HPGe1_55_Pb_Wrap_Thickness, HPGe1_55_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe1_55_Pb_Wrap_Logical = new G4LogicalVolume(HPGe1_55_Pb_Wrap_Solid, Pb, "HPGe1_55_Pb_Wrap_Logical");
		HPGe1_55_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	                  HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz),
	    HPGe1_55_Pb_Wrap_Logical, "HPGe1_55_Pb_Wrap", Detectors_2nd_Logical, false, 0);

		HPGe1_55_rt -= HPGe1_55_Pb_Wrap_Length * 0.5;
	}

	if(HPGe1_55_Cu_Thickness > 0.){
		HPGe1_55_rt -= HPGe1_55_Cu_Thickness * 0.5;

		G4Tubs* HPGe1_55_Cu_Solid = new G4Tubs("HPGe1_55_Cu_Solid", 0., HPGe1_55_Cu_Radius, HPGe1_55_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_55_Cu_Logical = new G4LogicalVolume(HPGe1_55_Cu_Solid, Cu, "HPGe1_55_Cu_Logical");
		HPGe1_55_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	                  HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz),
	    HPGe1_55_Cu_Logical, "HPGe1_55_Cu", Detectors_2nd_Logical, false, 0);
	}

	HPGe1_55_rt -= HPGe1_55_Cu_Thickness*0.5;

	if(HPGe1_55_Pb_Thickness > 0.){
		HPGe1_55_rt -= HPGe1_55_Pb_Thickness * 0.5;

		G4Tubs* HPGe1_55_Pb_Solid = new G4Tubs("HPGe1_55_Pb_Solid", 0., HPGe1_55_Pb_Radius, HPGe1_55_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_55_Pb_Logical = new G4LogicalVolume(HPGe1_55_Pb_Solid, Pb, "HPGe1_55_Pb_Logical");
		HPGe1_55_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	                  HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz),
	    HPGe1_55_Pb_Logical, "HPGe1_55_Pb", Detectors_2nd_Logical, false, 0);
	}
}
