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
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "DetectorConstruction.hh"

class FilterCaseWall {
  private:
	G4LogicalVolume *FilterCaseWall_Logical;
	G4LogicalVolume *World_Logical;

	G4RotationMatrix *rot;

  public:
	G4double FilterCase_Length;
	G4double FilterCase_Inner_Radius;
	G4double FilterCase_Wall_Thickness;

	G4double Length;
	G4double Radius;

	FilterCaseWall(G4LogicalVolume *world_Logical) {
		FilterCase_Length = 80. * mm;        // Estimated
		FilterCase_Inner_Radius = 45. * mm;  // Estimated
		FilterCase_Wall_Thickness = 2. * mm; // Estimated

		Length = FilterCase_Length;
		Radius = FilterCase_Inner_Radius + FilterCase_Wall_Thickness;

		G4Colour white(1., 1., 1.);
		// G4Colour yellow(1., 1., 0.);

		World_Logical = world_Logical;

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

		G4Tubs *FilterCaseWall_Solid =
		    new G4Tubs("FilterCaseWall_Solid", FilterCase_Inner_Radius,
		               FilterCase_Inner_Radius + FilterCase_Wall_Thickness,
		               FilterCase_Length * 0.5, 0. * deg, 360. * deg);

		FilterCaseWall_Logical = new G4LogicalVolume(
		    FilterCaseWall_Solid, PE, "FilterCaseWall_Logical", 0, 0, 0);

		FilterCaseWall_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~FilterCaseWall(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FilterCaseWall_Logical,
		                  "FilterCase", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), FilterCaseWall_Logical,
		                  "FilterCase", World_Logical, false, 0);
	}
};

class FilterCaseBottom {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *FilterCaseBottom_Logical;
	G4RotationMatrix *rot;

  public:
	G4double FilterCaseBottom_Thickness;
	G4double FilterCaseBottom_Radius;
	G4double FilterCaseBottom_Inner_Radius;

	G4double Thickness;
	G4double Radius;

	FilterCaseBottom(G4LogicalVolume *world_Logical) {
		FilterCaseBottom_Thickness = 2. * mm;     // Estimated
		FilterCaseBottom_Radius = 45. * mm;       // Estimated
		FilterCaseBottom_Inner_Radius = 40. * mm; // Estimated

		Thickness = FilterCaseBottom_Thickness;
		Radius = FilterCaseBottom_Radius;

		G4Colour white(1., 1., 1.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

		World_Logical = world_Logical;

		G4Tubs *FilterCaseBottom_Solid =
		    new G4Tubs("FilterCaseBottom_Solid", FilterCaseBottom_Inner_Radius,
		               FilterCaseBottom_Radius,
		               FilterCaseBottom_Thickness * 0.5, 0. * deg, 360. * deg);
		FilterCaseBottom_Logical = new G4LogicalVolume(
		    FilterCaseBottom_Solid, PE, "FilterCaseBottom_Logical", 0, 0, 0);

		FilterCaseBottom_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~FilterCaseBottom(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FilterCaseBottom_Logical,
		                  "FilterCaseBottom", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), FilterCaseBottom_Logical,
		                  "FilterCaseBottom", World_Logical, false, 0);
	}
};

class FilterCaseRing {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *FilterCaseRing_Logical;
	G4RotationMatrix *rot;

  public:
	G4double FilterCaseRing_Thickness;
	G4double FilterCaseRing_Radius;
	G4double FilterCaseRing_Inner_Radius;

	G4double Thickness;
	G4double Radius;

	FilterCaseRing(G4LogicalVolume *world_Logical) {
		FilterCaseRing_Thickness = 2. * mm;     // Estimated
		FilterCaseRing_Radius = 45. * mm;       // Estimated
		FilterCaseRing_Inner_Radius = 40. * mm; // Estimated

		Thickness = FilterCaseRing_Thickness;
		Radius = FilterCaseRing_Radius;

		G4Colour white(1., 1., 1.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

		World_Logical = world_Logical;

		G4Tubs *FilterCaseRing_Solid =
		    new G4Tubs("FilterCaseRing_Solid", FilterCaseRing_Inner_Radius,
		               FilterCaseRing_Radius, FilterCaseRing_Thickness * 0.5,
		               0. * deg, 360. * deg);
		FilterCaseRing_Logical = new G4LogicalVolume(
		    FilterCaseRing_Solid, PE, "FilterCaseRing_Logical", 0, 0, 0);

		FilterCaseRing_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~FilterCaseRing(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FilterCaseRing_Logical,
		                  "FilterCaseRing", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), FilterCaseRing_Logical,
		                  "FilterCaseRing", World_Logical, false, 0);
	}
};

