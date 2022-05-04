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

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "First_UTR_Wall.hh"
#include "Units.hh"

#include "Bricks.hh"

First_UTR_Wall::First_UTR_Wall(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                                             First_UTR_Wall_Length(200. * mm) {}

void First_UTR_Wall::Construct(G4ThreeVector global_coordinates) {

  NormBrick *nb = new NormBrick(World_Logical);
  NormBrickWithHole *nbh = new NormBrickWithHole(World_Logical);

  for (int ny = -2; ny <= 2; ny++) {
    for (int nx = -1; nx <= 1; nx++) {
      for (int nz = 0; nz < 4; nz++) {
        if (ny == 0 && nx == 0) {
          nbh->Put(global_coordinates.x() + nb->L * nx,
                   global_coordinates.y() + nb->M * ny,
                   global_coordinates.z() - nb->S * (nz + 0.5), 0. * deg,
                   90. * deg, 90. * deg);
          continue;
        }

        nb->Put(global_coordinates.x() + nb->L * nx,
                global_coordinates.y() + nb->M * ny,
                global_coordinates.z() - nb->S * (nz + 0.5), 0. * deg,
                90. * deg, 90. * deg);
      }
    }
  }
}
