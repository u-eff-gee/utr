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
 * Target for a hypothetical DHIPS experiment that measures two B(E2) strengths simultaneously.
 * The target consists of two parts: an enriched 112Sn and an enriched 116Sn target
 */

#include <sstream>
#include <vector>

#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Sn112116_Target.hh"

using std::stringstream;
using std::vector;

Sn112116_Target::Sn112116_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Sn112116_Target::Construct(G4ThreeVector global_coordinates){

	G4double target_radius = 10.*mm;

	// Target data
	G4double sn112_density = 7.265*g/cm3; // Density of natural beta-tin from Wikipedia.
	G4double sn112_mass = 4.4285*g; // +- 0.0003 g
	G4double sn112_thickness = sn112_mass/(sn112_density*pi*target_radius*target_radius);

	G4double sn116_density = 7.265*g/cm3; // Density of natural beta-tin from Wikipedia.
	G4double sn116_mass = 9.1516*g; // TODO: find the uncertainty in the data sheet
	G4double sn116_thickness = sn116_mass/(sn116_density*pi*target_radius*target_radius);

	// Enriched 112Sn
	G4Isotope *sn112 = new G4Isotope("Sn112", 50, 62, 111.904824877*g/mole); // Enrichment was actually 99.994%, but assume here that the target was monoisotopic.
	G4Element *sn112_element = new G4Element("Sn112_element", "Sn", 1);
	sn112_element->AddIsotope(sn112, 100.*perCent);
	G4Material *sn112_material = new G4Material("Sn112_material", sn112_density, 1);
	sn112_material->AddElement(sn112_element, 1);
	
	// Enriched 116Sn
	G4Isotope *sn116 = new G4Isotope("Sn116", 50, 66, 115.901742824*g/mole); // Enrichment was actually 97.80(2)%, but assume here that the target was monoisotopic.
	G4Element *sn116_element = new G4Element("Sn116_element", "Sn", 1);
	sn116_element->AddIsotope(sn116, 100.*perCent);
	G4Material *sn116_material = new G4Material("Sn116_material", sn116_density, 1);
	sn116_material->AddElement(sn116_element, 1);


	// Construct targets

	G4Tubs *sn112_solid = new G4Tubs("Sn112_solid", 0., target_radius, sn112_thickness*0.5, 0., twopi);
	G4LogicalVolume *sn112_logical = new G4LogicalVolume(sn112_solid, sn112_material, "Sn112_logical");
	sn112_logical->SetVisAttributes(G4Color::Red());
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -0.5*sn112_thickness), sn112_logical, "Sn112", World_Logical, false, 0);

	G4Tubs *sn116_solid = new G4Tubs("Sn116_solid", 0., target_radius, sn116_thickness*0.5, 0., twopi);
	G4LogicalVolume *sn116_logical = new G4LogicalVolume(sn116_solid, sn116_material, "Sn116_logical");
	sn116_logical->SetVisAttributes(G4Color::Yellow());
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., +0.5*sn116_thickness), sn116_logical, "Sn116", World_Logical, false, 0);
}
