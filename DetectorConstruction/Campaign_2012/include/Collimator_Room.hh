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

#ifndef COLLIMATOR_ROOM_HH
#define COLLIMATOR_ROOM_HH 1

#include "G4LogicalVolume.hh"

class Collimator_Room {
  public:
  Collimator_Room(G4LogicalVolume *World_Log, G4double Coll_Radius);
  ~Collimator_Room(){};

  void Construct(G4ThreeVector global_coordinates);
  G4double Get_Length() { return Collimator_Room_Length; };

  private:
  G4LogicalVolume *World_Logical;
  G4double Collimator_Radius;
  G4double Collimator_Room_Length;
};

#endif
