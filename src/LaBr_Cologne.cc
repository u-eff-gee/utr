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
//	LaBr Crystal (Saint Gobain BrilLanCe 380) 1.5x1.5" (Cologne)
//**************************************************************//

#include "LaBr_Cologne.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"

LaBr_Cologne::LaBr_Cologne(G4String Detector_Name)
    : lengthCrystal(38.1 * mm), radiusCrystal(38.1 / 2. * mm),
      thicknessHousing(1 * mm), radiusHousing(20.75 * mm),
      lengthHousing(45. * mm), lengthEndcap(55. * mm), radiusEndcap(24.75 * mm),
      innerRadiusEndcap(18.75 * mm), thicknessEndcap(1. * mm),
      thicknessFoam(2. * mm), thicknessVacuumFront(0. * mm) {
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

	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

	G4Material *LaBr3Ce = nist->FindOrBuildMaterial("Brillance380");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	G4double zPos = 0.;

	// Thickness of vacuum between aluminum housing and crystal surface
	thicknessVacuumFront =
	    (lengthHousing - lengthCrystal - 2 * thicknessHousing) / 2.;

	/***********************LaBr Mothervolume*********************/

	Radius = radiusEndcap;
	Length = lengthEndcap;

	G4Tubs *labrSolid = new G4Tubs("labrSolid", 0, radiusEndcap,
	                               lengthEndcap / 2., 0. * deg, 360. * deg);
	detectorLogical =
	    new G4LogicalVolume(labrSolid, air, "labrLogical", 0, 0, 0);
	G4VisAttributes *LaBr_CologneVisAtt = new G4VisAttributes(cyan);
	detectorLogical->SetVisAttributes(LaBr_CologneVisAtt);

	/***********************LaBr Aluminum Endcap*********************/

	G4Tubs *endcapSolid =
	    new G4Tubs("endcapSolid", innerRadiusEndcap, radiusEndcap,
	               thicknessEndcap / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *endcapLogical =
	    new G4LogicalVolume(endcapSolid, Al, "endcapLogical", 0, 0, 0);

	G4VisAttributes *endcapVisAtt = new G4VisAttributes(grey);
	endcapLogical->SetVisAttributes(endcapVisAtt);

	zPos = lengthEndcap / 2. - thicknessEndcap / 2.;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), endcapLogical,
	                  "endcapFront", detectorLogical, false, 0);

	/***********************LaBr Aluminum Outer Shell*********************/

	G4Tubs *shellSolid =
	    new G4Tubs("shellSolid", radiusEndcap - thicknessEndcap, radiusEndcap,
	               lengthEndcap / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *shellLogical =
	    new G4LogicalVolume(shellSolid, Al, "shellLogical", 0, 0, 0);

	G4VisAttributes *shellVisAtt = new G4VisAttributes(grey);
	endcapLogical->SetVisAttributes(shellVisAtt);

	zPos = 0.;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), shellLogical,
	                  "endcapShell", detectorLogical, false, 0);

	/***********************LaBr Foam Front********************/

	G4Tubs *foamFrontSolid = new G4Tubs(
	    "foamFrontSolid", innerRadiusEndcap, radiusEndcap - thicknessEndcap,
	    thicknessFoam / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *foamFrontLogical = new G4LogicalVolume(
	    foamFrontSolid, vacuum, "foamFrontLogical", 0, 0, 0);

	G4VisAttributes *foamFrontVisAtt = new G4VisAttributes(white);
	foamFrontLogical->SetVisAttributes(foamFrontVisAtt);

	zPos = lengthEndcap / 2. - thicknessEndcap - thicknessFoam / 2.;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), foamFrontLogical,
	                  "foamFront", detectorLogical, false, 0);

	/***********************LaBr Foam Shell*********************/

	G4Tubs *foamShellSolid = new G4Tubs(
	    "foamShellSolid", radiusEndcap - thicknessEndcap - thicknessFoam,
	    radiusEndcap - thicknessEndcap,
	    lengthEndcap / 2. - thicknessEndcap - thicknessFoam, 0. * deg,
	    360. * deg);

	G4LogicalVolume *foamShellLogical = new G4LogicalVolume(
	    foamShellSolid, vacuum, "foamShellLogical", 0, 0, 0);

	G4VisAttributes *foamShellVisAtt = new G4VisAttributes(white);
	endcapLogical->SetVisAttributes(foamShellVisAtt);

	zPos = 0.;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), foamShellLogical,
	                  "foamShell", detectorLogical, false, 0);

	/***********************LaBr Aluminum Hull*********************/

	G4Tubs *AluHull_Solid =
	    new G4Tubs("AluHull1_Solid", radiusHousing - thicknessHousing,
	               radiusHousing, lengthHousing / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *aluHullLogical =
	    new G4LogicalVolume(AluHull_Solid, Al, "aluHullLogical", 0, 0, 0);

	G4VisAttributes *aluHullVisAtt = new G4VisAttributes(grey);
	aluHullLogical->SetVisAttributes(aluHullVisAtt);

	zPos = lengthEndcap / 2. - lengthHousing / 2. - thicknessFoam -
	       thicknessEndcap;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), aluHullLogical, "aluHull",
	                  detectorLogical, false, 0);

	/************************ Vacuum Layer *****************************/

	G4Tubs *Vacuum_Solid = new G4Tubs(
	    "Vacuum1_Solid", 0, radiusHousing - thicknessHousing,
	    lengthHousing / 2. - thicknessHousing * 2., 0. * deg, 360. * deg);

	G4LogicalVolume *Vacuum_Logical =
	    new G4LogicalVolume(Vacuum_Solid, vacuum, "Vacuum1_Logical", 0, 0, 0);

	G4VisAttributes *VacuumVisAtt = new G4VisAttributes(white);
	Vacuum_Logical->SetVisAttributes(VacuumVisAtt);

	zPos = lengthEndcap / 2. - lengthHousing / 2. + thicknessHousing * 2. -
	       thicknessFoam - thicknessEndcap;
	new G4PVPlacement(0, G4ThreeVector(0., 0., zPos), Vacuum_Logical, "Vacuum1",
	                  detectorLogical, false, 0);

	/************************LaBr Crystal*************************/

	G4Tubs *Crystal_Solid =
	    new G4Tubs("Crystal1_Solid", 0. * cm, radiusCrystal, lengthCrystal / 2.,
	               0. * deg, 360. * deg);

	G4LogicalVolume *Crystal1_Logical =
	    new G4LogicalVolume(Crystal_Solid, LaBr3Ce, Detector_Name, 0, 0, 0);

	Crystal1_Logical->SetVisAttributes(new G4VisAttributes(green));

	new G4PVPlacement(0, G4ThreeVector(), Crystal1_Logical, "Crystal1",
	                  Vacuum_Logical, false, 0);
}

LaBr_Cologne::~LaBr_Cologne() {}
