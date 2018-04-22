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
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "Wheel.hh"

// Sensitive Detectors
//#include "EnergyDepositionSD.hh"
//#include "G4SDManager.hh"
//#include "ParticleSD.hh"
//#include "SecondarySD.hh"

// Units
//#include "G4PhysicalConstants.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4UnitsTable.hh"

// Detectors
//#include "Germanium1_TUD.hh"
//#include "Germanium2_TUD.hh"
//#include "HPGe1.hh"
//#include "HPGe1_55.hh"
//#include "HPGe2.hh"
//#include "HPGe2_55.hh"
//#include "HPGe3.hh"
//#include "HPGe4.hh"
//#include "LaBr_Cologne.hh"
//#include "LaBr_TUD.hh"
//#include "Polarimeter_TUD.hh"
//#include "ZeroDegree.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

	/*
	 * Fast-forward to specific parts of the geometry by searching for
	 * WORLD (world volume)
	 * WHEEL (g3 wheel)
	 */

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

	// G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");
	// G4Material* Au = nist->FindOrBuildMaterial("G4_Au");
	//G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	//	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	//G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	//G4Material *Plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	//G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
	//G4Material *Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
	//G4Material *Scintillator_Plastic =
	//    nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	//G4Material *pump_vacuum = materials->Get_Pump_Vacuum();

	/***************** WORLD *****************/

	G4double World_x = 1000. * mm;
	G4double World_y = 1000. * mm;
	G4double World_z = 1000. * mm;

	G4Box *World_dim =
	    new G4Box("World_Solid", World_x * 0.5, World_y * 0.5, World_z * 0.5);

	G4LogicalVolume *World_Logical =
	    new G4LogicalVolume(World_dim, air, "World_Logical", 0, 0, 0);

	World_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *World_Physical =
	    new G4PVPlacement(0, G4ThreeVector(), World_Logical, "World", 0, false, 0);

	/***************** WHEEL *****************/

	Wheel wheel;
	G4LogicalVolume *Wheel_Logical = wheel.Get_Logical();

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), Wheel_Logical, "Wheel", World_Logical, false, 0, false);

	return World_Physical;
}

void DetectorConstruction::ConstructSDandField() {

}
