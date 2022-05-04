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

#include <sstream>
#include <vector>

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "G4SDManager.hh"
#include "ParticleSD.hh"

#include "G4PhysicalConstants.hh"

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

const size_t n_target_layers = 100; // Determines the number of layers of the target.

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

  /***************** Target Properties *****************/

  const double target_length = 100. * mm;
  const double target_radius = 10. * mm;
  const G4String target_material_name = "G4_Pb";

  /***************** Materials *****************/

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *target_material = nist->FindOrBuildMaterial(target_material_name);

  /***************** World Volume *****************/

  const double world_x = 4. * target_radius;
  const double world_y = 4. * target_radius;
  const double world_z = target_length;

  G4Box *world_solid = new G4Box("world_solid", world_x, world_y, world_z);
  G4LogicalVolume *world_logical = new G4LogicalVolume(world_solid, vacuum, "world_logical");
  G4VPhysicalVolume *world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world", 0, false, 0);

  /***************** Target *****************/

  const double target_layer_length = target_length / n_target_layers;
  std::stringstream target_solid_name, target_logical_name, target_physical_name;
  std::vector<G4VSolid *> target_solid(n_target_layers);
  std::vector<G4LogicalVolume *> target_logical(n_target_layers);

  for (size_t i = 0; i < n_target_layers; ++i) {
    target_solid_name << "target_layer_" << i << "_solid";
    target_solid[i] = new G4Tubs(target_solid_name.str(), 0., target_radius, 0.5 * target_layer_length, 0., twopi);
    target_solid_name.str("");

    target_logical_name << "target_layer_" << i << "_logical";
    target_logical[i] = new G4LogicalVolume(target_solid[i], target_material, target_logical_name.str());
    target_logical_name.str("");
    if (i % 2 == 1)
      target_logical[i]->SetVisAttributes(G4Color::Yellow());
    else
      target_logical[i]->SetVisAttributes(G4Color::Red());

    target_physical_name << "target_layer_" << i;
    new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5 * target_length + (i + 0.5) * target_layer_length), target_logical[i], target_physical_name.str(), world_logical, false, 0);
    target_physical_name.str("");
  }

  return world_physical;
}

void DetectorConstruction::ConstructSDandField() {

  std::stringstream target_logical_name, target_physical_name;
  ParticleSD *detector = nullptr;

  for (size_t i = 0; i < n_target_layers; ++i) {
    target_physical_name << "target_layer_" << i;
    detector = new ParticleSD(target_physical_name.str(), target_physical_name.str());
    target_physical_name.str("");

    G4SDManager::GetSDMpointer()->AddNewDetector(detector);
    detector->SetDetectorID(i);
    target_logical_name << "target_layer_" << i << "_logical";
    SetSensitiveDetector(target_logical_name.str(), detector, true);
    target_logical_name.str("");
  }
}
