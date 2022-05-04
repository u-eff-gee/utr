/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2019 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.)
utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Filter_Case.hh"
#include "Units.hh"

using std::stringstream;

// All dimensions in this file were read off from a technical drawing by
// B. Loeher and J. Isaak if not indicated otherwise

Filter_Case::Filter_Case(G4LogicalVolume *World_Logical, G4String name) : World_Log(World_Logical),
                                                                          filter_case_name(name),
                                                                          filter_case_ring_thickness(0.1 * inch), // Estimated from thickness of filter case bottom
                                                                          filter_case_bottom_thickness(0.1 * inch) {}

void Filter_Case::Construct_Ring(G4ThreeVector global_coordinates, G4double theta,
                                 G4double phi, G4double dist_from_center) {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector symmetry_axis(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)); // Symmetry axis along which the ring is constructed
  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);

  G4double filter_case_ring_outer_radius = 1.78 * inch;
  G4double filter_case_ring_inner_radius = 1.5 * inch;

  stringstream filter_case_ring_solid_name;
  filter_case_ring_solid_name << "filter_case_ring_" << filter_case_name << "_solid";
  G4Tubs *filter_case_ring_solid = new G4Tubs(filter_case_ring_solid_name.str(),
                                              filter_case_ring_inner_radius, filter_case_ring_outer_radius,
                                              filter_case_ring_thickness * 0.5, 0., twopi);

  stringstream filter_case_ring_logical_name;
  filter_case_ring_logical_name << "filter_case_ring_" << filter_case_name << "_logical";
  G4LogicalVolume *filter_case_ring_logical = new G4LogicalVolume(filter_case_ring_solid,
                                                                  nist->FindOrBuildMaterial("G4_POLYETHYLENE"), filter_case_ring_logical_name.str());
  filter_case_ring_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));

  stringstream filter_case_ring_name;
  filter_case_ring_name << "filter_case_ring_" << filter_case_name << "";
  new G4PVPlacement(rotation, global_coordinates + dist_from_center * symmetry_axis, filter_case_ring_logical, filter_case_ring_name.str(), World_Log, 0, 0, false);
}

void Filter_Case::Construct_Case(G4ThreeVector global_coordinates, G4double theta,
                                 G4double phi, G4double dist_from_center) {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector symmetry_axis(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)); // Symmetry axis along which the ring is constructed
  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);

  G4double filter_case_length = 2. * inch;
  G4double filter_case_outer_radius = 1.85 * inch;
  G4double filter_case_inner_radius = 1.78 * inch;

  G4double filter_case_bottom_outer_radius = 1.78 * inch;
  G4double filter_case_bottom_inner_radius = 1.5 * inch;

  stringstream filter_case_solid_name;
  filter_case_solid_name << "filter_case_" << filter_case_name << "_solid";
  G4Tubs *filter_case_solid = new G4Tubs(filter_case_solid_name.str(),
                                         filter_case_inner_radius, filter_case_outer_radius,
                                         filter_case_length * 0.5, 0., twopi);

  stringstream filter_case_logical_name;
  filter_case_logical_name << "filter_case_" << filter_case_name << "_logical";
  G4LogicalVolume *filter_case_logical = new G4LogicalVolume(filter_case_solid,
                                                             nist->FindOrBuildMaterial("G4_POLYETHYLENE"), filter_case_logical_name.str());
  filter_case_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));

  stringstream filter_case_case_name;
  filter_case_case_name << "filter_case_" << filter_case_name << "";
  new G4PVPlacement(rotation, global_coordinates + (dist_from_center + filter_case_length * 0.5 - filter_case_bottom_thickness * 0.5) * symmetry_axis, filter_case_logical, filter_case_case_name.str(), World_Log, 0, 0, false);

  stringstream filter_case_bottom_solid_name;
  filter_case_bottom_solid_name << "filter_case_bottom_" << filter_case_name << "_solid";
  G4Tubs *filter_case_bottom_solid = new G4Tubs(filter_case_bottom_solid_name.str(),
                                                filter_case_bottom_inner_radius, filter_case_bottom_outer_radius,
                                                filter_case_bottom_thickness * 0.5, 0., twopi);

  stringstream filter_case_bottom_logical_name;
  filter_case_bottom_logical_name << "filter_case_bottom_" << filter_case_name << "_logical";
  G4LogicalVolume *filter_case_bottom_logical = new G4LogicalVolume(filter_case_bottom_solid,
                                                                    nist->FindOrBuildMaterial("G4_POLYETHYLENE"), filter_case_bottom_logical_name.str());
  filter_case_bottom_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));

  stringstream filter_case_bottom_name;
  filter_case_bottom_name << "filter_case_bottom_" << filter_case_name << "";
  new G4PVPlacement(rotation, global_coordinates + dist_from_center * symmetry_axis, filter_case_bottom_logical, filter_case_bottom_name.str(), World_Log, 0, 0, false);
}
