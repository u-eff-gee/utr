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

// Geometry of 82Se - 82Kr experiment
// Valid from 16.11. 10:37 pm - 17.11. 11:20 am
// Run 706 - Run 709

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
Materials *Materials::instance = NULL;
Materials *materials = Materials::Instance();

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Bricks
#include "Bricks.hh"

// Filters
#include "Filters.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "G4SDManager.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// Detectors
#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "HPGe1.hh"
#include "HPGe1_55.hh"
#include "HPGe2.hh"
#include "HPGe2_55.hh"
#include "HPGe3.hh"
#include "HPGe4.hh"
#include "LaBr_Cologne.hh"
#include "LaBr_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "ZeroDegree.hh"

// Targets
#include "Targets.hh"

#define PI 3.141592

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

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

	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	//	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *Plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
	G4Material *Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
	G4Material *Scintillator_Plastic =
	    nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	G4Material *pump_vacuum = materials->Get_Pump_Vacuum();

	/***************** World Volume *****************/

	G4double world_x = 4000. * mm;
	G4double world_y = 4000. * mm;
	G4double world_z = 9000. * mm;

	G4Box *world_dim =
	    new G4Box("world_dim", world_x * 0.5, world_y * 0.5, world_z * 0.5);

	G4LogicalVolume *world_log =
	    new G4LogicalVolume(world_dim, air, "world_log", 0, 0, 0);

	world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *world_phys =
	    new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	/************************* g3 Wheel *****************/
	// Consists of 7 rings with different outer diameter and thickness
	// Numbering goes from 0 for upstream to 6 for downstream

	G4double Wall6_To_Target = 250. * mm; // Measured

	G4double Wheel_Inner_Radius =
	    205 * mm; // Used in simulation by J. Isaak and B. Loeher

	G4double Wheel_Outer_Radius0 = 270. * mm;  // Measured
	G4double Wheel_Outer_Radius1 = 455 * mm;   // Used in simulation by J. Isaak
	                                           // and B. Loeher. Measured radii
	                                           // relative to this one.
	G4double Wheel_Outer_Radius2 = 355. * mm;  // Measured
	G4double Wheel_Outer_Radius3 = 254.2 * mm; // Measured
	G4double Wheel_Outer_Radius4 = 355. * mm;  // Measured
	G4double Wheel_Outer_Radius5 = 292. * mm;  // Measured
	G4double Wheel_Outer_Radius6 = 312. * mm;  // Measured

	G4double Wheel_Thickness0 = 20. * mm;
	G4double Wheel_Thickness1 = 13. * mm;
	G4double Wheel_Thickness2 = 13. * mm;
	G4double Wheel_Thickness3 = 24. * mm;
	G4double Wheel_Thickness4 = 13. * mm;
	G4double Wheel_Thickness5 = 20. * mm;
	G4double Wheel_Thickness6 = 25. * mm;

	G4double Wheel_Total_Thickness = Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 + Wheel_Thickness3 +
	                                 Wheel_Thickness4 + Wheel_Thickness5 +
	                                 Wheel_Thickness6;

	G4double Wheel_To_Target_Position = Wall6_To_Target - Wheel_Total_Thickness;
	G4Material *Wheel_Material = Al;

	// Ring 0
	G4Tubs *Wheel0_Solid =
	    new G4Tubs("Wheel0_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius0,
	               Wheel_Thickness0 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel0_Logical = new G4LogicalVolume(
	    Wheel0_Solid, Wheel_Material, "Wheel0_Logical", 0, 0, 0);
	Wheel0_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(0, G4ThreeVector(0., 0., Wall6_To_Target -
	                                               Wheel_Total_Thickness +
	                                               Wheel_Thickness0 * 0.5),
	                  Wheel0_Logical, "Wheel0", world_log, false, 0);

	// Ring 1
	G4Tubs *Wheel1_Solid =
	    new G4Tubs("Wheel1_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius1,
	               Wheel_Thickness1 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel1_Logical = new G4LogicalVolume(
	    Wheel1_Solid, Wheel_Material, "Wheel1_Logical", 0, 0, 0);
	Wheel1_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 * 0.5),
	    Wheel1_Logical, "Wheel1", world_log, false, 0);

	// Ring 2
	G4Tubs *Wheel2_Solid =
	    new G4Tubs("Wheel2_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius2,
	               Wheel_Thickness2 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel2_Logical = new G4LogicalVolume(
	    Wheel2_Solid, Wheel_Material, "Wheel2_Logical", 0, 0, 0);
	Wheel2_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 * 0.5),
	    Wheel2_Logical, "Wheel2", world_log, false, 0);

	// Ring 3
	G4Tubs *Wheel3_Solid =
	    new G4Tubs("Wheel3_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius3,
	               Wheel_Thickness3 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel3_Logical = new G4LogicalVolume(
	    Wheel3_Solid, Wheel_Material, "Wheel3_Logical", 0, 0, 0);
	Wheel3_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 + Wheel_Thickness3 * 0.5),
	    Wheel3_Logical, "Wheel3", world_log, false, 0);

	// Ring 4
	G4Tubs *Wheel4_Solid =
	    new G4Tubs("Wheel4_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius4,
	               Wheel_Thickness4 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel4_Logical = new G4LogicalVolume(
	    Wheel4_Solid, Wheel_Material, "Wheel4_Logical", 0, 0, 0);
	Wheel4_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 + Wheel_Thickness3 +
	                                 Wheel_Thickness4 * 0.5),
	    Wheel4_Logical, "Wheel4", world_log, false, 0);

	// Ring 5
	G4Tubs *Wheel5_Solid =
	    new G4Tubs("Wheel5_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius5,
	               Wheel_Thickness5 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel5_Logical = new G4LogicalVolume(
	    Wheel5_Solid, Wheel_Material, "Wheel5_Logical", 0, 0, 0);
	Wheel5_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 + Wheel_Thickness3 +
	                                 Wheel_Thickness4 + Wheel_Thickness5 * 0.5),
	    Wheel5_Logical, "Wheel5", world_log, false, 0);

	// Ring 6
	G4Tubs *Wheel6_Solid =
	    new G4Tubs("Wheel6_Solid", Wheel_Inner_Radius, Wheel_Outer_Radius6,
	               Wheel_Thickness6 * 0.5, 0 * deg, 360 * deg);
	G4LogicalVolume *Wheel6_Logical = new G4LogicalVolume(
	    Wheel6_Solid, Wheel_Material, "Wheel6_Logical", 0, 0, 0);
	Wheel6_Logical->SetVisAttributes(new G4VisAttributes(grey));
	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall6_To_Target - Wheel_Total_Thickness +
	                                 Wheel_Thickness0 + Wheel_Thickness1 +
	                                 Wheel_Thickness2 + Wheel_Thickness3 +
	                                 Wheel_Thickness4 + Wheel_Thickness5 +
	                                 Wheel_Thickness6 * 0.5),
	    Wheel6_Logical, "Wheel6", world_log, false, 0);

	/************************* Beam Tube  *****************/

	// Tube

	G4double BeamTube_Outer_Radius = 1. * inch;     // Measured
	G4double BeamTube_Length_Upstream = 2600. * mm; // Estimated
	G4double BeamTube_Length_Downstream =
	    2526.5 * mm - 15. * inch; // Measured, 2526.5*mm is ZeroDegree_Z. This
	                              // variable will be defined later, but for
	                              // readability reasons, used only the
	                              // numerical value here.
	G4double BeamTube_Length =
	    BeamTube_Length_Upstream + BeamTube_Length_Downstream;
	G4double BeamTube_Thickness = 3 * mm; // Estimated

	G4Tubs *BeamTube_Solid = new G4Tubs(
	    "BeamTube_Solid", BeamTube_Outer_Radius - BeamTube_Thickness,
	    BeamTube_Outer_Radius, BeamTube_Length / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *BeamTube_Logical = new G4LogicalVolume(
	    BeamTube_Solid, Plexiglass, "BeamTube_Logial", 0, 0, 0);

	BeamTube_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(0, G4ThreeVector(0., 0., BeamTube_Length_Downstream -
	                                               BeamTube_Length / 2.),
	                  BeamTube_Logical, "BeamTube", world_log, false, 0);

	// End cap on upstream side

	G4double BeamTubeEndCap_Outer_Radius = 1.3 * inch; // Estimated
	G4double BeamTubeEndCap_Length = 30. * mm;         // Estimated

	G4Tubs *BeamTubeEndCap_Solid =
	    new G4Tubs("BeamTubeEndCap_Solid", BeamTube_Outer_Radius,
	               BeamTubeEndCap_Outer_Radius, BeamTubeEndCap_Length / 2.,
	               0. * deg, 360. * deg);
	G4LogicalVolume *BeamTubeEndCap_Logical = new G4LogicalVolume(
	    BeamTubeEndCap_Solid, Plexiglass, "BeamTubeEndCap_Logical", 0, 0, 0);

	BeamTubeEndCap_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(0, G4ThreeVector(0., 0., -BeamTube_Length_Upstream +
	                                               BeamTubeEndCap_Length / 2.),
	                  BeamTubeEndCap_Logical, "BeamTubeEndCap", world_log,
	                  false, 0);

	G4double BeamTubeEndCapWindow_Thickness = 2. * mm; // Estimated

	G4Tubs *BeamTubeEndCapWindow_Solid = new G4Tubs(
	    "BeamTubeEndCapWindow_Solid", 0., BeamTubeEndCap_Outer_Radius,
	    BeamTubeEndCapWindow_Thickness / 2., 0. * deg, 360. * deg);
	G4LogicalVolume *BeamTubeEndCapWindow_Logical =
	    new G4LogicalVolume(BeamTubeEndCapWindow_Solid, Plexiglass,
	                        "BeamTubeEndCapWindow_Logical", 0, 0, 0);

	BeamTubeEndCapWindow_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., -BeamTube_Length_Upstream -
	                                 BeamTubeEndCapWindow_Thickness / 2.),
	    BeamTubeEndCapWindow_Logical, "BeamTubeEndCapWindow", world_log, false,
	    0);

	// Vacuum inside beam tube

	G4Tubs *BeamTubeVacuum_Solid =
	    new G4Tubs("BeamTubeVacuum_Solid", 0. * mm,
	               BeamTube_Outer_Radius - BeamTube_Thickness,
	               BeamTube_Length / 2., 0. * deg, 360. * deg);
	G4LogicalVolume *BeamTubeVacuum_Logical = new G4LogicalVolume(
	    BeamTubeVacuum_Solid, pump_vacuum, "BeamTubeVacuum_Logical", 0, 0, 0);

	BeamTubeVacuum_Logical->SetVisAttributes(new G4VisAttributes(cyan));

	new G4PVPlacement(0, G4ThreeVector(0., 0., BeamTube_Length_Downstream -
	                                               BeamTube_Length / 2.),
	                  BeamTubeVacuum_Logical, "BeamTubeVacuum", world_log,
	                  false, 0);

	/******************** Target Holder Tube without cap ***********/

	G4double TargetTube_Outer_Radius =
	    BeamTube_Outer_Radius - BeamTube_Thickness;
	G4double TargetTubeBack_Length = 500. * mm;  // Estimated
	G4double TargetTubeBack_Thickness = 2. * mm; // Measured

	G4double TargetTubeFront_Length = 50. * mm;   // Measured
	G4double TargetTubeFront_Thickness = 1. * mm; // Measured

	G4double TargetTube_Total_Length =
	    TargetTubeFront_Length + TargetTubeBack_Length;

	G4Tubs *TargetTubeBack_Solid =
	    new G4Tubs("TargetTubeback_Solid",
	               TargetTube_Outer_Radius - TargetTubeBack_Thickness,
	               TargetTube_Outer_Radius, TargetTubeBack_Length * 0.5,
	               0. * deg, 360. * deg);
	G4Tubs *TargetTubeFront_Solid =
	    new G4Tubs("TargetTubeFront_Solid",
	               TargetTube_Outer_Radius - TargetTubeFront_Thickness,
	               TargetTube_Outer_Radius, TargetTubeFront_Length * 0.5,
	               0. * deg, 360. * deg);

	G4UnionSolid *TargetTube_Solid = new G4UnionSolid(
	    "TargetTube_Solid", TargetTubeBack_Solid, TargetTubeFront_Solid, 0,
	    G4ThreeVector(0., 0., -TargetTubeBack_Length * 0.5 -
	                              TargetTubeFront_Length * 0.5));

	G4LogicalVolume *TargetTube_Logical = new G4LogicalVolume(
	    TargetTube_Solid, Plexiglass, "TargetTube_Logical", 0, 0, 0);

	TargetTube_Logical->SetVisAttributes(new G4VisAttributes(orange));

	new G4PVPlacement(0, G4ThreeVector(0., 0., TargetTube_Total_Length * 0.5),
	                  TargetTube_Logical, "TargetTube", world_log, false, 0);

	/********************* Target holder ring *********************/

	G4double TargetRingWall_Thickness = 2. * mm; // Measured
	G4double TargetRing_Length = 20. * mm;       // Measured

	G4Tubs *TargetRing_Solid =
	    new G4Tubs("TargetRing_Solid",
	               TargetTube_Outer_Radius - TargetTubeFront_Thickness -
	                   TargetRingWall_Thickness,
	               TargetTube_Outer_Radius - TargetTubeFront_Thickness,
	               TargetRing_Length * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *TargetRing_Logical = new G4LogicalVolume(
	    TargetRing_Solid, Plexiglass, "TargetRing_Logical", 0, 0, 0);

	TargetRing_Logical->SetVisAttributes(new G4VisAttributes(red));

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), TargetRing_Logical,
	                  "TargetRing", world_log, false, 0);

	/******************* Walls and shielding ************************/
	//                   2
	//             00  11233 44  55       66   77
	// ----=== --- 00--11233-44--55---o---66---77---o-----> beam
	//      |      00| 11233 44  55   |   66   77   |
	//      |        |   2            |             |
	//      |        |                |             |
	//  collimator  paddle            g3        2nd setup
	//
	// Jump to the implementations in the source code by searching for "WALL*"
	//
	// WALL0: Lead wall after collimator (permanent)
	// WALL1: Lead wall after paddle (permanent)
	// WALL2: Concrete wall between collimator room and UTR (permanent)
	// WALL3: Lead wall after concrete wall (permanent)
	// WALL4: Second lead wall after concrete wall (permanent)
	// WALL5: Lead shielding in front of g3 setup (nonexistent here)
	// WALL6: Lead shielding after g3 setup (since 82Se/82Kr experiment)
	// WALL7: Lead + concrete shielding of 2nd setup (nonexistent here)
	//

	// Load Bricks
	NormBrick *nb = new NormBrick(world_log);
	NormBrickWithHole *nbh = new NormBrickWithHole(world_log);
	ShortNormBrick *snb = new ShortNormBrick(world_log);
	// ShortBrickWithHole* sbh = new ShortBrickWithHole(world_log);
	// HalfShortBrickWithHole *hsbh = new HalfShortBrickWithHole(world_log);
	ConcreteBrick *cb = new ConcreteBrick(world_log);
	// FlatConcreteBrick *fcb = new FlatConcreteBrick(world_log);
	// BridgeBrick *bb = new BridgeBrick(world_log);
	// ThinNormBrick *tb = new ThinNormBrick(world_log);
	// ShortThinNormBrick *stb = new ShortThinNormBrick(world_log);
	// FlatFlatThinNormBrick *fftb = new FlatFlatThinNormBrick(world_log);
	ThreeQuarterShortNormBrick *tqsb =
	    new ThreeQuarterShortNormBrick(world_log);

	G4double Wall5_To_Target = 420. * mm; // Measured
	G4double Wall4_To_Target =
	    Wall5_To_Target + 3.5 * nb->M + 615. * mm; // Measured
	G4double Wall3_To_Target =
	    Wall4_To_Target + 3. * nb->M + 100. * mm;                  // Measured
	G4double ConcreteWall_To_Target = Wall3_To_Target + 4 * nb->S; // Measured
	G4double ConcreteWall_Thickness = 140. * mm;                   // Measured
	G4double Wall1_To_Target =
	    Wall3_To_Target + 4 * nb->S + ConcreteWall_Thickness; // Measured
	G4double Wall0_To_Target = Wall3_To_Target + 4 * nb->S +
	                           ConcreteWall_Thickness + 8 * nb->S +
	                           320. * mm; // Measured

	/**************** WALL0 Lead wall after collimator
	 * *************************/

	for (int ny = -2; ny <= 2; ny++) {
		for (int nx = -1; nx <= 1; nx++) {
			for (int nz = 0; nz < 8; nz++) {
				if (ny == 0 && nx == 0) {
					nbh->Put(nb->L * nx, nb->M * ny,
					         -Wall0_To_Target - nb->S * (nz + 0.5), 0. * deg,
					         90. * deg, 90. * deg);
					continue;
				}

				nb->Put(nb->L * nx, nb->M * ny,
				        -Wall0_To_Target - nb->S * (nz + 0.5), 0. * deg,
				        90. * deg, 90. * deg);
			}
		}
	}

	/**************** Paddle (Scintillator
	 * Detector)****************************/

	G4double Paddle_Thickness = 2. * mm; // Estimated
	G4double Paddle_X = 150. * mm;       // Estimated
	G4double Paddle_Y = 50. * mm;        // Estimated

	G4Box *Paddle_Solid =
	    new G4Box("Paddle_Solid", Paddle_X, Paddle_Y, Paddle_Thickness);
	G4LogicalVolume *Paddle_Logical = new G4LogicalVolume(
	    Paddle_Solid, Scintillator_Plastic, "Paddle_Logical", 0, 0, 0);

	Paddle_Logical->SetVisAttributes(new G4VisAttributes(red));

	new G4PVPlacement(0, G4ThreeVector(0., 0., -Wall0_To_Target + 150. * mm),
	                  Paddle_Logical, "Paddle", world_log, false,
	                  0); // Position estimated

	/**************** WALL1 Lead wall after paddle
	 * *****************************/

	for (int ny = -2; ny <= 2; ny++) {
		for (int nx = -1; nx <= 1; nx++) {
			for (int nz = 0; nz < 8; nz++) {
				if (ny == 0 && nx == 0) {
					nbh->Put(nb->L * nx, nb->M * ny,
					         -Wall1_To_Target - nb->S * (nz + 0.5), 0. * deg,
					         90. * deg, 90. * deg);
					continue;
				}

				nb->Put(nb->L * nx, nb->M * ny,
				        -Wall1_To_Target - nb->S * (nz + 0.5), 0. * deg,
				        90. * deg, 90. * deg);
			}
		}
	}

	/**************** WALL2 Concrete wall between collimator room and UTR
	 * ******/

	G4double ConcreteWall_X = world_x;
	G4double ConcreteWall_Y = world_y;

	G4double ConcreteWallHole_Radius = 50. * mm; // Estimated

	G4Box *ConcreteWall_Solid =
	    new G4Box("ConcreteWall_Solid", ConcreteWall_X / 2.,
	              ConcreteWall_Y / 2., ConcreteWall_Thickness / 2.);
	G4Tubs *ConcreteWallHole_Solid =
	    new G4Tubs("ConcreteWallHole_Solid", 0., ConcreteWallHole_Radius,
	               ConcreteWall_Thickness / 2., 0. * deg, 360. * deg);

	G4SubtractionSolid *ConcreteWallWithHole_Solid =
	    new G4SubtractionSolid("ConcreteWallWithHole_Solid", ConcreteWall_Solid,
	                           ConcreteWallHole_Solid);

	G4LogicalVolume *ConcreteWallWithHole_Logical =
	    new G4LogicalVolume(ConcreteWallWithHole_Solid, Concrete,
	                        "ConcreteWallWithHole_Logical", 0, 0, 0);

	ConcreteWallWithHole_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(0, G4ThreeVector(0., 0., -ConcreteWall_To_Target -
	                                               ConcreteWall_Thickness / 2.),
	                  ConcreteWallWithHole_Logical, "ConcreteWallWithHole",
	                  world_log, false, 0);

	/**************** WALL3 Lead wall after concrete wall
	 * ***********************/

	for (int ny = -2; ny <= 2; ny++) {
		for (int nx = -1; nx <= 1; nx++) {
			for (int nz = 0; nz < 4; nz++) {
				if (ny == 0 && nx == 0) {
					nbh->Put(nb->L * nx, nb->M * ny,
					         -Wall3_To_Target - nb->S * (nz + 0.5), 0. * deg,
					         90. * deg, 90. * deg);
					continue;
				}

				nb->Put(nb->L * nx, nb->M * ny,
				        -Wall3_To_Target - nb->S * (nz + 0.5), 0. * deg,
				        90. * deg, 90. * deg);
			}
		}
	}

	/**************** WALL4 Second wall after concrete wall
	 * ********************/

	for (int ny = -2; ny <= 2; ny++) {
		for (int nz = 0; nz < 4; nz++) {
			for (int nx = 0; nx < 1; nx++) {
				if (ny == 0 && nx == 0) {
					nbh->Put(0., 0., -Wall4_To_Target - nb->S * (nz + 0.5),
					         0. * deg, 90. * deg, 90. * deg);
					tqsb->Put(nb->L * 0.5 + tqsb->L * 0.5, 0.,
					          -Wall4_To_Target - nb->S * (nz + 0.5), 0. * deg,
					          90. * deg, 90. * deg);
					tqsb->Put(-nb->L * 0.5 - tqsb->L * 0.5, 0.,
					          -Wall4_To_Target - nb->S * (nz + 0.5), 0. * deg,
					          90. * deg, 90. * deg);
					continue;
				}
				nb->Put(0., nb->M * ny, -Wall4_To_Target - nb->S * (nz + 0.5),
				        0. * deg, 90. * deg, 90. * deg);
				tqsb->Put(nb->L * 0.5 + tqsb->L * 0.5, nb->M * ny,
				          -Wall4_To_Target - nb->S * (nz + 0.5), 0. * deg,
				          90. * deg, 90. * deg);
				tqsb->Put(-nb->L * 0.5 - tqsb->L * 0.5, nb->M * ny,
				          -Wall4_To_Target - nb->S * (nz + 0.5), 0. * deg,
				          90. * deg, 90. * deg);
			}
		}
	}

	/**************** WALL5 Lead wall in front of g3 setup
	 * *********************/

	// (nonexistent here)

	/**************** WALL6 Lead shielding after g3 setup
	 * *************************/

	G4double FirstLayer_OffsetY = -8. * mm; // Measured

	// First layer in beam direction
	nb->Put(0., -BeamTube_Outer_Radius + nb->S * 5.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(0., -BeamTube_Outer_Radius + nb->S * 4.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius + nb->S * 3.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius + nb->S * 3.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius + nb->S * 2.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius + nb->S * 2.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5 - BeamTube_Outer_Radius,
	        -BeamTube_Outer_Radius + nb->S * 1.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5 + BeamTube_Outer_Radius,
	        -BeamTube_Outer_Radius + nb->S * 1.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5 - BeamTube_Outer_Radius,
	        -BeamTube_Outer_Radius + nb->S * 0.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5 + BeamTube_Outer_Radius,
	        -BeamTube_Outer_Radius + nb->S * 0.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 0.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 0.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 1.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 1.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 2.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 2.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 3.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 3.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 4.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 4.5 + FirstLayer_OffsetY,
	        Wall6_To_Target + nb->M * 0.5, 0., 90. * deg, 0.);

	// Second layer in beam direction

	nb->Put(-BeamTube_Outer_Radius - nb->M * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 3. + nb->L * 0.5,
	        Wall6_To_Target + nb->M + nb->S * 0.5, 90. * deg, 0. * deg,
	        0. * deg);
	nb->Put(BeamTube_Outer_Radius + nb->M * 0.5,
	        -BeamTube_Outer_Radius - nb->S * 3. + nb->L * 0.5,
	        Wall6_To_Target + nb->M + nb->S * 0.5, 90. * deg, 0. * deg,
	        0. * deg);
	nb->Put(0., BeamTube_Outer_Radius + nb->S * 0.5,
	        Wall6_To_Target + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5, -BeamTube_Outer_Radius - nb->S * 3.5,
	        Wall6_To_Target + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5, -BeamTube_Outer_Radius - nb->S * 3.5,
	        Wall6_To_Target + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(-nb->L * 0.5, -BeamTube_Outer_Radius - nb->S * 4.5,
	        Wall6_To_Target + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
	nb->Put(nb->L * 0.5, -BeamTube_Outer_Radius - nb->S * 4.5,
	        Wall6_To_Target + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);

	// Third layer in beam direction

	nb->Put(0., -BeamTube_Outer_Radius - nb->S * 0.5,
	        Wall6_To_Target + nb->M * 2., 0., 90. * deg, 0.);
	nb->Put(0., -BeamTube_Outer_Radius - nb->S * 1.5,
	        Wall6_To_Target + nb->M * 2., 0., 90. * deg, 0.);
	nb->Put(0., -BeamTube_Outer_Radius - nb->S * 2.5,
	        Wall6_To_Target + nb->M * 2., 0., 90. * deg, 0.);
	nb->Put(0., -BeamTube_Outer_Radius - nb->S * 3. - nb->M * 0.5,
	        Wall6_To_Target + nb->M * 2. + nb->S * 0.5, 0., 90. * deg,
	        90. * deg);
	snb->Put(-BeamTube_Outer_Radius - nb->M * 0.5,
	         -BeamTube_Outer_Radius + nb->S * 0.5,
	         Wall6_To_Target + nb->M * 2.);
	snb->Put(BeamTube_Outer_Radius + nb->M * 0.5,
	         -BeamTube_Outer_Radius + nb->S * 0.5,
	         Wall6_To_Target + nb->M * 2.);

	/********************* Table Plate ******************/

	G4double TablePlate_Width = 21. * inch;   // Measured
	G4double TablePlate_Length = 36.5 * inch; // Measured
	G4double TablePlate_Thickness = 20. * mm; // Estimated

	G4double TablePlate_Height =
	    -BeamTube_Outer_Radius - nb->S * 5. - 25. * mm; // Measured

	G4Material *TablePlate_Material = Fe; // Actually, it's made of steel

	G4Box *TablePlate_Solid =
	    new G4Box("TablePlate_Solid", TablePlate_Width / 2.,
	              TablePlate_Thickness / 2., TablePlate_Length / 2.);
	G4LogicalVolume *TablePlate_Logical = new G4LogicalVolume(
	    TablePlate_Solid, TablePlate_Material, "TablePlate_Logical", 0, 0, 0);

	TablePlate_Logical->SetVisAttributes(new G4VisAttributes(grey));

	new G4PVPlacement(
	    0, G4ThreeVector(0., TablePlate_Height - TablePlate_Thickness / 2.,
	                     Wheel_To_Target_Position + Wheel_Total_Thickness +
	                         TablePlate_Length * 0.5),
	    TablePlate_Logical, "TablePlate", world_log, false, 0);

	/**************** WALL7 *****************/

	G4double Wall7_To_Target = Wheel_To_Target_Position +
	                           Wheel_Total_Thickness + TablePlate_Length -
	                           75. * mm - 3. * nb->S; // Measured

	// Table for second setup

	G4double Target2_To_Target = Wheel_To_Target_Position +
	                             Wheel_Total_Thickness + TablePlate_Length +
	                             435. * mm; // Measured

	G4double Table2_OuterRadius = 435. * mm; // Measured
	G4double Table2_InnerRadius = 230. * mm; // Measured
	G4double Table2_Thickness = 20. * mm;    // Estimated
	G4double Table2_Y = -cb->M;

	G4double PipeHolderTube_OuterRadius = 75. * mm;       // Measured
	G4double PipeHolderTube_InnerRadius = 68. * mm;       // Measured
	G4double PipeHolderTubeDownStream_Length = 250. * mm; // Measured
	G4double PipeHolderTubeUpStream_Length = 160. * mm;   // Measured

	G4Material *PipeHolderTube_Material = Al;

	// Downstream pipe holder
	G4Tubs *PipeHolderTubeDownStream_Solid =
	    new G4Tubs("PipeHolderTubeDownStream_Solid", PipeHolderTube_InnerRadius,
	               PipeHolderTube_OuterRadius,
	               PipeHolderTubeDownStream_Length / 2., 0. * deg, 360. * deg);
	G4LogicalVolume *PipeHolderTubeDownStream_Logical = new G4LogicalVolume(
	    PipeHolderTubeDownStream_Solid, PipeHolderTube_Material,
	    "PipeHolderTubeDownStream_Logical", 0, 0, 0);

	PipeHolderTubeDownStream_Logical->SetVisAttributes(
	    new G4VisAttributes(white));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Target2_To_Target + Table2_InnerRadius +
	                                 50. * mm +
	                                 PipeHolderTubeDownStream_Length * 0.5),
	    PipeHolderTubeDownStream_Logical, "PipeHolderTubeDownStream", world_log,
	    false, 0); // Measured

	// Upstream pipe holder
	G4Tubs *PipeHolderTubeUpStream_Solid =
	    new G4Tubs("PipeHolderTubeUpStream_Solid", PipeHolderTube_InnerRadius,
	               PipeHolderTube_OuterRadius,
	               PipeHolderTubeUpStream_Length / 2., 0. * deg, 360. * deg);
	G4LogicalVolume *PipeHolderTubeUpStream_Logical = new G4LogicalVolume(
	    PipeHolderTubeUpStream_Solid, PipeHolderTube_Material,
	    "PipeHolderTubeUpStream_Logical", 0, 0, 0);

	PipeHolderTubeUpStream_Logical->SetVisAttributes(
	    new G4VisAttributes(white));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Wall7_To_Target + nb->S * 3. +
	                                 PipeHolderTubeUpStream_Length * 0.5),
	    PipeHolderTubeUpStream_Logical, "PipeHolderTubeUpStream", world_log,
	    false, 0);

	G4Material *Table2_Material = Al;

	G4Tubs *Table2Plate_Solid =
	    new G4Tubs("Table2Plate_Solid", Table2_InnerRadius, Table2_OuterRadius,
	               Table2_Thickness * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Table2Plate_Logical = new G4LogicalVolume(
	    Table2Plate_Solid, Table2_Material, "Table2Plate_Logical", 0, 0, 0);

	Table2Plate_Logical->SetVisAttributes(new G4VisAttributes(white));

	G4RotationMatrix *rotateTable2Plate = new G4RotationMatrix();
	rotateTable2Plate->rotateX(90. * deg);

	new G4PVPlacement(
	    rotateTable2Plate,
	    G4ThreeVector(0., Table2_Y - Table2_Thickness * 0.5, Target2_To_Target),
	    Table2Plate_Logical, "Table2Plate", world_log, false, 0);

	/************************ WALL7 Part on the second setup table ********/

	// nonexistent here

	/******************** Target Holder Tube with cap ***********/

	// Tube
	G4double TargetTubeWithCapFront_Length = 50. * mm; // Measured
	G4double TargetTubeWithCapBack_Length =
	    BeamTube_Length_Downstream - Target2_To_Target -
	    TargetTubeWithCapFront_Length * 0.5; // Measured

	G4Tubs *TargetTubeWithCapBack_Solid =
	    new G4Tubs("TargetTubeWithCapBack_Solid",
	               TargetTube_Outer_Radius - TargetTubeBack_Thickness,
	               TargetTube_Outer_Radius, TargetTubeWithCapBack_Length * 0.5,
	               0. * deg, 360. * deg);
	G4Tubs *TargetTubeWithCapFront_Solid =
	    new G4Tubs("TargetTubeWithCapFront_Solid",
	               TargetTube_Outer_Radius - TargetTubeFront_Thickness,
	               TargetTube_Outer_Radius, TargetTubeWithCapFront_Length * 0.5,
	               0. * deg, 360. * deg);

	G4UnionSolid *TargetTubeWithCap_Solid = new G4UnionSolid(
	    "TargetTubeWithCap_Solid", TargetTubeWithCapBack_Solid,
	    TargetTubeWithCapFront_Solid, 0,
	    G4ThreeVector(0., 0., -TargetTubeWithCapBack_Length * 0.5 -
	                              TargetTubeWithCapFront_Length * 0.5));

	G4LogicalVolume *TargetTubeWithCap_Logical =
	    new G4LogicalVolume(TargetTubeWithCap_Solid, Plexiglass,
	                        "TargetTubeWithCap_Logical", 0, 0, 0);

	TargetTubeWithCap_Logical->SetVisAttributes(new G4VisAttributes(orange));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., BeamTube_Length_Downstream -
	                                 TargetTubeWithCapBack_Length / 2.),
	    TargetTubeWithCap_Logical, "TargetTubeWithCap", world_log, false, 0);

	// Target holder ring 2
	new G4PVPlacement(0, G4ThreeVector(0., 0., Target2_To_Target),
	                  TargetRing_Logical, "TargetRing", world_log, false, 0);

	// Cap
	G4double TargetTubeCap_Thickness = 2. * mm;
	G4double TargetTubeCap_Outer_Radius = 1.1 * inch;

	G4Tubs *TargetTubeCap_Solid =
	    new G4Tubs("TargetTubeCap_Solid", 0., TargetTubeCap_Outer_Radius,
	               TargetTubeCap_Thickness / 2., 0. * deg, 360. * deg);

	G4LogicalVolume *TargetTubeCap_Logical = new G4LogicalVolume(
	    TargetTubeCap_Solid, Plexiglass, "TargetTubeCap_Logical", 0, 0, 0);

	TargetTubeCap_Logical->SetVisAttributes(new G4VisAttributes(orange));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., BeamTube_Length_Downstream +
	                                 TargetTubeCap_Thickness / 2.),
	    TargetTubeCap_Logical, "TargetTubeCap", world_log, false, 0);

	/**************** Collimator *************************/

	G4double Collimator_Length = 270. * mm;            // Measured
	G4double Collimator_XY = 60. * mm;                 // Measured
	G4double CollimatorHole_Radius = 0.75 * inch / 2.; // Measured
	G4Material *Collimator_Material = Pb;

	G4double Collimator_To_Target = Wall0_To_Target + nb->S * 8. + 200. * mm +
	                                Collimator_Length * 0.5; // Estimated

	G4Box *CollimatorBox_Solid =
	    new G4Box("CollimatorBox_Solid", Collimator_XY / 2., Collimator_XY / 2.,
	              Collimator_Length / 2.);
	G4Tubs *CollimatorHole_Solid =
	    new G4Tubs("CollimatorHole_Solid", 0., CollimatorHole_Radius,
	               Collimator_Length / 2., 0. * deg, 360. * deg);

	G4SubtractionSolid *Collimator_Solid = new G4SubtractionSolid(
	    "Collimator_Solid", CollimatorBox_Solid, CollimatorHole_Solid);
	G4LogicalVolume *Collimator_Logical = new G4LogicalVolume(
	    Collimator_Solid, Collimator_Material, "Collimator_Solid", 0, 0, 0);

	Collimator_Logical->SetVisAttributes(new G4VisAttributes(grey));

	G4RotationMatrix *rotateCollimator = new G4RotationMatrix();
	rotateCollimator->rotateY(0. * deg);

	new G4PVPlacement(
	    rotateCollimator,
	    G4ThreeVector(0., 0., -Collimator_To_Target - Collimator_Length / 2.),
	    Collimator_Logical, "Collimator", world_log, false, 0);

	/**************** g3 Target/Source ******************/

	Se82_Target *se82_Target = new Se82_Target();

	G4LogicalVolume *Se82_Target_Logical = se82_Target->Get_Logical();

	G4RotationMatrix *rotateG3Target = new G4RotationMatrix();
	rotateG3Target->rotateY(180. * deg);

	new G4PVPlacement(
	    rotateG3Target,
	    G4ThreeVector(0., 0., -BeamTube_Length_Downstream +
	                              BeamTube_Length * 0.5 +
	                              se82_Target->Get_Target_Center()),
	    Se82_Target_Logical, "Se82_Target_Physical", BeamTubeVacuum_Logical,
	    false, 0);

	/**************** Second Target/Source ***************/

	Kr82_Target *kr82_Target = new Kr82_Target();

	G4LogicalVolume *Kr82_Target_Logical = kr82_Target->Get_Logical();

	G4RotationMatrix *rotateSecondTarget = new G4RotationMatrix();
	G4double SecondTarget_AngleX = -22. * deg; // Rotation angle to have the
	                                           // valve rest on the inside of
	                                           // the TargetTube
	rotateSecondTarget->rotateX(SecondTarget_AngleX);

	new G4PVPlacement(
	    rotateSecondTarget,
	    G4ThreeVector(0., 20. * mm * sin(SecondTarget_AngleX),
	                  -BeamTube_Length_Downstream + BeamTube_Length * 0.5 +
	                      Target2_To_Target - kr82_Target->Get_Target_Center() -
	                      20. * mm * (1 - cos(SecondTarget_AngleX))),
	    Kr82_Target_Logical, "Kr82_Target_Physical", BeamTubeVacuum_Logical,
	    false, 0);

	// Box around the target/source. Inside this box, random coordinates for the
	// AngularDistributionGenerator are sampled in order to find random points
	// inside the target/source. The box should wrap the target/source volume as
	// tightly as possible. Remove the box when doing an actual simulation.

	//	G4double AuxBox_LengthX = 20. * mm;
	//	G4double AuxBox_LengthY = 20. * mm;
	//	G4double AuxBox_LengthZ = 4. * mm;
	//
	//	G4double AuxBox_X = 0. * mm;
	//	G4double AuxBox_Y = 0. * mm;
	//	G4double AuxBox_Z = 0. * mm;
	//
	//	G4Box *AuxiliaryBox_Solid =
	//	    new G4Box("AuxiliaryBox_Solid", AuxBox_LengthX * 0.5,
	//	              AuxBox_LengthY * 0.5, AuxBox_LengthZ * 0.5);
	//	G4LogicalVolume *AuxiliaryBox_Logical =
	//	    new G4LogicalVolume(AuxiliaryBox_Solid, vacuum,
	//"AuxiliaryBox_Logical");
	//
	//	AuxiliaryBox_Logical->SetVisAttributes(new G4VisAttributes(white));

	// new G4PVPlacement(0, G4ThreeVector(AuxBox_X, AuxBox_Y, AuxBox_Z),
	// AuxiliaryBox_Logical, "AuxiliaryBox", world_log, false, 0);

	/************************** Detectors ***************************/
	//
	// 0) ZERODEGREE
	// 1) HPGE1
	// 2) HPGE2
	// 3) HPGE3
	// 4) HPGE4
	// 5) POLARIMETER
	// 6) DETECTOR2
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

	/****************** Initialize Filters **************************/

	FilterCaseWall *fcw = new FilterCaseWall(world_log);
	FilterCaseRing *fcr = new FilterCaseRing(world_log);
	FilterCaseBottom *fcbo = new FilterCaseBottom(world_log);

	Pb_41_5mm_3_64in *pbthinsmall = new Pb_41_5mm_3_64in(world_log);
	Pb_44mm_3_64in *pbthinmedium = new Pb_44mm_3_64in(world_log);
	Pb_45mm_1_4in *pbthick = new Pb_45mm_1_4in(world_log);
	Pb_45mm_5_64in *pbmedium = new Pb_45mm_5_64in(world_log);
	Pb_45mm_3_64in *pbthin = new Pb_45mm_3_64in(world_log);
	Pb_50mm_5_64in *pbmedium50 = new Pb_50mm_5_64in(world_log);
	// Pb_70mm_5mm* pb70 = new Pb_70mm_5mm(world_log);

	Cu_45mm_1_8in *cumedium = new Cu_45mm_1_8in(world_log);
	Cu_45mm_1mm *cuthin = new Cu_45mm_1mm(world_log);
	Cu_50mm_1mm *cuthin50 = new Cu_50mm_1mm(world_log);
	// Cu_73mm_1_20in* cu73 = new Cu_73mm_1_20in(world_log);

	/************************* ZERODEGREE ***************************/

	G4double ZeroDegree_X =
	    0. * mm; // X = 0, if beam profile is measured, otherwise, X < 0
	G4double ZeroDegree_Y = 20. * mm;                       // Measured
	G4double ZeroDegree_Z = Target2_To_Target + 36. * inch; // Measured
	G4double ZeroDegree_Theta = 0. * deg;
	// G4double ZeroDegree_Phi = 0.*deg; Should always be zero

	ZeroDegree *ZeroDegree_Instance = new ZeroDegree("ZeroDegree");
	G4LogicalVolume *ZeroDegree_Logical = ZeroDegree_Instance->Get_Logical();

	G4RotationMatrix *rotateZeroDegree = new G4RotationMatrix();
	rotateZeroDegree->rotateY(180. * deg);
	rotateZeroDegree->rotateY(ZeroDegree_Theta);

	new G4PVPlacement(
	    rotateZeroDegree,
	    G4ThreeVector(ZeroDegree_X, ZeroDegree_Y,
	                  ZeroDegree_Z + ZeroDegree_Instance->Get_Length() * 0.5),
	    ZeroDegree_Logical, "ZeroDegree", world_log, false, 0);

	/************************* HPGe1 ********************************/

	G4double HPGe1_rt = 94. * mm;
	G4double HPGe1_dy = 17. * mm;
	G4double HPGe1_dz = 6. * mm;
	G4double HPGe1_phi = 0. * deg;
	G4double HPGe1_theta = 90. * deg;

	G4double HPGe1_AngleX = 180. * deg;
	G4double HPGe1_AngleY = 90. * deg;

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
	    HPGe1_Logical, "HPGe1", world_log, false, 0);

	// HPGe1 Wrapping

	G4double HPGe1_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe1_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe1_Wrapping_Solid =
	    new G4Tubs("HPGe1_Wrapping_Solid", HPGe1_Instance->Get_Radius(),
	               HPGe1_Instance->Get_Radius() + HPGe1_Wrapping_Thickness,
	               HPGe1_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe1_Wrapping_Logical = new G4LogicalVolume(
	    HPGe1_Wrapping_Solid, Pb, "HPGe1_Wrapping_Logical", 0, 0, 0);

	HPGe1_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe1_rt = HPGe1_rt + HPGe1_Wrapping_Length * 0.5 -
	           HPGe1_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateHPGe1,
	    G4ThreeVector(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                  HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                  HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz),
	    HPGe1_Wrapping_Logical, "HPGe1_Wrapping", world_log, false, 0);
	HPGe1_rt = HPGe1_rt - HPGe1_Wrapping_Length * 0.5;

	// HPGe1 Filters

	HPGe1_rt -= fcr->Thickness * 0.5;
	fcr->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	         HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	         HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX, HPGe1_AngleY,
	         0.);
	HPGe1_rt -= fcr->Thickness * 0.5;

	HPGe1_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	            HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	            HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX,
	            HPGe1_AngleY, 0.);
	HPGe1_rt -= cuthin->Thickness * 0.5;

	HPGe1_rt -= pbthick->Thickness * 0.5;
	pbthick->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	             HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	             HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX,
	             HPGe1_AngleY, 0.);
	HPGe1_rt -= pbthick->Thickness * 0.5;

	HPGe1_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	         HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	         HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX, HPGe1_AngleY,
	         0.);
	HPGe1_rt -= fcw->FilterCase_Length * 0.5;

	HPGe1_rt += fcbo->Thickness * 0.5;
	fcbo->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	          HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	          HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX,
	          HPGe1_AngleY, 0.);
	HPGe1_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe2 ********************************/

	G4double HPGe2_rt = 78.4 * mm;
	G4double HPGe2_dy = 18. * mm;
	G4double HPGe2_dz = 6. * mm;
	G4double HPGe2_phi = 90. * deg;
	G4double HPGe2_theta = 90. * deg;

	HPGe2 *HPGe2_Instance = new HPGe2("HPGe2");
	G4LogicalVolume *HPGe2_Logical = HPGe2_Instance->Get_Logical();

	G4double HPGe2_AngleX = 270. * deg;
	G4double HPGe2_AngleY = 0. * deg;

	G4RotationMatrix *rotateHPGe2 = new G4RotationMatrix();
	rotateHPGe2->rotateX(HPGe2_AngleX);
	rotateHPGe2->rotateY(HPGe2_AngleY);

	HPGe2_rt = HPGe2_rt + HPGe2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Logical, "HPGe2", world_log, false, 0);

	// HPGe2 Wrapping

	G4double HPGe2_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe2_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe2_Wrapping_Solid =
	    new G4Tubs("HPGe2_Wrapping_Solid", HPGe2_Instance->Get_Radius(),
	               HPGe2_Instance->Get_Radius() + HPGe2_Wrapping_Thickness,
	               HPGe2_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe2_Wrapping_Logical = new G4LogicalVolume(
	    HPGe2_Wrapping_Solid, Pb, "HPGe2_Wrapping_Logical", 0, 0, 0);

	HPGe2_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe2_rt = HPGe2_rt + HPGe2_Wrapping_Length * 0.5 -
	           HPGe2_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateHPGe2,
	    G4ThreeVector(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	                  HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	                  HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz),
	    HPGe2_Wrapping_Logical, "HPGe2_Wrapping", world_log, false, 0);
	HPGe2_rt = HPGe2_rt - HPGe2_Wrapping_Length * 0.5;

	// HPGe2 Filters

	HPGe2_rt -= fcr->Thickness * 0.5;
	fcr->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	         HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	         HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX, HPGe2_AngleY,
	         0.);
	HPGe2_rt -= fcr->Thickness * 0.5;

	HPGe2_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	            HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	            HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	            HPGe2_AngleY, 0.);
	HPGe2_rt -= cuthin->Thickness * 0.5;

	HPGe2_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	              HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	              HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	              HPGe2_AngleY, 0.);
	HPGe2_rt -= pbmedium->Thickness * 0.5;

	HPGe2_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	              HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	              HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	              HPGe2_AngleY, 0.);
	HPGe2_rt -= pbmedium->Thickness * 0.5;

	HPGe2_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	              HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	              HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	              HPGe2_AngleY, 0.);
	HPGe2_rt -= pbmedium->Thickness * 0.5;

	HPGe2_rt -= pbthin->Thickness * 0.5;
	pbthin->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	            HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	            HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	            HPGe2_AngleY, 0.);
	HPGe2_rt -= pbthin->Thickness * 0.5;

	HPGe2_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	         HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	         HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX, HPGe2_AngleY,
	         0.);
	HPGe2_rt -= fcw->FilterCase_Length * 0.5;

	HPGe2_rt += fcbo->Thickness * 0.5;
	fcbo->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	          HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	          HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	          HPGe2_AngleY, 0.);
	HPGe2_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe3
	 * ********************************/

	G4double HPGe3_rt = 112.5 * mm;
	G4double HPGe3_dy = 0. * mm;
	G4double HPGe3_dz = -6.5 * mm;
	G4double HPGe3_phi = 135. * deg;
	G4double HPGe3_theta = 135. * deg;

	G4double HPGe3_AngleX = 144.736 * deg;
	G4double HPGe3_AngleY = 210. * deg;

	HPGe3 *HPGe3_Instance = new HPGe3("HPGe3");
	G4LogicalVolume *HPGe3_TUD_Logical = HPGe3_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe3 = new G4RotationMatrix();
	rotateHPGe3->rotateX(HPGe3_AngleX);
	rotateHPGe3->rotateY(HPGe3_AngleY);

	HPGe3_rt = HPGe3_rt + HPGe3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_TUD_Logical, "HPGe3", world_log, false, 0);

	// HPGe3 Wrapping

	G4double HPGe3_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe3_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe3_Wrapping_Solid =
	    new G4Tubs("HPGe3_Wrapping_Solid", HPGe3_Instance->Get_Radius(),
	               HPGe3_Instance->Get_Radius() + HPGe3_Wrapping_Thickness,
	               HPGe3_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe3_Wrapping_Logical = new G4LogicalVolume(
	    HPGe3_Wrapping_Solid, Pb, "HPGe3_Wrapping_Logical", 0, 0, 0);

	HPGe3_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe3_rt = HPGe3_rt + HPGe3_Wrapping_Length * 0.5 -
	           HPGe3_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateHPGe3,
	    G4ThreeVector(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                  HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                  HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz),
	    HPGe3_Wrapping_Logical, "HPGe3_Wrapping", world_log, false, 0);
	HPGe3_rt = HPGe3_rt - HPGe2_Wrapping_Length * 0.5;

	// HPGe3 Filters

	HPGe3_rt -= fcr->Thickness * 0.5;
	fcr->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	         HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	         HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX, HPGe3_AngleY,
	         0.);
	HPGe3_rt -= fcr->Thickness * 0.5;

	HPGe3_rt -= cumedium->Thickness * 0.5;
	cumedium->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	              HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	              HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX,
	              HPGe3_AngleY, 0.);
	HPGe3_rt -= cumedium->Thickness * 0.5;

	HPGe3_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	              HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	              HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX,
	              HPGe3_AngleY, 0.);
	HPGe3_rt -= pbmedium->Thickness * 0.5;

	HPGe3_rt -= pbthinsmall->Thickness * 0.5;
	pbthinsmall->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	                 HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	                 HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX,
	                 HPGe3_AngleY, 0.);
	HPGe3_rt -= pbthinsmall->Thickness * 0.5;

	HPGe3_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	         HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	         HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX, HPGe3_AngleY,
	         0.);
	HPGe3_rt -= fcw->FilterCase_Length * 0.5;

	HPGe3_rt += fcbo->Thickness * 0.5;
	fcbo->Put(HPGe3_rt * sin(HPGe3_theta) * cos(HPGe3_phi),
	          HPGe3_rt * sin(HPGe3_theta) * sin(HPGe3_phi) + HPGe3_dy,
	          HPGe3_rt * cos(HPGe3_theta) + HPGe3_dz, HPGe3_AngleX,
	          HPGe3_AngleY, 0.);
	HPGe3_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe4 ********************************/

	G4double HPGe4_rt = 103. * mm;
	G4double HPGe4_dy = 0. * mm;
	G4double HPGe4_dz = 3.5 * mm;
	G4double HPGe4_phi = 45. * deg;
	G4double HPGe4_theta = 135. * deg;

	G4double HPGe4_AngleX = 144.736 * deg;
	G4double HPGe4_AngleY = 150. * deg;

	HPGe4 *HPGe4_Instance = new HPGe4("HPGe4");
	G4LogicalVolume *HPGe4_Logical = HPGe4_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe4 = new G4RotationMatrix();
	rotateHPGe4->rotateX(HPGe4_AngleX);
	rotateHPGe4->rotateY(HPGe4_AngleY);

	HPGe4_rt = HPGe4_rt + HPGe4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Logical, "LaBr", world_log, false, 0);

	// HPGe4 Wrapping

	G4double HPGe4_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe4_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe4_Wrapping_Solid =
	    new G4Tubs("HPGe4_Wrapping_Solid", HPGe4_Instance->Get_Radius(),
	               HPGe4_Instance->Get_Radius() + HPGe4_Wrapping_Thickness,
	               HPGe4_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe4_Wrapping_Logical = new G4LogicalVolume(
	    HPGe4_Wrapping_Solid, Pb, "HPGe4_Wrapping_Logical", 0, 0, 0);

	HPGe4_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe4_rt = HPGe4_rt + HPGe4_Wrapping_Length * 0.5 -
	           HPGe4_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateHPGe4,
	    G4ThreeVector(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	                  HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	                  HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz),
	    HPGe4_Wrapping_Logical, "HPGe4_Wrapping", world_log, false, 0);
	HPGe4_rt = HPGe4_rt - HPGe4_Wrapping_Length * 0.5;

	// HPGe4 Filters

	HPGe4_rt -= fcr->Thickness * 0.5;
	fcr->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	         HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	         HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX, HPGe4_AngleY,
	         0.);
	HPGe4_rt -= fcr->Thickness * 0.5;

	HPGe4_rt -= cumedium->Thickness * 0.5;
	cumedium->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	              HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	              HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	              HPGe4_AngleY, 0.);
	HPGe4_rt -= cumedium->Thickness * 0.5;

	HPGe4_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	              HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	              HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	              HPGe4_AngleY, 0.);
	HPGe4_rt -= pbmedium->Thickness * 0.5;

	HPGe4_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	         HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	         HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX, HPGe4_AngleY,
	         0.);
	HPGe4_rt -= fcw->FilterCase_Length * 0.5;

	HPGe4_rt += fcbo->Thickness * 0.5;
	fcbo->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	          HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	          HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	          HPGe4_AngleY, 0.);
	HPGe4_rt -= fcbo->Thickness * 0.5;

	/************************* LaBr1 (LaBr_TUD)
	 * ********************************/

	G4double LaBr1_rt = 60. * mm;
	G4double LaBr1_dy = 16.5 * mm;
	G4double LaBr1_dz = -6. * mm;
	G4double LaBr1_theta = 90. * deg;
	G4double LaBr1_phi = 180. * deg;

	G4double LaBr1_AngleX = 180. * deg;
	G4double LaBr1_AngleY = 90. * deg;

	LaBr_TUD *LaBr1_Instance = new LaBr_TUD("LaBr1");
	G4LogicalVolume *LaBr1_TUD_Logical = LaBr1_Instance->Get_Logical();

	G4RotationMatrix *rotateLaBr1 = new G4RotationMatrix();
	rotateLaBr1->rotateX(LaBr1_AngleX);
	rotateLaBr1->rotateY(LaBr1_AngleY);

	LaBr1_rt = LaBr1_rt + LaBr1_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr1,
	    G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_TUD_Logical, "LaBr_TUD", world_log, false, 0);

	// LaBr1 Wrapping

	G4double LaBr1_Wrapping_Thickness = 2 * 0.050 * inch;          // Measured
	G4double LaBr1_Wrapping_Length = LaBr1_Instance->Get_Length(); // Estimated

	G4Tubs *LaBr1_Wrapping_Solid =
	    new G4Tubs("LaBr1_Wrapping_Solid", LaBr1_Instance->Get_Radius(),
	               LaBr1_Instance->Get_Radius() + LaBr1_Wrapping_Thickness,
	               LaBr1_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *LaBr1_Wrapping_Logical = new G4LogicalVolume(
	    LaBr1_Wrapping_Solid, Pb, "LaBr1_Wrapping_Logical", 0, 0, 0);

	LaBr1_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	LaBr1_rt = LaBr1_rt + LaBr1_Wrapping_Length * 0.5 -
	           LaBr1_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateLaBr1,
	    G4ThreeVector(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	                  LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	                  LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz),
	    LaBr1_Wrapping_Logical, "LaBr1_Wrapping", world_log, false, 0);
	LaBr1_rt = LaBr1_rt - LaBr1_Wrapping_Length * 0.5;

	// LaBr1 Filters

	LaBr1_rt -= fcr->Thickness * 0.5;
	fcr->Put(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	         LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	         LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz, LaBr1_AngleX, LaBr1_AngleY,
	         0.);
	LaBr1_rt -= fcr->Thickness * 0.5;

	LaBr1_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	            LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	            LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz, LaBr1_AngleX,
	            LaBr1_AngleY, 0.);
	LaBr1_rt -= cuthin->Thickness * 0.5;

	LaBr1_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	         LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	         LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz, LaBr1_AngleX, LaBr1_AngleY,
	         0.);
	LaBr1_rt -= fcw->FilterCase_Length * 0.5;

	LaBr1_rt += fcbo->Thickness * 0.5;
	fcbo->Put(LaBr1_rt * sin(LaBr1_theta) * cos(LaBr1_phi),
	          LaBr1_rt * sin(LaBr1_theta) * sin(LaBr1_phi) + LaBr1_dy,
	          LaBr1_rt * cos(LaBr1_theta) + LaBr1_dz, LaBr1_AngleX,
	          LaBr1_AngleY, 0.);
	LaBr1_rt -= fcbo->Thickness * 0.5;

	/************************* LaBr2 (LaBr_TUD)
	 * ********************************/

	G4double LaBr2_rt = 71. * mm;
	G4double LaBr2_dy = 18. * mm;
	G4double LaBr2_dz = -2.5 * mm;
	G4double LaBr2_phi = 270. * deg;
	G4double LaBr2_theta = 90. * deg;

	LaBr_TUD *LaBr2_Instance = new LaBr_TUD("LaBr2");
	G4LogicalVolume *LaBr2_TUD_Logical = LaBr2_Instance->Get_Logical();

	G4double LaBr2_AngleX = 270. * deg;
	G4double LaBr2_AngleY = 180. * deg;

	G4RotationMatrix *rotateLaBr2 = new G4RotationMatrix();
	rotateLaBr2->rotateX(LaBr2_AngleX);
	rotateLaBr2->rotateY(LaBr2_AngleY);

	LaBr2_rt = LaBr2_rt + LaBr2_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_TUD_Logical, "LaBr_TUD", world_log, false, 0);

	// LaBr2 Wrapping

	G4double LaBr2_Wrapping_Thickness = 2 * 0.050 * inch;          // Measured
	G4double LaBr2_Wrapping_Length = LaBr2_Instance->Get_Length(); // Estimated

	G4Tubs *LaBr2_Wrapping_Solid =
	    new G4Tubs("LaBr2_Wrapping_Solid", LaBr2_Instance->Get_Radius(),
	               LaBr2_Instance->Get_Radius() + LaBr2_Wrapping_Thickness,
	               LaBr2_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *LaBr2_Wrapping_Logical = new G4LogicalVolume(
	    LaBr2_Wrapping_Solid, Pb, "LaBr2_Wrapping_Logical", 0, 0, 0);

	LaBr2_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	LaBr2_rt = LaBr2_rt + LaBr2_Wrapping_Length * 0.5 -
	           LaBr2_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateLaBr2,
	    G4ThreeVector(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz),
	    LaBr2_Wrapping_Logical, "LaBr2_Wrapping", world_log, false, 0);

	LaBr2_rt = LaBr2_rt - LaBr2_Wrapping_Length * 0.5;

	// LaBr2 Filters

	LaBr2_rt -= fcr->Thickness * 0.5;
	fcr->Put(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	         LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	         LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz, LaBr2_AngleX, LaBr2_AngleY,
	         0.);
	LaBr2_rt -= fcr->Thickness * 0.5;

	LaBr2_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	            LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	            LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz, LaBr2_AngleX,
	            LaBr2_AngleY, 0.);
	LaBr2_rt -= cuthin->Thickness * 0.5;

	LaBr2_rt -= pbthinmedium->Thickness * 0.5;
	pbthinmedium->Put(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	                  LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	                  LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz, LaBr2_AngleX,
	                  LaBr2_AngleY, 0.);
	LaBr2_rt -= pbthinmedium->Thickness * 0.5;

	LaBr2_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	         LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	         LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz, LaBr2_AngleX, LaBr2_AngleY,
	         0.);
	LaBr2_rt -= fcw->FilterCase_Length * 0.5;

	LaBr2_rt += fcbo->Thickness * 0.5;
	fcbo->Put(LaBr2_rt * sin(LaBr2_theta) * cos(LaBr2_phi),
	          LaBr2_rt * sin(LaBr2_theta) * sin(LaBr2_phi) + LaBr2_dy,
	          LaBr2_rt * cos(LaBr2_theta) + LaBr2_dz, LaBr2_AngleX,
	          LaBr2_AngleY, 0.);
	LaBr2_rt -= fcbo->Thickness * 0.5;

	/************************* LaBr3 (LaBr_TUD)
	 * ********************************/

	G4double LaBr3_rt = 117. * mm;
	G4double LaBr3_dy = 0. * mm;
	G4double LaBr3_dz = -1. * mm;
	G4double LaBr3_phi = 315. * deg;
	G4double LaBr3_theta = 135. * deg;

	LaBr_TUD *LaBr3_Instance = new LaBr_TUD("LaBr3");
	G4LogicalVolume *LaBr3_TUD_Logical = LaBr3_Instance->Get_Logical();

	/*G4double LaBr3_X = LaBr3_rt*sin(LaBr3_theta)*cos(LaBr3_phi);
	G4double LaBr3_Y = LaBr3_rt*sin(LaBr3_theta)*sin(LaBr3_phi);
	G4double LaBr3_Z = LaBr3_rt*cos(LaBr3_theta);
*/

	G4double LaBr3_AngleX = 215.264 * deg;
	G4double LaBr3_AngleY = 150. * deg;

	G4RotationMatrix *rotateLaBr3 = new G4RotationMatrix();
	rotateLaBr3->rotateX(LaBr3_AngleX);
	rotateLaBr3->rotateY(LaBr3_AngleY);

	LaBr3_rt = LaBr3_rt + LaBr3_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr3,
	    G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_TUD_Logical, "LaBr_TUD", world_log, false, 0);

	// LaBr3 Wrapping

	G4double LaBr3_Wrapping_Thickness = 2 * 0.050 * inch;          // Measured
	G4double LaBr3_Wrapping_Length = LaBr3_Instance->Get_Length(); // Estimated

	G4Tubs *LaBr3_Wrapping_Solid =
	    new G4Tubs("LaBr3_Wrapping_Solid", LaBr3_Instance->Get_Radius(),
	               LaBr3_Instance->Get_Radius() + LaBr3_Wrapping_Thickness,
	               LaBr3_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *LaBr3_Wrapping_Logical = new G4LogicalVolume(
	    LaBr3_Wrapping_Solid, Pb, "LaBr3_Wrapping_Logical", 0, 0, 0);

	LaBr3_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	LaBr3_rt = LaBr3_rt + LaBr3_Wrapping_Length * 0.5 -
	           LaBr3_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateLaBr3,
	    G4ThreeVector(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	                  LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	                  LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz),
	    LaBr3_Wrapping_Logical, "LaBr3_Wrapping", world_log, false, 0);
	LaBr3_rt = LaBr3_rt - LaBr3_Wrapping_Length * 0.5;

	// LaBr3 Filters

	LaBr3_rt -= fcr->Thickness * 0.5;
	fcr->Put(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	         LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	         LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz, LaBr3_AngleX, LaBr3_AngleY,
	         0.);
	LaBr3_rt -= fcr->Thickness * 0.5;

	LaBr3_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	            LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	            LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz, LaBr3_AngleX,
	            LaBr3_AngleY, 0.);
	LaBr3_rt -= cuthin->Thickness * 0.5;

	LaBr3_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	         LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	         LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz, LaBr3_AngleX, LaBr3_AngleY,
	         0.);
	LaBr3_rt -= fcw->FilterCase_Length * 0.5;

	LaBr3_rt += fcbo->Thickness * 0.5;
	fcbo->Put(LaBr3_rt * sin(LaBr3_theta) * cos(LaBr3_phi),
	          LaBr3_rt * sin(LaBr3_theta) * sin(LaBr3_phi) + LaBr3_dy,
	          LaBr3_rt * cos(LaBr3_theta) + LaBr3_dz, LaBr3_AngleX,
	          LaBr3_AngleY, 0.);
	LaBr3_rt -= fcbo->Thickness * 0.5;

	/************************* LaBr4 (LaBr_TUD)
	 *********************************/

	G4double LaBr4_rt = 103. * mm;
	G4double LaBr4_dy = 0. * mm;
	G4double LaBr4_dz = -5.5 * mm;
	G4double LaBr4_phi = 225. * deg;
	G4double LaBr4_theta = 135. * deg;

	LaBr_TUD *LaBr4_Instance = new LaBr_TUD("LaBr4");
	G4LogicalVolume *LaBr4_TUD_Logical = LaBr4_Instance->Get_Logical();

	/*	G4double LaBr4_X = LaBr4_rt*sin(LaBr4_theta)*cos(LaBr4_phi);
	    G4double LaBr4_Y = LaBr4_rt*sin(LaBr4_theta)*sin(LaBr4_phi);
	    G4double LaBr4_Z = LaBr4_rt*cos(LaBr4_theta);
	*/
	G4double LaBr4_AngleX = 215.264 * deg;
	G4double LaBr4_AngleY = 210. * deg;

	G4RotationMatrix *rotateLaBr4 = new G4RotationMatrix();

	rotateLaBr4->rotateX(LaBr4_AngleX);
	rotateLaBr4->rotateY(LaBr4_AngleY);

	LaBr4_rt = LaBr4_rt + LaBr4_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateLaBr4,
	    G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_TUD_Logical, "LaBr_TUD", world_log, false, 0);

	// LaBr4 Wrapping

	G4double LaBr4_Wrapping_Thickness = 2 * 0.050 * inch;          // Measured
	G4double LaBr4_Wrapping_Length = LaBr4_Instance->Get_Length(); // Estimated

	G4Tubs *LaBr4_Wrapping_Solid =
	    new G4Tubs("LaBr4_Wrapping_Solid", LaBr4_Instance->Get_Radius(),
	               LaBr4_Instance->Get_Radius() + LaBr4_Wrapping_Thickness,
	               LaBr4_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *LaBr4_Wrapping_Logical = new G4LogicalVolume(
	    LaBr4_Wrapping_Solid, Pb, "LaBr4_Wrapping_Logical", 0, 0, 0);

	LaBr4_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	LaBr4_rt = LaBr4_rt + LaBr4_Wrapping_Length * 0.5 -
	           LaBr4_Instance->Get_Length() * 0.5;
	new G4PVPlacement(
	    rotateLaBr4,
	    G4ThreeVector(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	                  LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	                  LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz),
	    LaBr4_Wrapping_Logical, "LaBr4_Wrapping", world_log, false, 0);

	// LaBr4 Filters

	LaBr4_rt -= LaBr4_Instance->Get_Length() * 0.5;

	LaBr4_rt -= fcr->Thickness * 0.5;
	fcr->Put(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	         LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	         LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz, LaBr4_AngleX, LaBr4_AngleY,
	         0.);
	LaBr4_rt -= fcr->Thickness * 0.5;

	LaBr4_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	            LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	            LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz, LaBr4_AngleX,
	            LaBr4_AngleY, 0.);
	LaBr4_rt -= cuthin->Thickness * 0.5;

	LaBr4_rt += fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	         LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	         LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz, LaBr4_AngleX, LaBr4_AngleY,
	         0.);
	LaBr4_rt -= fcw->FilterCase_Length * 0.5;

	LaBr4_rt += fcbo->Thickness * 0.5;
	fcbo->Put(LaBr4_rt * sin(LaBr4_theta) * cos(LaBr4_phi),
	          LaBr4_rt * sin(LaBr4_theta) * sin(LaBr4_phi) + LaBr4_dy,
	          LaBr4_rt * cos(LaBr4_theta) + LaBr4_dz, LaBr4_AngleX,
	          LaBr4_AngleY, 0.);
	LaBr4_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe6 (Germanium2_TUD)
	 *********************************/

	G4double Germanium2_TUD_rt = 90. * mm;
	G4double Germanium2_TUD_dy = 0. * mm;
	G4double Germanium2_TUD_dz = 3.5 * mm;
	G4double Germanium2_TUD_phi = 90. * deg;
	G4double Germanium2_TUD_theta =
	    93. * deg; // Detector was visibly tilted in the experiment

	G4double Germanium2_TUD_AngleX = 93. * deg;
	G4double Germanium2_TUD_AngleY = 180. * deg;

	Germanium2_TUD *Germanium2_TUD_Instance =
	    new Germanium2_TUD("Germanium2_TUD");
	G4LogicalVolume *Germanium2_TUD_TUD_Logical =
	    Germanium2_TUD_Instance->Get_Logical();

	G4RotationMatrix *rotateGermanium2_TUD = new G4RotationMatrix();
	rotateGermanium2_TUD->rotateX(Germanium2_TUD_AngleX);
	rotateGermanium2_TUD->rotateY(Germanium2_TUD_AngleY);

	Germanium2_TUD_rt =
	    Germanium2_TUD_rt + Germanium2_TUD_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateGermanium2_TUD,
	    G4ThreeVector(Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	                      cos(Germanium2_TUD_phi),
	                  Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	                          sin(Germanium2_TUD_phi) +
	                      Germanium2_TUD_dy,
	                  Target2_To_Target +
	                      Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	                      Germanium2_TUD_dz),
	    Germanium2_TUD_TUD_Logical, "Germanium2_TUD", world_log, false, 0);

	// Germanium2_TUD Wrapping

	G4double Germanium2_TUD_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double Germanium2_TUD_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *Germanium2_TUD_Wrapping_Solid = new G4Tubs(
	    "Germanium2_TUD_Wrapping_Solid", Germanium2_TUD_Instance->Get_Radius(),
	    Germanium2_TUD_Instance->Get_Radius() +
	        Germanium2_TUD_Wrapping_Thickness,
	    Germanium2_TUD_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Germanium2_TUD_Wrapping_Logical =
	    new G4LogicalVolume(Germanium2_TUD_Wrapping_Solid, Pb,
	                        "Germanium2_TUD_Wrapping_Logical", 0, 0, 0);

	Germanium2_TUD_Wrapping_Logical->SetVisAttributes(
	    new G4VisAttributes(grey));

	Germanium2_TUD_rt = Germanium2_TUD_rt +
	                    Germanium2_TUD_Wrapping_Length * 0.5 -
	                    Germanium2_TUD_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateGermanium2_TUD,
	    G4ThreeVector(Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	                      cos(Germanium2_TUD_phi),
	                  Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	                          sin(Germanium2_TUD_phi) +
	                      Germanium2_TUD_dy,
	                  Target2_To_Target +
	                      Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	                      Germanium2_TUD_dz),
	    Germanium2_TUD_Wrapping_Logical, "Germanium2_TUD_Wrapping", world_log,
	    false, 0);
	Germanium2_TUD_rt =
	    Germanium2_TUD_rt - Germanium2_TUD_Wrapping_Length * 0.5;

	// Germanium2_TUD Filters

	// cuthin50
	Germanium2_TUD_rt -= cuthin50->Thickness * 0.5;
	cuthin50->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= cuthin50->Thickness * 0.5;

	// cuthin50
	Germanium2_TUD_rt -= cuthin50->Thickness * 0.5;
	cuthin50->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= cuthin50->Thickness * 0.5;

	// pbmedium50
	Germanium2_TUD_rt -= pbmedium50->Thickness * 0.5;
	pbmedium50->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= pbmedium50->Thickness * 0.5;

	// pbmedium
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;

	// pbmedium
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;

	// pbmedium
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) * cos(Germanium2_TUD_phi),
	    Germanium2_TUD_rt * sin(Germanium2_TUD_theta) *
	            sin(Germanium2_TUD_phi) +
	        Germanium2_TUD_dy,
	    Target2_To_Target + Germanium2_TUD_rt * cos(Germanium2_TUD_theta) +
	        Germanium2_TUD_dz,
	    Germanium2_TUD_AngleX, Germanium2_TUD_AngleY, 0.);
	Germanium2_TUD_rt -= pbmedium->Thickness * 0.5;

	/************************* HPGe7 (HPGe1_55)
	 *********************************/

	G4double HPGe1_55_rt = 47. * mm;
	G4double HPGe1_55_dy = 18. * mm;
	G4double HPGe1_55_dz = 4. * mm;
	G4double HPGe1_55_phi = 0. * deg;
	G4double HPGe1_55_theta = 90. * deg;

	G4double HPGe1_55_AngleX = 180. * deg;
	G4double HPGe1_55_AngleY = 90. * deg;

	HPGe1_55 *HPGe1_55_Instance = new HPGe1_55("HPGe1_55");
	G4LogicalVolume *HPGe1_55_TUD_Logical = HPGe1_55_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe1_55 = new G4RotationMatrix();
	rotateHPGe1_55->rotateX(HPGe1_55_AngleX);
	rotateHPGe1_55->rotateY(HPGe1_55_AngleY);

	HPGe1_55_rt = HPGe1_55_rt + HPGe1_55_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) +
	                      HPGe1_55_dy,
	                  Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) +
	                      HPGe1_55_dz),
	    HPGe1_55_TUD_Logical, "HPGe1_55", world_log, false, 0);

	// HPGe1_55 Wrapping

	G4double HPGe1_55_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe1_55_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe1_55_Wrapping_Solid = new G4Tubs(
	    "HPGe1_55_Wrapping_Solid", HPGe1_55_Instance->Get_Radius(),
	    HPGe1_55_Instance->Get_Radius() + HPGe1_55_Wrapping_Thickness,
	    HPGe1_55_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe1_55_Wrapping_Logical = new G4LogicalVolume(
	    HPGe1_55_Wrapping_Solid, Pb, "HPGe1_55_Wrapping_Logical", 0, 0, 0);

	HPGe1_55_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe1_55_rt = HPGe1_55_rt + HPGe1_55_Wrapping_Length * 0.5 -
	              HPGe1_55_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe1_55,
	    G4ThreeVector(HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	                  HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) +
	                      HPGe1_55_dy,
	                  Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) +
	                      HPGe1_55_dz),
	    HPGe1_55_Wrapping_Logical, "HPGe1_55_Wrapping", world_log, false, 0);
	HPGe1_55_rt = HPGe1_55_rt - HPGe1_55_Wrapping_Length * 0.5;

	// HPGe1_55 Filters

	HPGe1_55_rt -= fcr->Thickness * 0.5;
	fcr->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= fcr->Thickness * 0.5;

	HPGe1_55_rt -= cumedium->Thickness * 0.5;
	cumedium->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= cumedium->Thickness * 0.5;

	HPGe1_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= pbmedium->Thickness * 0.5;

	HPGe1_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= pbmedium->Thickness * 0.5;

	HPGe1_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= pbmedium->Thickness * 0.5;

	HPGe1_55_rt +=
	    fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= fcw->FilterCase_Length * 0.5;

	HPGe1_55_rt += fcbo->Thickness * 0.5;
	fcbo->Put(
	    HPGe1_55_rt * sin(HPGe1_55_theta) * cos(HPGe1_55_phi),
	    HPGe1_55_rt * sin(HPGe1_55_theta) * sin(HPGe1_55_phi) + HPGe1_55_dy,
	    Target2_To_Target + HPGe1_55_rt * cos(HPGe1_55_theta) + HPGe1_55_dz,
	    HPGe1_55_AngleX, HPGe1_55_AngleY, 0.);
	HPGe1_55_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe8 (HPGe2_55)
	 * ********************************/

	G4double HPGe2_55_rt = 57. * mm;
	G4double HPGe2_55_dy = 16. * mm;
	G4double HPGe2_55_dz = 3. * mm;
	G4double HPGe2_55_phi = 180. * deg;
	G4double HPGe2_55_theta = 90. * deg;

	G4double HPGe2_55_AngleX = 180. * deg;
	G4double HPGe2_55_AngleY = 270. * deg;

	HPGe2_55 *HPGe2_55_Instance = new HPGe2_55("HPGe2_55");
	G4LogicalVolume *HPGe2_55_TUD_Logical = HPGe2_55_Instance->Get_Logical();

	G4RotationMatrix *rotateHPGe2_55 = new G4RotationMatrix();
	rotateHPGe2_55->rotateX(HPGe2_55_AngleX);
	rotateHPGe2_55->rotateY(HPGe2_55_AngleY);

	HPGe2_55_rt = HPGe2_55_rt + HPGe2_55_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe2_55,
	    G4ThreeVector(HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	                  HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) +
	                      HPGe2_55_dy,
	                  Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) +
	                      HPGe2_55_dz),
	    HPGe2_55_TUD_Logical, "HPGe2_55", world_log, false, 0);

	// HPGe2_55 Wrapping

	G4double HPGe2_55_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double HPGe2_55_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *HPGe2_55_Wrapping_Solid = new G4Tubs(
	    "HPGe2_55_Wrapping_Solid", HPGe2_55_Instance->Get_Radius(),
	    HPGe2_55_Instance->Get_Radius() + HPGe2_55_Wrapping_Thickness,
	    HPGe2_55_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *HPGe2_55_Wrapping_Logical = new G4LogicalVolume(
	    HPGe2_55_Wrapping_Solid, Pb, "HPGe2_55_Wrapping_Logical", 0, 0, 0);

	HPGe2_55_Wrapping_Logical->SetVisAttributes(new G4VisAttributes(grey));

	HPGe2_55_rt = HPGe2_55_rt + HPGe2_55_Wrapping_Length * 0.5 -
	              HPGe2_55_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotateHPGe2_55,
	    G4ThreeVector(HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	                  HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) +
	                      HPGe2_55_dy,
	                  Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) +
	                      HPGe2_55_dz),
	    HPGe2_55_Wrapping_Logical, "HPGe2_55_Wrapping", world_log, false, 0);
	HPGe2_55_rt = HPGe2_55_rt - HPGe2_55_Wrapping_Length * 0.5;

	// HPGe2_55 Filters

	HPGe2_55_rt -= fcr->Thickness * 0.5;
	fcr->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= fcr->Thickness * 0.5;

	HPGe2_55_rt -= cumedium->Thickness * 0.5;
	cumedium->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= cumedium->Thickness * 0.5;

	HPGe2_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= pbmedium->Thickness * 0.5;

	HPGe2_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= pbmedium->Thickness * 0.5;

	HPGe2_55_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= pbmedium->Thickness * 0.5;

	HPGe2_55_rt +=
	    fcw->FilterCase_Length * 0.5 - fcbo->FilterCaseBottom_Thickness;
	fcw->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= fcw->FilterCase_Length * 0.5;

	HPGe2_55_rt += fcbo->Thickness * 0.5;
	fcbo->Put(
	    HPGe2_55_rt * sin(HPGe2_55_theta) * cos(HPGe2_55_phi),
	    HPGe2_55_rt * sin(HPGe2_55_theta) * sin(HPGe2_55_phi) + HPGe2_55_dy,
	    Target2_To_Target + HPGe2_55_rt * cos(HPGe2_55_theta) + HPGe2_55_dz,
	    HPGe2_55_AngleX, HPGe2_55_AngleY, 0.);
	HPGe2_55_rt -= fcbo->Thickness * 0.5;

	/************************* HPGe9 (Polarimeter_TUD)
	 *********************************/

	G4double Polarimeter_TUD_rt = 99. * mm;
	G4double Polarimeter_TUD_dy = 0. * mm;
	G4double Polarimeter_TUD_dz = 21. * mm;
	G4double Polarimeter_TUD_phi = 270. * deg;
	G4double Polarimeter_TUD_theta = 90. * deg;

	G4double Polarimeter_TUD_AngleX = 270. * deg;
	G4double Polarimeter_TUD_AngleY = 180. * deg;

	Polarimeter_TUD *Polarimeter_TUD_Instance =
	    new Polarimeter_TUD("Polarimeter_TUD");
	G4LogicalVolume *Polarimeter_TUD_TUD_Logical =
	    Polarimeter_TUD_Instance->Get_Logical();

	G4RotationMatrix *rotatePolarimeter_TUD = new G4RotationMatrix();
	rotatePolarimeter_TUD->rotateX(Polarimeter_TUD_AngleX);
	rotatePolarimeter_TUD->rotateY(Polarimeter_TUD_AngleY);

	Polarimeter_TUD_rt =
	    Polarimeter_TUD_rt + Polarimeter_TUD_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotatePolarimeter_TUD,
	    G4ThreeVector(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      cos(Polarimeter_TUD_phi),
	                  Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                          sin(Polarimeter_TUD_phi) +
	                      Polarimeter_TUD_dy,
	                  Target2_To_Target +
	                      Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                      Polarimeter_TUD_dz),
	    Polarimeter_TUD_TUD_Logical, "Polarimeter_TUD", world_log, false, 0);

	// Polarimeter_TUD Wrapping

	G4double Polarimeter_TUD_Wrapping_Thickness = 2 * 0.050 * inch; // Measured
	G4double Polarimeter_TUD_Wrapping_Length = 140. * mm;           // Measured

	G4Tubs *Polarimeter_TUD_Wrapping_Solid =
	    new G4Tubs("Polarimeter_TUD_Wrapping_Solid",
	               Polarimeter_TUD_Instance->Get_Radius(),
	               Polarimeter_TUD_Instance->Get_Radius() +
	                   Polarimeter_TUD_Wrapping_Thickness,
	               Polarimeter_TUD_Wrapping_Length * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Polarimeter_TUD_Wrapping_Logical =
	    new G4LogicalVolume(Polarimeter_TUD_Wrapping_Solid, Pb,
	                        "Polarimeter_TUD_Wrapping_Logical", 0, 0, 0);

	Polarimeter_TUD_Wrapping_Logical->SetVisAttributes(
	    new G4VisAttributes(grey));

	Polarimeter_TUD_rt = Polarimeter_TUD_rt +
	                     Polarimeter_TUD_Wrapping_Length * 0.5 -
	                     Polarimeter_TUD_Instance->Get_Length() * 0.5;

	new G4PVPlacement(
	    rotatePolarimeter_TUD,
	    G4ThreeVector(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      cos(Polarimeter_TUD_phi),
	                  Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                          sin(Polarimeter_TUD_phi) +
	                      Polarimeter_TUD_dy,
	                  Target2_To_Target +
	                      Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                      Polarimeter_TUD_dz),
	    Polarimeter_TUD_Wrapping_Logical, "Polarimeter_TUD_Wrapping", world_log,
	    false, 0);
	Polarimeter_TUD_rt =
	    Polarimeter_TUD_rt - Polarimeter_TUD_Wrapping_Length * 0.5;

	// Polarimeter_TUD Filters

	// cuthin50
	Polarimeter_TUD_rt -= cuthin50->Thickness * 0.5;
	cuthin50->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                  cos(Polarimeter_TUD_phi),
	              Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      sin(Polarimeter_TUD_phi) +
	                  Polarimeter_TUD_dy,
	              Target2_To_Target +
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                  Polarimeter_TUD_dz,
	              Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= cuthin50->Thickness * 0.5;

	// cuthin50
	Polarimeter_TUD_rt -= cuthin50->Thickness * 0.5;
	cuthin50->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                  cos(Polarimeter_TUD_phi),
	              Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      sin(Polarimeter_TUD_phi) +
	                  Polarimeter_TUD_dy,
	              Target2_To_Target +
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                  Polarimeter_TUD_dz,
	              Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= cuthin50->Thickness * 0.5;

	// pbmedium50
	Polarimeter_TUD_rt -= pbmedium50->Thickness * 0.5;
	pbmedium50->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                    cos(Polarimeter_TUD_phi),
	                Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                        sin(Polarimeter_TUD_phi) +
	                    Polarimeter_TUD_dy,
	                Target2_To_Target +
	                    Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                    Polarimeter_TUD_dz,
	                Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= pbmedium50->Thickness * 0.5;

	// pbmedium
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                  cos(Polarimeter_TUD_phi),
	              Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      sin(Polarimeter_TUD_phi) +
	                  Polarimeter_TUD_dy,
	              Target2_To_Target +
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                  Polarimeter_TUD_dz,
	              Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;

	// pbmedium
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                  cos(Polarimeter_TUD_phi),
	              Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      sin(Polarimeter_TUD_phi) +
	                  Polarimeter_TUD_dy,
	              Target2_To_Target +
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                  Polarimeter_TUD_dz,
	              Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;

	// pbmedium
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;
	pbmedium->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                  cos(Polarimeter_TUD_phi),
	              Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                      sin(Polarimeter_TUD_phi) +
	                  Polarimeter_TUD_dy,
	              Target2_To_Target +
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                  Polarimeter_TUD_dz,
	              Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= pbmedium->Thickness * 0.5;

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

	// HPGe detectors in g3
	ParticleSD *HPGe1SD = new ParticleSD("HPGe1", "HPGe1");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe1SD);
	HPGe1SD->SetDetectorID(1);
	SetSensitiveDetector("HPGe1", HPGe1SD, true);

	SecondarySD *HPGe2SD = new SecondarySD("HPGe2", "HPGe2");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2SD);
	HPGe2SD->SetDetectorID(2);
	SetSensitiveDetector("HPGe2", HPGe2SD, true);

	SecondarySD *HPGe3SD = new SecondarySD("HPGe3", "HPGe3");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe3SD);
	HPGe3SD->SetDetectorID(3);
	SetSensitiveDetector("HPGe3", HPGe3SD, true);

	ParticleSD *HPGe4SD = new ParticleSD("HPGe4", "HPGe4");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe4SD);
	HPGe4SD->SetDetectorID(4);
	SetSensitiveDetector("HPGe4", HPGe4SD, true);

	// LaBr detectors in g3
	ParticleSD *LaBr1SD = new ParticleSD("LaBr1", "LaBr1");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr1SD);
	LaBr1SD->SetDetectorID(11);
	SetSensitiveDetector("LaBr1", LaBr1SD, true);

	ParticleSD *LaBr2SD = new ParticleSD("LaBr2", "LaBr2");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr2SD);
	LaBr2SD->SetDetectorID(22);
	SetSensitiveDetector("LaBr2", LaBr2SD, true);

	ParticleSD *LaBr3SD = new ParticleSD("LaBr3", "LaBr3");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr3SD);
	LaBr3SD->SetDetectorID(33);
	SetSensitiveDetector("LaBr3", LaBr3SD, true);

	ParticleSD *LaBr4SD = new ParticleSD("LaBr4", "LaBr4");
	G4SDManager::GetSDMpointer()->AddNewDetector(LaBr4SD);
	LaBr4SD->SetDetectorID(44);
	SetSensitiveDetector("LaBr4", LaBr4SD, true);

	// HPGe detectors in second setup
	ParticleSD *Germanium2_TUDSD =
	    new ParticleSD("Germanium2_TUD", "Germanium2_TUD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Germanium2_TUDSD);
	Germanium2_TUDSD->SetDetectorID(6);
	SetSensitiveDetector("Germanium2_TUD", Germanium2_TUDSD, true);

	ParticleSD *HPGe1_55SD = new ParticleSD("HPGe1_55", "HPGe1_55");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe1_55SD);
	HPGe1_55SD->SetDetectorID(7);
	SetSensitiveDetector("HPGe1_55", HPGe1_55SD, true);

	ParticleSD *HPGe2_55SD = new ParticleSD("HPGe2_55", "HPGe2_55");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2_55SD);
	HPGe2_55SD->SetDetectorID(8);
	SetSensitiveDetector("HPGe2_55", HPGe2_55SD, true);

	ParticleSD *Polarimeter_TUDSD =
	    new ParticleSD("Polarimeter_TUD", "Polarimeter_TUD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Polarimeter_TUDSD);
	Polarimeter_TUDSD->SetDetectorID(9);
	SetSensitiveDetector("Polarimeter_TUD", Polarimeter_TUDSD, true);

	// ZeroDegree detector
	ParticleSD *ZeroDegreeSD = new ParticleSD("ZeroDegree", "ZeroDegree");
	G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
	ZeroDegreeSD->SetDetectorID(5);
	SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);
}
