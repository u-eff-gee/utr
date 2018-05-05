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

//**************************************************************//
//	LaBr Crystal (Saint Gobain BrilLanCe 380) 3x3" (TUD)
//**************************************************************//

#include "LaBr_TUD.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4VisAttributes.hh"
#include "Materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "Units.hh"

LaBr_TUD::LaBr_TUD(G4String Detector_Name){
	G4Colour white(1.0, 1.0, 1.0);
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour black(0.0, 0.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	G4Colour cyan(0.0, 1.0, 1.0);
	G4Colour magenta(1.0, 0.0, 1.0);
	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour light_orange(1.0, 0.82, 0.36);

	G4NistManager *nist = G4NistManager::Instance();
	Materials *mat = Materials::Instance();

	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air= nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	G4Material *LaBr3Ce = mat->Get_LaBr3Ce();

	// Dimensions from previous implementation by B. Loeher and J. Isaak (especially the inner parts) or measured in 2018

	G4double Crystal_Length = 3.*inch;
	G4double Crystal_Radius = 3.*inch * 0.5;
	G4double Crystal_Housing_Thickness = 0.5*mm;
	G4double Crystal_Housing_Outer_Radius = 3.25*inch*0.5;
	G4double Vacuum_Thickness_Front = 2.*mm;
	G4double Vacuum_Thickness_Back = 2.*mm;

	G4double Circuit_Housing_1_Length = 2.25*inch;
	G4double Circuit_Housing_1_Radius = 3.5*inch *0.5;

	G4double Circuit_Housing_2_Length = 3.*cm;
	G4double Circuit_Housing_2_Rmin = 2.5*inch * 0.5;
	G4double Circuit_Housing_2_Rmax = 3.5*inch * 0.5;
	
	G4double Circuit_Housing_3_Length = 3.*cm;
	G4double Circuit_Housing_3_Radius = 2.5*inch *0.5;

	G4double PMT_Housing_Length = 7.*cm;
	G4double PMT_Housing_Radius = 2.5*inch * 0.5;

	/*********************** Mother volume *********************/

	Length = Crystal_Housing_Thickness*2. + Crystal_Length + Vacuum_Thickness_Front + Vacuum_Thickness_Back + Circuit_Housing_1_Length + Circuit_Housing_2_Length + Circuit_Housing_3_Length + PMT_Housing_Length;

	Radius = Circuit_Housing_1_Radius;
	Front_Radius = Crystal_Housing_Outer_Radius;

	G4Tubs *LaBr_TUD_Solid = new G4Tubs("LaBr_TUD_Solid", 0., Radius, Length * 0.5,  0., twopi);
	LaBr_TUD_Logical = new G4LogicalVolume(LaBr_TUD_Solid, air, "LaBr_TUD_Logical");
	LaBr_TUD_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

	/*********************** LaBr Crystal Housing *********************/

	G4Tubs *Crystal_Housing_Lid_Solid = new G4Tubs("Crystal_Housing_Lid_Solid", 0., Crystal_Housing_Outer_Radius, Crystal_Housing_Thickness*0.5, 0., twopi);
	G4LogicalVolume *Crystal_Housing_Lid_Logical = new G4LogicalVolume(Crystal_Housing_Lid_Solid, Al, "Crystal_Housing_Lid_Logical");
	Crystal_Housing_Lid_Logical->SetVisAttributes(grey);

	G4double Crystal_Housing_Wall_Length = Vacuum_Thickness_Front + Crystal_Length + Vacuum_Thickness_Back;
	G4Tubs *Crystal_Housing_Wall_Solid = new G4Tubs("Crystal_Housing_Wall_Solid", Crystal_Housing_Outer_Radius - Crystal_Housing_Thickness, Crystal_Housing_Outer_Radius, Crystal_Housing_Wall_Length*0.5, 0., twopi);
	G4LogicalVolume *Crystal_Housing_Wall_Logical = new G4LogicalVolume(Crystal_Housing_Wall_Solid, Al, "Crystal_Housing_Wall_Logical");
	Crystal_Housing_Wall_Logical->SetVisAttributes(cyan);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness*0.5), Crystal_Housing_Lid_Logical, "Crystal_Housing_Lid1", LaBr_TUD_Logical, false, 0, false);	

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness - Crystal_Housing_Wall_Length*0.5), Crystal_Housing_Wall_Logical, "Crystal_Housing_Lid1", LaBr_TUD_Logical, false, 0, false);	

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness*1.5 - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back), Crystal_Housing_Lid_Logical, "Crystal_Housing_Lid1", LaBr_TUD_Logical, false, 0, false);	

	/************************ Vacuum Layer *****************************/

	G4Tubs *Vacuum_Front_Solid = new G4Tubs("Vacuum_Front_Solid", 0, Crystal_Housing_Outer_Radius - Crystal_Housing_Thickness, Vacuum_Thickness_Front*0.5, 0., twopi);
	G4LogicalVolume *Vacuum_Front_Logical =
	    new G4LogicalVolume(Vacuum_Front_Solid, vacuum, "Vacuum_Front_Logical");
	Vacuum_Front_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness - Vacuum_Thickness_Front*0.5), Vacuum_Front_Logical, "Vacuum_Front", LaBr_TUD_Logical, false, 0, false);

	G4Tubs *Vacuum_Back_Solid = new G4Tubs("Vacuum_Back_Solid", 0, Crystal_Housing_Outer_Radius - Crystal_Housing_Thickness, Vacuum_Thickness_Back*0.5, 0., twopi);
	G4LogicalVolume *Vacuum_Back_Logical =
	    new G4LogicalVolume(Vacuum_Back_Solid, vacuum, "Vacuum_Back_Logical");
	Vacuum_Back_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back*0.5), Vacuum_Back_Logical, "Vacuum_Back", LaBr_TUD_Logical, false, 0, false);

	/************************LaBr Crystal*************************/

	G4Tubs *Crystal_Solid =
	    new G4Tubs("Crystal_Solid", 0., Crystal_Radius,
	               Crystal_Length*0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Crystal_Logical =
	    new G4LogicalVolume(Crystal_Solid, LaBr3Ce, Detector_Name, 0, 0, 0);

	Crystal_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Crystal_Housing_Thickness - Vacuum_Thickness_Front - Crystal_Length*0.5), Crystal_Logical, "Crystal", LaBr_TUD_Logical, false, 0, false);

	/************************ Circuit Housing 1 *************************/

	G4Tubs *Circuit_Housing_1_Solid =
	    new G4Tubs("Circuit_Housing_1_Solid", 0., Circuit_Housing_1_Radius,
	               Circuit_Housing_1_Length*0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Circuit_Housing_1_Logical =
	    new G4LogicalVolume(Circuit_Housing_1_Solid, Al, "Circuit_Housing_1_Logical", 0, 0, 0);

	Circuit_Housing_1_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back - Crystal_Housing_Thickness*2. - Circuit_Housing_1_Length*0.5), Circuit_Housing_1_Logical, "Circuit_Housing_1", LaBr_TUD_Logical, false, 0, false);

	/************************ Circuit Housing 2 *************************/

	G4Cons *Circuit_Housing_2_Solid =
	    new G4Cons("Circuit_Housing_2_Solid", 0., Circuit_Housing_2_Rmin, 0., Circuit_Housing_2_Rmax, Circuit_Housing_2_Length*0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Circuit_Housing_2_Logical =
	    new G4LogicalVolume(Circuit_Housing_2_Solid, Al, "Circuit_Housing_2_Logical", 0, 0, 0);

	Circuit_Housing_2_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back - Crystal_Housing_Thickness*2. - Circuit_Housing_1_Length - Circuit_Housing_2_Length*0.5), Circuit_Housing_2_Logical, "Circuit_Housing_2", LaBr_TUD_Logical, false, 0, false);

	/************************ Circuit Housing 3 *************************/

	G4Tubs *Circuit_Housing_3_Solid =
	    new G4Tubs("Circuit_Housing_3_Solid", 0., Circuit_Housing_3_Radius,
	               Circuit_Housing_3_Length*0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Circuit_Housing_3_Logical =
	    new G4LogicalVolume(Circuit_Housing_3_Solid, Al, "Circuit_Housing_3_Logical", 0, 0, 0);

	Circuit_Housing_3_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back - Crystal_Housing_Thickness*2. - Circuit_Housing_1_Length - Circuit_Housing_2_Length - Circuit_Housing_3_Length*0.5), Circuit_Housing_3_Logical, "Circuit_Housing_3", LaBr_TUD_Logical, false, 0, false);

	/************************ PMT Housing *************************/

	G4Tubs *PMT_Housing_Solid =
	    new G4Tubs("PMT_Housing_Solid", 0., PMT_Housing_Radius,
	               PMT_Housing_Length*0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *PMT_Housing_Logical =
	    new G4LogicalVolume(PMT_Housing_Solid, Al, "PMT_Housing_Logical", 0, 0, 0);

	PMT_Housing_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length*0.5 - Vacuum_Thickness_Front - Crystal_Length - Vacuum_Thickness_Back - Crystal_Housing_Thickness*2. - Circuit_Housing_1_Length - Circuit_Housing_2_Length - Circuit_Housing_2_Length - PMT_Housing_Length*0.5), PMT_Housing_Logical, "PMT_Housing", LaBr_TUD_Logical, false, 0, false);
}
