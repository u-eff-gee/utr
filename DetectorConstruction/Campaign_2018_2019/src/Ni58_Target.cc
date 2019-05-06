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
 * Two-component 58Ni Target 
 * One part was provided by the Scheck group from the University of the West of Scotland (UWS)
 * The other by the Helmholtzzentrum Dresden-Rossendorf (HZDR)
 * In the ELOG, the enrichment, mass, radius, and chemical composition (pure metal)
 * of the disk-shaped target are given.
 */

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Ni58_Target.hh"

Ni58_Target::Ni58_Target():
World_Logical(nullptr)
{}

Ni58_Target::Ni58_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Ni58_Target::Construct(G4ThreeVector global_coordinates){

	// Abundances from Wikipedia
	// G4double Ni58_natural_abundance = 68.077*perCent;
	G4double Ni60_natural_abundance = 26.233*perCent;
	G4double Ni61_natural_abundance =  1.14*perCent;
	G4double Ni62_natural_abundance =  3.634*perCent;
	G4double Ni64_natural_abundance =  0.926*perCent;

	G4double rest_abundance = Ni60_natural_abundance + Ni61_natural_abundance + Ni62_natural_abundance + Ni64_natural_abundance; 

	// Atomic masses from Atomic Mass Evaluation
	G4Isotope *Ni58 = new G4Isotope("58Ni", 28, 58, 57.935341780*g/mole);
	G4Isotope *Ni60 = new G4Isotope("60Ni", 28, 60, 59.930785256*g/mole);
	G4Isotope *Ni61 = new G4Isotope("61Ni", 28, 61, 60.931054945*g/mole);
	G4Isotope *Ni62 = new G4Isotope("62Ni", 28, 62, 61.928344871*g/mole);
	G4Isotope *Ni64 = new G4Isotope("64Ni", 28, 64, 63.927966341*g/mole);

	// UWS Target
	G4double target_UWS_density = 8.908*g/cm3; // Wikipedia	
	G4double target_UWS_mass = 2.0009*g; // ELOG
	G4double target_UWS_radius = 10.*mm; // ELOG
	G4double target_UWS_length = target_UWS_mass/(target_UWS_density*pi*target_UWS_radius*target_UWS_radius);
	G4double target_UWS_enrichment = 99.981*perCent; // ELOG

	G4Element *target_UWS_Element = new G4Element("Ni58_UWS", "Ni58_UWS", 5);

	// Abundances of residual elements assumed to have the same abundance ratio as in
	// natural nickel.
	target_UWS_Element->AddIsotope(Ni58, target_UWS_enrichment);
	target_UWS_Element->AddIsotope(Ni60, (100.*perCent-target_UWS_enrichment)*Ni60_natural_abundance/rest_abundance);
	target_UWS_Element->AddIsotope(Ni61, (100.*perCent-target_UWS_enrichment)*Ni61_natural_abundance/rest_abundance);
	target_UWS_Element->AddIsotope(Ni62, (100.*perCent-target_UWS_enrichment)*Ni62_natural_abundance/rest_abundance);
	target_UWS_Element->AddIsotope(Ni64, (100.*perCent-target_UWS_enrichment)*Ni64_natural_abundance/rest_abundance);

	G4Material *target_UWS_Material = new G4Material("Ni58_UWS", target_UWS_density, 1);
	target_UWS_Material->AddElement(target_UWS_Element, 1);

	// Create physical target
	
	G4Tubs *target_UWS_solid = new G4Tubs("Ni58_UWS_solid", 0., target_UWS_radius, target_UWS_length*0.5, 0., twopi);

	G4LogicalVolume *target_UWS_logical = new G4LogicalVolume(target_UWS_solid, target_UWS_Material, "Ni58_UWS_logical");
	target_UWS_logical->SetVisAttributes(G4Color::Yellow());

	// new G4PVPlacement(...) further down, because it depends of the lengths of both targets

	// HZDR Target
	G4double target_HZDR_density = 8.908*g/cm3; // Wikipedia
	G4double target_HZDR_mass = 1.0083*g; // ELOG
	G4double target_HZDR_radius = 10.*mm; // ELOG
	G4double target_HZDR_length = target_HZDR_mass/(target_HZDR_density*pi*target_HZDR_radius*target_HZDR_radius);
	G4double target_HZDR_enrichment = 99.50*perCent; // ELOG

	G4Element *target_HZDR_Element = new G4Element("Ni58_HZDR", "Ni58_HZDR", 5);

	// Abundances of residual elements assumed to have the same abundance ratio as in
	// natural nickel.
	target_HZDR_Element->AddIsotope(Ni58, target_HZDR_enrichment);
	target_HZDR_Element->AddIsotope(Ni60, (100.*perCent-target_HZDR_enrichment)*Ni60_natural_abundance/rest_abundance);
	target_HZDR_Element->AddIsotope(Ni61, (100.*perCent-target_HZDR_enrichment)*Ni61_natural_abundance/rest_abundance);
	target_HZDR_Element->AddIsotope(Ni62, (100.*perCent-target_HZDR_enrichment)*Ni62_natural_abundance/rest_abundance);
	target_HZDR_Element->AddIsotope(Ni64, (100.*perCent-target_HZDR_enrichment)*Ni64_natural_abundance/rest_abundance);

	G4Material *target_HZDR_Material = new G4Material("Ni58_HZDR", target_HZDR_density, 1);
	target_HZDR_Material->AddElement(target_HZDR_Element, 1);

	// Create physical target
	
	G4Tubs *target_HZDR_solid = new G4Tubs("Ni58_HZDR_solid", 0., target_HZDR_radius, target_HZDR_length*0.5, 0., twopi);

	G4LogicalVolume *target_HZDR_logical = new G4LogicalVolume(target_HZDR_solid, target_HZDR_Material, "Ni58_HZDR_logical");
	target_HZDR_logical->SetVisAttributes(G4Color::Red());

	G4double target_total_length = target_UWS_length + target_HZDR_length;
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -target_total_length*0.5 + target_UWS_length*0.5), target_UWS_logical, "Ni58_UWS_Target", World_Logical, false, 0);
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -target_total_length*0.5 + target_UWS_length + target_HZDR_length*0.5), target_HZDR_logical, "Ni58_HZDR_Target", World_Logical, false, 0);
}
