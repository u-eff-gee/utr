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
// of the table plate.

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G3_Table.hh"
#include "Units.hh"

#include "Bricks.hh"

G3_Table::G3_Table(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                                 G3_Table_Length(36.25 * inch) {}

void G3_Table::Construct(G4ThreeVector global_coordinates) {

  G4Colour green(0., 0.5, 0.);
  G4Colour yellow(1.0, 1.0, 0.0);
  G4Colour grey(0.5, 0.5, 0.5);
  G4Colour white(1.0, 1.0, 1.0);

  G4NistManager *nist = G4NistManager::Instance();

  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

  G4double G3_Table_X = 21. * inch;

  // The following three variables are need for the construction of the
  // 18/19 geometry parts
  G4double Lead_Wall_Y = 12. * inch;
  G4double Concrete_Base_Y = 7.25 * inch;
  G4double Plastic_Base_Y = 1. * inch;

  /*********************************************************
   * Upstream concrete and lead shielding
   ********************************************************/

  G4double FirstLayer_OffsetY = -8. * mm; // Measured
  G4double BeamTube_Outer_Radius = 1. * inch;

  NormBrick *nb = new NormBrick(World_Logical);
  ShortNormBrick *snb = new ShortNormBrick(World_Logical);

  // First layer in beam direction
  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 5.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 4.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 3.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 3.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 2.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 2.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5 - BeamTube_Outer_Radius,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 1.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5 + BeamTube_Outer_Radius,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 1.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5 - BeamTube_Outer_Radius,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5 + BeamTube_Outer_Radius,
          global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 0.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 0.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 1.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 1.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 2.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 2.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 4.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 4.5 + FirstLayer_OffsetY,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 0.5, 0., 90. * deg, 0.);

  // Second layer in beam direction

  nb->Put(global_coordinates.x() - BeamTube_Outer_Radius - nb->M * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. + nb->L * 0.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->S * 0.5, 90. * deg, 0. * deg,
          0. * deg);
  nb->Put(global_coordinates.x() + BeamTube_Outer_Radius + nb->M * 0.5,
          global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. + nb->L * 0.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->S * 0.5, 90. * deg, 0. * deg,
          0. * deg);
  nb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + nb->S * 0.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() - nb->L * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 4.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x() + nb->L * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 4.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M + nb->M * 0.5, 0., 90. * deg, 0.);

  // Third layer in beam direction

  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 0.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2., 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 1.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2., 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 2.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2., 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - nb->M * 0.5,
          global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2. + nb->S * 0.5, 0., 90. * deg,
          90. * deg);
  snb->Put(global_coordinates.x() - BeamTube_Outer_Radius - nb->M * 0.5,
           global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5,
           global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2.);
  snb->Put(global_coordinates.x() + BeamTube_Outer_Radius + nb->M * 0.5,
           global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5,
           global_coordinates.z() - G3_Table_Length * 0.5 + nb->M * 2.);

  // Plastic (polyethylene) layer below the upstream lead wall to fill the space between
  // the wall and the table top. Actually, it was a mixed layer of plastic and wooden
  // plates.
  G4double Lead_Wall_Plastic_Bottom_Thickness = 0.5 * Lead_Wall_Y + Concrete_Base_Y + Plastic_Base_Y - 2. * inch - 5. * nb->S + FirstLayer_OffsetY;
  G4Box *Lead_Wall_Plastic_Bottom_Solid = new G4Box("Lead_Wall_Plastic_Bottom_Solid", nb->L, 0.5 * Lead_Wall_Plastic_Bottom_Thickness, 1.5 * nb->M);
  G4LogicalVolume *Lead_Wall_Plastic_Bottom_Logical = new G4LogicalVolume(Lead_Wall_Plastic_Bottom_Solid, PE, "Lead_Wall_Plastic_Bottom_Logical");
  Lead_Wall_Plastic_Bottom_Logical->SetVisAttributes(G4Colour::Yellow());
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch - 0.5 * Lead_Wall_Y - Concrete_Base_Y - Plastic_Base_Y + Lead_Wall_Plastic_Bottom_Thickness * 0.5, -G3_Table_Length * 0.5 + 1.5 * nb->M), Lead_Wall_Plastic_Bottom_Logical, "Lead_Wall_Plastic_Bottom", World_Logical, false, 0, false);

  /*********************************************************
   * Downstream concrete and lead shielding
   ********************************************************/

  HalfShortBrickWithHole *hsbh = new HalfShortBrickWithHole(World_Logical);

  G4double G3_Table_Table2_Overlap = 2.25 * inch; // This is how far the beam pipe holder of
  // Table 2 extends into the G3 table on the downstream side. It is important for the placement
  // of the lead shielding.
  G4double Wall7_Width = 2. * nb->L + nb->M;

  for (int i = 0; i < 7; i++) {
    nb->Put(global_coordinates.x() + nb->L - Wall7_Width * 0.5 - nb->L * 0.5,
            global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * (0.5 - i),
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L + Wall7_Width * 0.5 + nb->L * 0.5,
            global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * (0.5 - i),
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);
    if (i > 1) {
      snb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * (0.5 - i),
               global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5);
    }
  }

  nb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * 1.5,
          global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * 2.5,
          global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * 3.5,
          global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);
  nb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius + nb->S * 4.5,
          global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S + nb->M * 0.5, 0., 90. * deg, 0.);

  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + nb->S / 2. - hsbh->Hole_Radius,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 0.5, -90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius - nb->S / 2.,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 0.5, 90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + nb->S / 2. - hsbh->Hole_Radius,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 1.5, -90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius - nb->S / 2.,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 1.5, 90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + nb->S / 2. - hsbh->Hole_Radius,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 2.5, -90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius - hsbh->Hole_Radius - nb->S / 2.,
            global_coordinates.z() + G3_Table_Length * 0.5 - G3_Table_Table2_Overlap - 3. * nb->S + nb->S * 2.5, 90. * deg, 90. * deg, 0.);

  // Table plate
  G4double G3_Table_Plate_Thickness = 0.5 * inch; // Estimated, probably too large for the geometry that is really inside

  G4Box *G3_Table_Plate_Solid = new G4Box("G3_Table_Plate_Solid", G3_Table_X * 0.5, G3_Table_Plate_Thickness * 0.5, G3_Table_Length * 0.5);
  G4LogicalVolume *G3_Table_Plate_Logical = new G4LogicalVolume(G3_Table_Plate_Solid, Al, "G3_Table_Plate_Logical");
  G3_Table_Plate_Logical->SetVisAttributes(grey);

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 1. * inch - 0.5 * Lead_Wall_Y - Concrete_Base_Y - Plastic_Base_Y - G3_Table_Plate_Thickness * 0.5, 0.), G3_Table_Plate_Logical, "G3_Table_Plate", World_Logical, false, 0, false);
}
