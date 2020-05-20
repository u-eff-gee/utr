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

void Sn112116_Target::Construct(G4ThreeVector global_coordinates) {
	Construct(new G4RotationMatrix(), global_coordinates);
}

void Sn112116_Target::Construct(G4RotationMatrix* rotation, G4ThreeVector global_coordinates){

	G4double target_radius = 5.*mm;

	// Target data
	G4double Sn112_density = 7.265*g/cm3; // Density of natural beta-tin from Wikipedia.
	G4double Sn116_density = 7.265*g/cm3; // Density of natural beta-tin from Wikipedia.
	
	// TU-Sn-116-2013 #2
	G4double Sn116_mass_2 = 2.40668*g;
	G4double Sn116_thickness_2 = Sn116_mass_2/(Sn116_density*pi*target_radius*target_radius);
	
	// TU-Sn-112-2014 #2
	G4double Sn112_mass_2 = 2.40403*g;
	G4double Sn112_thickness_2 = Sn112_mass_2/(Sn112_density*pi*target_radius*target_radius);
	
	// TU-Sn-116-2013 #3
	G4double Sn116_mass_3 = 2.39617*g;
	G4double Sn116_thickness_3 = Sn116_mass_3/(Sn116_density*pi*target_radius*target_radius);
	
	// TU-Sn-112-2014 #1
	G4double Sn112_mass_1 = 2.39414*g;
	G4double Sn112_thickness_1 = Sn112_mass_1/(Sn112_density*pi*target_radius*target_radius);


	// Target container
	Sn112116_Container_OuterHeight = 20. * mm;
	Sn112116_Container_InnerHeight = Sn112116_Container_OuterHeight - 2. * mm;
	Sn112116_Container_OuterRadius = 6. * mm;
	Sn112116_Container_InnerRadius = Sn112116_Container_OuterRadius - 1. * mm;
	Sn112116_Container_CapThickness = ( Sn112116_Container_OuterHeight - Sn112116_Container_InnerHeight )/2;


	// Enriched 112Sn
	G4Isotope *Sn112 = new G4Isotope("Sn112", 50, 62, 111.904824877*g/mole); // Enrichment was actually 99.994%, but assume here that the target was monoisotopic.
	G4Element *Sn112_element = new G4Element("Sn112_element", "Sn", 1);
	Sn112_element->AddIsotope(Sn112, 100.*perCent);
	G4Material *Sn112_material = new G4Material("Sn112_material", Sn112_density, 1);
	Sn112_material->AddElement(Sn112_element, 1);
	
	// Enriched 116Sn
	G4Isotope *Sn116 = new G4Isotope("Sn116", 50, 66, 115.901742824*g/mole); // Enrichment was actually 97.80(2)%, but assume here that the target was monoisotopic.
	G4Element *Sn116_element = new G4Element("Sn116_element", "Sn", 1);
	Sn116_element->AddIsotope(Sn116, 100.*perCent);
	G4Material *Sn116_material = new G4Material("Sn116_material", Sn116_density, 1);
	Sn116_material->AddElement(Sn116_element, 1);

	// PVC
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");


	// Mother volume consisting of Air
	G4Tubs *Sn112116_Mother_Solid =
	    new G4Tubs("Sn112116_Mother_Solid", 0, Sn112116_Container_OuterRadius,
		       Sn112116_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *Sn112116_Mother_Logical = new G4LogicalVolume(
	    Sn112116_Mother_Solid, nist->FindOrBuildMaterial("G4_AIR"), "Sn112116_Mother_Logical");
	Sn112116_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	new G4PVPlacement(rotation, global_coordinates + G4ThreeVector(), Sn112116_Mother_Logical,
			  "Sn112116_Target", World_Logical, false, 0);

	// Target Container Barrel
	G4Tubs *Sn112116_Container_Solid =
	    new G4Tubs("Sn112116_Container_Solid", Sn112116_Container_InnerRadius,
		       Sn112116_Container_OuterRadius,
		       Sn112116_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Sn112116_Container_Logical = new G4LogicalVolume(
	    Sn112116_Container_Solid, PVC, "Sn112116_Container_Logical");
	Sn112116_Container_Logical->SetVisAttributes(G4Color::Grey());

	new G4PVPlacement(rotation, G4ThreeVector(), Sn112116_Container_Logical,
			  "Sn112116_Container", Sn112116_Mother_Logical, false, 0);

	// Target Container Caps
	G4Tubs *Sn112116_ContainerCap_Solid = new G4Tubs(
	    "Sn112116_ContainerCap_Solid", 0. * mm, Sn112116_Container_InnerRadius,
	    Sn112116_Container_CapThickness * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Sn112116_ContainerCap_Logical = new G4LogicalVolume(
	    Sn112116_ContainerCap_Solid, PVC, "Sn112116_ContainerCap_Logical");
	Sn112116_ContainerCap_Logical->SetVisAttributes(G4Color::Grey());


	//Target Container Bottom
	new G4PVPlacement(
		0, G4ThreeVector(0., 0.,
							(Sn112116_Container_OuterHeight -
								Sn112116_Container_CapThickness) * 0.5),
		Sn112116_ContainerCap_Logical, "Sn112116_ContainerBottom",
		Sn112116_Mother_Logical, false, 0);

	// Target Container Top
	new G4PVPlacement(
		0, G4ThreeVector(0., 0.,
							-(Sn112116_Container_OuterHeight -
								Sn112116_Container_CapThickness) * 0.5),
		Sn112116_ContainerCap_Logical, "Sn112116_ContainerTop",
		Sn112116_Mother_Logical, false, 0);


	// Construct targets
	auto total_target_thickness = Sn116_thickness_2 + Sn112_thickness_2 + Sn116_thickness_3 + Sn112_thickness_1;
	auto target_spacing = (Sn112116_Container_InnerHeight - total_target_thickness) / 3;

	G4Tubs *Sn116_solid_2 = new G4Tubs("Sn116_solid_2", 0., target_radius, Sn116_thickness_2*0.5, 0., twopi);
	G4LogicalVolume *Sn116_logical_2 = new G4LogicalVolume(Sn116_solid_2, Sn116_material, "Sn116_logical_2");
	Sn116_logical_2->SetVisAttributes(G4Color::Yellow());
	new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*Sn112116_Container_InnerHeight + 0.5*Sn116_thickness_2), Sn116_logical_2, "Sn116_2", Sn112116_Mother_Logical, false, 0);
	
	G4Tubs *Sn112_solid_2 = new G4Tubs("Sn112_solid_2", 0., target_radius, Sn112_thickness_2*0.5, 0., twopi);
	G4LogicalVolume *Sn112_logical_2 = new G4LogicalVolume(Sn112_solid_2, Sn112_material, "Sn112_logical_2");
	Sn112_logical_2->SetVisAttributes(G4Color::Red());
	new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*Sn112116_Container_InnerHeight + 1.*target_spacing + Sn116_thickness_2 + 0.5*Sn112_thickness_2), Sn112_logical_2, "Sn112_2", Sn112116_Mother_Logical, false, 0);
	
	G4Tubs *Sn116_solid_3 = new G4Tubs("Sn116_solid_3", 0., target_radius, Sn116_thickness_3*0.5, 0., twopi);
	G4LogicalVolume *Sn116_logical_3 = new G4LogicalVolume(Sn116_solid_3, Sn116_material, "Sn116_logical_3");
	Sn116_logical_3->SetVisAttributes(G4Color::Yellow());
	new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*Sn112116_Container_InnerHeight + 2.*target_spacing + Sn116_thickness_2 + Sn112_thickness_2 + 0.5*Sn116_thickness_3), Sn116_logical_3, "Sn116_3", Sn112116_Mother_Logical, false, 0);
	
	G4Tubs *Sn112_solid_1 = new G4Tubs("Sn112_solid_1", 0., target_radius, Sn112_thickness_1*0.5, 0., twopi);
	G4LogicalVolume *Sn112_logical_1 = new G4LogicalVolume(Sn112_solid_1, Sn112_material, "Sn112_logical_1");
	Sn112_logical_1->SetVisAttributes(G4Color::Red());
	new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*Sn112116_Container_InnerHeight + 3.*target_spacing + Sn116_thickness_2 + Sn112_thickness_2 + Sn116_thickness_3 + 0.5 * Sn112_thickness_1), Sn112_logical_1, "Sn112_1", Sn112116_Mother_Logical, false, 0);

}
