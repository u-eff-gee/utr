/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2019 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.)
utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class Filter_Case {
  public:
  Filter_Case(G4LogicalVolume *World_Logical, G4String name);
  ~Filter_Case(){};

  void Construct_Ring(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                      G4double dist_from_center);
  void Construct_Case(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                      G4double dist_from_center);

  G4double get_filter_case_ring_thickness() { return filter_case_ring_thickness; };
  G4double get_filter_case_bottom_thickness() { return filter_case_bottom_thickness; };

  private:
  G4LogicalVolume *World_Log;
  G4String filter_case_name;
  G4double filter_case_ring_thickness;
  G4double filter_case_bottom_thickness;
};
