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

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"

#include "Units.hh"
#include "Beampipe.hh"

Beampipe::Beampipe(G4LogicalVolume *World_Log):
World_Logical(World_Log),
Beampipe_Downstream_Length(85.*inch), // Estimated
Beampipe_Length(110.*inch + 85.*inch) // Estimated
{
	Z_Axis_Offset_Z = (Beampipe_Downstream_Length - 0.5*Beampipe_Length);

}

void Beampipe::Construct(G4ThreeVector global_coordinates, G4double relative_density){

	G4NistManager *nist = G4NistManager::Instance();

	G4Element *nat_O = nist->FindOrBuildElement("O");
	G4Element *nat_N = nist->FindOrBuildElement("N");
	G4Element *nat_C = nist->FindOrBuildElement("C");
	G4Element *nat_Ar = nist->FindOrBuildElement("Ar");


	G4double density_of_air = 1.225e-3 * g / cm3; // Density of air at sea level and 288K, Wikipedia
	G4double density = relative_density*density_of_air;
	G4Material *vacuum_material = new G4Material("beam_pipe_vacuum", density, 4); 

	vacuum_material->AddElement(nat_O, 23.1781 * perCent);
	vacuum_material->AddElement(nat_N, 75.5268 * perCent);
	vacuum_material->AddElement(nat_Ar, 1.2827 * perCent);
	vacuum_material->AddElement(nat_C,  0.0124 * perCent);

	G4Material *plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	
	// Tube

	G4double Beampipe_Outer_Radius = 1. * inch;     // Measured
	G4double Beampipe_Thickness = 0.125 * inch; 	// Measured

	G4Tubs *Beampipe_Solid = new G4Tubs(
	    "Beampipe_Solid", Beampipe_Outer_Radius - Beampipe_Thickness,
	    Beampipe_Outer_Radius, Beampipe_Length *0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Beampipe_Logical = new G4LogicalVolume(
	    Beampipe_Solid, plexiglass, "Beampipe_Logial", 0, 0, 0);

	Beampipe_Logical->SetVisAttributes(G4Colour::White());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Z_Axis_Offset_Z),
	                  Beampipe_Logical, "Beampipe", World_Logical, false, 0);

	// End cap on upstream side

	G4double BeampipeEndCap_Outer_Radius = 1.3 * inch; // Estimated
	G4double BeampipeEndCap_Length = 30. * mm;         // Estimated

	G4Tubs *BeampipeEndCap_Solid =
	    new G4Tubs("BeampipeEndCap_Solid", Beampipe_Outer_Radius,
	               BeampipeEndCap_Outer_Radius, BeampipeEndCap_Length *0.5,
	               0. * deg, 360. * deg);
	G4LogicalVolume *BeampipeEndCap_Logical = new G4LogicalVolume(
	    BeampipeEndCap_Solid, plexiglass, "BeampipeEndCap_Logical", 0, 0, 0);

	BeampipeEndCap_Logical->SetVisAttributes(G4Colour::White());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Z_Axis_Offset_Z - Beampipe_Length*0.5 +
	                                               BeampipeEndCap_Length *0.5),
	                  BeampipeEndCap_Logical, "BeampipeEndCap", World_Logical,
	                  false, 0);

	G4double BeampipeEndCapWindow_Thickness = 2. * mm; // Estimated

	G4Tubs *BeampipeEndCapWindow_Solid = new G4Tubs(
	    "BeampipeEndCapWindow_Solid", 0., BeampipeEndCap_Outer_Radius,
	    BeampipeEndCapWindow_Thickness *0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *BeampipeEndCapWindow_Logical =
	    new G4LogicalVolume(BeampipeEndCapWindow_Solid, plexiglass,
	                        "BeampipeEndCapWindow_Logical", 0, 0, 0);

	BeampipeEndCapWindow_Logical->SetVisAttributes(G4Colour::White());

	new G4PVPlacement(
	    0, global_coordinates + G4ThreeVector(0., 0., Z_Axis_Offset_Z - Beampipe_Length*0.5 -
	                                 BeampipeEndCapWindow_Thickness *0.5),
	    BeampipeEndCapWindow_Logical, "BeampipeEndCapWindow", World_Logical, false,
	    0);

	// Vacuum inside beam tube

	G4Tubs *BeampipeVacuum_Solid =
	    new G4Tubs("BeampipeVacuum_Solid", 0. * mm,
	               Beampipe_Outer_Radius - Beampipe_Thickness,
	               Beampipe_Length *0.5, 0. * deg, 360. * deg);
	G4LogicalVolume *BeampipeVacuum_Logical = new G4LogicalVolume(
	    BeampipeVacuum_Solid, vacuum_material, "BeampipeVacuum_Logical", 0, 0, 0);

	BeampipeVacuum_Logical->SetVisAttributes(G4Colour::Cyan());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Z_Axis_Offset_Z),
	                  BeampipeVacuum_Logical, "BeampipeVacuum", World_Logical,
	                  false, 0);

	// Target holder tube in which the second target is mounted
	
	G4double TargetTube_Outer_Radius = Beampipe_Outer_Radius - Beampipe_Thickness;

	G4double TargetTubeFront_Length = 2.*inch; 
	G4double TargetTubeFront_Inner_Radius = TargetTube_Outer_Radius - 1. * mm; // Measured

	G4double TargetTubeBack_Length = 23.*inch;
	G4double TargetTubeBack_Inner_Radius = TargetTube_Outer_Radius - 2. * mm; 

	G4double TargetTube_Total_Length =
	    TargetTubeFront_Length + TargetTubeBack_Length;

	G4Tubs *TargetTubeBack_Solid =
	    new G4Tubs("TargetTubeBack_Solid",
	               TargetTubeBack_Inner_Radius,
	               TargetTube_Outer_Radius, TargetTubeBack_Length * 0.5,
	               0. * deg, 360. * deg);

	G4Tubs *TargetTubeFront_Solid =
	    new G4Tubs("TargetTubeFront_Solid",
	               TargetTubeFront_Inner_Radius,
	               TargetTube_Outer_Radius, TargetTubeFront_Length * 0.5,
	               0. * deg, 360. * deg);

	G4UnionSolid *TargetTube_Solid = new G4UnionSolid(
	    "TargetTube_Solid", TargetTubeBack_Solid, TargetTubeFront_Solid, 0,
	    G4ThreeVector(0., 0., -TargetTubeBack_Length * 0.5 -
	                              TargetTubeFront_Length * 0.5));

	// G3 setup target tube

	G4LogicalVolume *TargetTube_G3_Logical = new G4LogicalVolume(
	    TargetTube_Solid, plexiglass, "TargetTube_G3_Logical", 0, 0, 0);

	TargetTube_G3_Logical->SetVisAttributes(G4Colour::Magenta());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Z_Axis_Offset_Z + 0.5*TargetTube_Total_Length), TargetTube_G3_Logical, "TargetTube_G3", BeampipeVacuum_Logical, false, 0);
	
	// Second setup target tube

	G4LogicalVolume *TargetTube_2nd_Logical = new G4LogicalVolume(
	    TargetTube_Solid, plexiglass, "TargetTube_2nd_Logical", 0, 0, 0);

	TargetTube_2nd_Logical->SetVisAttributes(G4Colour::Magenta());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Z_Axis_Offset_Z + Beampipe_Length*0.5 - TargetTubeBack_Length*0.5), TargetTube_2nd_Logical, "TargetTube_2nd", World_Logical, false, 0);

	G4Tubs *TargetTubeLid_Solid = new G4Tubs("TargetTubeLid_Solid", 0., Beampipe_Outer_Radius+2.*mm, BeampipeEndCapWindow_Thickness*0.5, 0., twopi);
	G4LogicalVolume *TargetTubeLid_Logical = new G4LogicalVolume(TargetTubeLid_Solid, plexiglass, "TargetTubeLid_Logical");
	TargetTubeLid_Logical->SetVisAttributes(G4Colour::Magenta());
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Length*0.5 + BeampipeEndCapWindow_Thickness*0.5), TargetTubeLid_Logical, "TargetTubeLid", BeampipeVacuum_Logical, false, 0);

	// Target ring
	
	G4double Target_Ring_Length = 1.5*inch; // Estimated
	G4double Target_Ring_Outer_Radius = TargetTubeFront_Inner_Radius;
	G4double Target_Ring_Inner_Radius = TargetTubeFront_Inner_Radius - 1.*mm;
	
	G4Tubs *Target_Ring_Solid = new G4Tubs("Target_Ring_Solid", Target_Ring_Inner_Radius, Target_Ring_Outer_Radius, Target_Ring_Length*0.5, 0., twopi);

	G4LogicalVolume *Target_Ring_G3_Logical = new G4LogicalVolume(Target_Ring_Solid, plexiglass, "Target_Ring_G3_Logical");

	Target_Ring_G3_Logical->SetVisAttributes(G4Colour::Red());
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Z_Axis_Offset_Z), Target_Ring_G3_Logical, "Target_Ring_G3", BeampipeVacuum_Logical, false, 0, false);

	G4LogicalVolume *Target_Ring_2nd_Logical = new G4LogicalVolume(Target_Ring_Solid, plexiglass, "Target_Ring_2nd_Logical");

	Target_Ring_2nd_Logical->SetVisAttributes(G4Colour::Red());
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Length*0.5 - TargetTubeBack_Length - Target_Ring_Length*0.5), Target_Ring_2nd_Logical, "Target_Ring_2nd", BeampipeVacuum_Logical, false, 0, false);
}
