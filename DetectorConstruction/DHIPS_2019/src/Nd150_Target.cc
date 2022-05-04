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

#include <iostream>

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Nd150_Target.hh"
#include "Units.hh"

Nd150_Target::Nd150_Target() : World_Logical(nullptr) {
}

Nd150_Target::Nd150_Target(G4LogicalVolume *World_Log) : World_Logical(World_Log) {
}

void Nd150_Target::Construct(G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  /*************************************************/
  // Enriched 150Nd target from 2016/2018 NRF experiments
  // The target was borrowed from Oak Ridge National Laboratory
  // All information from the data sheet if not indicated otherwise
  /*************************************************/

  // Target material
  G4Isotope *Nd142 = new G4Isotope("142Nd", 60, 142);
  G4Isotope *Nd143 = new G4Isotope("143Nd", 60, 143);
  G4Isotope *Nd144 = new G4Isotope("144Nd", 60, 144);
  G4Isotope *Nd145 = new G4Isotope("145Nd", 60, 145);
  G4Isotope *Nd146 = new G4Isotope("146Nd", 60, 146);
  G4Isotope *Nd148 = new G4Isotope("148Nd", 60, 148);
  G4Isotope *Nd150 = new G4Isotope("150Nd", 60, 150);

  G4Element *enriched_Nd150 =
      new G4Element("enriched Nd150", "Nd-150e", 7);

  enriched_Nd150->AddIsotope(Nd142, 1.26 * perCent);
  enriched_Nd150->AddIsotope(Nd143, 0.82 * perCent);
  enriched_Nd150->AddIsotope(Nd144, 1.34 * perCent);
  enriched_Nd150->AddIsotope(Nd145, 0.74 * perCent);
  enriched_Nd150->AddIsotope(Nd146, 1.31 * perCent);
  enriched_Nd150->AddIsotope(Nd148, 0.94 * perCent);
  enriched_Nd150->AddIsotope(Nd150, 93.59 * perCent);

  G4double Nd150_Mass = 11582.8 * mg;

  G4double Nd150_Container_OuterHeight = 7.5 * mm; // According to Oak Ridge
  G4double Nd150_Container_InnerHeight = Nd150_Container_OuterHeight - 1. * mm; // Estimated

  // TODO: Conflicting information: Is inner or outer radius 0.5 inch?
  G4double Nd150_Container_OuterRadius = 0.5 * inch; // According to Oak Ridge
  G4double Nd150_Container_InnerRadius = Nd150_Container_OuterRadius - 1. * mm; // Estimated

  G4double Nd150_Density = Nd150_Mass / (pi * pow(Nd150_Container_InnerRadius, 2) *
                                         Nd150_Container_InnerHeight);

  G4Material *target_Nd150 = new G4Material("target_Nd150", Nd150_Density, 2);
  target_Nd150->AddElement(enriched_Nd150, 2); // Nd(2)O(3) according to Oak Ridge
  G4Element *nat_O = nist->FindOrBuildElement("O");
  target_Nd150->AddElement(nat_O, 3);

  // Target dimensions

  Nd150_Container_OuterHeight = 7.5 * mm; // According to Oak Ridge
  Nd150_Container_InnerHeight = Nd150_Container_OuterHeight - 1. * mm; // Estimated
  Nd150_Container_OuterRadius = 0.5 * inch; // According to Oak Ridge
  Nd150_Container_InnerRadius = Nd150_Container_OuterRadius - 1. * mm; // Estimated
  G4double Nd150_Container_CapThickness = (Nd150_Container_OuterHeight - Nd150_Container_InnerHeight) / 2;

  // Target Container Barrel

  G4Tubs *Nd150_Container_Solid =
      new G4Tubs("Nd150_Container_Solid", Nd150_Container_InnerRadius,
                 Nd150_Container_OuterRadius,
                 Nd150_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);

  // HDPE (High-density polyethylene) used for container according to Oak Ridge, G4_POLYETHYLENE matches the definition of HDPE in density
  G4LogicalVolume *Nd150_Container_Logical = new G4LogicalVolume(
      Nd150_Container_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Nd150_Container_Logical");
  Nd150_Container_Logical->SetVisAttributes(G4Color::Grey());

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Nd150_Container_Logical,
                    "Nd150_Container", World_Logical, false, 0);

  // Target Container Caps
  G4Tubs *Nd150_ContainerCap_Solid = new G4Tubs(
      "Nd150_ContainerCap_Solid", 0. * mm, Nd150_Container_InnerRadius,
      Nd150_Container_CapThickness * 0.5, 0. * deg, 360. * deg);

  G4LogicalVolume *Nd150_ContainerCap_Logical = new G4LogicalVolume(
      Nd150_ContainerCap_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Nd150_ContainerCap_Logical");
  Nd150_ContainerCap_Logical->SetVisAttributes(G4Color::Grey());

  // Target Container Bottom
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(0., 0., (Nd150_Container_OuterHeight - Nd150_Container_CapThickness) * 0.5),
      Nd150_ContainerCap_Logical, "Nd150_ContainerBottom",
      World_Logical, false, 0);

  // Target Container Top
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(0., 0., -(Nd150_Container_OuterHeight - Nd150_Container_CapThickness) * 0.5),
      Nd150_ContainerCap_Logical, "Nd150_ContainerTop",
      World_Logical, false, 0);

  // Nd150 Target Material
  G4Tubs *Nd150_Solid =
      new G4Tubs("Nd150_Solid", 0. * mm, Nd150_Container_InnerRadius,
                 Nd150_Container_InnerHeight * 0.5, 0. * deg, 360. * deg);

  G4LogicalVolume *Nd150_Logical = new G4LogicalVolume(
      Nd150_Solid, target_Nd150, "Nd150_Logical");
  Nd150_Logical->SetVisAttributes(G4Color::Yellow());

  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(), Nd150_Logical, "Nd150_Target", World_Logical, false, 0);

  // Aluminum plates
  G4Material *Al27 = nist->FindOrBuildMaterial("G4_Al");

  G4double Al27_mass_1 = 0.6795 * g;
  G4double Al27_mass_2 = 0.6894 * g;
  G4double Al27_radius = 0.5 * inch;

  G4double Al27_thickness_1 = Al27_mass_1 / (Al27->GetDensity() * pi * Al27_radius * Al27_radius);
  G4double Al27_thickness_2 = Al27_mass_2 / (Al27->GetDensity() * pi * Al27_radius * Al27_radius);

  G4Tubs *Al27_Solid_1 = new G4Tubs("Al27_Solid_1", 0, Al27_radius, Al27_thickness_1 * 0.5, 0, twopi);
  G4LogicalVolume *Al27_Logical_1 = new G4LogicalVolume(Al27_Solid_1, Al27, "Al27_Logical_1");
  Al27_Logical_1->SetVisAttributes(G4Color::White());

  G4Tubs *Al27_Solid_2 = new G4Tubs("Al27_Solid_2", 0, Al27_radius, Al27_thickness_2 * 0.5, 0, twopi);
  G4LogicalVolume *Al27_Logical_2 = new G4LogicalVolume(Al27_Solid_2, Al27, "Al27_Logical_2");
  Al27_Logical_2->SetVisAttributes(G4Color::White());

  auto Al27_posz_1 = -0.5 * Nd150_Container_OuterHeight - 0.5 * Al27_thickness_1;
  auto Al27_posz_2 = 0.5 * Nd150_Container_OuterHeight + 0.5 * Al27_thickness_2;
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Al27_posz_1), Al27_Logical_1, "Al27_1", World_Logical, false, 0);
  new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Al27_posz_2), Al27_Logical_2, "Al27_2", World_Logical, false, 0);

  std::cout << "********** Al27 target information **********\n";
  std::cout << "Al27_diameter = " << 2 * Al27_radius << '\n';
  std::cout << "Al27_thickness_1 = " << Al27_thickness_1 << '\n';
  std::cout << "Al27_thickness_2 = " << Al27_thickness_2 << '\n';
  std::cout << "Al27_posz_1 = " << Al27_posz_1 << '\n';
  std::cout << "Al27_posz_2 = " << Al27_posz_2 << '\n';
  std::cout << "*********************************************\n";
}