class Pb_45mm_1_4in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_45mm_1_4in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_45mm_1_4in(G4LogicalVolume *world_Logical) {
		Thickness = 0.25 * inch; // Measured
		Radius = 45. * mm;       // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_45mm_1_4in_Solid =
		    new G4Tubs("Pb_45mm_1_4in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_45mm_1_4in_Logical = new G4LogicalVolume(
		    Pb_45mm_1_4in_Solid, Pb, "Pb_45mm_1_4in_Logical", 0, 0, 0);

		Pb_45mm_1_4in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_1_4in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_1_4in_Logical,
		                  "Pb_45mm_1_4in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_1_4in_Logical,
		                  "Pb_45mm_1_4in", World_Logical, false, 0);
	}
};

class Pb_41_5mm_3_64in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_41_5mm_3_64in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_41_5mm_3_64in(G4LogicalVolume *world_Logical) {
		Thickness = 3. / 64. * inch; // Measured
		Radius = 41.5 * mm;          // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_41_5mm_3_64in_Solid =
		    new G4Tubs("Pb_41_5mm_3_64in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_41_5mm_3_64in_Logical = new G4LogicalVolume(
		    Pb_41_5mm_3_64in_Solid, Pb, "Pb_41_5mm_3_64in_Logical", 0, 0, 0);

		Pb_41_5mm_3_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_41_5mm_3_64in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_41_5mm_3_64in_Logical,
		                  "Pb_41_5mm_3_64in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_41_5mm_3_64in_Logical,
		                  "Pb_41_5mm_3_64in", World_Logical, false, 0);
	}
};

class Pb_44mm_3_64in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_44mm_3_64in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_44mm_3_64in(G4LogicalVolume *world_Logical) {
		Thickness = 3. / 64. * inch; // Measured
		Radius = 44. * mm;           // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_44mm_3_64in_Solid =
		    new G4Tubs("Pb_44mm_3_64in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_44mm_3_64in_Logical = new G4LogicalVolume(
		    Pb_44mm_3_64in_Solid, Pb, "Pb_44mm_3_64in_Logical", 0, 0, 0);

		Pb_44mm_3_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_44mm_3_64in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_44mm_3_64in_Logical,
		                  "Pb_44mm_3_64in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_44mm_3_64in_Logical,
		                  "Pb_44mm_3_64in", World_Logical, false, 0);
	}
};

class Pb_45mm_5_64in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_45mm_5_64in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_45mm_5_64in(G4LogicalVolume *world_Logical) {
		Thickness = 5. / 64. * inch; // Measured
		Radius = 45. * mm;           // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_45mm_5_64in_Solid =
		    new G4Tubs("Pb_45mm_5_64in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_45mm_5_64in_Logical = new G4LogicalVolume(
		    Pb_45mm_5_64in_Solid, Pb, "Pb_45mm_5_64in_Logical", 0, 0, 0);

		Pb_45mm_5_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_5_64in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_5_64in_Logical,
		                  "Pb_45mm_5_64in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_5_64in_Logical,
		                  "Pb_45mm_5_64in", World_Logical, false, 0);
	}
};

class Pb_45mm_3_64in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_45mm_3_64in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_45mm_3_64in(G4LogicalVolume *world_Logical) {
		Thickness = 3. / 64. * inch; // Measured
		Radius = 45. * mm;           // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_45mm_3_64in_Solid =
		    new G4Tubs("Pb_45mm_3_64in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_45mm_3_64in_Logical = new G4LogicalVolume(
		    Pb_45mm_3_64in_Solid, Pb, "Pb_45mm_3_64in_Logical", 0, 0, 0);

		Pb_45mm_3_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_3_64in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_3_64in_Logical,
		                  "Pb_45mm_3_64in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_3_64in_Logical,
		                  "Pb_45mm_3_64in", World_Logical, false, 0);
	}
};

class Pb_50mm_5_64in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_50mm_5_64in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_50mm_5_64in(G4LogicalVolume *world_Logical) {
		Thickness = 5. / 64. * inch; // Measured
		Radius = 50. * mm;           // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_50mm_5_64in_Solid =
		    new G4Tubs("Pb_50mm_5_64in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_50mm_5_64in_Logical = new G4LogicalVolume(
		    Pb_50mm_5_64in_Solid, Pb, "Pb_50mm_5_64in_Logical", 0, 0, 0);

