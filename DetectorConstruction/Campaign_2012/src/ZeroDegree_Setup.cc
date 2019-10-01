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
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "ZeroDegree_Setup.hh"
#include "ZeroDegree.hh"
#include "Units.hh"

#include "utrConfig.h"

ZeroDegree_Setup::ZeroDegree_Setup(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void ZeroDegree_Setup::Construct(G4ThreeVector global_coordinates){

	// Zero degree detector
	
	ZeroDegree zeroDegree("ZeroDegree");

	G4double ZeroDegree_X = 0.*mm; // X = 0, if beam profile is measured, otherwise, X < 0
	G4double ZeroDegree_Y = zerodegree_offset*mm; // Seems to reproduce the beam spectra best
	G4double ZeroDegree_Z = zeroDegree.Get_Length()*0.5;

	G4double ZeroDegree_AngleX = 0 * deg;
	G4double ZeroDegree_AngleY = 180. * deg;

	G4RotationMatrix *rotateZeroDegree = new G4RotationMatrix();
	rotateZeroDegree->rotateX(ZeroDegree_AngleX);
	rotateZeroDegree->rotateY(ZeroDegree_AngleY);

	new G4PVPlacement(rotateZeroDegree, global_coordinates + G4ThreeVector(ZeroDegree_X, ZeroDegree_Y, ZeroDegree_Z), zeroDegree.Get_Logical(), "ZeroDegree", World_Logical, false, 0, false);

}
