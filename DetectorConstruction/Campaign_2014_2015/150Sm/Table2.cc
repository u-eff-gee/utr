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
#include "G4ExtrudedSolid.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4TwoVector.hh"
#include "G4VisAttributes.hh"

#include "NamedColors.hh"
#include "Table2.hh"
#include "Units.hh"

Table2::Table2(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                             Table2_Length(38. * inch),
                                             Z_Axis_Offset_Z(-2.25 * inch) // This is how far the beam tube holder extends into the G3 table
{}

void Table2::Construct(G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material *one_third_density_brass = nist->FindOrBuildMaterial("one_third_density_brass");

  G4double Table_Plate_Radius = 17. * inch;
  G4double Table_Plate_Hole_Radius = 9. * inch;

  // Upstream beam pipe holder

  G4double Holder_Base_To_Table = 3. * inch; // Estimated

  // The following variables are about the beampipe holder, which does not exist in this setup, but the variables
  // were also used to place other parts of the stetup (relativ to its position) and hence they are kept here
  G4double Upstream_Holder_Ring_Outer_Radius = 3. * inch;
  G4double Upstream_Holder_Base_Y = 1.5 * inch;
  G4double Upstream_Holder_Hole_Depth = 0.5 * inch;

  // Holding structure for vertical detectors

  G4double Brass_Column_Height = 24. * inch;
  G4double Brass_Column_Base = 1.5 * inch;

  G4double Al_Bar_Thickness = 0.5 * inch;
  G4double Al_Bar_X = 27.5 * inch;
  G4double Al_Bar_Y = 4. * inch;

  G4Box *Brass_Column_Solid = new G4Box("Brass_Column_Solid", Brass_Column_Base * 0.5, Brass_Column_Height * 0.5, Brass_Column_Base * 0.5);

  G4LogicalVolume *Brass_Column_Logical = new G4LogicalVolume(Brass_Column_Solid, one_third_density_brass, "Brass_Column_Solid");
  Brass_Column_Logical->SetVisAttributes(light_orange);

  // Table plate
  // The table plate is divided into two parts with different inner radii and thicknesses
  G4double Table_Plate_Thickness = 0.5 * inch; // Estimated
  G4double Table_Thin_Plate_Thickness = 0.25 * inch; // Estimated

  G4double Table_Thin_Plate_Hole_Radius = 12.5 * inch;

  G4Tubs *Table_Plate_Solid_Solid = new G4Tubs("Table_Plate_Solid_Solid", Table_Plate_Hole_Radius, Table_Plate_Radius, Table_Plate_Thickness * 0.5, (90. + 46.05) * deg, (360. - 2. * 46.05) * deg); // Angles estimated
  G4Box *Table_Plate_Hole = new G4Box("Table_Plate_Hole", Table_Plate_Hole_Radius, 8.6747 * inch * 0.5, Table_Plate_Thickness);

  G4SubtractionSolid *Table_Plate_Solid = new G4SubtractionSolid("Table_Plate_Solid", Table_Plate_Solid_Solid, Table_Plate_Hole, 0, G4ThreeVector(0., 8.6747 * inch * 0.5, 0.)); // Cut out a rectangular piece to fit both plates into each other

  G4LogicalVolume *Table_Plate_Logical = new G4LogicalVolume(Table_Plate_Solid, Al, "Table_Plate_Logical");
  Table_Plate_Logical->SetVisAttributes(grey);

  G4RotationMatrix *rot_Table = new G4RotationMatrix();
  rot_Table->rotateX(90. * deg);
  new G4PVPlacement(rot_Table, global_coordinates + G4ThreeVector(0., -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table - Table_Plate_Thickness * 0.5, 0.), Table_Plate_Logical, "Table_Plate", World_Logical, false, 0, false);

  G4Tubs *Table_Thin_Plate_Solid = new G4Tubs("Table_Thin_Plate_Solid", Table_Thin_Plate_Hole_Radius, Table_Plate_Radius, Table_Thin_Plate_Thickness * 0.5, (90. - 46.05) * deg, 2. * 46.05 * deg); // Angles estimated

  G4LogicalVolume *Table_Thin_Plate_Logical = new G4LogicalVolume(Table_Thin_Plate_Solid, Al, "Table_Plate_Logical");
  Table_Thin_Plate_Logical->SetVisAttributes(grey);

  new G4PVPlacement(rot_Table, global_coordinates + G4ThreeVector(0., -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table - Table_Plate_Thickness * 0.5 + Table_Thin_Plate_Thickness * 0.5, 0.), Table_Thin_Plate_Logical, "Table_Plate", World_Logical, false, 0, false);

  // Construct holding structure on top of the table

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(Table_Plate_Hole_Radius + 2.5 * inch + Brass_Column_Base * 0.5, -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table + Brass_Column_Height * 0.5, -3.75 * inch - Brass_Column_Base * 0.5), Brass_Column_Logical, "Brass_Column_1", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(-Table_Plate_Hole_Radius - 2.5 * inch - Brass_Column_Base * 0.5, -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table + Brass_Column_Height * 0.5, -3.75 * inch - Brass_Column_Base * 0.5), Brass_Column_Logical, "Brass_Column_2", World_Logical, false, 0, false);

  G4Box *Al_Bar_Solid = new G4Box("Al_Bar_Solid", Al_Bar_X * 0.5, Al_Bar_Y * 0.5, Al_Bar_Thickness * 0.5);

  G4LogicalVolume *Al_Bar_Logical = new G4LogicalVolume(Al_Bar_Solid, Al, "Al_Bar_Logical");
  Al_Bar_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table + 18. * inch, -3.75 * inch - Brass_Column_Base - Al_Bar_Thickness * 0.5), Al_Bar_Logical, "Al_Bar_1", World_Logical, false, 0, false); // Estimated vertical position

  // Construct holding structure below table

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(Table_Plate_Hole_Radius + 2.5 * inch + Brass_Column_Base * 0.5, -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table - Table_Plate_Thickness - Brass_Column_Height * 0.5, -3.75 * inch - Brass_Column_Base * 0.5), Brass_Column_Logical, "Brass_Column_1", World_Logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(-Table_Plate_Hole_Radius - 2.5 * inch - Brass_Column_Base * 0.5, -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table - Table_Plate_Thickness - Brass_Column_Height * 0.5, -3.75 * inch - Brass_Column_Base * 0.5), Brass_Column_Logical, "Brass_Column_2", World_Logical, false, 0, false); // Estimated length. Actually measured only the length of the holding structure on top

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Upstream_Holder_Ring_Outer_Radius + Upstream_Holder_Hole_Depth - Upstream_Holder_Base_Y - Holder_Base_To_Table - Table_Plate_Thickness - 18. * inch, -3.75 * inch - Brass_Column_Base - Al_Bar_Thickness * 0.5), Al_Bar_Logical, "Al_Bar_2", World_Logical, false, 0, false); // Estimated vertical position
}
