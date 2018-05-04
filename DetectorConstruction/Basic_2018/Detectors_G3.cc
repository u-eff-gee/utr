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
#include "HPGe_60_TUNL_40663.hh"
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
//	G4double HPGe1_phi = 315. * deg;
//	G4double HPGe1_theta = 135. * deg;
//
//	G4double HPGe1_AngleX = 215.264 * deg;
//	G4double HPGe1_AngleY = 150. * deg;

	G4double HPGe1_phi = 270. * deg;
	G4double HPGe1_theta = 90. * deg;

	G4double HPGe1_AngleX = 90 * deg;
	G4double HPGe1_AngleY = 0. * deg;

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

	FilterCase filterCase(HPGe1_Pb_Thickness + HPGe1_Cu_Thickness, true);
	HPGe1_rt -= filterCase.Get_Offset_From_Detector();

	new G4PVPlacement(rotateHPGe1, 
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    filterCase.Get_Logical(), "HPGe1_FilterCase", Detectors_G3_Logical, false, 0, false
	    );
	
	HPGe1_rt += filterCase.Get_Offset_From_Detector();
	HPGe1_rt -= filterCase.Get_FilterCaseRing_Thickness();

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
}
