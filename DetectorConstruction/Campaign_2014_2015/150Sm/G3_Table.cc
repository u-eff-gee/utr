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
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G3_Table.hh"
#include "NamedColors.hh"
#include "Units.hh"

G3_Table::G3_Table(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                                 G3_Table_Length(36.25 * inch) {}

void G3_Table::Construct(G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

  G4double G3_Table_X = 21. * inch;

  // Concrete and lead shielding

  G4double Wall_To_Table_Edge = 8. * inch;

  G4double Lead_Wall_X = 20. * inch;
  G4double Lead_Wall_Y = 12. * inch;
  G4double Lead_Wall_Z = 12. * inch;

  G4double Hole_X = 4. * inch;
  G4double Hole_Y = 4. * inch;

  G4Box *Lead_Solid_Wall_Solid = new G4Box("Lead_Solid_Wall_Solid", Lead_Wall_X * 0.5, Lead_Wall_Y * 0.5, Lead_Wall_Z * 0.5);

  G4Box *Hole_Solid = new G4Box("Hole_Solid", Hole_X * 0.5, Hole_Y * 0.5, Lead_Wall_Z);

  G4SubtractionSolid *Lead_Wall_Solid = new G4SubtractionSolid("Lead_Wall_Solid", Lead_Solid_Wall_Solid, Hole_Solid, 0, G4ThreeVector(0., -1. * inch, 0.));

  G4LogicalVolume *Lead_Wall_Logical = new G4LogicalVolume(Lead_Wall_Solid, Pb, "Lead_Solid_Wall_Logical");

  Lead_Wall_Logical->SetVisAttributes(green);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch, -Wall_To_Table_Edge + Lead_Wall_Z * 0.5), Lead_Wall_Logical, "Lead_Wall", World_Logical, false, 0, false);

  G4double Lead_Collimator_Inner_Radius = 1.1 * inch; // Estimated, but definitely larger than outer radius of beam pipe
  G4double Lead_Collimator_Outer_Radius = 2. * inch;
  G4double Lead_Collimator_Length = 4. * inch;
  G4Tubs *Lead_Collimator_Solid = new G4Tubs("Lead_Collimator_Solid", Lead_Collimator_Inner_Radius, Lead_Collimator_Outer_Radius, Lead_Collimator_Length * 0.5, 0., twopi);

  G4LogicalVolume *Lead_Collimator_Logical = new G4LogicalVolume(Lead_Collimator_Solid, Pb, "Lead_Collimator_Logical");
  Lead_Collimator_Logical->SetVisAttributes(green);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Wall_To_Table_Edge + 10. * inch), Lead_Collimator_Logical, "Lead_Collimator", World_Logical, false, 0, false);

  G4double Concrete_Base_X = 20. * inch;
  G4double Concrete_Base_Y = 7.25 * inch;
  G4double Concrete_Base_Z = 1.5 * 7.75 * inch;

  G4Box *Concrete_Base_Solid = new G4Box("Concrete_Base_Solid", Concrete_Base_X * 0.5, Concrete_Base_Y * 0.5, Concrete_Base_Z * 0.5);
  G4LogicalVolume *Concrete_Base_Logical = new G4LogicalVolume(Concrete_Base_Solid, concrete, "Concrete_Base_Logical");
  Concrete_Base_Logical->SetVisAttributes(white);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch - Lead_Wall_Y * 0.5 - Concrete_Base_Y * 0.5, -Wall_To_Table_Edge + Concrete_Base_Z * 0.5), Concrete_Base_Logical, "Concrete_Base", World_Logical, false, 0, false);

  G4double Plastic_Base_X = 20. * inch;
  G4double Plastic_Base_Y = 1. * inch;
  G4double Plastic_Base_Z = 12. * inch;

  G4Box *Plastic_Base_Solid = new G4Box("Plastic_Base_Solid", Plastic_Base_X * 0.5, Plastic_Base_Y * 0.5, Plastic_Base_Z * 0.5);
  G4LogicalVolume *Plastic_Base_Logical = new G4LogicalVolume(Plastic_Base_Solid, PE, "Plastic_Base_Logical");

  Plastic_Base_Logical->SetVisAttributes(yellow);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch - Lead_Wall_Y * 0.5 - Concrete_Base_Y - Plastic_Base_Y * 0.5, -Wall_To_Table_Edge + Plastic_Base_Z * 0.5), Plastic_Base_Logical, "Plastic_Base", World_Logical, false, 0, false);

  // Table plate
  G4double G3_Table_Plate_Thickness = 0.5 * inch; // Estimated, probably too large for the geometry that is really inside

  G4Box *G3_Table_Plate_Solid = new G4Box("G3_Table_Plate_Solid", G3_Table_X * 0.5, G3_Table_Plate_Thickness * 0.5, G3_Table_Length * 0.5);
  G4LogicalVolume *G3_Table_Plate_Logical = new G4LogicalVolume(G3_Table_Plate_Solid, Al, "G3_Table_Plate_Logical");
  G3_Table_Plate_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch - 0.5 * Lead_Wall_Y - Concrete_Base_Y - Plastic_Base_Y - G3_Table_Plate_Thickness * 0.5, 0.), G3_Table_Plate_Logical, "G3_Table_Plate", World_Logical, false, 0, false);

  // Upstream beam pipe holder
  G4double Upstream_Holder_X = 2.75 * inch;
  G4double Upstream_Holder_Y = -(1. * inch - Lead_Wall_Y * 0.5 - Concrete_Base_Y - Plastic_Base_Y);
  G4double Upstream_Holder_Z = 0.5 * inch;

  G4Box *Upstream_Holder_Solid_Solid = new G4Box("Upstream_Holder_Solid_Solid", Upstream_Holder_X * 0.5, Upstream_Holder_Y * 0.5, Upstream_Holder_Z * 0.5);

  G4double Beam_Pipe_Outer_Radius = 2. * inch;
  G4Box *Upstream_Holder_Hole_Solid = new G4Box("Upstream_Holder_Hole_Solid", Beam_Pipe_Outer_Radius * 0.5, Beam_Pipe_Outer_Radius * 0.5, Upstream_Holder_Z);

  G4RotationMatrix *rotZ = new G4RotationMatrix();
  rotZ->rotateZ(45. * deg);
  G4SubtractionSolid *Upstream_Holder_Solid = new G4SubtractionSolid("Upstream_Holder_Solid", Upstream_Holder_Solid_Solid, Upstream_Holder_Hole_Solid, rotZ, G4ThreeVector(0., Upstream_Holder_Y * 0.5, 0.));

  G4LogicalVolume *Upstream_Holder_Logical = new G4LogicalVolume(Upstream_Holder_Solid, Al, "Upstream_Holder_Logical");

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Upstream_Holder_Y * 0.5, -G3_Table_Length * 0.5 + 0.5 * inch + Upstream_Holder_Z * 0.5), Upstream_Holder_Logical, "Upstream_Holder", World_Logical, false, 0, false);

  // Downstream beam pipe holder
  /* There was no second setup beam pipe in 2014/2015 so no beam pipe holder
  G4double Downstream_Holder_X = 6.*inch;
  G4double Downstream_Holder_Y = -(1.*inch - Lead_Wall_Y*0.5 - Concrete_Base_Y - Plastic_Base_Y);
  G4double Downstream_Holder_Z = 0.5*inch;

  G4Box *Downstream_Holder_Solid_Solid = new G4Box("Downstream_Holder_Solid_Solid", Downstream_Holder_X*0.5, Downstream_Holder_Y*0.5, Downstream_Holder_Z*0.5);

  G4Box *Downstream_Holder_Hole_Solid = new G4Box("Downstream_Holder_Hole_Solid", 3.*inch, 3.*inch, Downstream_Holder_Z);

  G4SubtractionSolid* Downstream_Holder_Solid = new G4SubtractionSolid("Downstream_Holder_Solid", Downstream_Holder_Solid_Solid, Downstream_Holder_Hole_Solid, rotZ, G4ThreeVector(0., Downstream_Holder_Y*0.5, 0.));

  G4LogicalVolume *Downstream_Holder_Logical = new G4LogicalVolume(Downstream_Holder_Solid, Al, "Downstream_Holder_Logical");

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., -Downstream_Holder_Y*0.5, G3_Table_Length*0.5 - 1.75*inch - Downstream_Holder_Z*0.5), Downstream_Holder_Logical, "Downstream_Holder", World_Logical, false, 0, false);
  */
}
