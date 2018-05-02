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
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4PhysicalConstants.hh"

#include "Units.hh"
#include "G3_Wall.hh"
#include "Materials.hh"

G3_Wall::G3_Wall(){

	G4Colour green(0., 0.5, 0.);
	G4Colour white(1.0, 1.0, 1.0);

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

	G4double Concrete_Base_Hor_Z = 8.*inch;
	G4double Concrete_Base_Ver_Z = 15.5*inch;

	G3_Wall_Length =Concrete_Base_Hor_Z + Concrete_Base_Ver_Z;
	G3_Wall_X = 100*inch;
	G3_Wall_Y = 100.*inch;

	// Construct mother volume
	G4Box *G3_Wall_Solid = new G4Box("G3_Wall_Solid", G3_Wall_X*0.5, G3_Wall_Y*0.5, G3_Wall_Length*0.5);
	
	G3_Wall_Logical = new G4LogicalVolume(G3_Wall_Solid, air, "G3_Wall_Logical");
	//G3_Wall_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	

	// Lead Wall
	
	G4double Lead_Wall_Tunnel_X = 12.*inch;
	G4double Lead_Wall_Tunnel_Y = 8.*inch; // Estimtated
	G4double Lead_Wall_Tunnel_Z = 6.*inch;

	G4Box *Lead_Wall_Tunnel_Solid_Solid = new G4Box("Lead_Wall_Tunnel_Solid", Lead_Wall_Tunnel_X*0.5, Lead_Wall_Tunnel_Y*0.5, Lead_Wall_Tunnel_Z*0.5);
	G4Tubs *Lead_Wall_Tunnel_Hole_Solid = new G4Tubs("Lead_Wall_Tunnel_Hole_Solid", 0., 1.*inch, Lead_Wall_Tunnel_Z, 0., twopi);

	G4SubtractionSolid *Lead_Wall_Tunnel_Solid = new G4SubtractionSolid("Lead_Wall_Tunnel_Solid", Lead_Wall_Tunnel_Solid_Solid, Lead_Wall_Tunnel_Hole_Solid);

	G4LogicalVolume *Lead_Wall_Tunnel_Logical = new G4LogicalVolume(Lead_Wall_Tunnel_Solid, Pb, "Lead_Wall_Tunnel_Logical");
	Lead_Wall_Tunnel_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., 0., -G3_Wall_Length*0.5 + 2.*inch + Lead_Wall_Tunnel_Z*0.5), Lead_Wall_Tunnel_Logical, "Lead_Wall_Tunnel", G3_Wall_Logical, false, 0, false);

	G4double Lead_Wall_Base_X = 12.*inch; // Estimated
	G4double Lead_Wall_Base_Y = 6.*inch;
	G4double Lead_Wall_Base_Z = 8.*inch;

	G4Box *Lead_Wall_Base_Solid = new G4Box("Lead_Wall_Base_Solid", Lead_Wall_Base_X*0.5, Lead_Wall_Base_Y*0.5, Lead_Wall_Base_Z*0.5);

	G4LogicalVolume *Lead_Wall_Base_Logical = new G4LogicalVolume(Lead_Wall_Base_Solid, Pb, "Lead_Wall_Base_Logical");
	Lead_Wall_Base_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., -Lead_Wall_Tunnel_Y*0.5 - Lead_Wall_Base_Y*0.5, -G3_Wall_Length*0.5 + Lead_Wall_Base_Z*0.5), Lead_Wall_Base_Logical, "Lead_Wall_Base", G3_Wall_Logical, false, 0, false);

	G4double Lead_Wall_Top_X = 8.*inch;
	G4double Lead_Wall_Top_Y = 2.*inch;
	G4double Lead_Wall_Top_Z = 4.*inch;

	G4Box *Lead_Wall_Top_Solid = new G4Box("Lead_Wall_Top_Solid", Lead_Wall_Top_X*0.5, Lead_Wall_Top_Y*0.5, Lead_Wall_Top_Z*0.5);

	G4LogicalVolume *Lead_Wall_Top_Logical = new G4LogicalVolume(Lead_Wall_Top_Solid, Pb, "Lead_Wall_Top_Logical");
	Lead_Wall_Top_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, G4ThreeVector(0., Lead_Wall_Tunnel_Y*0.5 + Lead_Wall_Top_Y*0.5, -G3_Wall_Length*0.5 + Lead_Wall_Base_Z - Lead_Wall_Top_Z*0.5), Lead_Wall_Top_Logical, "Lead_Wall_Top", G3_Wall_Logical, false, 0, false);
	
	// Concrete base
	
	G4double Concrete_Base_Hor_X = 15.5*inch;
	G4double Concrete_Base_Hor_Y = 41.*inch;

	G4Box *Concrete_Base_Hor_Solid = new G4Box("Concrete_Base_Hor_Solid", Concrete_Base_Hor_X*0.5, Concrete_Base_Hor_Y*0.5, Concrete_Base_Hor_Z*0.5);

	G4LogicalVolume *Concrete_Base_Hor_Logical = new G4LogicalVolume(Concrete_Base_Hor_Solid, concrete, "Concrete_Base_Hor_Logical");
	Concrete_Base_Hor_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, G4ThreeVector(0., -Lead_Wall_Tunnel_Y*0.5 - Lead_Wall_Base_Y - Concrete_Base_Hor_Y*0.5, -G3_Wall_Length*0.5 + Concrete_Base_Hor_Z*0.5), Concrete_Base_Hor_Logical, "Concrete_Base_Hor", G3_Wall_Logical, false, 0, false);

	G4double Concrete_Base_Ver_X = 8.*inch;
	G4double Concrete_Base_Ver_Y = 30.*inch; // Estimated

	G4Box *Concrete_Base_Ver_Solid = new G4Box("Concrete_Base_Ver_Solid", Concrete_Base_Ver_X*0.5, Concrete_Base_Ver_Y*0.5, Concrete_Base_Ver_Z*0.5);

	G4LogicalVolume *Concrete_Base_Ver_Logical = new G4LogicalVolume(Concrete_Base_Ver_Solid, concrete, "Concrete_Base_Ver_Logical");
	Concrete_Base_Ver_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, G4ThreeVector(0., -Lead_Wall_Tunnel_Y*0.5 - Lead_Wall_Base_Y - Concrete_Base_Hor_Y + Concrete_Base_Ver_Y*0.5, -G3_Wall_Length*0.5 + Concrete_Base_Hor_Z + Concrete_Base_Ver_Z*0.5), Concrete_Base_Ver_Logical, "Concrete_Base_Ver", G3_Wall_Logical, false, 0, false);
}
