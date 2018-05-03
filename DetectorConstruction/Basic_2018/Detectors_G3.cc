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
#include "HPGe1.hh"

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
	G4double HPGe1_phi = 270. * deg;
	G4double HPGe1_theta = 90. * deg;

	G4double HPGe1_AngleX = 90. * deg;
	G4double HPGe1_AngleY = 0. * deg;

	G4double HPGe1_Cu_Radius = 50.*mm; // Estimated
	G4double HPGe1_Cu_Thickness = 1.*mm; // Estimated
	G4double HPGe1_Pb_Radius = 50.*mm; // Estimated
	G4double HPGe1_Pb_Thickness = 3.*mm; // Estimated
	G4double HPGe1_Pb_Wrap_Thickness = 2.*mm; // Estimated
	G4double HPGe1_Pb_Wrap_Length = 150.*mm; // Estimated

	HPGe1 *HPGe1_Instance = new HPGe1("HPGe1");
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
}
