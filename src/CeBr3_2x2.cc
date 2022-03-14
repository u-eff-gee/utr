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

#include <sstream>
using std::stringstream;

#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "CeBr3_2x2.hh"

void CeBr3_2x2::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center, G4double intrinsic_rotation_angle) const {
  /*********** Dimensions ***********/

  // Total length of the CeBr disregarding the signal and HV connectors
  const auto total_housing_length = 220. * mm;

  // Metal casing covering the crystal and PMT.
  // Assumed to continue all the way even below the magnetic shielding.
  // Hence for most of the detector there are two metal walls: This case and the magnetic shielding.
  // From the technical drawings it's hard to tell whether this is really the case, but there is anyway barely any information on the PMT part and due to the lack of information most of the detector is anyway an empty metal casing...
  // So one thin wall more or less doesn't really matter
  const auto main_case_length = 185. * mm; // From nutr, probably estimated or measured
  const auto main_case_wall_thickness = 0.4 * mm; // From technical drawing
  const auto main_case_entrance_window_thickness = main_case_wall_thickness; // Technical drawing gives no reason to assume it should be different from main_wall_thickness, but chose to implement an independent variable anyway.
  const auto main_case_outer_radius = 57. / 2. * mm; // From technical drawing
  const auto main_case_inner_radius = main_case_outer_radius - main_case_wall_thickness;
  const auto main_case_inner_length = main_case_length - main_case_entrance_window_thickness - main_case_wall_thickness;

  const auto magnetic_shielding_offset = 42.5 * mm; // From technical drawing
  const auto magnetic_shielding_length = main_case_length - magnetic_shielding_offset;
  const auto magnetic_shielding_wall_thickness = 0.64 * mm; // From technical drawing
  const auto magnetic_shielding_outer_radius = 58.8 / 2. * mm; // From technical drawing
  const auto magnetic_shielding_inner_radius = magnetic_shielding_outer_radius - magnetic_shielding_wall_thickness;

  // Crystal
  // The crystal dimensions are given as '51 mm x 51 mm', but this is probably the rounded value of 2 x 2 inch.
  // Take the exact value here.
  const auto crystal_length = 50.8 * mm;
  const auto crystal_and_pmt_radius = 50.8 / 2. * mm;
  const auto crystal_to_entrance_window_gap = main_case_inner_radius - crystal_and_pmt_radius; // Estimated that the gap between the crystal and the entrance window is as wide as the gap between the crystal and the case on its side.

  // PMT
  const auto pmt_length = main_case_inner_length - crystal_to_entrance_window_gap - crystal_length; // Assume it extends all the way to the back of the main case.
  const auto pmt_wall_thickness = main_case_wall_thickness; // Estimated to be the same as the thickness of the case wall. Not clear from the drawing, which leaves out most of the PMT anyway.

  // Connector
  const auto connector_base_length = total_housing_length - main_case_length;
  const auto connector_base_wall_thickness = main_case_wall_thickness; // Assumption
  const auto connector_base_outer_radius = magnetic_shielding_outer_radius; // From technical drawing
  const auto connector_signal_radius = 3. * mm; // Estimated
  const auto connector_signal_length = 8. * mm; // Estimated
  const auto connector_hv_radius = 3. * mm; // Estimated
  const auto connector_hv_length = 8. * mm; // Estimated

  /*********** Materials ***********/

  G4NistManager *nist = G4NistManager::Instance();
  const G4String main_case_material = "G4_Al";
  const G4String pmt_material = "G4_Al"; // Assumption
  const G4String magnetic_shielding_material = "G4_Al"; // Assumption (it is called 'magnetic shield' in the drawing)
  const G4String connector_material = "G4_Al"; // Assume that the connectors on top are made of the same material
  auto *CeBr3 = new G4Material("CeBr3", 5.1 * g / cm3, 2); // Density from Wikipedia
  CeBr3->AddElement(nist->FindOrBuildElement("Ce"), 1);
  CeBr3->AddElement(nist->FindOrBuildElement("Br"), 3);

  /*********** Orientation in space ***********/

  G4ThreeVector e_r = G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
  G4ThreeVector e_theta = G4ThreeVector(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta));
  G4ThreeVector e_phi = G4ThreeVector(-sin(phi), cos(phi), 0.);
  auto *rotation_matrix = new G4RotationMatrix();
  rotation_matrix->rotateZ(-phi);
  rotation_matrix->rotateY(-theta);
  if (intrinsic_rotation_angle != 0.) {
    rotation_matrix->rotateZ(intrinsic_rotation_angle);
  }

  /*********** Front ***********/

  // Main case, mother volume for all internals

  auto *main_case_solid = new G4Tubs(detector_name + "_main_case_solid", 0., main_case_outer_radius, main_case_length / 2., 0., twopi);
  auto *main_case_logical = new G4LogicalVolume(main_case_solid, nist->FindOrBuildMaterial(main_case_material), detector_name + "_main_case_logical");
  main_case_logical->SetVisAttributes(G4Color::Grey());
  new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + main_case_length / 2.) * e_r, main_case_logical, detector_name + "_main_case", world_Logical, 0, 0, false);

  // Main case vacuum mother volume for crystal and pmt, which are assumed to be sourounded by this vacuum

  auto *main_case_vacuum_solid = new G4Tubs(detector_name + "_main_case_vacuum_solid", 0., main_case_inner_radius, main_case_inner_length / 2., 0., twopi);
  auto *main_case_vacuum_logical = new G4LogicalVolume(main_case_vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"), detector_name + "_main_case_vacuum_logical");
  main_case_vacuum_logical->SetVisAttributes(G4Color(1., 1., 1., 0.75));
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, (main_case_entrance_window_thickness - main_case_wall_thickness) / 2.), main_case_vacuum_logical, detector_name + "_main_case_vacuum", main_case_logical, 0, 0, false);

  // Crystal

  auto *crystal_solid = new G4Tubs(detector_name + "_crystal_solid", 0., crystal_and_pmt_radius, crystal_length / 2., 0., twopi);
  auto *crystal_logical = new G4LogicalVolume(crystal_solid, CeBr3, detector_name);
  crystal_logical->SetVisAttributes(G4Color::Green());
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -main_case_inner_length / 2. + crystal_to_entrance_window_gap + crystal_length / 2.), crystal_logical, detector_name + "_crystal", main_case_vacuum_logical, 0, 0, false);

  // PMT wall (PMT is a hollow cylinder here)

  auto *pmt_solid = new G4Tubs(detector_name + "_pmt_solid", crystal_and_pmt_radius - pmt_wall_thickness, crystal_and_pmt_radius, pmt_length / 2., 0., twopi);
  auto *pmt_logical = new G4LogicalVolume(pmt_solid, nist->FindOrBuildMaterial(pmt_material), detector_name + "_pmt_logical");
  pmt_logical->SetVisAttributes(G4Color::Blue());
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, main_case_inner_length / 2 - pmt_length / 2.), pmt_logical, detector_name + "_pmt", main_case_vacuum_logical, 0, 0, false);

  // Magnetic shielding

  auto *magnetic_shielding_solid = new G4Tubs(detector_name + "_magnetic_shielding_solid", magnetic_shielding_inner_radius, magnetic_shielding_outer_radius, magnetic_shielding_length / 2., 0., twopi);
  auto *magnetic_shielding_logical = new G4LogicalVolume(magnetic_shielding_solid, nist->FindOrBuildMaterial(magnetic_shielding_material), detector_name + "_magnetic_shielding_logical");
  magnetic_shielding_logical->SetVisAttributes(G4Color(0.75, 0.75, 0.75));
  new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + magnetic_shielding_offset + magnetic_shielding_length / 2.) * e_r, magnetic_shielding_logical, detector_name + "_magnetic_shielding", world_Logical, 0, 0, false);

  // Connector base

  auto *connector_base_solid = new G4Tubs(detector_name + "_connector_base_solid", 0., connector_base_outer_radius, 0.5 * connector_base_length, 0., twopi);
  auto *connector_base_logical = new G4LogicalVolume(connector_base_solid, nist->FindOrBuildMaterial(connector_material), detector_name + "_connector_base_logical");
  connector_base_logical->SetVisAttributes(G4Color::Grey());
  new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + main_case_length + 0.5 * connector_base_length) * e_r, connector_base_logical, detector_name + "_connector_base", world_Logical, 0, 0, false);

  auto *connector_base_inside_solid = new G4Tubs(detector_name + "_connector_base_inside_solid", 0., connector_base_outer_radius - connector_base_wall_thickness, connector_base_length / 2. - connector_base_wall_thickness, 0., twopi);
  auto *connector_base_inside_logical = new G4LogicalVolume(connector_base_inside_solid, nist->FindOrBuildMaterial("G4_AIR"), detector_name + "_connector_base_inside_logical"); // Assume it is filled predominantly with low-density material.
  connector_base_inside_logical->SetVisAttributes(G4Color::White());
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), connector_base_inside_logical, detector_name + "_connector_base_inside", connector_base_logical, 0, 0, false);

  if (use_connectors) {
    // HV connector
    auto *connector_hv_solid = new G4Tubs(detector_name + "_connector_hv_solid", 0., connector_hv_radius, 0.5 * connector_hv_length, 0., twopi);
    auto *connector_hv_logical = new G4LogicalVolume(connector_hv_solid, nist->FindOrBuildMaterial(connector_material), detector_name + "_connector_hv_logical");
    connector_hv_logical->SetVisAttributes(G4Color::Grey());
    new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + total_housing_length + 0.5 * connector_hv_length) * e_r + 0.5 / sqrt(2.) * connector_base_outer_radius * e_theta + 0.5 / sqrt(2.) * connector_base_outer_radius * e_phi, connector_hv_logical, detector_name + "_connector_hv", world_Logical, 0, 0, false);

    // Signal connector
    auto *connector_signal_solid = new G4Tubs(detector_name + "_connector_signal_solid", 0., connector_signal_radius, 0.5 * connector_signal_length, 0., twopi);
    auto *connector_signal_logical = new G4LogicalVolume(connector_signal_solid, nist->FindOrBuildMaterial(connector_material), detector_name + "_connector_signal_logical");
    connector_signal_logical->SetVisAttributes(G4Color::Grey());
    new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + total_housing_length + 0.5 * connector_signal_length) * e_r + 0.5 / sqrt(2.) * connector_base_outer_radius * e_theta - 0.5 / sqrt(2.) * connector_base_outer_radius * e_phi, connector_signal_logical, detector_name + "_connector_signal", world_Logical, 0, 0, false);
  }

  /************* Filters *************/
  G4double filter_position_z = 0.; // Will be gradually increased to be able to place filters on top of each other

  if (filter_materials.size()) {
    G4Tubs *filter_solid = nullptr;
    G4LogicalVolume *filter_logical = nullptr;
    G4double filter_radius;
    stringstream filter_base_name_ss;
    for (unsigned int i = 0; i < filter_materials.size(); ++i) {
      filter_radius = (filter_radii[i] < 0.) ? main_case_outer_radius : filter_radii[i]; // A negative filter radius value tells us to use the detector front radius as the filter radius
      filter_base_name_ss << detector_name << "_filter_" << i + 1 << "_" << filter_materials[i] << "_" << filter_thicknesses[i] / mm << "mm_x_" << filter_radius / mm << "mm";
      filter_solid = new G4Tubs(filter_base_name_ss.str() + "_solid", 0., filter_radius, filter_thicknesses[i] / 2., 0., twopi);
      filter_logical = new G4LogicalVolume(filter_solid, nist->FindOrBuildMaterial(filter_materials[i]), filter_base_name_ss.str() + "_logical");
      if (i % 2 == 0) {
        filter_logical->SetVisAttributes(G4Color::Red());
      } else {
        filter_logical->SetVisAttributes(G4Color::Green());
      }
      new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center - filter_position_z - filter_thicknesses[i] / 2.) * e_r, filter_logical, filter_base_name_ss.str(), world_Logical, 0, 0, false);
      filter_position_z = filter_position_z + filter_thicknesses[i];
      filter_base_name_ss.str("");
    }
  }

  /************* Wraps *************/
  if (wrap_materials.size()) {
    G4Tubs *wrap_solid = nullptr;
    G4LogicalVolume *wrap_logical = nullptr;
    G4double wrap_radius = main_case_outer_radius; // Will be gradually increased to be able to place wraps on top of each other
    stringstream wrap_base_name_ss;
    for (unsigned int i = 0; i < wrap_materials.size(); ++i) {
      wrap_base_name_ss << detector_name << "_wrap_" << i + 1 << "_" << wrap_materials[i] << "_" << wrap_thicknesses[i] / mm << "mm";
      wrap_solid = new G4Tubs(wrap_base_name_ss.str() + "_solid", wrap_radius, wrap_radius + wrap_thicknesses[i], magnetic_shielding_offset / 2., 0., twopi);
      wrap_logical = new G4LogicalVolume(wrap_solid, nist->FindOrBuildMaterial(wrap_materials[i]), wrap_base_name_ss.str() + "_logical");
      if (i % 2 == 0) {
        wrap_logical->SetVisAttributes(G4Color::Green());
      } else {
        wrap_logical->SetVisAttributes(G4Color::Red());
      }
      new G4PVPlacement(rotation_matrix, global_coordinates + (dist_from_center + magnetic_shielding_offset / 2.) * e_r, wrap_logical, wrap_base_name_ss.str(), world_Logical, 0, 0, false);
      wrap_radius = wrap_radius + wrap_thicknesses[i];
      wrap_base_name_ss.str("");
    }
  }
}

void CeBr3_2x2::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center) const {
  Construct(global_coordinates, theta, phi, dist_from_center, 0.);
}