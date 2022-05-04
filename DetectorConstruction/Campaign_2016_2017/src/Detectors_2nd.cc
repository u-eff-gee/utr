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

#include "Detectors_2nd.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "Units.hh"

Detectors_2nd::Detectors_2nd(G4LogicalVolume *World_Log) : World_Logical(World_Log) {}

void Detectors_2nd::Construct(G4ThreeVector global_coordinates) {

  HPGe_Collection hpge_Collection;

  /**************** HPGE6 *******************/

  G4double hpge6_rt = 75.4 * mm;
  G4double hpge6_dy = 0. * mm;
  G4double hpge6_dz = 0. * mm;
  G4double hpge6_phi = 90. * deg;
  G4double hpge6_theta = 90. * deg;

  HPGe_Coaxial hpge6(World_Logical, "HPGe6");
  hpge6.setProperties(hpge_Collection.HPGe_100_TUD_72902);
  hpge6.useDewar();
  hpge6.Add_Filter("G4_Cu", 2. * 1.15 * mm, 50. * mm);
  hpge6.Add_Filter("G4_Pb", 2. * 2. * mm, 50. * mm);
  hpge6.Add_Wrap("G4_Pb", 2. * 1.2 * mm);
  hpge6.Construct(global_coordinates + G4ThreeVector(0., hpge6_dy, hpge6_dz),
                  hpge6_theta, hpge6_phi, hpge6_rt, 0. * deg);

  /**************** HPGE7 *******************/

  G4double hpge7_rt = 75.4 * mm;
  G4double hpge7_dy = 0. * mm;
  G4double hpge7_dz = 0. * mm;
  G4double hpge7_phi = 180. * deg;
  G4double hpge7_theta = 90. * deg;

  HPGe_Coaxial hpge7(World_Logical, "HPGe7");
  hpge7.setProperties(hpge_Collection.HPGe_55_TUNL_31524);
  hpge7.useFilterCase();
  hpge7.useFilterCaseRing();
  hpge7.useDewar();
  hpge7.Add_Filter("G4_Cu", 2. * 1.15 * mm, 45. * mm);
  hpge7.Add_Filter("G4_Pb", 2. * 2. * mm, 45. * mm);
  hpge7.Add_Wrap("G4_Pb", 1. * 1.2 * mm);
  hpge7.Construct(global_coordinates + G4ThreeVector(0., hpge7_dy, hpge7_dz),
                  hpge7_theta, hpge7_phi, hpge7_rt, 0. * deg);

  /**************** HPGE8 *******************/

  G4double hpge8_rt = 75.4 * mm;
  G4double hpge8_dy = 0. * mm;
  G4double hpge8_dz = 0. * mm;
  G4double hpge8_phi = 0. * deg;
  G4double hpge8_theta = 90. * deg;

  HPGe_Coaxial hpge8(World_Logical, "HPGe8");
  hpge8.setProperties(hpge_Collection.HPGe_55_TUNL_21638);
  hpge8.useFilterCase();
  hpge8.useFilterCaseRing();
  hpge8.useDewar();
  hpge8.Add_Filter("G4_Cu", 2. * 1.15 * mm, 45. * mm);
  hpge8.Add_Filter("G4_Pb", 2. * 2. * mm, 45. * mm);
  hpge8.Add_Wrap("G4_Pb", 2. * 1.2 * mm);
  hpge8.Construct(global_coordinates + G4ThreeVector(0., hpge8_dy, hpge8_dz),
                  hpge8_theta, hpge8_phi, hpge8_rt, 0. * deg);
}
