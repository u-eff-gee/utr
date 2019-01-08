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
// Valid from 20.11. 11:37 am - 28.11. 8:33 pm
// Run 726 - Run 743

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
#include "G4Sphere.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "G4SDManager.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

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

	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	/***************** World Volume *****************/

	G4double world_x = 4000. * mm;
	G4double world_y = 4000. * mm;
	G4double world_z = 9000. * mm;

	G4Box *world_dim =
	    new G4Box("world_dim", world_x * 0.5, world_y * 0.5, world_z * 0.5);

	G4LogicalVolume *world_log =
	    new G4LogicalVolume(world_dim, vacuum, "world_log", 0, 0, 0);

	//world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *world_phys =
	    new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	/***************** Source Volume *****************/

	G4double source_rmin = 0.*mm;
	G4double source_rmax = 1.*mm;

	G4Sphere *source_Solid = new G4Sphere("source_Solid", source_rmin, source_rmax, 0., twopi, 0., pi);

	G4LogicalVolume *source_Logical = new G4LogicalVolume(source_Solid, vacuum, "source_Logical");
	source_Logical->SetVisAttributes(yellow);

	new G4PVPlacement(0, G4ThreeVector(), source_Logical, "source", world_log, false, 0);

	/***************** Detection Volume *****************/

	G4double detector_rmin = 1000.*mm;
	G4double detector_rmax = 1100.*mm;

	G4Sphere *detector_Solid = new G4Sphere("detector_Solid", detector_rmin, detector_rmax, 0., twopi, 0., pi);

	G4LogicalVolume *detector_Logical = new G4LogicalVolume(detector_Solid, vacuum, "detector_Logical");
	detector_Logical->SetVisAttributes(red);
	
	new G4PVPlacement(0, G4ThreeVector(), detector_Logical, "detector", world_log, false, 0);
	

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

	// HPGe detectors in g3
	ParticleSD *detectorSD = new ParticleSD("Detector", "Detector");
	G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
	detectorSD->SetDetectorID(0);
	SetSensitiveDetector("detector_Logical", detectorSD, true);
}
