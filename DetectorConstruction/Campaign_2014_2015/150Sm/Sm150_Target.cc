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

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Sm150_Target.hh"
#include "Units.hh"

Sm150_Target::Sm150_Target() : World_Logical(nullptr) {}

Sm150_Target::Sm150_Target(G4LogicalVolume *World_Log) : World_Logical(World_Log) {}

void Sm150_Target::Construct(G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();

  /*************************************************/
  // Enriched 150Nd target from 2016/2018 NRF experiments
  // The target was borrowed from Oak Ridge National Laboratory
  // All information from the data sheet if not indicated otherwise
  /*************************************************/

  // Target material Sm isotopic composition
  // There is currently no data sheet for the target, only the enrichment of the material in the isotope of interest (150Sm) is known
  // Hence, the remaining target material percentage is modeled to be the remaining natural occuring isotopes of Sm with their respective
  // natural abbundance taken from https://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl
  G4Isotope *Sm144 = new G4Isotope("144Sm", 62, 144);
  G4Isotope *Sm147 = new G4Isotope("147Sm", 62, 147);
  G4Isotope *Sm148 = new G4Isotope("148Sm", 62, 148);
  G4Isotope *Sm149 = new G4Isotope("149Sm", 62, 149);
  G4Isotope *Sm150 = new G4Isotope("150Sm", 62, 150);
  G4Isotope *Sm152 = new G4Isotope("152Sm", 62, 152);
  G4Isotope *Sm154 = new G4Isotope("154Sm", 62, 154);

  G4Element *enriched_Sm150 =
      new G4Element("enriched Sm150", "Sm-150e", 7);

  enriched_Sm150->AddIsotope(Sm144, 05.00 * 3.07 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);
  enriched_Sm150->AddIsotope(Sm147, 05.00 * 14.99 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);
  enriched_Sm150->AddIsotope(Sm148, 05.00 * 11.24 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);
  enriched_Sm150->AddIsotope(Sm149, 05.00 * 13.82 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);
  enriched_Sm150->AddIsotope(Sm150, 95.00 * perCent);
  enriched_Sm150->AddIsotope(Sm152, 05.00 * 26.75 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);
  enriched_Sm150->AddIsotope(Sm154, 05.00 * 22.75 / (3.07 + 14.99 + 11.24 + 13.82 + 26.75 + 22.75) * perCent);

  // Target dimensions and weight
  G4double Sm150_Mass = 2279.10 * mg;

  G4double Sm150_Container_OuterRadius = 11.0 * mm; // Estimated
  G4double Sm150_Container_InnerRadius = 10.0 * mm; // Estimated
  G4double Sm150_Container_InnerHeight = 2.0 * mm; // Estimated from a Sm(2)O(3) powder density of ≈ 5,08g/cm^3 and the target material weight
  G4double Sm150_Container_CapThickness = 1.0 * mm; // Estimated
  G4double Sm150_Container_OuterHeight = Sm150_Container_InnerHeight + 2 * Sm150_Container_CapThickness; // Estimated
  // Target Material
  G4double Sm150_Density = Sm150_Mass / (pi * pow(Sm150_Container_InnerRadius, 2) * Sm150_Container_InnerHeight);

  G4Material *target_Sm150 = new G4Material("target_Sm150", Sm150_Density, 2);
  target_Sm150->AddElement(enriched_Sm150, 2); // Sm(2)O(3) according to ELOG
  G4Element *nat_O = nist->FindOrBuildElement("O");
  target_Sm150->AddElement(nat_O, 3);

  // Target Container Barrel
  G4Tubs *Sm150_Container_Barrel_Solid =
      new G4Tubs("Sm150_Container_Barrel_Solid", Sm150_Container_InnerRadius,
                 Sm150_Container_OuterRadius,
                 Sm150_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);

  G4LogicalVolume *Sm150_Container_Barrel_Logical = new G4LogicalVolume(
      Sm150_Container_Barrel_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Sm150_Container_Barrel_Logical");
  Sm150_Container_Barrel_Logical->SetVisAttributes(G4Color::Grey());

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Sm150_Container_Barrel_Logical,
                    "Sm150_Container_Barrel", World_Logical, false, 0);

  // Target Container Caps
  G4Tubs *Sm150_Container_Cap_Solid = new G4Tubs(
      "Sm150_Container_Cap_Solid", 0. * mm, Sm150_Container_InnerRadius,
      Sm150_Container_CapThickness * 0.5, 0. * deg, 360. * deg);

  G4LogicalVolume *Sm150_Container_Cap_Logical = new G4LogicalVolume(
      Sm150_Container_Cap_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Sm150_Container_Cap_Logical");
  Sm150_Container_Cap_Logical->SetVisAttributes(G4Color::Grey());

  // Target Container Bottom
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(0., 0., (Sm150_Container_OuterHeight - Sm150_Container_CapThickness) * 0.5),
      Sm150_Container_Cap_Logical, "Sm150_Container_Bottom",
      World_Logical, false, 0);

  // Target Container Top
  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(0., 0., -(Sm150_Container_OuterHeight - Sm150_Container_CapThickness) * 0.5),
      Sm150_Container_Cap_Logical, "Sm150_Container_Top",
      World_Logical, false, 1);

  // Sm150 Target Material
  G4Tubs *Sm150_Target_Solid =
      new G4Tubs("Sm150_Target_Solid", 0. * mm, Sm150_Container_InnerRadius,
                 Sm150_Container_InnerHeight * 0.5, 0. * deg, 360. * deg);

  G4LogicalVolume *Sm150_Target_Logical = new G4LogicalVolume(
      Sm150_Target_Solid, target_Sm150, "Sm150_Target_Logical");
  Sm150_Target_Logical->SetVisAttributes(G4Color::Yellow());

  new G4PVPlacement(
      0, global_coordinates + G4ThreeVector(), Sm150_Target_Logical, "Sm150_Target", World_Logical, false, 0);
}
