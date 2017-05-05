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
//	Germanium Crystal - DET1 (Serial Number 73760)
//**************************************************************//

#include "Germanium1_TUD.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

Germanium1_TUD::Germanium1_TUD(G4String Detector_Name) {

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

	G4NistManager *man = G4NistManager::Instance();

	G4Material *Cu = man->FindOrBuildMaterial("G4_Cu");
	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *Ge = man->FindOrBuildMaterial("G4_Ge");
	G4Material *vacuum = man->FindOrBuildMaterial("G4_Galactic");

	/***********************Germanium Mothervolume*********************/

	G4double case_Radius = 5. * cm;
	// G4double case_Thickness=0.1*cm;
	G4double case_Length = 16. * cm;

	Length = case_Length;
	Radius = case_Radius;

	G4Tubs *case_Solid = new G4Tubs("AluCase1_Solid", 0, case_Radius,
	                                case_Length / 2, 0. * deg, 360. * deg);

	germanium1_Logical =
	    new G4LogicalVolume(case_Solid, Al, "AluCase1_Logical", 0, 0, 0);

	// G4VisAttributes* germanium1VisAtt=new G4VisAttributes(grey);
	germanium1_Logical->SetVisAttributes(grey);

	/************************Vacuumlayer*****************************/

	G4double vacuum_Radius = 49. * mm;
	G4double vacuum_front_Thickness = 3.5 * mm;
	G4double vacuum_Length = 158. * mm;

	G4Tubs *vacuum_Solid = new G4Tubs("Vacuum1_Solid", 0, vacuum_Radius,
	                                  vacuum_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *vacuum_Logical =
	    new G4LogicalVolume(vacuum_Solid, vacuum, "Vacuum1_Logical", 0, 0, 0);

	//  G4VisAttributes* vacuumVisAtt=new G4VisAttributes(G4Color(0.,0.,0.5));
	vacuum_Logical->SetVisAttributes(cyan);

	new G4PVPlacement(0, G4ThreeVector(), vacuum_Logical, "Vacuum1",
	                  germanium1_Logical, false, 0);

	/************************Germanium Crystal*************************/

	G4double crystal_Radius = 39.45 * mm;
	G4double crystal_Length = 79. * mm;

	G4Tubs *crystal_Solid =
	    new G4Tubs("Crystal1_Solid", 0. * cm, crystal_Radius,
	               crystal_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *crystal1_Logical =
	    new G4LogicalVolume(crystal_Solid, Ge, Detector_Name, 0, 0, 0);

	crystal1_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., 0., vacuum_Length / 2 -
	                                               vacuum_front_Thickness -
	                                               crystal_Length / 2),
	                  crystal1_Logical, "Crystal1", vacuum_Logical, false, 0);

	/****************************Coldfinger*******************************/

	G4double coldfinger_Radius = 6. * mm;
	G4double coldfinger_Length = 39. * mm;

	G4Tubs *coldfinger_Solid =
	    new G4Tubs("Coldfinger1_Solid", 0. * cm, coldfinger_Radius,
	               coldfinger_Length / 2, 0. * deg, 360. * deg);

	G4LogicalVolume *coldfinger_Logical = new G4LogicalVolume(
	    coldfinger_Solid, Cu, "Coldfinger1_Logical", 0, 0, 0);

	coldfinger_Logical->SetVisAttributes(orange);

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., -(crystal_Length / 2 - coldfinger_Length / 2)),
	    coldfinger_Logical, "Coldfinger1", crystal1_Logical, false, 0);
}

Germanium1_TUD::~Germanium1_TUD() {}
