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
#pragma once

#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4Polycone.hh"

class BGO
{
public:
  BGO(G4LogicalVolume *World_Log, G4String name);

  G4Polycone *Get_CaseFilled_Solid() { return caseFilled_Solid; };

  // Length and radius of the BGO mother volume which just encloses all of the parts of the BGO
  G4double Get_Length() { return length; };
  G4double Get_Radius() { return radius; };
  // max_penetration depth denotes how far (measured from the backward end of the BGO) a detector can be pushed inside the BGO
  G4double Get_Max_Penetration_Depth() { return max_penetration_depth; };
  void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                 G4double dist_from_center);
  G4ThreeVector GetPos(G4double theta, G4double phi, G4double dist_from_center) const;

private:
  G4LogicalVolume *World_Logical;
  G4Polycone *caseFilled_Solid;

  G4String bgo_name;

  G4double length;
  G4double radius;
  G4double max_penetration_depth;

  G4LogicalVolume *bgo_case_Logical;
  G4LogicalVolume *bgo_Logical;
};
