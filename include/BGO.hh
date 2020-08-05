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
#include "G4SystemOfUnits.hh"

#include <vector>
#include <array>
#include <algorithm>

constexpr G4double get_edge(G4double midpoint, G4double angle) {
  return midpoint / cos(angle);
}

template<int size>
constexpr std::array<G4double,size> get_edges(std::array<G4double,size> midpoints, G4double angle) {
  for(G4double &r : midpoints) {
    r = get_edge(r, angle);
  }
  return midpoints;
}

class BGO
{
public:
  BGO(G4LogicalVolume *World_Log, G4String name);

  G4Polycone *Get_CaseFilled_Solid() { return caseFilled_Solid; };

  // Length and radius of the BGO mother volume which just encloses all of the parts of the BGO
  static constexpr G4double Get_Length() { return *std::max_element(caseOut_z.begin(), caseOut_z.end()); };
  static constexpr G4double Get_Radius() { return *std::max_element(caseOut_r.begin(), caseOut_r.end()); };
  // max_penetration depth denotes how far (measured from the backward end of the BGO) a detector can be pushed inside the BGO
  static constexpr G4double Get_Max_Penetration_Depth() { return Get_Length() - caseOut_z[11]; };
  void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                 G4double dist_from_center);
  G4ThreeVector GetPos(G4double theta, G4double phi, G4double dist_from_center) const;

private:
	static constexpr std::array<G4double,12> caseOut_z {0., 0., 86.*mm, 172.*mm, 172.*mm, 192.*mm, 192.*mm, 193.*mm, 202.*mm, 202.*mm, 42.*mm, 42.*mm};
	static constexpr std::array<G4double,12> caseOut_r {25.*mm, 62.*mm, 95.*mm, 95.*mm, 115.*mm, 115.*mm, 92.*mm, 92.*mm, 58.5*mm, 50.5*mm, 50.5*mm, 37.44*mm};
	static constexpr std::array<G4double,5> caseFilled_z {0., 0., 86.*mm, 202.*mm, 202.*mm};
	static constexpr std::array<G4double,5> caseFilled_r {0., 62.*mm, 95.*mm, 95.*mm, 0.};
	static constexpr std::array<G4double,7> caseIn_z {8.*mm, 8.*mm, 86.*mm, 179.*mm, 190.*mm, 41.2*mm, 41.2*mm};
  static constexpr std::array<G4double,7> caseIn_r {27.5*mm, 62.45*mm, 92.*mm, 92.*mm,51.5*mm, 51.3*mm, 37.28*mm};
	
	static constexpr std::array<G4double,7> bgo_z {10.*mm, 10.*mm, 70.*mm, 170.*mm, 185.*mm, 40.*mm, 40.*mm};
	static constexpr auto bgo_r = get_edges<7>({29.2*mm, 60.4*mm, 82.3*mm, 82.3*mm, 52.3*mm, 52.3*mm, 37.*mm}, 22.5 * deg);
	static constexpr std::array<G4double,4> bgo_abrased_z {10.*mm, 10.*mm, 185.*mm, 185.*mm};
	static constexpr auto bgo_abrased_r = get_edges<4>({0.*mm, 61.17*mm, 128.34*mm, 0.*mm}, 22.5 * deg);
	static constexpr std::array<G4double,7> caseGap_z {9.*mm, 9.*mm, 87.*mm, 178.*mm, 189.*mm, 41.5*mm, 41.5*mm};
	static constexpr std::array<G4double,7> caseGap_r {27.6*mm, 62.4*mm, 91.5*mm, 91.5*mm,51.7*mm, 51.5*mm, 37.38*mm};


  G4LogicalVolume *World_Logical;
  G4Polycone *caseFilled_Solid;

  G4String bgo_name;

  G4LogicalVolume *bgo_case_Logical;
  std::vector<G4LogicalVolume*> bgo_Logical;
};
