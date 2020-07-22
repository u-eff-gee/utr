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

#include <tuple>
#include <string>

#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"

struct ConsDims
{
	G4double length;
	G4double inner_angle;
	G4double outer_angle;
	G4double Rmin1;
	G4double Rmax1;
	G4double Rmin2;
	G4double Rmax2;

	ConsDims() = default;
	ConsDims(G4double _length, G4double _inner_angle, G4double _outer_angle, G4double _Rmin1, G4double _Rmax1, G4double _Rmin2, G4double _Rmax2)
		: length(_length), inner_angle(_inner_angle), outer_angle(_outer_angle), Rmin1(_Rmin1), Rmax1(_Rmax1), Rmin2(_Rmin2), Rmax2(_Rmax2) {}
	ConsDims(G4double _length, G4double _Rmin1, G4double _Rmax1, G4double _Rmin2, G4double _Rmax2)
		: length(_length), inner_angle(0.), outer_angle(0.), Rmin1(_Rmin1), Rmax1(_Rmax1), Rmin2(_Rmin2), Rmax2(_Rmax2) {}
	static ConsDims angled(G4double _length, G4double _inner_angle, G4double _outer_angle, G4double _Rmin1, G4double _Rmax1)
	{
		return ConsDims(_length, _inner_angle, _outer_angle, _Rmin1, _Rmax1, _Rmin1 + _length * tan(_inner_angle), _Rmax1 + _length * tan(_outer_angle));
	}

	static ConsDims straight(G4double _length, G4double Rmin, G4double Rmax)
	{
		return ConsDims(_length, Rmin, Rmax, Rmin, Rmax);
	}

  static ConsDims filled(ConsDims cd)
  {
    return ConsDims(cd.length, 0., cd.outer_angle, 0., cd.Rmax1, 0., cd.Rmax2);
  }
};
class BGO
{
public:
  BGO(G4LogicalVolume *World_Log, G4String name);

  G4Polycone *Get_Al_Case_Solid() { return al_Case_Solid; };

  // Length and radius of the BGO mother volume which just encloses all of the parts of the BGO
  G4double Get_Length() { return length; };
  G4double Get_Radius() { return radius; };
  // max_penetration depth denotes how far (measured from the backward end of the BGO) a detector can be pushed inside the BGO
  G4double Get_Max_Penetration_Depth() { return max_penetration_depth; };
  void Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
                 G4double dist_from_center);

private:
  G4LogicalVolume *bgo_Mother_Logical;
  G4LogicalVolume *World_Logical;
  G4Polycone *al_Case_Solid;

  G4String bgo_Name;

  G4double length;
  G4double radius;
  G4double max_penetration_depth;

  std::map<int, ConsDims> al_Case;
  std::map<int, G4Cons *> al_Case_Solids;
  std::map<int, G4LogicalVolume *> al_Case_Logical;
  std::map<int, ConsDims> bgo_Crystal;
  std::map<int, std::string> bgo_Crystal_name;
  std::map<int, G4Cons *> bgo_Crystal_Solids;
  std::map<int, G4LogicalVolume *> bgo_Crystal_Logical;
};
