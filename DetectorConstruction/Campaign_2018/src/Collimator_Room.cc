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
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Units.hh"
#include "Collimator_Room.hh"

Collimator_Room::Collimator_Room(G4LogicalVolume *World_Log, G4double Coll_Radius):
World_Logical(World_Log),
Collimator_Radius(Coll_Radius),
Collimator_Room_Length(270.*mm + 200.*mm + 16.*inch + 320.*mm + 16.*inch) // Collimator_Length + Collimator_To_Wall1 + Wall1_Z + Wall1_To_Wall2 + Wall2_Z
{}

void Collimator_Room::Construct(G4ThreeVector global_coordinates){

	G4Colour green(0.0, 1.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *Scintillator_Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

	G4double Wall1_To_Wall2 = 320.*mm;
	G4double Collimator_To_Wall1 = 200.*mm;
	G4double BeamTube_Outer_Radius = 1.*inch;


	/**************** COLLIMATOR
	 * *****************************/

	G4double Collimator_X = 60.*mm;
	G4double Collimator_Y = 60.*mm;
	G4double Collimator_Z = 270.*mm;

	G4Box *Collimator_Solid_Solid = new G4Box("Collimator_Solid_Solid", Collimator_X*0.5, Collimator_Y*0.5, Collimator_Z*0.5);
	G4Tubs *Collimator_Hole_Solid = new G4Tubs("Collimator_Hole_Solid", 0., Collimator_Radius, Collimator_Z, 0., twopi);

	G4SubtractionSolid *Collimator_Solid = new G4SubtractionSolid("Collimator_Solid", Collimator_Solid_Solid, Collimator_Hole_Solid);

	G4LogicalVolume *Collimator_Logical = new G4LogicalVolume(Collimator_Solid, Pb, "Collimator_Logical");
	Collimator_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Collimator_Room_Length*0.5 + Collimator_Z*0.5), Collimator_Logical, "Collimator", World_Logical, false, 0);


	/**************** WALL1 Lead wall after paddle
	 * *****************************/

	G4double Wall1_X = 24.*inch;
	G4double Wall1_Y = 16.*inch;
	G4double Wall1_Z = 16.*inch;

	G4Box *Wall1_Solid_Solid = new G4Box("Wall1_Solid_Solid", Wall1_X*0.5, Wall1_Y*0.5, Wall1_Z*0.5);
	G4Tubs *Wall1_Hole_Solid = new G4Tubs("Wall1_Hole_Solid", 0., BeamTube_Outer_Radius, Wall1_Z, 0., twopi);

	G4SubtractionSolid *Wall1_Solid = new G4SubtractionSolid("Wall1_Solid", Wall1_Solid_Solid, Wall1_Hole_Solid);

	G4LogicalVolume *Wall1_Logical = new G4LogicalVolume(Wall1_Solid, Pb, "Wall1_Logical");
	Wall1_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., -Collimator_Room_Length*0.5 + Collimator_Z + Collimator_To_Wall1 + Wall1_Z*0.5), Wall1_Logical, "Wall1", World_Logical, false, 0);

	/**************** WALL2 Lead wall after paddle
	 * *****************************/

	G4double Wall2_X = 24.*inch;
	G4double Wall2_Y = 16.*inch;
	G4double Wall2_Z = 16.*inch;

	G4Box *Wall2_Solid_Solid = new G4Box("Wall2_Solid_Solid", Wall2_X*0.5, Wall2_Y*0.5, Wall2_Z*0.5);
	G4Tubs *Wall2_Hole_Solid = new G4Tubs("Wall2_Hole_Solid", 0., BeamTube_Outer_Radius, Wall2_Z, 0., twopi);

	G4SubtractionSolid *Wall2_Solid = new G4SubtractionSolid("Wall2_Solid", Wall2_Solid_Solid, Wall2_Hole_Solid);

	G4LogicalVolume *Wall2_Logical = new G4LogicalVolume(Wall2_Solid, Pb, "Wall2_Logical");
	Wall2_Logical->SetVisAttributes(green);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Collimator_Room_Length*0.5 - Wall2_Z*0.5), Wall2_Logical, "Wall2", World_Logical, false, 0);

	/**************** Paddle (Scintillator
	 * Detector)****************************/

	G4double Paddle_Thickness = 2. * mm; // Estimated
	G4double Paddle_X = 150. * mm;       // Estimated
	G4double Paddle_Y = 50. * mm;        // Estimated

	G4Box *Paddle_Solid =
	    new G4Box("Paddle_Solid", Paddle_X, Paddle_Y, Paddle_Thickness);
	G4LogicalVolume *Paddle_Logical = new G4LogicalVolume(
	    Paddle_Solid, Scintillator_Plastic, "Paddle_Logical", 0, 0, 0);

	Paddle_Logical->SetVisAttributes(new G4VisAttributes(red));

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Collimator_Room_Length*0.5 - Wall1_Z - 0.5*Wall1_To_Wall2 + 0.5*Paddle_Thickness),
	                  Paddle_Logical, "Paddle", World_Logical, false,
	                  0); // Position estimated

}
