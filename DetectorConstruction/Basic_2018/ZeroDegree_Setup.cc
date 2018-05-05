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

ZeroDegree_Setup::ZeroDegree_Setup(){

	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	G4double ZeroDegree_Setup_X = 40.*inch; // Dimension of mother volume, arbitrary
	G4double ZeroDegree_Setup_Y = 20.*inch; // Dimension of mother volume, arbitrary
	ZeroDegree_Setup_Length = 20.*inch; // Dimension of mother volume, arbitrary
	
	// Mother volume
	G4Box *ZeroDegree_Setup_Solid = new G4Box("ZeroDegree_Setup_Solid", ZeroDegree_Setup_X*0.5, ZeroDegree_Setup_Y*0.5, ZeroDegree_Setup_Length*0.5);
	ZeroDegree_Setup_Logical = new G4LogicalVolume(ZeroDegree_Setup_Solid, air, "ZeroDegree_Setup_Logical");

	// Zero degree detector
	
	ZeroDegree zeroDegree("ZeroDegree");

	G4double ZeroDegree_X = 0.*mm; // X = 0, if beam profile is measured, otherwise, X < 0
	G4double ZeroDegree_Y = 30.*mm; // Seems to reproduce the beam spectra best
	G4double ZeroDegree_Z = zeroDegree.Get_Length()*0.5;

	G4double ZeroDegree_AngleX = 0 * deg;
	G4double ZeroDegree_AngleY = 180. * deg;

	G4RotationMatrix *rotateZeroDegree = new G4RotationMatrix();
	rotateZeroDegree->rotateX(ZeroDegree_AngleX);
	rotateZeroDegree->rotateY(ZeroDegree_AngleY);

	new G4PVPlacement(rotateZeroDegree, G4ThreeVector(ZeroDegree_X, ZeroDegree_Y, ZeroDegree_Z), zeroDegree.Get_Logical(), "ZeroDegree", ZeroDegree_Setup_Logical, false, 0, false);

}
