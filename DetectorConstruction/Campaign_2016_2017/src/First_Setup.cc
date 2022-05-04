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

// This geometry is derived from the more modern 18/19 geometries.
// Some variables are defined at the beginning, which have no meaning any more,
// since the lead wall from the 18/19 geometries was replaced by the old lead
// wall from the 16/17 geometries. But still, they are needed to fix the dimensions
// of the table of the first setup.

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "First_Setup.hh"
#include "Units.hh"

#include "Bricks.hh"

First_Setup::First_Setup(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                                       First_Setup_Length(41.5 * inch),
                                                       First_Setup_X(44. * inch),
                                                       First_Setup_Y(50. * inch) // Arbitrary
{}

void First_Setup::Construct(G4ThreeVector global_coordinates) {

  G4Colour grey(0.5, 0.5, 0.5);
  G4Colour green(0., 0.5, 0.);
  G4Colour white(1.0, 1.0, 1.0);

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

  // The following three variables are need for the construction of the
  // 18/19 geometry parts
  G4double Concrete_Block_Y = 3.75 * inch;
  G4double Lead_Base_Y = 4. * inch;
  G4double Lead_Tunnel_Y = 4. * inch;

  // Lead wall

  NormBrick *nb = new NormBrick(World_Logical);
  NormBrickWithHole *nbh = new NormBrickWithHole(World_Logical);
  ThreeQuarterShortNormBrick *tqsb =
      new ThreeQuarterShortNormBrick(World_Logical);

  for (int ny = -2; ny <= 2; ny++) {
    for (int nz = 0; nz < 4; nz++) {
      for (int nx = 0; nx < 1; nx++) {
        if (ny == 0 && nx == 0) {
          nbh->Put(global_coordinates.x(),
                   global_coordinates.y(),
                   global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                   0. * deg, 90. * deg, 90. * deg);
          tqsb->Put(global_coordinates.x() + nb->L * 0.5 + tqsb->L * 0.5,
                    global_coordinates.y(),
                    global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                    0. * deg, 90. * deg, 90. * deg);
          tqsb->Put(global_coordinates.x() - nb->L * 0.5 - tqsb->L * 0.5,
                    global_coordinates.y(),
                    global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                    0. * deg, 90. * deg, 90. * deg);
          continue;
        }
        nb->Put(global_coordinates.x(),
                global_coordinates.y() + nb->M * ny,
                global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                0. * deg, 90. * deg, 90. * deg);
        tqsb->Put(global_coordinates.x() + nb->L * 0.5 + tqsb->L * 0.5,
                  global_coordinates.y() + nb->M * ny,
                  global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                  0. * deg, 90. * deg, 90. * deg);
        tqsb->Put(global_coordinates.x() - nb->L * 0.5 - tqsb->L * 0.5,
                  global_coordinates.y() + nb->M * ny,
                  global_coordinates.z() - nb->S * (nz + 0.5) - 4. * nb->S,
                  0. * deg, 90. * deg, 90. * deg);
      }
    }
  }

  // Construct table plate
  G4double First_Setup_Box_Y = 12. * inch;
  G4double First_Setup_Table_Thickness = 0.5 * inch;

  G4Box *Table_Solid_Solid = new G4Box("Table_Solid_Solid", First_Setup_X * 0.5, First_Setup_Table_Thickness * 0.5, First_Setup_Length * 0.5);

  G4double Table_Hole_X = 20. * inch;
  G4double Table_Hole_Thickness = 2. * First_Setup_Table_Thickness;
  G4double Table_Hole_Length = 24.5 * inch;

  G4Box *Table_Hole_Solid = new G4Box("Table_Hole_Solid", Table_Hole_X * 0.5, Table_Hole_Thickness * 0.5, Table_Hole_Length * 0.5);

  G4SubtractionSolid *Table_Solid = new G4SubtractionSolid("Table_Solid", Table_Solid_Solid, Table_Hole_Solid, 0, G4ThreeVector(0., 0., 6. * inch));

  G4LogicalVolume *Table_Logical = new G4LogicalVolume(Table_Solid, Al, "Table_Solid");
  Table_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y - First_Setup_Table_Thickness * 0.5, 0.), Table_Logical, "First_Setup_Table", World_Logical, false, 0, false);

  // Construct columns on table

  G4double Table_Column_Base_Length = 1.5 * inch;
  G4double Table_Column_Height = 18.5 * inch;

  G4Box *Table_Column_Solid = new G4Box("Table_Column_Solid", Table_Column_Base_Length * 0.5, Table_Column_Height * 0.5, Table_Column_Base_Length * 0.5);

  G4LogicalVolume *Table_Column_Logical = new G4LogicalVolume(Table_Column_Solid, Al, "Table_Column_Logical");
  Table_Column_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(Table_Hole_X * 0.5 + Table_Column_Base_Length * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height * 0.5, -First_Setup_Length * 0.5 + 3.5 * inch), Table_Column_Logical, "Table_Column_0", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(-Table_Hole_X * 0.5 - Table_Column_Base_Length * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height * 0.5, -First_Setup_Length * 0.5 + 3.5 * inch), Table_Column_Logical, "Table_Column_1", World_Logical, false, 0, false);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(Table_Hole_X * 0.5 + Table_Column_Base_Length * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height * 0.5, First_Setup_Length * 0.5 - 3.5 * inch), Table_Column_Logical, "Table_Column_2", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(-Table_Hole_X * 0.5 - Table_Column_Base_Length * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height * 0.5, First_Setup_Length * 0.5 - 3.5 * inch), Table_Column_Logical, "Table_Column_3", World_Logical, false, 0, false);

  // Construct box on top of table

  // "low-density Aluminium"
  // Since it tedious to construct metal beams of
  // holding structures, simply construct a solid
  // beam made of low-density aluminium

  G4Element *nat_Al = new G4Element("natural Al", "nat_Al", 13, 26.9815385 * g / mole);
  G4double Al_Density = 2.70 * g / cm3;
  G4Material *one_third_density_Al = new G4Material("one_third_density_Al", Al_Density / 3., 1);
  one_third_density_Al->AddElement(nat_Al, 1);

  G4double Box_Wall_Thickness = 2. * inch;
  G4Box *Box_Wall_Solid = new G4Box("Box_Wall_Solid", Box_Wall_Thickness * 0.5, First_Setup_Box_Y * 0.5, First_Setup_Length * 0.5);
  G4LogicalVolume *Box_Wall_Logical = new G4LogicalVolume(Box_Wall_Solid, one_third_density_Al, "Box_Wall_Logical");
  Box_Wall_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(-Table_Hole_X * 0.5 - Table_Column_Base_Length + Box_Wall_Thickness * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + First_Setup_Box_Y * 0.5, 0.), Box_Wall_Logical, "Box_Wall_0", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(Table_Hole_X * 0.5 + Table_Column_Base_Length - Box_Wall_Thickness * 0.5, -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + First_Setup_Box_Y * 0.5, 0.), Box_Wall_Logical, "Box_Wall_1", World_Logical, false, 0, false);

  // Construct beams on box

  G4double Beam_Base_Length = 1.5 * inch;
  G4Box *Box_Beam_Solid = new G4Box("Box_Beam_Solid", (Table_Hole_X - 1. * inch) * 0.5, Beam_Base_Length * 0.5, Beam_Base_Length * 0.5);

  G4LogicalVolume *Box_Beam_Logical = new G4LogicalVolume(Box_Beam_Solid, one_third_density_Al, "Box_Beam_Logical");
  Box_Beam_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + First_Setup_Box_Y - Beam_Base_Length * 0.5, -First_Setup_Length * 0.5 + Beam_Base_Length * 0.5), Box_Beam_Logical, "Box_Beam_0", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + First_Setup_Box_Y - Beam_Base_Length * 0.5, First_Setup_Length * 0.5 - Beam_Base_Length * 0.5), Box_Beam_Logical, "Box_Beam_1", World_Logical, false, 0, false);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + Beam_Base_Length * 0.5, -First_Setup_Length * 0.5 + Beam_Base_Length * 0.5), Box_Beam_Logical, "Box_Beam_0", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Lead_Tunnel_Y * 0.5 - Lead_Base_Y - Concrete_Block_Y + Table_Column_Height + Beam_Base_Length * 0.5, First_Setup_Length * 0.5 - Beam_Base_Length * 0.5), Box_Beam_Logical, "Box_Beam_1", World_Logical, false, 0, false);
}
