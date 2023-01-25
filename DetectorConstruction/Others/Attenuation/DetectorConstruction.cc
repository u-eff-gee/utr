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

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


// #include "G4UnitsTable.hh"
#include "utrConfig.h"
#include <iostream>
#include <array>


/*
 * Simple geometry to study the evolution of the photon flux when a particle beam traverses a target.
 * To get a snapshot of the kinetic energy distribution of the photon beam at a given penetration depth,
 * the target is segmented along the z-axis into n layers.
 *
 * The simulation uses the ParticleSD detector type, which records particle information at any time when a
 * particle propagates into a new target segment for the first time (or when it is created there).
 *
 * It is recommended to record the following information in the output file, to be able to filter out
 * backward-propagating particles or particles which come from outside the target:
 *
 * EVENT_EKIN
 * EVENT_MOMX
 * EVENT_MOMY
 * EVENT_MOMZ
 * EVENT_POSX
 * EVENT_POSY
 * EVENT_POSZ
 * EVENT_PARTICLE
 * EVENT_VOLUME
 */

const size_t n_target_layers = 1; // Determines the number of layers of the target.

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Target Properties *****************/

  const double detector_length = 1. * mm;
  const double distance = 5. * mm;
  const double target_length = 10. * mm;
  const double target_radius = 10. * mm;
  const G4String target_material_name = "G4_Pb";

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *target_material = nist->FindOrBuildMaterial(target_material_name);

  /***************** World Volume *****************/

  const double world_x = 4. * target_radius;
  const double world_y = 4. * target_radius;
  const double world_z = 4. * target_length + distance + detector_length;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, vacuum, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);

  /***************** Target *****************/

  auto *target_solid = new G4Tubs("target_solid", 0., target_radius, 0.5 * target_length, 0., twopi);
  auto *target_logical = new G4LogicalVolume(target_solid, target_material, "target_logical");
  target_logical->SetVisAttributes(new G4VisAttributes(G4Color::Yellow()));
  new G4PVPlacement(0, G4ThreeVector(0., 0., target_length/2), target_logical,  "Target", world_logical, false, 0);

  
  /******************** Detector ******************/

	G4Tubs *Detector_solid = new G4Tubs("Detector_solid", 0, target_radius, detector_length * 0.5, 0, twopi);
	G4LogicalVolume *Detector_logical = new G4LogicalVolume(Detector_solid, vacuum, "Detector_logical", 0, 0, 0);

	//Visualisierung (Farbe)
	Detector_logical->SetVisAttributes(new G4VisAttributes(G4Color::Blue()));
	new G4PVPlacement(0, G4ThreeVector(0, 0, target_length + distance + detector_length/2), Detector_logical, "Detector", world_logical, false, 0);
	

  return world_physical;
}

// Definiere das Detektorvolumen als Detektor/sensitives Volumen in Geant4
void DetectorConstruction::ConstructSDandField() {

	// Use ParticleSD instead of EnergyDepositionSD, as ParticleSD records the hits of each particle within a event individually regardless whether the particle actually deposited energy in the detector or not.
	// An EnergyDepositionSD however only records a single particle per event and only if it actually left some energy in the detector
	ParticleSD *DetectorSD = new ParticleSD("Detector_logical", "Detector_logical");
	G4SDManager::GetSDMpointer()->AddNewDetector(DetectorSD);
	DetectorSD->SetDetectorID(0);
	SetSensitiveDetector("Detector_logical", DetectorSD, true);
}
