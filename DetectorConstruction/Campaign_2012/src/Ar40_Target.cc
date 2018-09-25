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
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Sphere.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Ar40_Target.hh"

Ar40_Target::Ar40_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Ar40_Target::Construct(G4ThreeVector global_coordinates){

	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour white(1.0, 1.0, 1.0);
	G4Colour green(0.0, 1.0, 0.0);

	G4NistManager *nist = G4NistManager::Instance();

	// Target material: natural argon
	G4double Cylinder_Mass = 1408. * g;
	G4double Cylinder_Empty_Mass = 1028. * g;
	G4double Argon_Volume = 0.7e6 * mm3;
	G4double Argon_Mass = Cylinder_Mass - Cylinder_Empty_Mass;

	G4double Argon_Density = Argon_Mass/Argon_Volume;
	G4Isotope *Ar36 = new G4Isotope("36Ar", 18, 18, 35.967545*g/mole);
	G4Isotope *Ar38 = new G4Isotope("38Ar", 18, 20, 37.962732*g/mole);
	G4Isotope *Ar40 = new G4Isotope("40Ar", 18, 22, 39.962383*g/mole);

	G4Element *elAr = new G4Element("elemental_Argon", "elAr", 3);
	elAr->AddIsotope(Ar36,  0.333*perCent);
	elAr->AddIsotope(Ar38,  0.063*perCent);
	elAr->AddIsotope(Ar40, 99.604*perCent);

	G4Material *Target_Material = new G4Material("Target_Material", Argon_Density, 1);
	Target_Material->AddElement(elAr, 1);

	// Hull material of the gas bottle: aluminium wrapped with carbon fiber
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4double Carbon_Fiber_Density = 1.8 * g/cm3; // Wikipedia
	G4Isotope *C12 = new G4Isotope("12C", 6, 6, 12.      *g/mole);
	G4Isotope *C13 = new G4Isotope("13C", 6, 7, 13.003354*g/mole);

	G4Element *elC = new G4Element("elemental_Carbon", "elC", 2);
	elC->AddIsotope(C12, 98.9*perCent);
	elC->AddIsotope(C13,  1.1*perCent);

	G4Material *Carbon_Fiber = new G4Material("Carbon_Fiber", Carbon_Fiber_Density, 1);
	Carbon_Fiber->AddElement(elC, 1);

	// Material of the valve. Actually brass, but considering the uncertainties associated with this part, simply take copper
	G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");

	// Create physical target
	// Target dimensions reconstructed from the data of the gas bottle and the measured total mass. Estimated quantities are marked.
	// The gas cylinder was probably a Luxfer P07A, manufactored for use in paintball sport

	G4double Cylinder_Length = 203.*mm;
	G4double Cylinder_Radius = 46.5*mm;
	G4double Cylinder_Al_Wall_Thickness = 8.66*mm; // Estimated
	G4double Cylinder_Carbon_Wall_Thickness = 0.5*mm; // Estimated

	G4double Valve_Length = 30.*mm; // Estimated
	G4double Valve_Radius = 15.*mm; // Estimated

	// Target material (40Ar)
	G4UnionSolid *Target_Solid = Create_Pill_Shape("Target_Solid", 0.5*(Cylinder_Length - 2.*Cylinder_Radius), 0., Cylinder_Radius - Cylinder_Al_Wall_Thickness - Cylinder_Carbon_Wall_Thickness);

	G4LogicalVolume *Target_Logical = new G4LogicalVolume(Target_Solid, Target_Material, "Ar40_Logical");
	Target_Logical->SetVisAttributes(yellow);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Target_Logical, "Ar40_Target", World_Logical, false, 0);

	// Aluminium case of gas bottle

	G4UnionSolid *Aluminium_Case_Solid = Create_Pill_Shape("Aluminium_Case_Solid", 0.5*(Cylinder_Length - 2.*Cylinder_Radius), Cylinder_Radius - Cylinder_Al_Wall_Thickness - Cylinder_Carbon_Wall_Thickness, Cylinder_Radius - Cylinder_Carbon_Wall_Thickness);

	G4LogicalVolume *Aluminium_Case_Logical = new G4LogicalVolume(Aluminium_Case_Solid, Al, "Aluminium_Case_Logical");
	Aluminium_Case_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Aluminium_Case_Logical, "Aluminium_Case", World_Logical, false, 0);

	// Carbon case of gas bottle

	G4UnionSolid *Carbon_Case_Solid = Create_Pill_Shape("Carbon_Case_Solid", 0.5*(Cylinder_Length - 2.*Cylinder_Radius), Cylinder_Radius - Cylinder_Carbon_Wall_Thickness, Cylinder_Radius);

	G4LogicalVolume *Carbon_Case_Logical = new G4LogicalVolume(Carbon_Case_Solid, Carbon_Fiber, "Carbon_Case_Logical");
	Carbon_Case_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Carbon_Case_Logical, "Carbon_Case", World_Logical, false, 0);

	// Filling valve of gas bottle

	G4Tubs *Valve_Solid = new G4Tubs("Valve_Solid", 0., Valve_Radius, Valve_Length*0.5, 0., twopi);
	G4LogicalVolume *Valve_Logical = new G4LogicalVolume(Valve_Solid, Cu, "Valve_Logical");
	Valve_Logical->SetVisAttributes(orange);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Cylinder_Length*0.5 + Valve_Length*0.5), Valve_Logical, "Valve", World_Logical, false, 0);

	// Shielding around the downstream part of the gas bottle and the filling valve
	
	G4double Lead_Shield_Bottle_XY = 200.*mm;
	G4double Lead_Shield_Bottle_Z = 50.*mm;
	G4double Lead_Shield_Bottle_Hole_Radius = Cylinder_Radius + 2.*mm; // Gap estimated
	
	G4double Lead_Shield_Valve_XY = 200.*mm;
	G4double Lead_Shield_Valve_Z = 10.*mm;
	G4double Lead_Shield_Valve_Hole_Radius = Valve_Radius + 2.*mm; // Gap estimated

	G4Box *Lead_Shield_Bottle_Solid_Solid = new G4Box("Lead_Shield_Bottle_Solid_Solid", Lead_Shield_Bottle_XY*0.5, Lead_Shield_Bottle_XY*0.5, Lead_Shield_Bottle_Z*0.5);
	G4Tubs *Lead_Shield_Bottle_Hole_Solid = new G4Tubs("Lead_Shield_Bottle_Hole_Solid", 0., Lead_Shield_Bottle_Hole_Radius, Lead_Shield_Bottle_Z, 0., twopi);
	G4SubtractionSolid *Lead_Shield_Bottle_Solid = new G4SubtractionSolid("Lead_Shield_Bottle_Solid", Lead_Shield_Bottle_Solid_Solid, Lead_Shield_Bottle_Hole_Solid);

	G4LogicalVolume *Lead_Shield_Bottle_Logical = new G4LogicalVolume(Lead_Shield_Bottle_Solid, Pb, "Lead_Shield_Bottle_Logical");
	Lead_Shield_Bottle_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Cylinder_Length*0.5 - Lead_Shield_Bottle_Z*0.5), Lead_Shield_Bottle_Logical, "Lead_Shield_Bottle", World_Logical, false, 0);

	G4Box *Lead_Shield_Valve_Solid_Solid = new G4Box("Lead_Shield_Valve_Solid_Solid", Lead_Shield_Valve_XY*0.5, Lead_Shield_Valve_XY*0.5, Lead_Shield_Valve_Z*0.5);
	G4Tubs *Lead_Shield_Valve_Hole_Solid = new G4Tubs("Lead_Shield_Valve_Hole_Solid", 0., Lead_Shield_Valve_Hole_Radius, Lead_Shield_Valve_Z, 0., twopi);
	G4SubtractionSolid *Lead_Shield_Valve_Solid = new G4SubtractionSolid("Lead_Shield_Valve_Solid", Lead_Shield_Valve_Solid_Solid, Lead_Shield_Valve_Hole_Solid);

	G4LogicalVolume *Lead_Shield_Valve_Logical = new G4LogicalVolume(Lead_Shield_Valve_Solid, Pb, "Lead_Shield_Valve_Logical");
	Lead_Shield_Valve_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Cylinder_Length*0.5 + Lead_Shield_Valve_Z*0.5), Lead_Shield_Valve_Logical, "Lead_Shield_Valve", World_Logical, false, 0);
}

G4UnionSolid* Ar40_Target::Create_Pill_Shape(const G4String &Solid_Name, G4double Pill_Length, G4double Pill_Rmin, G4double Pill_Rmax){

	G4double safety_distance = 0.;

	G4Tubs *Pill_Cylinder_Solid = new G4Tubs("Pill_Cylinder_Solid", Pill_Rmin, Pill_Rmax, Pill_Length, 0., twopi);
	G4Sphere *Pill_Semisphere_Solid = new G4Sphere("Pill_Semisphere_Solid", Pill_Rmin, Pill_Rmax, 0., twopi, 0., 0.5*pi);

	G4UnionSolid *Pill_Temp = new G4UnionSolid("Pill_Temp", Pill_Cylinder_Solid, Pill_Semisphere_Solid, 0, G4ThreeVector(0., 0., Pill_Length - safety_distance));

	G4RotationMatrix *rot = new G4RotationMatrix();
	rot->rotateY(180.*deg);

	G4UnionSolid *Pill_Solid = new G4UnionSolid(Solid_Name, Pill_Temp, Pill_Semisphere_Solid, rot, G4ThreeVector(0., 0., -Pill_Length + safety_distance));

	return Pill_Solid;
}

