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

#include "G3_Wall.hh"
#include "Units.hh"

#include "Bricks.hh"

G3_Wall::G3_Wall(G4LogicalVolume *World_Log) : World_Logical(World_Log) {}

void G3_Wall::Construct(G4ThreeVector global_coordinates) {

  G4Colour green(0., 0.5, 0.);
  G4Colour white(1.0, 1.0, 1.0);

  G4NistManager *nist = G4NistManager::Instance();

  G4double BeamTube_Outer_Radius = 1. * inch;

  NormBrick *nb = new NormBrick(World_Logical);
  FlatFlatThinNormBrick *fftb = new FlatFlatThinNormBrick(World_Logical);
  ConcreteBrick *cb = new ConcreteBrick(World_Logical);
  FlatConcreteBrick *fcb = new FlatConcreteBrick(World_Logical);
  HalfShortBrickWithHole *hsbh = new HalfShortBrickWithHole(World_Logical);

  for (int i = 0; i < 3; i++) {
    nb->Put(global_coordinates.x() - nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 3.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 3.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 2.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 2.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2. - BeamTube_Outer_Radius,
            global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 1.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2. + BeamTube_Outer_Radius,
            global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 1.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2. - BeamTube_Outer_Radius,
            global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2. + BeamTube_Outer_Radius,
            global_coordinates.y() - BeamTube_Outer_Radius + nb->S * 0.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 0.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 0.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 1.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 1.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() - nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 2.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
    nb->Put(global_coordinates.x() + nb->L / 2., global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 2.5,
            global_coordinates.z() - nb->M * (i + 0.5), 0., 90. * deg, 0.);
  }

  // Fill the gap between beam pipe and wall by stuffing in flat stripes of
  // lead

  fftb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + fftb->S * 0.5,
            global_coordinates.z() - nb->M * 3. + fftb->L * 0.5);
  fftb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + fftb->S * 1.5,
            global_coordinates.z() - nb->M * 3. + fftb->L * 0.5);
  fftb->Put(global_coordinates.x(), global_coordinates.y() + BeamTube_Outer_Radius + fftb->S * 2.5,
            global_coordinates.z() - nb->M * 3. + fftb->L * 0.5);

  // Brick with hole in front of wall

  hsbh->Put(global_coordinates.x(), hsbh->M * 0.5, global_coordinates.z() - nb->M * 3. - hsbh->M * 0.5,
            -90. * deg, 90. * deg, 0.);
  hsbh->Put(global_coordinates.x(), -hsbh->M * 0.5, global_coordinates.z() - nb->M * 3. - hsbh->M * 0.5,
            90. * deg, 90. * deg, 0.);

  // Concrete blocks below Wall3

  cb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M * 0.5,
          global_coordinates.z() - nb->M * 3. + cb->M, 0., 90. * deg, 0.);
  fcb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M * 0.5,
           global_coordinates.z() - nb->M * 3. + fcb->S * 0.5, 0., 90. * deg,
           90. * deg);

  // Aluminium plate below Wall3

  G4double AlPlate_X = 460. * mm; // Measured
  G4double AlPlate_Y = 20. * mm; // Measured
  G4double AlPlate_Z = 405. * mm; // Measured

  G4Box *AlPlate_Solid = new G4Box("AlPlate_Solid", AlPlate_X * 0.5,
                                   AlPlate_Y * 0.5, AlPlate_Z * 0.5);
  G4LogicalVolume *AlPlate_Logical =
      new G4LogicalVolume(AlPlate_Solid, nist->FindOrBuildMaterial("G4_Al"), "AlPlate_Logical", 0, 0, 0);

  AlPlate_Logical->SetVisAttributes(G4Colour::Grey());

  new G4PVPlacement(
      0, G4ThreeVector(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M - AlPlate_Y * 0.5, global_coordinates.z() - nb->M * 3 + AlPlate_Z * 0.5),
      AlPlate_Logical, "AlPlate", World_Logical, false, 0);

  // Concrete Blocks below aluminium plate

  fcb->Put(global_coordinates.x(), global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M - AlPlate_Y - fcb->M * 0.5,
           global_coordinates.z() - nb->M * 3 + fcb->L * 0.5, 0., 0., 90. * deg);
  cb->Put(global_coordinates.x() - fcb->S * 0.5 - cb->M * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M - AlPlate_Y - cb->M * 0.5,
          global_coordinates.z() - nb->M * 3. + cb->L * 0.5);
  cb->Put(global_coordinates.x() + fcb->S * 0.5 + cb->M * 0.5, global_coordinates.y() - BeamTube_Outer_Radius - nb->S * 3. - cb->M - AlPlate_Y - cb->M * 0.5,
          global_coordinates.z() - nb->M * 3. + cb->L * 0.5);

  // Lead wraps around the beam pipe

  G4double lead_wrap_thickness = 1.8 * mm; // Measured
  G4double lead_wrap_length = 140 * mm; // Measured

  G4Tubs *lead_wrap_single_solid = new G4Tubs("lead_wrap_single_solid", BeamTube_Outer_Radius,
                                              BeamTube_Outer_Radius + lead_wrap_thickness, lead_wrap_length * 0.5, 0., twopi);
  G4Tubs *lead_wrap_double_solid = new G4Tubs("lead_wrap_double_solid", BeamTube_Outer_Radius,
                                              BeamTube_Outer_Radius + 2. * lead_wrap_thickness, lead_wrap_length * 0.5, 0., twopi);

  G4LogicalVolume *lead_wrap_upstream_logical = new G4LogicalVolume(lead_wrap_single_solid, nist->FindOrBuildMaterial("G4_Pb"), "lead_wrap_upstream_logical");
  lead_wrap_upstream_logical->SetVisAttributes(G4Colour::Green());
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -nb->M * 3. - nb->S - lead_wrap_length * 0.5), lead_wrap_upstream_logical, "lead_wrap_upstream", World_Logical, false, 0, false);

  G4LogicalVolume *lead_wrap_downstream_double_logical = new G4LogicalVolume(lead_wrap_double_solid, nist->FindOrBuildMaterial("G4_Pb"), "lead_wrap_downstream_double_logical");
  lead_wrap_downstream_double_logical->SetVisAttributes(G4Colour::Green());
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., lead_wrap_length * 0.5), lead_wrap_downstream_double_logical, "lead_wrap_downstream_double", World_Logical, false, 0, false);

  G4LogicalVolume *lead_wrap_downstream_single_logical = new G4LogicalVolume(lead_wrap_single_solid, nist->FindOrBuildMaterial("G4_Pb"), "lead_wrap_downstream_single_logical");
  lead_wrap_downstream_single_logical->SetVisAttributes(G4Colour::Green());
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., lead_wrap_length * 1.5), lead_wrap_downstream_single_logical, "lead_wrap_downstream_single", World_Logical, false, 0, false);
}
