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
#include "Vacuum.hh"
#include "Beampipe_Upstream.hh"

Beampipe_Upstream::Beampipe_Upstream(G4LogicalVolume *World_Log):
World_Logical(World_Log),
Beampipe_Upstream_Upstream_Length(112.*inch), // Estimated
Beampipe_Upstream_Downstream_Length(35.*inch) // Estimated
{
	Beampipe_Upstream_Length = Beampipe_Upstream_Upstream_Length + Beampipe_Upstream_Downstream_Length;
	Z_Axis_Offset_Z = 0.5*(Beampipe_Upstream_Downstream_Length - Beampipe_Upstream_Upstream_Length);

}

void Beampipe_Upstream::Construct(G4ThreeVector global_coordinates, G4double relative_density){
	
	G4Colour white(1.0, 1.0, 1.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour red(1.0, 0., 0.);

	G4NistManager *nist = G4NistManager::Instance();

	G4cout << "Creating vacuum" << G4endl;
	Vacuum vac(relative_density);
	G4Material *vacuum = vac.Get_Material();
	G4Material *plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");

	G4double Beampipe_Inner_Radius = 0.875*inch;
	G4double Beampipe_Outer_Radius = 1.*inch;
	G4double Beampipe_Joint_Outer_Radius = 1.4*inch; // Estimated
	G4double Beampipe_Joint_Length = 3.*inch; // Estimated
	
	// Beam pipe vacuum
	G4Tubs *Vacuum_Solid = new G4Tubs("Vacuum_Solid", 0., Beampipe_Inner_Radius, Beampipe_Upstream_Length*0.5, 0., twopi);
	Vacuum_Logical = new G4LogicalVolume(Vacuum_Solid, vacuum, "Vacuum_Logical");
	G4cout << Vacuum_Logical->GetName() << G4endl;

	//Vacuum_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	Vacuum_Logical->SetVisAttributes(red);
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., 0.), Vacuum_Logical, "Vacuum", World_Logical, false, 0, false);

	// Beam pipe
	G4Tubs *Pipe_Upstream_Solid = new G4Tubs("Pipe_Upstream_Solid", Beampipe_Inner_Radius, Beampipe_Outer_Radius, Beampipe_Upstream_Length*0.5, 0., twopi);
	G4LogicalVolume *Pipe_Upstream_Logical = new G4LogicalVolume(Pipe_Upstream_Solid, plexiglass, "Pipe_Upstream_Logical");

	Pipe_Upstream_Logical->SetVisAttributes(white);	
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., 0.), Pipe_Upstream_Logical, "Pipe_Upstream", World_Logical, false, 0, false);

	// Beampipe downstream joint
	
	G4double Downstream_Joint_To_Target = 12.*inch; // Estimated
	
	G4Tubs *Downstream_Joint_Solid = new G4Tubs("Downstream_Joint_Solid", Beampipe_Outer_Radius, Beampipe_Joint_Outer_Radius, Beampipe_Joint_Length*0.5, 0., twopi);
	G4LogicalVolume *Downstream_Joint_Logical = new G4LogicalVolume(Downstream_Joint_Solid, plexiglass, "Downstream_Joint_Logical");

	Downstream_Joint_Logical->SetVisAttributes(white);
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Upstream_Length*0.5 - Beampipe_Upstream_Downstream_Length + Downstream_Joint_To_Target), Downstream_Joint_Logical, "Downstream_Joint", World_Logical, false, 0, false);
	
	// Downstream exit window
	
	G4double Downstream_Exit_Window_Thickness = 2.*mm; // Estimated
	
	G4Tubs *Downstream_Exit_Window_Solid = new G4Tubs("Downstream_Exit_Window_Solid", 0., Beampipe_Inner_Radius, Downstream_Exit_Window_Thickness*0.5, 0., twopi);
	G4LogicalVolume *Downstream_Exit_Window_Logical = new G4LogicalVolume(Downstream_Exit_Window_Solid, plexiglass, "Downstream_Exit_Window_Logical");

	Downstream_Exit_Window_Logical->SetVisAttributes(white);
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Upstream_Length*0.5 - Downstream_Exit_Window_Thickness*0.5), Downstream_Exit_Window_Logical, "Downstream_Exit_Window", World_Logical, false, 0, false);

	// Upstream exit window
	
	G4double Upstream_Exit_Window_Thickness = 2.*mm; // Estimated
	
	G4Tubs *Upstream_Exit_Window_Solid = new G4Tubs("Upstream_Exit_Window_Solid", 0., Beampipe_Inner_Radius, Upstream_Exit_Window_Thickness*0.5, 0., twopi);
	G4LogicalVolume *Upstream_Exit_Window_Logical = new G4LogicalVolume(Upstream_Exit_Window_Solid, plexiglass, "Upstream_Exit_Window_Logical");

	Upstream_Exit_Window_Logical->SetVisAttributes(white);
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Beampipe_Upstream_Length*0.5 + Upstream_Exit_Window_Thickness*0.5), Upstream_Exit_Window_Logical, "Upstream_Exit_Window", World_Logical, false, 0, false);
	
	// Target Holder Tube
	
	G4double TargetTube_Outer_Radius = Beampipe_Inner_Radius;

	G4double TargetTubeFront_Length = 2.*inch; 
	G4double TargetTubeFront_Inner_Radius = TargetTube_Outer_Radius - 1. * mm;

	G4double TargetTubeBack_Length = Downstream_Joint_To_Target - TargetTubeFront_Length*0.5;
	G4double TargetTubeBack_Inner_Radius = TargetTube_Outer_Radius - 2. * mm; 

	G4double TargetTube_Total_Length =
	    TargetTubeFront_Length + TargetTubeBack_Length;

	G4Tubs *TargetTubeBack_Solid =
	    new G4Tubs("TargetTubeback_Solid",
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

	G4LogicalVolume *TargetTube_Logical = new G4LogicalVolume(
	    TargetTube_Solid, plexiglass, "TargetTube_Logical", 0, 0, 0);

	TargetTube_Logical->SetVisAttributes(new G4VisAttributes(orange));

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Upstream_Length*0.5 - Beampipe_Upstream_Downstream_Length + Downstream_Joint_To_Target - TargetTube_Total_Length * 0.5 + TargetTubeFront_Length*0.5), TargetTube_Logical, "TargetTube", World_Logical, false, 0);

	// Target ring
	
	G4double Target_Ring_Length = 1.5*inch; // Estimated
	G4double Target_Ring_Outer_Radius = TargetTubeFront_Inner_Radius;
	G4double Target_Ring_Inner_Radius = TargetTubeFront_Inner_Radius - 1.*mm;
	
	G4Tubs *Target_Ring_Solid = new G4Tubs("Target_Ring_Solid", Target_Ring_Inner_Radius, Target_Ring_Outer_Radius, Target_Ring_Length*0.5, 0., twopi);
	G4LogicalVolume *Target_Ring_Logical = new G4LogicalVolume(Target_Ring_Solid, plexiglass, "Target_Ring_Logical");

	Target_Ring_Logical->SetVisAttributes(red);
	
	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Beampipe_Upstream_Length*0.5 - Beampipe_Upstream_Downstream_Length), Target_Ring_Logical, "Target_Ring", World_Logical, false, 0, false);
}
