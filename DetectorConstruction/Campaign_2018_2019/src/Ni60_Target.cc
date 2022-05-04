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

/*
 * 60Ni Target
 * Property of the Technische Universität Darmstadt
 * In the ELOG, mass, chemical form (pure metal), radius, and enrichment are given
 */

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"

#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Ni60_Target.hh"

Ni60_Target::Ni60_Target() : World_Logical(nullptr) {}

Ni60_Target::Ni60_Target(G4LogicalVolume *World_Log) : World_Logical(World_Log) {}

void Ni60_Target::Construct(G4ThreeVector global_coordinates) {

  // Abundances from Wikipedia
  G4double Ni58_natural_abundance = 68.077 * perCent;
  // G4double Ni60_natural_abundance = 26.233*perCent;
  G4double Ni61_natural_abundance = 1.14 * perCent;
  G4double Ni62_natural_abundance = 3.634 * perCent;
  G4double Ni64_natural_abundance = 0.926 * perCent;

  G4double rest_abundance = Ni58_natural_abundance + Ni61_natural_abundance + Ni62_natural_abundance + Ni64_natural_abundance;

  // Atomic masses from Atomic Mass Evaluation
  G4Isotope *Ni58 = new G4Isotope("58Ni", 28, 58, 57.935341780 * g / mole);
  G4Isotope *Ni60 = new G4Isotope("60Ni", 28, 60, 59.930785256 * g / mole);
  G4Isotope *Ni61 = new G4Isotope("61Ni", 28, 61, 60.931054945 * g / mole);
  G4Isotope *Ni62 = new G4Isotope("62Ni", 28, 62, 61.928344871 * g / mole);
  G4Isotope *Ni64 = new G4Isotope("64Ni", 28, 64, 63.927966341 * g / mole);

  // UWS Target
  G4double target_density = 8.908 * g / cm3; // Wikipedia
  G4double target_mass = 2.9926 * g; // ELOG
  G4double target_radius = 7. * mm; // ELOG
  G4double target_length = target_mass / (target_density * pi * target_radius * target_radius);
  G4double target_enrichment = 99.8 * perCent; // ELOG

  G4Element *target_Element = new G4Element("Ni60", "Ni60", 5);

  // Abundances of residual elements assumed to have the same abundance ratio as in
  // natural nickel.
  target_Element->AddIsotope(Ni58, (100. * perCent - target_enrichment) * Ni58_natural_abundance / rest_abundance);
  target_Element->AddIsotope(Ni60, target_enrichment);
  target_Element->AddIsotope(Ni61, (100. * perCent - target_enrichment) * Ni61_natural_abundance / rest_abundance);
  target_Element->AddIsotope(Ni62, (100. * perCent - target_enrichment) * Ni62_natural_abundance / rest_abundance);
  target_Element->AddIsotope(Ni64, (100. * perCent - target_enrichment) * Ni64_natural_abundance / rest_abundance);

  G4Material *target_Material = new G4Material("Ni60", target_density, 1);
  target_Material->AddElement(target_Element, 1);

  // Create physical target

  G4Tubs *target_solid = new G4Tubs("Ni60_solid", 0., target_radius, target_length * 0.5, 0., twopi);

  G4LogicalVolume *target_logical = new G4LogicalVolume(target_solid, target_Material, "Ni60_logical");
  target_logical->SetVisAttributes(G4Color::Yellow());

  new G4PVPlacement(0, global_coordinates + G4ThreeVector(), target_logical, "Ni60_Target", World_Logical, false, 0);
}
