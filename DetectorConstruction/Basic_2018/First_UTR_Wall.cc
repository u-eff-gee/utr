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

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"

#include "Units.hh"
#include "First_UTR_Wall.hh"

First_UTR_Wall::First_UTR_Wall(){

	G4Colour green(0., 0.5, 0.);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

	First_UTR_Wall_Length = 8.*inch;
	G4double First_UTR_Wall_X = 22.*inch; // Estimated
	G4double First_UTR_Wall_Y = 26.*inch;
	Z_Axis_Offset_Y = 1.*inch;
	G4double Beam_Pipe_Outer_Radius = 1.*inch;

	// Construct mother volume
	G4Box *First_UTR_Wall_Solid = new G4Box("First_UTR_Wall_Solid", First_UTR_Wall_X*0.5, First_UTR_Wall_Y*0.5, First_UTR_Wall_Length*0.5);
	
	First_UTR_Wall_Logical = new G4LogicalVolume(First_UTR_Wall_Solid, air, "First_UTR_Wall_Logical");
	//First_UTR_Wall_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4Box *Lead_Wall_Solid_Solid = new G4Box("Lead_Wall_Solid_Solid", First_UTR_Wall_X*0.5, First_UTR_Wall_Y*0.5, First_UTR_Wall_Length*0.5);
	G4Tubs *Lead_Wall_Hole_Solid = new G4Tubs("Lead_Wall_Hole_Solid", 0., Beam_Pipe_Outer_Radius, First_UTR_Wall_Length, 0., twopi);

	G4SubtractionSolid *Lead_Wall_Solid = new G4SubtractionSolid("Lead_Wall_Solid", Lead_Wall_Solid_Solid, Lead_Wall_Hole_Solid, 0, G4ThreeVector(0., -Z_Axis_Offset_Y, 0.));

	G4LogicalVolume *Lead_Wall_Logical = new G4LogicalVolume(Lead_Wall_Solid, Pb, "Lead_Wall_Logical");
	Lead_Wall_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(), Lead_Wall_Logical, "Lead_Wall", First_UTR_Wall_Logical, false, 0, false);
}