		Pb_50mm_5_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_50mm_5_64in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_50mm_5_64in_Logical,
		                  "Pb_50mm_5_64in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_50mm_5_64in_Logical,
		                  "Pb_50mm_5_64in", World_Logical, false, 0);
	}
};

class Pb_70mm_5mm {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Pb_70mm_5mm_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Pb_70mm_5mm(G4LogicalVolume *world_Logical) {
		Thickness = 5. * mm; // Measured
		Radius = 70. * mm;   // Measured

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs *Pb_70mm_5mm_Solid =
		    new G4Tubs("Pb_70mm_5mm_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Pb_70mm_5mm_Logical = new G4LogicalVolume(
		    Pb_70mm_5mm_Solid, Pb, "Pb_70mm_5mm_Logical", 0, 0, 0);

		Pb_70mm_5mm_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_70mm_5mm(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_70mm_5mm_Logical,
		                  "Pb_70mm_5mm", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_70mm_5mm_Logical,
		                  "Pb_70mm_5mm", World_Logical, false, 0);
	}
};

class Cu_45mm_1_8in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Cu_45mm_1_8in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Cu_45mm_1_8in(G4LogicalVolume *world_Logical) {
		Thickness = 0.125 * inch; // Measured
		Radius = 45. * mm;        // Measured
		G4Colour orange(1., .5, 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs *Cu_45mm_1_8in_Solid =
		    new G4Tubs("Cu_45mm_1_8in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Cu_45mm_1_8in_Logical = new G4LogicalVolume(
		    Cu_45mm_1_8in_Solid, Cu, "Cu_45mm_1_8in_Logical", 0, 0, 0);

		Cu_45mm_1_8in_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_45mm_1_8in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_45mm_1_8in_Logical,
		                  "Cu_45mm_1_8in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_45mm_1_8in_Logical,
		                  "Cu_45mm_1_8in", World_Logical, false, 0);
	}
};

class Cu_45mm_1mm {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Cu_45mm_1mm_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Cu_45mm_1mm(G4LogicalVolume *world_Logical) {
		Thickness = 1. * mm; // Measured
		Radius = 45. * mm;   // Measured

		G4Colour orange(1., .5, 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs *Cu_45mm_1mm_Solid =
		    new G4Tubs("Cu_45mm_1mm_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Cu_45mm_1mm_Logical = new G4LogicalVolume(
		    Cu_45mm_1mm_Solid, Cu, "Cu_45mm_1mm_Logical", 0, 0, 0);

		Cu_45mm_1mm_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_45mm_1mm(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_45mm_1mm_Logical,
		                  "Cu_45mm_1mm", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_45mm_1mm_Logical,
		                  "Cu_45mm_1mm", World_Logical, false, 0);
	}
};

class Cu_50mm_1mm {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Cu_50mm_1mm_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Cu_50mm_1mm(G4LogicalVolume *world_Logical) {
		Thickness = 1. * mm; // Measured
		Radius = 50. * mm;   // Measured

		G4Colour orange(1., .5, 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs *Cu_50mm_1mm_Solid =
		    new G4Tubs("Cu_50mm_1mm_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Cu_50mm_1mm_Logical = new G4LogicalVolume(
		    Cu_50mm_1mm_Solid, Cu, "Cu_50mm_1mm_Logical", 0, 0, 0);

		Cu_50mm_1mm_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_50mm_1mm(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_50mm_1mm_Logical,
		                  "Cu_50mm_1mm", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_50mm_1mm_Logical,
		                  "Cu_50mm_1mm", World_Logical, false, 0);
	}
};

class Cu_73mm_1_20in {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *Cu_73mm_1_20in_Logical;
	G4RotationMatrix *rot;

  public:
	G4double Thickness;
	G4double Radius;

	Cu_73mm_1_20in(G4LogicalVolume *world_Logical) {
		Thickness = 0.05 * inch; // Measured
		Radius = 73.5 * mm;      // Measured

		G4Colour orange(1., .5, 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs *Cu_73mm_1_20in_Solid =
		    new G4Tubs("Cu_73mm_1_20in_Solid", 0., Radius, Thickness * 0.5,
		               0. * deg, 360. * deg);
		Cu_73mm_1_20in_Logical = new G4LogicalVolume(
		    Cu_73mm_1_20in_Solid, Cu, "Cu_73mm_1_20in_Logical", 0, 0, 0);

		Cu_73mm_1_20in_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_73mm_1_20in(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_73mm_1_20in_Logical,
		                  "Cu_73mm_1_20in", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_73mm_1_20in_Logical,
		                  "Cu_73mm_1_20in", World_Logical, false, 0);
	}
};
