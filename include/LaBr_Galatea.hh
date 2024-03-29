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
//	3 inch x 3 inch LaBr Detector of the Galatea Array
//	at the Institut für Kernphysik, TU Darmstadt
//**************************************************************//
#pragma once

#include <vector>

#include "G4LogicalVolume.hh"

#include "Detector.hh"

using std::vector;

class LaBr_Galatea : public Detector {
  public:
  LaBr_Galatea(G4LogicalVolume *World_Logical, G4String name, bool use_old_config) : Detector(World_Logical, name), use_old_configuration(use_old_config){};
  LaBr_Galatea(G4LogicalVolume *World_Logical, G4String name) : Detector(World_Logical, name), use_old_configuration(false){};
  ~LaBr_Galatea(){};

  void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                 G4double dist_from_center) const override;
  // When an intrinsic rotation angle is given, it will simply be ignored and
  // the Construct() method above is called
  void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                 G4double dist_from_center, G4double intrinsic_rotation_angle) const override;

  protected:
  bool use_old_configuration;
};
