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
#include "Detectors_G3.hh"
#include "HPGe_60_TUNL_30986.hh"
#include "HPGe_60_TUNL_31061.hh"
#include "HPGe_60_TUNL_40663.hh"
#include "HPGe_ANL.hh"
#include "LaBr_TUD.hh"
#include "FilterCase.hh"

Detectors_G3::Detectors_G3(){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");

	G4double Detectors_G3_X = 70.*inch; // Dimension of mother volume, arbitrary
	G4double Detectors_G3_Y = 70.*inch; // Dimension of mother volume, arbitrary
	Detectors_G3_Length = 70.*inch; // Dimension of mother volume, arbitrary
	
	// Mother volume
	G4Box *Detectors_G3_Solid = new G4Box("Detectors_G3_Solid", Detectors_G3_X*0.5, Detectors_G3_Y*0.5, Detectors_G3_Length*0.5);
	Detectors_G3_Logical = new G4LogicalVolume(Detectors_G3_Solid, air, "Detectors_G3_Logical");

	/************************** Detectors ***************************/
	//
	// 1) HPGE1
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

	G4double HPGe1_rt = 100. * mm; // Estimated
	G4double HPGe1_dy = 0. * mm; // Estimated
	G4double HPGe1_dz = 0. * mm; // Estimated
	G4double HPGe1_phi = 315. * deg;
	G4double HPGe1_theta = 135. * deg;

	G4double HPGe1_AngleX = 215.264 * deg;
	G4double HPGe1_AngleY = 150. * deg;

	G4double HPGe1_Cu_Radius = 45.*mm; // Estimated
	G4double HPGe1_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe1_Pb_Radius = 45.*mm; // Estimated
	G4double HPGe1_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe1_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe1_Pb_Wrap_Length = 100.*mm; // Estimated

	HPGe_60_TUNL_40663 *HPGe1_Instance = new HPGe_60_TUNL_40663("HPGe1");
	G4LogicalVolume *HPGe1_Logical = HPGe1_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe1 = new G4RotationMatrix();
	rotateHPGe1->rotateX(HPGe1_AngleX);
	rotateHPGe1->rotateY(HPGe1_AngleY);

	HPGe1_rt = HPGe1_rt + HPGe1_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe1,
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Logical, "HPGe1", Detectors_G3_Logical, false, 0);

	HPGe1_rt -= HPGe1_Instance->Get_Length() * 0.5;

	if(HPGe1_Pb_Wrap_Thickness != 0.){
		HPGe1_rt += HPGe1_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe1_Pb_Wrap_Solid = new G4Tubs("HPGe1_Pb_Wrap_Solid", HPGe1_Instance->Get_Front_Radius(), HPGe1_Instance->Get_Front_Radius() + HPGe1_Pb_Wrap_Thickness, HPGe1_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe1_Pb_Wrap_Logical = new G4LogicalVolume(HPGe1_Pb_Wrap_Solid, Pb, "HPGe1_Pb_Wrap_Logical");
		HPGe1_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1,
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Pb_Wrap_Logical, "HPGe1_Pb_Wrap", Detectors_G3_Logical, false, 0);

		HPGe1_rt -= HPGe1_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase1(HPGe1_Pb_Thickness + HPGe1_Cu_Thickness, true);
	HPGe1_rt -= filterCase1.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe1, 
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    filterCase1.Get_Logical(), "HPGe1_FilterCase", Detectors_G3_Logical, false, 0, false
	    );
	
	HPGe1_rt += filterCase1.Get_Offset_From_Detector();
	HPGe1_rt -= filterCase1.Get_FilterCaseRing_Thickness();

	if(HPGe1_Cu_Thickness > 0.){
		HPGe1_rt -= HPGe1_Cu_Thickness * 0.5;

		G4Tubs* HPGe1_Cu_Solid = new G4Tubs("HPGe1_Cu_Solid", 0., HPGe1_Cu_Radius, HPGe1_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_Cu_Logical = new G4LogicalVolume(HPGe1_Cu_Solid, Cu, "HPGe1_Cu_Logical");
		HPGe1_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe1,
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Cu_Logical, "HPGe1_Cu", Detectors_G3_Logical, false, 0);
	}

	HPGe1_rt -= HPGe1_Cu_Thickness*0.5;

	if(HPGe1_Pb_Thickness > 0.){
		HPGe1_rt -= HPGe1_Pb_Thickness * 0.5;

		G4Tubs* HPGe1_Pb_Solid = new G4Tubs("HPGe1_Pb_Solid", 0., HPGe1_Pb_Radius, HPGe1_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe1_Pb_Logical = new G4LogicalVolume(HPGe1_Pb_Solid, Pb, "HPGe1_Pb_Logical");
		HPGe1_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe1,
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Pb_Logical, "HPGe1_Pb", Detectors_G3_Logical, false, 0);
	}

	/**************** HPGE2 *******************/

	G4double HPGe2_rt = 100. * mm; // Estimated
	G4double HPGe2_dy = 0. * mm; // Estimated
	G4double HPGe2_dz = 0. * mm; // Estimated
	G4double HPGe2_phi = 90. * deg;
	G4double HPGe2_theta = 90. * deg;

	G4double HPGe2_AngleX = 270. * deg;
	G4double HPGe2_AngleY = 0. * deg;
	G4double HPGe2_AngleZ = 180. * deg;

	G4double HPGe2_Cu_Radius = 45.*mm; // Estimated
	G4double HPGe2_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe2_Pb_Radius = 45.*mm; // Estimated
	G4double HPGe2_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe2_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe2_Pb_Wrap_Length = 100.*mm; // Estimated

	HPGe_60_TUNL_30986 *HPGe2_Instance = new HPGe_60_TUNL_30986("HPGe2");
	G4LogicalVolume *HPGe2_Logical = HPGe2_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe2 = new G4RotationMatrix();
	rotateHPGe2->rotateX(HPGe2_AngleX);
	rotateHPGe2->rotateY(HPGe2_AngleY);
	rotateHPGe2->rotateZ(HPGe2_AngleZ);

	HPGe2_rt = HPGe2_rt + HPGe2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Logical, "HPGe2", Detectors_G3_Logical, false, 0);

	HPGe2_rt -= HPGe2_Instance->Get_Length() * 0.5;

	if(HPGe2_Pb_Wrap_Thickness != 0.){
		HPGe2_rt += HPGe2_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe2_Pb_Wrap_Solid = new G4Tubs("HPGe2_Pb_Wrap_Solid", HPGe2_Instance->Get_Front_Radius(), HPGe2_Instance->Get_Front_Radius() + HPGe2_Pb_Wrap_Thickness, HPGe2_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe2_Pb_Wrap_Logical = new G4LogicalVolume(HPGe2_Pb_Wrap_Solid, Pb, "HPGe2_Pb_Wrap_Logical");
		HPGe2_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Pb_Wrap_Logical, "HPGe2_Pb_Wrap", Detectors_G3_Logical, false, 0);

		HPGe2_rt -= HPGe2_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase2(HPGe2_Pb_Thickness + HPGe2_Cu_Thickness, true);
	HPGe2_rt -= filterCase2.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe2, 
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    filterCase2.Get_Logical(), "HPGe2_FilterCase", Detectors_G3_Logical, false, 0, false
	    );
	
	HPGe2_rt += filterCase2.Get_Offset_From_Detector();
	HPGe2_rt -= filterCase2.Get_FilterCaseRing_Thickness();

	if(HPGe2_Cu_Thickness > 0.){
		HPGe2_rt -= HPGe2_Cu_Thickness * 0.5;

		G4Tubs* HPGe2_Cu_Solid = new G4Tubs("HPGe2_Cu_Solid", 0., HPGe2_Cu_Radius, HPGe2_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe2_Cu_Logical = new G4LogicalVolume(HPGe2_Cu_Solid, Cu, "HPGe2_Cu_Logical");
		HPGe2_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Cu_Logical, "HPGe2_Cu", Detectors_G3_Logical, false, 0);
	}

	HPGe2_rt -= HPGe2_Cu_Thickness*0.5;

	if(HPGe2_Pb_Thickness > 0.){
		HPGe2_rt -= HPGe2_Pb_Thickness * 0.5;

		G4Tubs* HPGe2_Pb_Solid = new G4Tubs("HPGe2_Pb_Solid", 0., HPGe2_Pb_Radius, HPGe2_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe2_Pb_Logical = new G4LogicalVolume(HPGe2_Pb_Solid, Pb, "HPGe2_Pb_Logical");
		HPGe2_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Pb_Logical, "HPGe2_Pb", Detectors_G3_Logical, false, 0);
	}

	/**************** HPGE3 *******************/

	G4double HPGe3_rt = 100. * mm; // Estimated
	G4double HPGe3_dy = 0. * mm; // Estimated
	G4double HPGe3_dz = 0. * mm; // Estimated
	G4double HPGe3_phi = 45. * deg;
	G4double HPGe3_theta = 135. * deg;

	G4double HPGe3_AngleX = 144.736 * deg;
	G4double HPGe3_AngleY = 150. * deg;
	G4double HPGe3_AngleZ = 0. * deg;

	G4double HPGe3_Cu_Radius = 45.*mm; // Estimated
	G4double HPGe3_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe3_Pb_Radius = 45.*mm; // Estimated
	G4double HPGe3_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe3_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe3_Pb_Wrap_Length = 100.*mm; // Estimated

	HPGe_60_TUNL_31061 *HPGe3_Instance = new HPGe_60_TUNL_31061("HPGe3");
	G4LogicalVolume *HPGe3_Logical = HPGe3_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe3 = new G4RotationMatrix();
	rotateHPGe3->rotateX(HPGe3_AngleX);
	rotateHPGe3->rotateY(HPGe3_AngleY);
	rotateHPGe3->rotateZ(HPGe3_AngleZ);

	HPGe3_rt = HPGe3_rt + HPGe3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Logical, "HPGe3", Detectors_G3_Logical, false, 0);

	HPGe3_rt -= HPGe3_Instance->Get_Length() * 0.5;

	if(HPGe3_Pb_Wrap_Thickness != 0.){
		HPGe3_rt += HPGe3_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe3_Pb_Wrap_Solid = new G4Tubs("HPGe3_Pb_Wrap_Solid", HPGe3_Instance->Get_Front_Radius(), HPGe3_Instance->Get_Front_Radius() + HPGe3_Pb_Wrap_Thickness, HPGe3_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe3_Pb_Wrap_Logical = new G4LogicalVolume(HPGe3_Pb_Wrap_Solid, Pb, "HPGe3_Pb_Wrap_Logical");
		HPGe3_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Pb_Wrap_Logical, "HPGe3_Pb_Wrap", Detectors_G3_Logical, false, 0);

		HPGe3_rt -= HPGe3_Pb_Wrap_Length * 0.5;
	}

	FilterCase filterCase3(HPGe3_Pb_Thickness + HPGe3_Cu_Thickness, true);
	HPGe3_rt -= filterCase3.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe3, 
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    filterCase3.Get_Logical(), "HPGe3_FilterCase", Detectors_G3_Logical, false, 0, false
	    );
	
	HPGe3_rt += filterCase3.Get_Offset_From_Detector();
	HPGe3_rt -= filterCase3.Get_FilterCaseRing_Thickness();

	if(HPGe3_Cu_Thickness > 0.){
		HPGe3_rt -= HPGe3_Cu_Thickness * 0.5;

		G4Tubs* HPGe3_Cu_Solid = new G4Tubs("HPGe3_Cu_Solid", 0., HPGe3_Cu_Radius, HPGe3_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe3_Cu_Logical = new G4LogicalVolume(HPGe3_Cu_Solid, Cu, "HPGe3_Cu_Logical");
		HPGe3_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Cu_Logical, "HPGe3_Cu", Detectors_G3_Logical, false, 0);
	}

	HPGe3_rt -= HPGe3_Cu_Thickness*0.5;

	if(HPGe3_Pb_Thickness > 0.){
		HPGe3_rt -= HPGe3_Pb_Thickness * 0.5;

		G4Tubs* HPGe3_Pb_Solid = new G4Tubs("HPGe3_Pb_Solid", 0., HPGe3_Pb_Radius, HPGe3_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe3_Pb_Logical = new G4LogicalVolume(HPGe3_Pb_Solid, Pb, "HPGe3_Pb_Logical");
		HPGe3_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Pb_Logical, "HPGe3_Pb", Detectors_G3_Logical, false, 0);
	}

	/**************** HPGE4 *******************/

	G4double HPGe4_rt = 100. * mm; // Estimated
	G4double HPGe4_dy = 0. * mm; // Estimated
	G4double HPGe4_dz = 0. * mm; // Estimated
	G4double HPGe4_phi = 180. * deg;
	G4double HPGe4_theta = 90. * deg;

	G4double HPGe4_AngleX = 0. * deg;
	G4double HPGe4_AngleY = 270. * deg;
	G4double HPGe4_AngleZ = 0. * deg;

	G4double HPGe4_Cu_Radius = 45.*mm; // Estimated
	G4double HPGe4_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe4_Pb_Radius = 45.*mm; // Estimated
	G4double HPGe4_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe4_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe4_Pb_Wrap_Length = 100.*mm; // Estimated

	HPGe_ANL *HPGe4_Instance = new HPGe_ANL("HPGe4");
	G4LogicalVolume *HPGe4_Logical = HPGe4_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe4 = new G4RotationMatrix();
	rotateHPGe4->rotateX(HPGe4_AngleX);
	rotateHPGe4->rotateY(HPGe4_AngleY);
	rotateHPGe4->rotateZ(HPGe4_AngleZ);

	HPGe4_rt = HPGe4_rt + HPGe4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Logical, "HPGe4", Detectors_G3_Logical, false, 0);

	HPGe4_rt -= HPGe4_Instance->Get_Length() * 0.5;

	if(HPGe4_Pb_Wrap_Thickness != 0.){
		HPGe4_rt += HPGe4_Pb_Wrap_Length * 0.5;

		G4Tubs *HPGe4_Pb_Wrap_Solid = new G4Tubs("HPGe4_Pb_Wrap_Solid", HPGe4_Instance->Get_Front_Radius(), HPGe4_Instance->Get_Front_Radius() + HPGe4_Pb_Wrap_Thickness, HPGe4_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *HPGe4_Pb_Wrap_Logical = new G4LogicalVolume(HPGe4_Pb_Wrap_Solid, Pb, "HPGe4_Pb_Wrap_Logical");
		HPGe4_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Pb_Wrap_Logical, "HPGe4_Pb_Wrap", Detectors_G3_Logical, false, 0);

		HPGe4_rt -= HPGe4_Pb_Wrap_Length * 0.5;
	}

	if(HPGe4_Cu_Thickness > 0.){
		HPGe4_rt -= HPGe4_Cu_Thickness * 0.5;

		G4Tubs* HPGe4_Cu_Solid = new G4Tubs("HPGe4_Cu_Solid", 0., HPGe4_Cu_Radius, HPGe4_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe4_Cu_Logical = new G4LogicalVolume(HPGe4_Cu_Solid, Cu, "HPGe4_Cu_Logical");
		HPGe4_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Cu_Logical, "HPGe4_Cu", Detectors_G3_Logical, false, 0);
	}

	HPGe4_rt -= HPGe4_Cu_Thickness*0.5;

	if(HPGe4_Pb_Thickness > 0.){
		HPGe4_rt -= HPGe4_Pb_Thickness * 0.5;

		G4Tubs* HPGe4_Pb_Solid = new G4Tubs("HPGe4_Pb_Solid", 0., HPGe4_Pb_Radius, HPGe4_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *HPGe4_Pb_Logical = new G4LogicalVolume(HPGe4_Pb_Solid, Pb, "HPGe4_Pb_Logical");
		HPGe4_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Pb_Logical, "HPGe4_Pb", Detectors_G3_Logical, false, 0);
	}

	/**************** LABR2 *******************/

	G4double LaBr2_rt = 100. * mm; // Estimated
	G4double LaBr2_dy = 0. * mm; // Estimated
	G4double LaBr2_dz = 0. * mm; // Estimated
	G4double LaBr2_phi = 270. * deg;
	G4double LaBr2_theta = 90. * deg;

	G4double LaBr2_AngleX = 0. * deg;
	G4double LaBr2_AngleY = 0. * deg;
	G4double LaBr2_AngleZ = 0. * deg;

	G4double LaBr2_Cu_Radius = 45.*mm; // Estimated
	G4double LaBr2_Cu_Thickness = 1.*mm; // Estimated
	G4double LaBr2_Pb_Radius = 45.*mm; // Estimated
	G4double LaBr2_Pb_Thickness = 3.*mm; // Estimated
	G4double LaBr2_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double LaBr2_Pb_Wrap_Length = 50.*mm; // Estimated

	LaBr_TUD *LaBr2_Instance = new LaBr_TUD("LaBr2");
	G4LogicalVolume *LaBr2_Logical = LaBr2_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr2 = new G4RotationMatrix();
	rotateLaBr2->rotateX(LaBr2_AngleX);
	rotateLaBr2->rotateY(LaBr2_AngleY);
	rotateLaBr2->rotateZ(LaBr2_AngleZ);

	LaBr2_rt = LaBr2_rt + LaBr2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Logical, "LaBr2", Detectors_G3_Logical, false, 0);

	LaBr2_rt -= LaBr2_Instance->Get_Length() * 0.5;

	if(LaBr2_Pb_Wrap_Thickness != 0.){
		LaBr2_rt += LaBr2_Pb_Wrap_Length * 0.5;

		G4Tubs *LaBr2_Pb_Wrap_Solid = new G4Tubs("LaBr2_Pb_Wrap_Solid", LaBr2_Instance->Get_Front_Radius(), LaBr2_Instance->Get_Front_Radius() + LaBr2_Pb_Wrap_Thickness, LaBr2_Pb_Wrap_Length*0.5, 0., twopi);

		G4LogicalVolume *LaBr2_Pb_Wrap_Logical = new G4LogicalVolume(LaBr2_Pb_Wrap_Solid, Pb, "LaBr2_Pb_Wrap_Logical");
		LaBr2_Pb_Wrap_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Pb_Wrap_Logical, "LaBr2_Pb_Wrap", Detectors_G3_Logical, false, 0);

		LaBr2_rt -= LaBr2_Pb_Wrap_Length * 0.5;
	}

	if(LaBr2_Cu_Thickness > 0.){
		LaBr2_rt -= LaBr2_Cu_Thickness * 0.5;

		G4Tubs* LaBr2_Cu_Solid = new G4Tubs("LaBr2_Cu_Solid", 0., LaBr2_Cu_Radius, LaBr2_Cu_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr2_Cu_Logical = new G4LogicalVolume(LaBr2_Cu_Solid, Cu, "LaBr2_Cu_Logical");
		LaBr2_Cu_Logical->SetVisAttributes(orange);

		new G4PVPlacement(rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Cu_Logical, "LaBr2_Cu", Detectors_G3_Logical, false, 0);
	}

	LaBr2_rt -= LaBr2_Cu_Thickness*0.5;

	if(LaBr2_Pb_Thickness > 0.){
		LaBr2_rt -= LaBr2_Pb_Thickness * 0.5;

		G4Tubs* LaBr2_Pb_Solid = new G4Tubs("LaBr2_Pb_Solid", 0., LaBr2_Pb_Radius, LaBr2_Pb_Thickness*0.5, 0., twopi);
		G4LogicalVolume *LaBr2_Pb_Logical = new G4LogicalVolume(LaBr2_Pb_Solid, Pb, "LaBr2_Pb_Logical");
		LaBr2_Pb_Logical->SetVisAttributes(green);

		new G4PVPlacement(rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Pb_Logical, "LaBr2_Pb", Detectors_G3_Logical, false, 0);
	}
}
