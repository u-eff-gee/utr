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

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Kr82_Target.hh"

Kr82_Target::Kr82_Target():
World_Logical(nullptr)
{}

Kr82_Target::Kr82_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Kr82_Target::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi){

	G4ThreeVector symmetry_axis(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); // Symmetry axis along which the valve and the tube are constructed
	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(-phi);
	rotation->rotateY(-theta);

	G4double GasSphere_Outer_Radius = 10. * mm; // Measured
	G4double GasSphere_Inner_Radius = 9. * mm;  // Estimated

	G4double Tube_Length = 20. * mm;
	G4double Tube_Inner_Radius = 0.25 * mm; // Estimated
	G4double Tube_Outer_Radius = 1. * mm;   // Measured

	G4double Valve_Length = 20. * mm;       // Measured
	G4double Valve_Outer_Radius = 2.5 * mm; // Measured

	G4double Length = GasSphere_Outer_Radius * 2. + Tube_Length + Valve_Length;
	G4double Target_Center_Z =
	    Length * 0.5 - Valve_Length - Tube_Length - GasSphere_Outer_Radius;

	// Krypton gas

	G4Isotope *Kr78 = new G4Isotope("78Kr", 36, 78);
	G4Isotope *Kr80 = new G4Isotope("80Kr", 36, 80);
	G4Isotope *Kr82 = new G4Isotope("82Kr", 36, 82);
	G4Isotope *Kr83 = new G4Isotope("83Kr", 36, 83);
	G4Isotope *Kr84 = new G4Isotope("84Kr", 36, 84);
	G4Isotope *Kr86 = new G4Isotope("86Kr", 36, 86);

	G4Element *enriched_Kr =
	new G4Element("enriched Kr", "Kr", 6);

	enriched_Kr->AddIsotope(Kr78, 0.003 * perCent);
	enriched_Kr->AddIsotope(Kr80, 0.005 * perCent);
	enriched_Kr->AddIsotope(Kr82, 99.945 * perCent);
	enriched_Kr->AddIsotope(Kr83, 0.041 * perCent);
	enriched_Kr->AddIsotope(Kr84, 0.003 * perCent);
	enriched_Kr->AddIsotope(Kr86, 0.003 * perCent);

	G4double Kr_Mass = 1.50218 * g;

	G4double Kr_Density =
	Kr_Mass / (4. / 3. * pi * pow(GasSphere_Inner_Radius, 3));

	G4Material *target_Kr = new G4Material("target_Kr", Kr_Density, 1);
	target_Kr->AddElement(enriched_Kr, 1);

	G4Sphere *Krypton_Solid =
	    new G4Sphere("Krypton_Solid", 0., GasSphere_Inner_Radius, 0.,
			 360. * deg, 0., 180. * deg);

	G4LogicalVolume *Krypton_Logical = new G4LogicalVolume(
	    Krypton_Solid, target_Kr, "Krypton_Logical");

	Krypton_Logical->SetVisAttributes(G4Colour::Green());

	new G4PVPlacement(
	    0, global_coordinates + G4ThreeVector(0., 0., -Length * 0.5 + GasSphere_Outer_Radius - Target_Center_Z),
	    Krypton_Logical, "Krypton", World_Logical, false, 0);

	// Sphere

	// Stainless steel material.
	G4Element *nat_Fe = new G4Element("natural Fe", "nat_Fe",
					  26, 55.845 * g / mole);
	G4Element *nat_Cr = new G4Element("natural Cr", "nat_Cr",
					  24, 51.9961 * g / mole);

	G4double Fe_Density = 7.874 * g / cm3;
	G4double Cr_Density = 7.19 * g / cm3;

	G4double Fe_Percent = 0.85;
	G4double Cr_Percent = 0.15;

	G4double StainlessSteel_Density = 
	    Fe_Percent * Fe_Density + Cr_Percent * Cr_Density; // Density is the weighted mean of pure Fe and Cr

	G4Material *stainlessSteel = new G4Material("Stainless_Steel", StainlessSteel_Density, 2);

	stainlessSteel->AddElement(nat_Fe, 1);
	stainlessSteel->AddElement(nat_Cr, 1);

	G4Sphere *Sphere_Solid = new G4Sphere(
	    "Sphere_Solid", GasSphere_Inner_Radius, GasSphere_Outer_Radius, 0.,
	    360. * deg, 0., 180. * deg);

	G4LogicalVolume *Sphere_Logical = new G4LogicalVolume(
	    Sphere_Solid, stainlessSteel, "Sphere_Logical");

	Sphere_Logical->SetVisAttributes(G4Colour::Grey());

	new G4PVPlacement(
	    0, global_coordinates + G4ThreeVector(0., 0., -Length * 0.5 + GasSphere_Outer_Radius - Target_Center_Z),
	    Sphere_Logical, "Sphere", World_Logical, false, 0);

	// Tube

	G4Tubs *Tube_Solid =
	    new G4Tubs("Tube_Solid", Tube_Inner_Radius, Tube_Outer_Radius,
		       Tube_Length * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Tube_Logical = new G4LogicalVolume(
	    Tube_Solid, stainlessSteel, "Tube_Logical");
	Tube_Logical->SetVisAttributes(G4Colour::Grey());

	new G4PVPlacement(rotation, global_coordinates + (GasSphere_Outer_Radius + 
						       Tube_Length * 0.5)*symmetry_axis,
			  Tube_Logical, "Tube", World_Logical, false, 0);

	// Valve

	G4Tubs *Valve_Solid =
	    new G4Tubs("Valve_Solid", 0., Valve_Outer_Radius,
		       Valve_Length * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Valve_Logical = new G4LogicalVolume(
	    Valve_Solid, stainlessSteel, "Valve_Logical");
	Valve_Logical->SetVisAttributes(G4Colour::Grey());

	new G4PVPlacement(
	    rotation, global_coordinates + (GasSphere_Outer_Radius + Tube_Length + Valve_Length * 0.5)*symmetry_axis,
	    Valve_Logical, "Valve", World_Logical, false, 0);

}
