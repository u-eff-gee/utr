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
#include "Detectors_G3_271_279.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr_3x3.hh"
#include "LaBr_TUD.hh"
#include "FilterCase.hh"

Detectors_G3_271_279::Detectors_G3_271_279(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Detectors_G3_271_279::Construct(G4ThreeVector global_coordinates){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	
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
	
	HPGe_Collection hpge_Collection;

	/**************** HPGE1 *******************/

	G4double hpge1_rt = 169. * mm;
	G4double hpge1_dy = 0. * mm;
	G4double hpge1_dz = 0. * mm;
	G4double hpge1_phi = 315. * deg;
	G4double hpge1_theta = 135. * deg;

	HPGe_Coaxial hpge1(hpge_Collection.HPGe_60_TUNL_40663, World_Logical, "HPGe1");
	hpge1.Add_Filter("G4_Cu", 1.*1.15*mm, 45.*mm);
	hpge1.Add_Filter("G4_Pb", 2.*1.2*mm, 45.*mm);
	hpge1.Add_Wrap("G4_Pb", 1.2*mm);
	hpge1.Construct(global_coordinates + G4ThreeVector(0., hpge1_dy, hpge1_dz),
			hpge1_theta, hpge1_phi, hpge1_rt, true, true, true);

	/**************** HPGE2 *******************/

	G4double hpge2_rt = 124.4 * mm;
	G4double hpge2_dy = 0. * mm;
	G4double hpge2_dz = 0. * mm;
	G4double hpge2_phi = 90. * deg;
	G4double hpge2_theta = 90. * deg;

	HPGe_Coaxial hpge2(hpge_Collection.HPGe_60_TUNL_30986, World_Logical, "HPGe2");
	hpge2.Add_Filter("G4_Cu", 1.*1.15*mm, 45.*mm);
	hpge2.Add_Filter("G4_Pb", (1.*1.2 + 2.*2.)*mm, 45.*mm);
	hpge2.Add_Wrap("G4_Pb", 1.2*mm);
	hpge2.Construct(global_coordinates + G4ThreeVector(0., hpge2_dy, hpge2_dz),
			hpge2_theta, hpge2_phi, hpge2_rt, true, true, true);

	/**************** HPGE3 *******************/

	G4double hpge3_rt = 166. * mm;
	G4double hpge3_dy = 0. * mm; 
	G4double hpge3_dz = 0. * mm;
	G4double hpge3_phi = 45. * deg;
	G4double hpge3_theta = 135. * deg;

	HPGe_Coaxial hpge3(hpge_Collection.HPGe_60_TUNL_31061, World_Logical, "HPGe3");
	hpge3.Add_Filter("G4_Cu", 1.*1.15*mm, 45.*mm);
	hpge3.Add_Filter("G4_Pb", 1.*1.2*mm, 45.*mm);
	hpge3.Add_Wrap("G4_Pb", 2.*1.2*mm);
	hpge3.Construct(global_coordinates + G4ThreeVector(0., hpge3_dy, hpge3_dz),
			hpge3_theta, hpge3_phi, hpge3_rt, true, true, true);

	/**************** HPGE4 *******************/

	G4double hpge4_rt = 75. * mm; 
	G4double hpge4_dy = 0. * mm; 
	G4double hpge4_dz = 0. * mm; 
	G4double hpge4_phi = 180. * deg;
	G4double hpge4_theta = 90. * deg;

	HPGe_Coaxial hpge4(hpge_Collection.HPGe_ANL_P075689, World_Logical, "HPGe4");
	hpge4.Add_Filter("G4_Pb", 5.*1.2*mm, 45.*mm);
	hpge4.Add_Wrap("G4_Pb", 2.*1.2*mm);
	hpge4.Construct(global_coordinates + G4ThreeVector(0., hpge4_dy, hpge4_dz),
			hpge4_theta, hpge4_phi, hpge4_rt, false, false, true);

	/**************** LABR1 *******************/

	G4double labr1_rt = 83.4 * mm; 
	G4double labr1_dy = 0. * mm; 
	G4double labr1_dz = 0. * mm; 
	G4double labr1_phi = 0. * deg;
	G4double labr1_theta = 90. * deg;

	LaBr_3x3 labr1(World_Logical, "LaBr1");
	labr1.Add_Filter("G4_Cu", 1.*1.15*mm, 45.*mm);
	labr1.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr1.Construct(global_coordinates + G4ThreeVector(0., labr1_dy, labr1_dz),
			labr1_theta, labr1_phi, labr1_rt, true, true, true);

	/**************** LABR2 *******************/

	G4double labr2_rt = 87.4 * mm; 
	G4double labr2_dy = 0. * mm; 
	G4double labr2_dz = 0. * mm; 
	G4double labr2_phi = 270. * deg;
	G4double labr2_theta = 90. * deg;

	LaBr_3x3 labr2(World_Logical, "LaBr2");
	labr2.Add_Filter("G4_Cu", 1.*1.15*mm, 45.*mm);
	labr2.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr2.Construct(global_coordinates + G4ThreeVector(0., labr2_dy, labr2_dz),
			labr2_theta, labr2_phi, labr2_rt, true, true, true);

	/**************** LABR3 *******************/

	G4double labr3_rt = 99. * mm; 
	G4double labr3_dy = 0. * mm; 
	G4double labr3_dz = 0. * mm; 
	G4double labr3_phi = 225. * deg;
	G4double labr3_theta = 135. * deg;

	LaBr_3x3 labr3(World_Logical, "LaBr3");
	labr3.Add_Filter("G4_Cu", 1.*3.15*mm, 45.*mm);
	labr3.Add_Wrap("G4_Pb", 2.*1.2*mm);
	labr3.Construct(global_coordinates + G4ThreeVector(0., labr3_dy, labr3_dz),
			labr3_theta, labr3_phi, labr3_rt, true, true, true);

	/**************** LABR4 *******************/

	G4double labr4_rt = 124. * mm;
	G4double labr4_dy = 0. * mm;
	G4double labr4_dz = 0. * mm;
	G4double labr4_phi = 135. * deg;
	G4double labr4_theta = 135. * deg;

	LaBr_3x3 labr4(World_Logical, "LaBr4");
	labr4.Add_Filter("G4_Cu", 1.*3.15*mm, 45.*mm);
	labr4.Add_Wrap("G4_Pb", 2.*1.2*mm);
	labr4.Construct(global_coordinates + G4ThreeVector(0., labr4_dy, labr4_dz),
			labr4_theta, labr4_phi, labr4_rt, true, true, true);
}
