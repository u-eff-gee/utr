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

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

  G4Colour orange(1.0, 0.5, 0.0);

  G4NistManager *nist = G4NistManager::Instance();

  G4Material *Ge = nist->FindOrBuildMaterial("G4_Ge");

  G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

  /***************** World Volume *****************/

  G4double world_r = 300. * mm;

  G4Sphere *world_dim =
      new G4Sphere("world_dim", 0., world_r, 0., twopi, 0., pi);

  G4LogicalVolume *world_log =
      new G4LogicalVolume(world_dim, vacuum, "world_log", 0, 0, 0);

  // world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VPhysicalVolume *world_phys =
      new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

  /***************** Detector ********************/

  G4double LEPS_Radius = 8 * mm;
  G4double LEPS_Length = 10. * mm;

  G4Tubs *LEPS_Solid =
      new G4Tubs("LEPS_Solid", 0. * cm, LEPS_Radius,
                 LEPS_Length / 2, 0. * deg, 360. * deg);

  G4LogicalVolume *LEPS_Logical =
      new G4LogicalVolume(LEPS_Solid, Ge, "LEPS_Logical", 0, 0, 0);

  G4Colour green(0.0, 1.0, 0.0);
  LEPS_Logical->SetVisAttributes(green);

  new G4PVPlacement(0, G4ThreeVector(0., 0., 80. * mm),
                    LEPS_Logical, "LEPS", world_log, false, 0);

  return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

  EnergyDepositionSD *LEPS_EnergyDepositionSD = new EnergyDepositionSD("LEPS_edep", "LEPS_edep");
  G4SDManager::GetSDMpointer()->AddNewDetector(LEPS_EnergyDepositionSD);
  LEPS_EnergyDepositionSD->SetDetectorID(0);
  SetSensitiveDetector("LEPS_Logical", LEPS_EnergyDepositionSD, true);
}
