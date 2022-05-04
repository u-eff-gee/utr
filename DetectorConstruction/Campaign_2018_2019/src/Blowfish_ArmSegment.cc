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

#include "Blowfish_ArmSegment.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

Blowfish_ArmSegment::Blowfish_ArmSegment() {
  theArmSegLogicalVolume = 0;
  contArmWidth = 0.0;
  outerRadius = 0.0;
  innerRadius = 0.0;
}

void Blowfish_ArmSegment::Construct(G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  G4double density;
  G4int ncomponents, natoms;
  // get a pointer to the needed materials
  G4Material *Aluminum = nist->FindOrBuildMaterial("G4_Al");
  G4Material *BC505 = G4Material::GetMaterial("BC505");
  G4Material *Poly = nist->FindOrBuildMaterial("G4_POLYETHYLENE"); // Polyethelene (Plastic)
  G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");
  // Polymethylmethacrylate (PMMA) aka acrylic, Lucite or Plexiglass
  G4Material *Lucite = nist->FindOrBuildMaterial("G4_LUCITE");
  G4Material *Glass = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Material *Silicone = new G4Material("Silicone_Rubber", density = 1.25 * g / cm3, ncomponents = 2);
  G4Element *elO = nist->FindOrBuildElement("O");
  G4Element *elSi = nist->FindOrBuildElement("Si");
  Silicone->AddElement(elSi, natoms = 1);
  Silicone->AddElement(elO, natoms = 1);

  //---------------------------------- The constant parameters

  // Much of Blowfish is designed to integral numbers of inches
  const G4double in = 2.54 * cm;

  // parameters for the aluminum arm
  // From Blowfish CAD drawing inner_bar_l_3-b.dwb
  // 1/2 x 3 1/2 x 60" Al stock rolled to inner radius of 21*in
  const G4double arm_inner_radius = 21 * in;
  const G4double arm_thickness = 0.5 * in;
  const G4double arm_width = 3.5 * in;

  // parameters for the hole in the arm
  // From Blowfish CAD drawing inner_bar_l_3-b.dwb
  const G4double hole_diam = (2.0 + 5.0 / 8.0) * in;

  // New aluminum support collars for the cells
  // include as part of each arm segment
  const G4double collar_length = 4. * in;
  const G4double collar_thick = 0.134 * in;
  const G4double collar_end_thick = 0.95 * in;
  const G4double collar_end_length = 0.5 * in;

  // front face of cell from centre of Blowfish
  const G4double cell_front_face_dist = 16. * in;

  const G4int num_cells = 11;
  const G4double cell_start_angle = 22.5 * deg;
  const G4double delta_angle = (180. * deg - 2. * cell_start_angle) / (num_cells - 1);
  // thickness of the cell walls
  const G4double wall_thickness = 1.0 / 8.0 * in;

  // active dimensions of the cell (square base)
  const G4double active_depth = 2.5 * in;
  const G4double active_width = 3.0 * in;

  // depth of the light guide
  //(not counting the part that is considered to
  // be the top of the cell)
  const G4double light_depth = 7.0 / 4.0 * in;

  // thickness of the wrapping
  // This prarameter is not known as well as
  // the others GET A BETTER VALUE IF POSSIBLE
  const G4double wrap_thickness = 0.5 * mm;

  // PMT Diameter used for the top of the light guide
  const G4double pmt_diam = 2.0 * in;

  // PMT Length and glass wall thickness
  const G4double pmt_length = 120. * mm;
  const G4double pmt_thick = 1.5 * mm; // Who knows?
  // PMT housing
  const G4double housing_length = 245. * mm;
  const G4double housing_thick = 3. / 16. * in;
  const G4double housing_diam = 2.5 * in;
  const G4double housing_overlap = 0.5 * cm;
  // Silicone cookie
  const G4double cookie_thick = 0.5 * cm;

  //---------------------------------- The arm container

  // The largest radius of the cell and segment
  const G4double cell_largest_radius = cell_front_face_dist +
                                       wrap_thickness +
                                       2. * wall_thickness +
                                       active_depth + // the cell
                                       light_depth + // light guide
                                       cookie_thick + // cookie
                                       housing_length - housing_overlap + // pmt housing
                                       housing_thick; // end cap

  // make the container volume slightly larger than the arm
  const G4double extra_outer_radius = 1.0 * cm;
  const G4double extra_inner_radius = 0.5 * mm; // a little extra to make david/dawn happy
  const G4double arm_Rmax = cell_largest_radius + extra_outer_radius;
  const G4double arm_Rmin = cell_front_face_dist - extra_inner_radius;
  contArmWidth = arm_width;
  const G4double collar_outer_diam = hole_diam + 2. * collar_end_thick;
  if (contArmWidth < collar_outer_diam)
    contArmWidth = collar_outer_diam;
  contArmWidth += 0.1 * mm;

  // sets the globals variables that are of interest to other classes
  outerRadius = arm_Rmax;
  innerRadius = arm_Rmin;

  // and make an arm segment container
  G4Tubs *armSegContTubs = new G4Tubs("ArmSegContainerTubs", arm_Rmin, arm_Rmax, contArmWidth / 2., 0., delta_angle);
  theArmSegLogicalVolume = new G4LogicalVolume(armSegContTubs, Air, "ArmSegContainerLV");

  // do we want to show the container volume

  // visible attributes for debugging
  G4VisAttributes *armContAtt = new G4VisAttributes();
  armContAtt->SetForceAuxEdgeVisible(true);
  armContAtt->SetForceWireframe(true);
  theArmSegLogicalVolume->SetVisAttributes(armContAtt);

  // invisible container
  //  		theArmSegLogicalVolume->SetVisAttributes( G4VisAttributes::Invisible);

  //---------------------------------- The aluminum arm
  // Just a arc segment
  G4Tubs *armAlTubs = new G4Tubs("ArmSegTubs", arm_inner_radius, arm_inner_radius + arm_thickness,
                                 arm_width / 2., 0., delta_angle);

  // now put a hole in the arm segment
  G4Tubs *holecyl = new G4Tubs("HoleCyl", 0., hole_diam / 2., arm_width, 0., 360. * deg);

  G4RotationMatrix *rotp = new G4RotationMatrix();
  G4double arm_radius = arm_inner_radius + arm_thickness / 2.;
  rotp->rotateY(-90. * deg);
  rotp->rotateZ(-delta_angle / 2.);
  G4ThreeVector position = global_coordinates + G4ThreeVector(0., 0., 0.);
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  G4SubtractionSolid *
      ArmSegWithHole = new G4SubtractionSolid("armhole", armAlTubs, holecyl, rotp, position);
  G4LogicalVolume *ArmSegWithHoleLV = new G4LogicalVolume(ArmSegWithHole, Aluminum, "ArmSegWithHoleLV");
  // the arm's visual attributes
  G4Colour yellowOffWhite = G4Colour(1.0, 1.0, 224.0 / 255.0);
  G4VisAttributes *armAtt = new G4VisAttributes(yellowOffWhite);
  armAtt->SetForceSolid(true);
  ArmSegWithHoleLV->SetVisAttributes(armAtt);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., 0.), ArmSegWithHoleLV, "Arm Al segment place", theArmSegLogicalVolume, false, 0);

  // add cell support collar
  G4double collar_diam = housing_diam + 0.01 * in;
  G4Tubs *colend = new G4Tubs("collar end",
                              collar_diam / 2., collar_outer_diam / 2., collar_end_length / 2., 0., 360. * deg);
  G4LogicalVolume *colendLV = new G4LogicalVolume(colend, Aluminum, "CollarEndLV");
  colendLV->SetVisAttributes(armAtt);
  arm_radius += arm_thickness / 2. + collar_end_length / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  G4RotationMatrix pRot;
  pRot.rotateY(90. * deg);
  pRot.rotateZ(delta_angle / 2.);
  // new G4PVPlacement(rotp, position, colendLV, "Collar end 1 place", theArmSegLogicalVolume, false, 0);
  new G4PVPlacement(G4Transform3D(pRot, position), colendLV, "Collar end 1", theArmSegLogicalVolume, false, 0);
  G4Tubs *colsleeve = new G4Tubs("collar sleeve",
                                 collar_diam / 2., collar_diam / 2. + collar_thick, collar_length / 2. - collar_end_length, 0., 360. * deg);
  G4LogicalVolume *colsleeveLV = new G4LogicalVolume(colsleeve, Aluminum, "CollarSleeveLV");
  colsleeveLV->SetVisAttributes(armAtt);
  arm_radius += collar_length / 2. - collar_end_length / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), colsleeveLV, "Collar sleeve", theArmSegLogicalVolume, false, 0);
  arm_radius += collar_length / 2. - collar_end_length / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), colendLV, "Collar end 2", theArmSegLogicalVolume, false, 0);
  // ------------------ The Cell --------------------------
  // Russian doll style: BC505 inside Lucite inside wrapper

  // a box that will become the wrapper after another box is placed inside it
  const G4double wrap_z = wrap_thickness + 2.0 * wall_thickness + active_depth;
  const G4double wrap_xy = active_width + 2. * wall_thickness + 2. * wrap_thickness;

  G4Box *wrapBox = new G4Box("CellWrapBox", wrap_xy / 2.0, wrap_xy / 2.0, wrap_z / 2.0);
  G4LogicalVolume *wrapLV = new G4LogicalVolume(wrapBox, Poly, "CellWrapLV");

  // sets attributes: user determines if it is solid or wireframe
  // Color is "dark slate blue"
  G4VisAttributes *wrapAtt = new G4VisAttributes(G4Colour(72.0 / 256.0, 61.0 / 256.0, 139.0 / 256.0));
  wrapAtt->SetForceSolid(true);
  wrapLV->SetVisAttributes(wrapAtt);

  //---------------------------------- The cell's Lucite box

  // a box that will become a container after the BC505 box is placed inside
  const G4double lucite_z = 2.0 * wall_thickness + active_depth;
  const G4double lucite_xy = 2.0 * wall_thickness + active_width;
  G4Box *luciteBox = new G4Box("CellLuciteBox", lucite_xy / 2.0, lucite_xy / 2.0, lucite_z / 2.0);
  G4LogicalVolume *luciteLV = new G4LogicalVolume(luciteBox, Lucite, "CellLuciteLV");

  // inside the wrapper box
  const G4ThreeVector lucitePos = global_coordinates + G4ThreeVector(0.0, 0.0, wrap_thickness / 2.);
  new G4PVPlacement(0, lucitePos, luciteLV, "CellLucitePhys", wrapLV, false, 0);

  // sets attributes (used for all Lucite objects)
  G4VisAttributes *luciteAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); // white
  luciteAtt->SetForceSolid(true);
  luciteLV->SetVisAttributes(luciteAtt);

  //------------------- The cell's Active volume: BC-505 and sensitive detector
  G4Box *activeBox = new G4Box("CellActiveBox", active_width / 2.0, active_width / 2.0, active_depth / 2.0);
  G4LogicalVolume *activeLV = new G4LogicalVolume(activeBox, BC505, "CellActiveLV");

  // inside the Lucite box
  const G4ThreeVector activePos = global_coordinates + G4ThreeVector(0.0, 0.0, 0.0);
  new G4PVPlacement(0, activePos, activeLV, "CellActivePhys", luciteLV, false, 0);

  // set the visible attribues: always solid blue
  G4VisAttributes *activeAtt =
      new G4VisAttributes(G4Colour(100.0 / 256.0, 149.0 / 256.0, 237.0 / 256.0)); // Cornflower blue
  activeAtt->SetForceSolid(true);
  activeLV->SetVisAttributes(activeAtt);

  // place it in the segment
  arm_radius = cell_front_face_dist + wrap_z / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), wrapLV, "Cell", theArmSegLogicalVolume, false, 0);

  // Now the light guide
  // make a cone and cut the sides off with boxes
  G4double wid = active_width + 2.0 * wall_thickness;
  G4double bottom_diam = wid * sqrt(2.);
  G4Cons *LGconeraw = new G4Cons("LGcone", 0., bottom_diam / 2., 0., pmt_diam / 2., light_depth / 2., 0., 360. * deg);
  G4double add = 0.001 * in; // because we loose precision in the rotations
  G4double hi = light_depth / 2.;
  G4double boxwid = (bottom_diam - wid) / 2. + add;
  // box to use as a cutter
  G4Box *Cutbox = new G4Box("CutBox", boxwid / 2., wid / 2., hi / 2. + add);
  G4double cut_d = (wid + boxwid + add) / 2.;
  G4ThreeVector cutposition(cut_d, 0., -hi / 2.);
  G4RotationMatrix *cutrot = new G4RotationMatrix();
  G4SubtractionSolid *LGconecut1 =
      new G4SubtractionSolid("LGconecut", LGconeraw, Cutbox, cutrot, cutposition);
  cutrot->rotateZ(90.0 * deg);
  cutposition.setX(0.);
  cutposition.setY(-cut_d);
  G4SubtractionSolid *LGconecut2 =
      new G4SubtractionSolid("LGconecut", LGconecut1, Cutbox, cutrot, cutposition);
  cutrot->rotateZ(90.0 * deg);
  cutposition.setX(-cut_d);
  cutposition.setY(0.);
  G4SubtractionSolid *LGconecut3 =
      new G4SubtractionSolid("LGconecut", LGconecut2, Cutbox, cutrot, cutposition);
  cutrot->rotateZ(90.0 * deg);
  cutposition.setX(0.);
  cutposition.setY(cut_d);
  G4SubtractionSolid *LGconecut =
      new G4SubtractionSolid("LGconecut", LGconecut3, Cutbox, cutrot, cutposition);
  G4LogicalVolume *LightGuideLV = new G4LogicalVolume(LGconecut, Lucite, "Lightguide LV");
  LightGuideLV->SetVisAttributes(luciteAtt);

  // place it in the segment
  arm_radius += wrap_z / 2. + light_depth / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), LightGuideLV, "Light Guide", theArmSegLogicalVolume, false, 0);
  // Silicone cookie
  G4Tubs *cookie_cyl = new G4Tubs("cookieS", 0., pmt_diam / 2., cookie_thick / 2., 0., 360. * deg);
  G4LogicalVolume *CookieLV = new G4LogicalVolume(cookie_cyl, Silicone, "CookieLV");
  CookieLV->SetVisAttributes(luciteAtt);
  arm_radius += light_depth / 2. + cookie_thick / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), CookieLV, "Silicone Cookie", theArmSegLogicalVolume, false, 0);

  // Phototube - modeled as just a glass tube
  G4Tubs *pmt_cyl = new G4Tubs("pmtS", pmt_diam / 2. - pmt_thick, pmt_diam / 2., pmt_length / 2., 0., 360. * deg);
  G4LogicalVolume *pmtLV = new G4LogicalVolume(pmt_cyl, Glass, "pmtLV");
  pmtLV->SetVisAttributes(luciteAtt);
  arm_radius += cookie_thick / 2. + pmt_length / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), pmtLV, "Phototube", theArmSegLogicalVolume, false, 0);

  // Housing around tube - modeled as a plastic tube
  G4Tubs *housing_cyl = new G4Tubs("housingS", housing_diam / 2. - housing_thick, housing_diam / 2., housing_length / 2., 0., 360. * deg);
  G4LogicalVolume *housingLV = new G4LogicalVolume(housing_cyl, Poly, "housingLV");
  G4VisAttributes *housingAtt = new G4VisAttributes(G4Colour(21.0 / 256.0, 10.0 / 256.0, 120.0 / 256.0)); // deep blue
  housingAtt->SetForceSolid(true);
  housingLV->SetVisAttributes(housingAtt);
  arm_radius += (-pmt_length / 2.) - housing_overlap + housing_length / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), housingLV, "PMT Housing", theArmSegLogicalVolume, false, 0);

  // and put and end cap on it
  G4Tubs *end_cyl = new G4Tubs("housingEnd", 0., housing_diam / 2., housing_thick / 2., 0., 360. * deg);
  G4LogicalVolume *endLV = new G4LogicalVolume(end_cyl, Poly, "endLV");
  endLV->SetVisAttributes(housingAtt);
  arm_radius += housing_length / 2. + housing_thick / 2.;
  position.setX(arm_radius * cos(delta_angle / 2.));
  position.setY(arm_radius * sin(delta_angle / 2.));
  new G4PVPlacement(G4Transform3D(pRot, position), endLV, "Housing Endcap", theArmSegLogicalVolume, false, 0);
}
