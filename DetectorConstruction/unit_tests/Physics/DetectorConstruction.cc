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
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
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
  G4Material *target_material = nist->FindOrBuildMaterial("G4_Dy");
  G4Material *particle_detector_material = vacuum;
  G4Material *secondary_detector_material = vacuum;
  G4Material *energy_deposition_detector_material = vacuum;

  /***************** World Volume *****************/

  G4double world_r = 1000. * mm;

  G4Sphere *world_dim =
      new G4Sphere("world_dim", 0., world_r, 0., twopi, 0., pi);

  G4LogicalVolume *world_log =
      new G4LogicalVolume(world_dim, vacuum, "world_log", 0, 0, 0);

  // world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume *world_phys =
      new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

  /***************** Target *****************/

  G4double target_rmin = 0. * mm;
  G4double target_rmax = 10. * mm;
  G4double target_length = 100. * mm;

  G4Tubs *target_Solid = new G4Tubs("target_Solid", target_rmin, target_rmax, target_length * 0.5, 0., twopi);
  G4LogicalVolume *target_Logical = new G4LogicalVolume(target_Solid, target_material, "target_Logical");
  target_Logical->SetVisAttributes(yellow);

  new G4PVPlacement(0, G4ThreeVector(), target_Logical, "target", world_log, false, 0);

  /***************** Particle Detector *****************/

  G4double particle_detector_rmin = 500. * mm;
  G4double particle_detector_rmax = 540. * mm;

  G4Sphere *particle_detector_Solid = new G4Sphere("particle_detector_Solid", particle_detector_rmin, particle_detector_rmax, 0., twopi, 0., pi);

  G4LogicalVolume *particle_detector_Logical = new G4LogicalVolume(particle_detector_Solid, particle_detector_material, "particle_detector_Logical");
  particle_detector_Logical->SetVisAttributes(green);

  new G4PVPlacement(0, G4ThreeVector(), particle_detector_Logical, "particle_detector", world_log, false, 0);

  /***************** Secondary Detector *****************/

  G4double secondary_detector_rmin = 550. * mm;
  G4double secondary_detector_rmax = 590. * mm;

  G4Sphere *secondary_detector_Solid = new G4Sphere("secondary_detector_Solid", secondary_detector_rmin, secondary_detector_rmax, 0., twopi, 0., pi);

  G4LogicalVolume *secondary_detector_Logical = new G4LogicalVolume(secondary_detector_Solid, secondary_detector_material, "secondary_detector_Logical");
  secondary_detector_Logical->SetVisAttributes(red);

  new G4PVPlacement(0, G4ThreeVector(), secondary_detector_Logical, "secondary_detector", world_log, false, 0);

  /***************** Energy Deposition Detector *****************/

  G4double energy_deposition_detector_rmin = 600. * mm;
  G4double energy_deposition_detector_rmax = 640. * mm;

  G4Sphere *energy_deposition_detector_Solid = new G4Sphere("energy_deposition_detector_Solid", energy_deposition_detector_rmin, energy_deposition_detector_rmax, 0., twopi, 0., pi);

  G4LogicalVolume *energy_deposition_detector_Logical = new G4LogicalVolume(energy_deposition_detector_Solid, energy_deposition_detector_material, "energy_deposition_detector_Logical");
  energy_deposition_detector_Logical->SetVisAttributes(cyan);

  new G4PVPlacement(0, G4ThreeVector(), energy_deposition_detector_Logical, "energy_deposition_detector", world_log, false, 0);

  return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

  ParticleSD *particle_detectorSD = new ParticleSD("ParticleDetector", "ParticleDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(particle_detectorSD);
  particle_detectorSD->SetDetectorID(0);
  SetSensitiveDetector("particle_detector_Logical", particle_detectorSD, true);

  SecondarySD *secondary_detectorSD = new SecondarySD("SecondaryDetector", "SecondaryDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(secondary_detectorSD);
  secondary_detectorSD->SetDetectorID(1);
  SetSensitiveDetector("secondary_detector_Logical", secondary_detectorSD, true);

  EnergyDepositionSD *energy_deposition_detectorSD = new EnergyDepositionSD("EnergyDepositionDetector", "EnergyDepositionDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(energy_deposition_detectorSD);
  energy_deposition_detectorSD->SetDetectorID(0);
  SetSensitiveDetector("energy_deposition_detector_Logical", energy_deposition_detectorSD, true);
}
