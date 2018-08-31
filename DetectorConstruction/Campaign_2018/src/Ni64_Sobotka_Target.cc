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

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Ni64_Sobotka_Target.hh"

Ni64_Sobotka_Target::Ni64_Sobotka_Target():
World_Logical(nullptr)
{}

Ni64_Sobotka_Target::Ni64_Sobotka_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Ni64_Sobotka_Target::Construct(G4ThreeVector global_coordinates){

	G4Colour yellow(1.0, 1.0, 0.0);

	// Create Materials
	G4double Target_Density = 8.908*g/cm3; // Wikipedia	
	G4double Target_Mass = 3.9942*g;
	G4double Target_Radius = 4.*mm;
	G4double Target_Length = Target_Mass/(Target_Density*pi*Target_Radius*Target_Radius);

	G4Isotope *Ni58 = new G4Isotope("58Ni", 28, 30);
	G4Isotope *Ni60 = new G4Isotope("60Ni", 28, 32);
	G4Isotope *Ni61 = new G4Isotope("61Ni", 28, 33);
	G4Isotope *Ni62 = new G4Isotope("62Ni", 28, 34);
	G4Isotope *Ni64_Sobotka = new G4Isotope("64Ni", 28, 36);

	G4Element *Target_Element = new G4Element("64Ni_Element", "Ni", 5);

	Target_Element->AddIsotope(Ni58,  0.14*perCent);
	Target_Element->AddIsotope(Ni60,  0.72*perCent);
	Target_Element->AddIsotope(Ni61,  2.30*perCent);
	Target_Element->AddIsotope(Ni62,  4.68*perCent);
	Target_Element->AddIsotope(Ni64_Sobotka, 92.16*perCent);

	G4Material *Target_Material = new G4Material("Ni64_Sobotka", Target_Density, 1);
	Target_Material->AddElement(Target_Element, 1);

	// Create physical target
	
	G4Tubs *Target_Solid = new G4Tubs("Ni64_Sobotka_Solid", 0., Target_Radius, Target_Length*0.5, 0., twopi);

	G4LogicalVolume *Target_Logical = new G4LogicalVolume(Target_Solid, Target_Material, "Ni64_Sobotka_Logical");
	Target_Logical->SetVisAttributes(yellow);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Target_Logical, "Ni64_Sobotka_Target", World_Logical, false, 0);
}
