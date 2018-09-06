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

// Geometry of 40Ar experiment in 2017 by group of W. Tornow
// It is based on the geometry of Runs 726-743 of the 82Se - 82Kr experiment
// from fall/winter 2016. A lot of unnecessary things have been removed.

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
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
#include "HPGe_60_TUNL_30986.hh"
#include "HPGe_60_TUNL_21033.hh"
#include "HPGe_60_TUNL_40663.hh"
#include "HPGe_60_TUNL_31061.hh"
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
	//		G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
	G4Material *Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
	G4Material *Scintillator_Plastic =
	    nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

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
	// WALL7: Lead + concrete shielding of 2nd setup (82Se/82Kr experiment runs
	// 710 - 760)
	//

	// Load Bricks
	NormBrick *nb = new NormBrick(world_log);
	NormBrickWithHole *nbh = new NormBrickWithHole(world_log);
	ShortNormBrick *snb = new ShortNormBrick(world_log);
	// ShortBrickWithHole* sbh = new ShortBrickWithHole(world_log);
	HalfShortBrickWithHole *hsbh = new HalfShortBrickWithHole(world_log);
	ConcreteBrick *cb = new ConcreteBrick(world_log);
	FlatConcreteBrick *fcb = new FlatConcreteBrick(world_log);
	BridgeBrick *bb = new BridgeBrick(world_log);
	ThinNormBrick *tb = new ThinNormBrick(world_log);
	// ShortThinNormBrick *stb = new ShortThinNormBrick(world_log);
	FlatFlatThinNormBrick *fftb = new FlatFlatThinNormBrick(world_log);
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

	for (int i = 0; i < 3; i++) {
		nb->Put(-nb->L / 2., -BeamTube_Outer_Radius + nb->S * 3.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2., -BeamTube_Outer_Radius + nb->S * 3.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2., -BeamTube_Outer_Radius + nb->S * 2.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2., -BeamTube_Outer_Radius + nb->S * 2.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2. - BeamTube_Outer_Radius,
		        -BeamTube_Outer_Radius + nb->S * 1.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2. + BeamTube_Outer_Radius,
		        -BeamTube_Outer_Radius + nb->S * 1.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2. - BeamTube_Outer_Radius,
		        -BeamTube_Outer_Radius + nb->S * 0.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2. + BeamTube_Outer_Radius,
		        -BeamTube_Outer_Radius + nb->S * 0.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2., -BeamTube_Outer_Radius - nb->S * 0.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2., -BeamTube_Outer_Radius - nb->S * 0.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2., -BeamTube_Outer_Radius - nb->S * 1.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2., -BeamTube_Outer_Radius - nb->S * 1.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(-nb->L / 2., -BeamTube_Outer_Radius - nb->S * 2.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
		nb->Put(nb->L / 2., -BeamTube_Outer_Radius - nb->S * 2.5,
		        -Wall5_To_Target - nb->M * (i + 0.5), 0., 90. * deg, 0.);
	}

	// Fill the gap between beam pipe and wall by stuffing in flat stripes of
	// lead

	fftb->Put(0., BeamTube_Outer_Radius + fftb->S * 0.5,
	          -Wall5_To_Target - nb->M * 3. + fftb->L * 0.5);
	fftb->Put(0., BeamTube_Outer_Radius + fftb->S * 1.5,
	          -Wall5_To_Target - nb->M * 3. + fftb->L * 0.5);
	fftb->Put(0., BeamTube_Outer_Radius + fftb->S * 2.5,
	          -Wall5_To_Target - nb->M * 3. + fftb->L * 0.5);

	// Brick with hole in front of wall

	hsbh->Put(0., hsbh->M * 0.5, -Wall5_To_Target - nb->M * 3. - hsbh->M * 0.5,
	          -90. * deg, 90. * deg, 0.);
	hsbh->Put(0., -hsbh->M * 0.5, -Wall5_To_Target - nb->M * 3. - hsbh->M * 0.5,
	          90. * deg, 90. * deg, 0.);

	// Concrete blocks below Wall3

	cb->Put(0., -BeamTube_Outer_Radius - nb->S * 3. - cb->M * 0.5,
	        -Wall5_To_Target - nb->M * 3. + cb->M, 0., 90. * deg, 0.);
	fcb->Put(0., -BeamTube_Outer_Radius - nb->S * 3. - cb->M * 0.5,
	         -Wall5_To_Target - nb->M * 3. + fcb->S * 0.5, 0., 90. * deg,
	         90. * deg);

	// Aluminium plate below Wall3

	G4double AlPlate_X = 460. * mm; // Measured
	G4double AlPlate_Y = 20. * mm;  // Measured
	G4double AlPlate_Z = 405. * mm; // Measured

	G4Box *AlPlate_Solid = new G4Box("AlPlate_Solid", AlPlate_X * 0.5,
	                                 AlPlate_Y * 0.5, AlPlate_Z * 0.5);
	G4LogicalVolume *AlPlate_Logical =
	    new G4LogicalVolume(AlPlate_Solid, Al, "AlPlate_Logical", 0, 0, 0);

	AlPlate_Logical->SetVisAttributes(new G4VisAttributes(grey));

	new G4PVPlacement(
	    0, G4ThreeVector(0., -BeamTube_Outer_Radius - nb->S * 3. - cb->M -
	                             AlPlate_Y * 0.5,
	                     -Wall5_To_Target - nb->M * 3 + AlPlate_Z * 0.5),
	    AlPlate_Logical, "AlPlate", world_log, false, 0);

	// Concrete Blocks below aluminium plate

	fcb->Put(0., -BeamTube_Outer_Radius - nb->S * 3. - cb->M - AlPlate_Y -
	                 fcb->M * 0.5,
	         -Wall5_To_Target - nb->M * 3 + fcb->L * 0.5, 0., 0., 90. * deg);
	cb->Put(-fcb->S * 0.5 - cb->M * 0.5, -BeamTube_Outer_Radius - nb->S * 3. -
	                                         cb->M - AlPlate_Y - cb->M * 0.5,
	        -Wall5_To_Target - nb->M * 3. + cb->L * 0.5);
	cb->Put(fcb->S * 0.5 + cb->M * 0.5, -BeamTube_Outer_Radius - nb->S * 3. -
	                                        cb->M - AlPlate_Y - cb->M * 0.5,
	        -Wall5_To_Target - nb->M * 3. + cb->L * 0.5);

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

	cb->Put(PipeHolderTube_OuterRadius + cb->M * 0.5 - 7. * mm, -cb->M * 0.5,
	        Wall7_To_Target + nb->S * 3. + cb->L * 0.5, 0., 10. * deg, 0.);
	cb->Put(-PipeHolderTube_OuterRadius - cb->M * 0.5 + 7. * mm, -cb->M * 0.5,
	        Wall7_To_Target + nb->S * 3. + cb->L * 0.5, 0., -10. * deg, 0.);
	// Concrete blocks were slightly rotated in the setup. The 10.*deg are
	// estimated. The 7.*mm are chosen such that the concrete blocks barely
	// touch the PipeHolderTube.

	bb->Put(0., bb->M * 0.5, Wall7_To_Target + nb->S * 3. +
	                             PipeHolderTubeUpStream_Length + bb->S * 0.5,
	        -90. * deg, 90. * deg, 0.);
	bb->Put(0., bb->M * 0.5, Wall7_To_Target + nb->S * 3. +
	                             PipeHolderTubeUpStream_Length + bb->S * 1.5,
	        -90. * deg, 90. * deg, 0.);

	nb->Put(0., bb->M + nb->S * 0.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->M * 0.5,
	        0., 90. * deg, 0.);
	nb->Put(0., bb->M + nb->S * 1.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->M * 0.5,
	        0., 90. * deg, 0.);
	nb->Put(0., bb->M + nb->S * 2.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->M * 0.5,
	        0., 90. * deg, 0.);

	nb->Put(bb->L * 0.5 + nb->M * 0.5, nb->S * 0.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->L * 0.5);
	tb->Put(bb->L * 0.5 + tb->M * 0.5, nb->S * 1.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - tb->L * 0.5);

	nb->Put(-bb->L * 0.5 - nb->M * 0.5, nb->S * 0.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->L * 0.5);
	nb->Put(-bb->L * 0.5 - nb->M * 0.5, nb->S * 1.5,
	        Wall7_To_Target + nb->S * 3. + PipeHolderTubeUpStream_Length +
	            bb->S * 2. - nb->L * 0.5);

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

	/************* Lead and Cadmium wrapping around beam pipe ************/

	G4double LeadFoil_Thickness = 1.8 * mm; // Measured
	G4double LeadFoil_Width = 140. * mm;    // Measured

	G4Tubs *LeadWrap_Solid =
	    new G4Tubs("LeadWrap_Solid", BeamTube_Outer_Radius,
	               BeamTube_Outer_Radius + LeadFoil_Thickness,
	               LeadFoil_Width * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *LeadWrap_Logical =
	    new G4LogicalVolume(LeadWrap_Solid, Pb, "LeadWrap_Logical", 0, 0, 0);

	LeadWrap_Logical->SetVisAttributes(new G4VisAttributes(grey));

	G4Tubs *DoubleLeadWrap_Solid =
	    new G4Tubs("DoubleLeadWrap_Solid", BeamTube_Outer_Radius,
	               BeamTube_Outer_Radius + 2 * LeadFoil_Thickness,
	               LeadFoil_Width * 0.25, 0. * deg, 360. * deg);
	G4LogicalVolume *DoubleLeadWrap_Logical = new G4LogicalVolume(
	    DoubleLeadWrap_Solid, Pb, "DoubleLeadWrap_Logical", 0, 0, 0);

	DoubleLeadWrap_Logical->SetVisAttributes(new G4VisAttributes(grey));

	G4Tubs *QuadrupleLeadWrap_Solid =
	    new G4Tubs("QuadrupleLeadWrap_Solid", BeamTube_Outer_Radius,
	               BeamTube_Outer_Radius + 4 * LeadFoil_Thickness,
	               LeadFoil_Width * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *QuadrupleLeadWrap_Logical = new G4LogicalVolume(
	    QuadrupleLeadWrap_Solid, Pb, "QuadrupleLeadWrap_Logical", 0, 0, 0);

	QuadrupleLeadWrap_Logical->SetVisAttributes(new G4VisAttributes(grey));

	// Wrapping between Wall4 and Wall5

	new G4PVPlacement(0,
	                  G4ThreeVector(0., 0., -Wall5_To_Target - nb->M * 3. -
	                                            hsbh->S - LeadFoil_Width * 0.5),
	                  LeadWrap_Logical, "LeadWrap", world_log, false, 0);

	// Wrapping upstream of g3 setup

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., -Wall5_To_Target + LeadFoil_Width * 0.5),
	    LeadWrap_Logical, "LeadWrap", world_log, false, 0);

	/**************** g3 Target/Source ******************/

	// No target at first position

	/**************** Second Target/Source ***************/

	// No target at second position

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

	Pb_45mm_23mm *pb45_23 = new Pb_45mm_23mm(world_log);
	Pb_45mm_17_6mm *pb45_176 = new Pb_45mm_17_6mm(world_log);
	Pb_45mm_2_3in *pbverythick = new Pb_45mm_2_3in(world_log);
	Pb_45mm_almost_2_3in *pbalmostverythick =
	    new Pb_45mm_almost_2_3in(world_log);
	// Pb_70mm_5mm* pb70 = new Pb_70mm_5mm(world_log);

	Cu_45mm_1mm *cuthin = new Cu_45mm_1mm(world_log);
	// Cu_73mm_1_20in* cu73 = new Cu_73mm_1_20in(world_log);

	/************************* ZERODEGREE ***************************/

	G4double ZeroDegree_X =
	    0. * mm; // X = 0, if beam profile is measured, otherwise, X < 0
	G4double ZeroDegree_Y = 20. * mm;                       // Measured
	G4double ZeroDegree_Z = Target2_To_Target + 36. * inch; // Measured
	G4double ZeroDegree_Theta = 0. * deg;
	// G4double ZeroDegree_Phi = 0.*deg; // Should always be zero

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

	G4double HPGe1_rt = 137. * mm;
	G4double HPGe1_dy = 0. * mm;
	G4double HPGe1_dz = 0. * mm;
	G4double HPGe1_phi = 180. * deg;
	G4double HPGe1_theta = 90. * deg;

	G4double HPGe1_AngleX = 180. * deg;
	G4double HPGe1_AngleY = 270. * deg;

	HPGe_60_TUNL_30986 *HPGe1_Instance = new HPGe_60_TUNL_30986("HPGe1");
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

	// HPGe1 Filters

	HPGe1_rt = HPGe1_rt - HPGe1_Instance->Get_Length() * 0.5;

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

	HPGe1_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	            HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	            HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX,
	            HPGe1_AngleY, 0.);
	HPGe1_rt -= cuthin->Thickness * 0.5;

	HPGe1_rt -= pbverythick->Thickness * 0.5;
	pbverythick->Put(HPGe1_rt * sin(HPGe1_theta) * cos(HPGe1_phi),
	                 HPGe1_rt * sin(HPGe1_theta) * sin(HPGe1_phi) + HPGe1_dy,
	                 HPGe1_rt * cos(HPGe1_theta) + HPGe1_dz, HPGe1_AngleX,
	                 HPGe1_AngleY, 0.);
	HPGe1_rt -= pbverythick->Thickness * 0.5;

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

	G4double HPGe2_rt = 128. * mm;
	G4double HPGe2_dy = 0. * mm;
	G4double HPGe2_dz = 0. * mm;
	G4double HPGe2_phi = 0. * deg;
	G4double HPGe2_theta = 90. * deg;

	HPGe_60_TUNL_21033 *HPGe2_Instance = new HPGe_60_TUNL_21033("HPGe2");
	G4LogicalVolume *HPGe2_Logical = HPGe2_Instance->Get_Logical();

	G4double HPGe2_AngleX = 0. * deg;
	G4double HPGe2_AngleY = 90. * deg;

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

	// HPGe2 Filters

	HPGe2_rt = HPGe2_rt - HPGe2_Instance->Get_Length() * 0.5;

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

	HPGe2_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	            HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	            HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX,
	            HPGe2_AngleY, 0.);
	HPGe2_rt -= cuthin->Thickness * 0.5;

	HPGe2_rt -= pbalmostverythick->Thickness * 0.5;
	pbalmostverythick->Put(
	    HPGe2_rt * sin(HPGe2_theta) * cos(HPGe2_phi),
	    HPGe2_rt * sin(HPGe2_theta) * sin(HPGe2_phi) + HPGe2_dy,
	    HPGe2_rt * cos(HPGe2_theta) + HPGe2_dz, HPGe2_AngleX, HPGe2_AngleY, 0.);
	HPGe2_rt -= pbalmostverythick->Thickness * 0.5;

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

	/************************* Polarimeter_TUD
	 * ********************************/

	G4double Polarimeter_TUD_rt = 138. * mm;
	G4double Polarimeter_TUD_dy = 0. * mm;
	G4double Polarimeter_TUD_dz = 0. * mm;
	G4double Polarimeter_TUD_phi = 270. * deg;
	G4double Polarimeter_TUD_theta = 90. * deg;

	G4double Polarimeter_TUD_AngleX = 270 * deg;
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
	                  Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                      Polarimeter_TUD_dz),
	    Polarimeter_TUD_TUD_Logical, "Polarimeter_TUD", world_log, false, 0);

	// Polarimeter_TUD Filters

	Polarimeter_TUD_rt =
	    Polarimeter_TUD_rt - Polarimeter_TUD_Instance->Get_Length() * 0.5;

	Polarimeter_TUD_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                cos(Polarimeter_TUD_phi),
	            Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                    sin(Polarimeter_TUD_phi) +
	                Polarimeter_TUD_dy,
	            Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                Polarimeter_TUD_dz,
	            Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= cuthin->Thickness * 0.5;

	Polarimeter_TUD_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                cos(Polarimeter_TUD_phi),
	            Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                    sin(Polarimeter_TUD_phi) +
	                Polarimeter_TUD_dy,
	            Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                Polarimeter_TUD_dz,
	            Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= cuthin->Thickness * 0.5;

	Polarimeter_TUD_rt -= pb45_23->Thickness * 0.5;
	pb45_23->Put(Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                 cos(Polarimeter_TUD_phi),
	             Polarimeter_TUD_rt * sin(Polarimeter_TUD_theta) *
	                     sin(Polarimeter_TUD_phi) +
	                 Polarimeter_TUD_dy,
	             Polarimeter_TUD_rt * cos(Polarimeter_TUD_theta) +
	                 Polarimeter_TUD_dz,
	             Polarimeter_TUD_AngleX, Polarimeter_TUD_AngleY, 0.);
	Polarimeter_TUD_rt -= pb45_23->Thickness * 0.5;

	/************************* HPGe4 ********************************/

	G4double HPGe4_rt = 103. * mm;
	G4double HPGe4_dy = 0. * mm;
	G4double HPGe4_dz = 3.5 * mm;
	G4double HPGe4_phi = 45. * deg;
	G4double HPGe4_theta = 135. * deg;

	G4double HPGe4_AngleX = 144.736 * deg;
	G4double HPGe4_AngleY = 150. * deg;

	HPGe_60_TUNL_31061 *HPGe4_Instance = new HPGe_60_TUNL_31061("HPGe4");
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
	    HPGe4_Logical, "HPGe4", world_log, false, 0);

	// HPGe4 Filters

	HPGe4_rt = HPGe4_rt - HPGe4_Instance->Get_Length() * 0.5;

	HPGe4_rt -= fcr->Thickness * 0.5;
	fcr->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	         HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	         HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX, HPGe4_AngleY,
	         0.);
	HPGe4_rt -= fcr->Thickness * 0.5;

	HPGe4_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	            HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	            HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	            HPGe4_AngleY, 0.);
	HPGe4_rt -= cuthin->Thickness * 0.5;

	HPGe4_rt -= cuthin->Thickness * 0.5;
	cuthin->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	            HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	            HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	            HPGe4_AngleY, 0.);
	HPGe4_rt -= cuthin->Thickness * 0.5;

	HPGe4_rt -= pb45_176->Thickness * 0.5;
	pb45_176->Put(HPGe4_rt * sin(HPGe4_theta) * cos(HPGe4_phi),
	              HPGe4_rt * sin(HPGe4_theta) * sin(HPGe4_phi) + HPGe4_dy,
	              HPGe4_rt * cos(HPGe4_theta) + HPGe4_dz, HPGe4_AngleX,
	              HPGe4_AngleY, 0.);
	HPGe4_rt -= pb45_176->Thickness * 0.5;

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

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

	// HPGe detectors in g3
	EnergyDepositionSD *HPGe1SD = new EnergyDepositionSD("HPGe1", "HPGe1");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe1SD);
	HPGe1SD->SetDetectorID(1);
	SetSensitiveDetector("HPGe1", HPGe1SD, true);

	EnergyDepositionSD *HPGe2SD = new EnergyDepositionSD("HPGe2", "HPGe2");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe2SD);
	HPGe2SD->SetDetectorID(2);
	SetSensitiveDetector("HPGe2", HPGe2SD, true);

	EnergyDepositionSD *HPGe4SD = new EnergyDepositionSD("HPGe4", "HPGe4");
	G4SDManager::GetSDMpointer()->AddNewDetector(HPGe4SD);
	HPGe4SD->SetDetectorID(4);
	SetSensitiveDetector("HPGe4", HPGe4SD, true);

	EnergyDepositionSD *Polarimeter_TUDSD =
	    new EnergyDepositionSD("Polarimeter_TUD", "Polarimeter_TUD");
	G4SDManager::GetSDMpointer()->AddNewDetector(Polarimeter_TUDSD);
	Polarimeter_TUDSD->SetDetectorID(9);
	SetSensitiveDetector("Polarimeter_TUD", Polarimeter_TUDSD, true);

	// ZeroDegree detector
	//	EnergyDepositionSD *ZeroDegreeSD =
	//	    new EnergyDepositionSD("ZeroDegree", "ZeroDegree");
	//	G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
	//	ZeroDegreeSD->SetDetectorID(5);
	//	SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);
}
