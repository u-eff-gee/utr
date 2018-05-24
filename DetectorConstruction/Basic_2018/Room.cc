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
#include "G4SystemOfUnits.hh"

#include "Units.hh"
#include "Room.hh"

Room::Room(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Room::Construct(G4ThreeVector global_coordinates, G4double World_x, G4double World_y, G4double World_z,
        G4double Wall_pos, G4double Floor_pos){

	G4Colour grey(0.5, 0.5, 0.5);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    Wall_Thickness = 14.*cm; // Measured by U. Gayer
    Floor_Thickness = 26.8*cm; // Estimated from drawings

    G4double Beampipe_Outer_Radius = 1.*inch;

    // Construct Wall between UTR and collimator room
	G4Box *Room_Wall_Solid_Solid = new G4Box("Room_Wall_Solid_Solid", World_x*0.5, World_y*0.5, Wall_Thickness*0.5);

	G4Tubs *Room_Wall_Hole_Solid = new G4Tubs("Room_Wall_Hole_Solid", 0., Beampipe_Outer_Radius, Wall_Thickness*0.5, 0., twopi);

	G4SubtractionSolid *Room_Wall_Solid = new G4SubtractionSolid("Room_Wall_Solid", Room_Wall_Solid_Solid, Room_Wall_Hole_Solid);

	G4LogicalVolume *Room_Wall_Logical = new G4LogicalVolume(Room_Wall_Solid, concrete, "Room_Wall_Logical");
	Room_Wall_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., Wall_pos - Wall_Thickness*0.5), Room_Wall_Logical, "Room_Wall", World_Logical, false, 0, false);

    // Construct Floor between UTR and planet Earth
	G4Box *Room_Floor_Solid = new G4Box("Room_Floor_Solid", World_x*0.5, Floor_Thickness*0.5, World_z*0.5);

	G4LogicalVolume *Room_Floor_Logical = new G4LogicalVolume(Room_Floor_Solid, concrete, "Room_Floor_Logical");
	Room_Floor_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., Floor_pos - Floor_Thickness*0.5, 0), Room_Floor_Logical, "Room_Floor", World_Logical, false, 0, false);

}
