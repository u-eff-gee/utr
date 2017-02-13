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

class NormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *NormBrick_Logical;
	G4Box *NormBrick_Solid;
	G4RotationMatrix *rot;

	G4double NormBrick_medium;
	G4double NormBrick_short;
	G4double NormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	NormBrick(G4LogicalVolume *world_Logical) {
		L = 200. * mm;
		M = 100. * mm;
		S = 50. * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		NormBrick_Solid = new G4Box("NormBrick_Solid", M / 2., S / 2., L / 2.);
		NormBrick_Logical = new G4LogicalVolume(NormBrick_Solid, Pb,
		                                        "NormBrick_Logical", 0, 0, 0);

		NormBrick_Logical->SetVisAttributes(new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~NormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), NormBrick_Logical,
		                  "NormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), NormBrick_Logical,
		                  "NormBrick", World_Logical, false, 0);
	}
};

class NormBrickWithHole {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *NormBrickWithHole_Logical;
	G4SubtractionSolid *NormBrickWithHole_Solid;
	G4RotationMatrix *rot;

  public:
	G4double L;
	G4double M;
	G4double S;
	G4double Hole_Radius;

	NormBrickWithHole(G4LogicalVolume *world_Logical) {
		L = 200. * mm;
		M = 100. * mm;
		S = 50. * mm;
		Hole_Radius = 1. * inch;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Box *NormBrick_Solid =
		    new G4Box("NormBrick_Solid", M / 2., S / 2., L / 2.);
		G4Tubs *Hole_Solid =
		    new G4Tubs("Hole_Solid", 0., Hole_Radius, S, 0. * deg, 360. * deg);
		rot = new G4RotationMatrix();
		rot->rotateX(90. * deg);

		NormBrickWithHole_Solid =
		    new G4SubtractionSolid("NormBrickWithHole_Solid", NormBrick_Solid,
		                           Hole_Solid, rot, G4ThreeVector());

		NormBrickWithHole_Logical = new G4LogicalVolume(
		    NormBrickWithHole_Solid, Pb, "NormBrickWithHole_Logical", 0, 0, 0);

		NormBrickWithHole_Logical->SetVisAttributes(new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~NormBrickWithHole(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), NormBrickWithHole_Logical,
		                  "NormBrickWithHole", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  NormBrickWithHole_Logical, "NormBrickWithHole",
		                  World_Logical, false, 0);
	}
};

class ShortNormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ShortNormBrick_Logical;
	G4Box *ShortNormBrick_Solid;
	G4RotationMatrix *rot;

	G4double ShortNormBrick_medium;
	G4double ShortNormBrick_short;
	G4double ShortNormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	ShortNormBrick(G4LogicalVolume *world_Logical) {
		L = 100. * mm;
		M = 100. * mm;
		S = 50. * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		ShortNormBrick_Solid =
		    new G4Box("ShortNormBrick_Solid", M / 2., S / 2., L / 2.);
		ShortNormBrick_Logical = new G4LogicalVolume(
		    ShortNormBrick_Solid, Pb, "ShortNormBrick_Logical", 0, 0, 0);

		ShortNormBrick_Logical->SetVisAttributes(new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~ShortNormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
		                  "ShortNormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), ShortNormBrick_Logical,
		                  "ShortNormBrick", World_Logical, false, 0);
	}
};

class ShortBrickWithHole {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ShortBrickWithHole_Logical;
	G4SubtractionSolid *ShortBrickWithHole_Solid;
	G4RotationMatrix *rot;

	G4double ShortBrickWithHole_medium;
	G4double ShortBrickWithHole_short;
	G4double ShortBrickWithHole_long;

  public:
	G4double L;
	G4double M;
	G4double S;
	G4double Hole_Radius;

	ShortBrickWithHole(G4LogicalVolume *world_Logical) {
		L = 100. * mm;
		M = 100. * mm;
		S = 50. * mm;
		Hole_Radius = 22.5 * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Box *ShortBrick_Solid =
		    new G4Box("ShortBrickWithHole_Solid", M / 2., S / 2., L / 2.);
		G4Tubs *Hole_Solid =
		    new G4Tubs("Hole_Solid", 0., Hole_Radius, S, 0. * deg, 360. * deg);

		rot = new G4RotationMatrix();
		rot->rotateX(90. * deg);

		ShortBrickWithHole_Solid =
		    new G4SubtractionSolid("ShortBrickWithHole_Solid", ShortBrick_Solid,
		                           Hole_Solid, rot, G4ThreeVector());

		ShortBrickWithHole_Logical =
		    new G4LogicalVolume(ShortBrickWithHole_Solid, Pb,
		                        "ShortBrickWithHole_Logical", 0, 0, 0);

		ShortBrickWithHole_Logical->SetVisAttributes(
		    new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~ShortBrickWithHole(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), ShortBrickWithHole_Logical,
		                  "ShortBrickWithHole", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  ShortBrickWithHole_Logical, "ShortBrickWithHole",
		                  World_Logical, false, 0);
	}
};

class HalfShortBrickWithHole {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *HalfShortBrickWithHole_Logical;
	G4SubtractionSolid *HalfShortBrickWithHole_Solid;
	G4RotationMatrix *rot;

	G4double HalfShortBrickWithHole_medium;
	G4double HalfShortBrickWithHole_short;
	G4double HalfShortBrickWithHole_long;

  public:
	G4double L;
	G4double M;
	G4double S;
	G4double Hole_Radius;

	HalfShortBrickWithHole(G4LogicalVolume *world_Logical) {
		L = 100. * mm;
		M = 50. * mm;
		S = 50. * mm;
		Hole_Radius = 27.5 * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Box *ShortBrick_Solid =
		    new G4Box("HalfShortBrickWithHole_Solid", M / 2., S / 2., L / 2.);
		G4Tubs *Hole_Solid =
		    new G4Tubs("Hole_Solid", 0., Hole_Radius, S, 0. * deg, 360. * deg);

		rot = new G4RotationMatrix();
		rot->rotateX(90. * deg);

		HalfShortBrickWithHole_Solid = new G4SubtractionSolid(
		    "HalfShortBrickWithHole_Solid", ShortBrick_Solid, Hole_Solid, rot,
		    G4ThreeVector(S / 2., 0., 0.));

		HalfShortBrickWithHole_Logical =
		    new G4LogicalVolume(HalfShortBrickWithHole_Solid, Pb,
		                        "HalfShortBrickWithHole_Logical", 0, 0, 0);

		HalfShortBrickWithHole_Logical->SetVisAttributes(
		    new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~HalfShortBrickWithHole(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z),
		                  HalfShortBrickWithHole_Logical,
		                  "HalfShortBrickWithHole", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  HalfShortBrickWithHole_Logical,
		                  "HalfShortBrickWithHole", World_Logical, false, 0);
	}
};

class ConcreteBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ConcreteBrick_Logical;
	G4Box *ConcreteBrick_Solid;
	G4RotationMatrix *rot;

	G4double ConcreteBrick_medium;
	G4double ConcreteBrick_short;
	G4double ConcreteBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	ConcreteBrick(G4LogicalVolume *world_Logical) {
		L = 390. * mm;
		M = 190. * mm;
		S = 190. * mm;

		G4Colour white(1., 1., 1.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

		World_Logical = world_Logical;

		ConcreteBrick_Solid =
		    new G4Box("ConcreteBrick_Solid", M / 2., S / 2., L / 2.);
		ConcreteBrick_Logical = new G4LogicalVolume(
		    ConcreteBrick_Solid, Concrete, "ConcreteBrick_Logical", 0, 0, 0);

		ConcreteBrick_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~ConcreteBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), ConcreteBrick_Logical,
		                  "ConcreteBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), ConcreteBrick_Logical,
		                  "ConcreteBrick", World_Logical, false, 0);
	}
};

class BridgeBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *BridgeBrick_Logical;
	G4SubtractionSolid *BridgeBrick_Solid;
	G4RotationMatrix *rot;

	G4double BridgeBrick_medium;
	G4double BridgeBrick_short;
	G4double BridgeBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;
	G4double Hole_Radius;

	BridgeBrick(G4LogicalVolume *world_Logical) {
		L = 190. * mm;
		M = 95. * mm;
		S = 60. * mm;
		Hole_Radius = 50. * mm;

		G4Colour grey(0.5, 0.5, 0.5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Box *ShortBrick_Solid =
		    new G4Box("BridgeBrick_Solid", M / 2., S / 2., L / 2.);
		G4Tubs *Hole_Solid =
		    new G4Tubs("Hole_Solid", 0., Hole_Radius, S, 0. * deg, 360. * deg);

		rot = new G4RotationMatrix();
		rot->rotateX(90. * deg);

		BridgeBrick_Solid = new G4SubtractionSolid(
		    "BridgeBrick_Solid", ShortBrick_Solid, Hole_Solid, rot,
		    G4ThreeVector(S / 2., 0., 0.));

		BridgeBrick_Logical = new G4LogicalVolume(
		    BridgeBrick_Solid, Pb, "BridgeBrick_Logical", 0, 0, 0);

		BridgeBrick_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~BridgeBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), BridgeBrick_Logical,
		                  "BridgeBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), BridgeBrick_Logical,
		                  "BridgeBrick", World_Logical, false, 0);
	}
};

class ThinNormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ThinNormBrick_Logical;
	G4Box *ThinNormBrick_Solid;
	G4RotationMatrix *rot;

	G4double ThinNormBrick_medium;
	G4double ThinNormBrick_short;
	G4double ThinNormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	ThinNormBrick(G4LogicalVolume *world_Logical) {
		L = 200. * mm;
		M = 50. * mm;
		S = 50. * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		ThinNormBrick_Solid =
		    new G4Box("ThinNormBrick_Solid", M / 2., S / 2., L / 2.);
		ThinNormBrick_Logical = new G4LogicalVolume(
		    ThinNormBrick_Solid, Pb, "ThinNormBrick_Logical", 0, 0, 0);

		ThinNormBrick_Logical->SetVisAttributes(new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~ThinNormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), ThinNormBrick_Logical,
		                  "ThinNormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), ThinNormBrick_Logical,
		                  "ThinNormBrick", World_Logical, false, 0);
	}
};

class ShortThinNormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ShortThinNormBrick_Logical;
	G4Box *ShortThinNormBrick_Solid;
	G4RotationMatrix *rot;

	G4double ShortThinNormBrick_medium;
	G4double ShortThinNormBrick_short;
	G4double ShortThinNormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	ShortThinNormBrick(G4LogicalVolume *world_Logical) {
		L = 100. * mm;
		M = 50. * mm;
		S = 50. * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		ShortThinNormBrick_Solid =
		    new G4Box("ShortThinNormBrick_Solid", M / 2., S / 2., L / 2.);
		ShortThinNormBrick_Logical =
		    new G4LogicalVolume(ShortThinNormBrick_Solid, Pb,
		                        "ShortThinNormBrick_Logical", 0, 0, 0);

		ShortThinNormBrick_Logical->SetVisAttributes(
		    new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~ShortThinNormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), ShortThinNormBrick_Logical,
		                  "ShortThinNormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  ShortThinNormBrick_Logical, "ShortThinNormBrick",
		                  World_Logical, false, 0);
	}
};

class FlatFlatThinNormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *FlatFlatThinNormBrick_Logical;
	G4Box *FlatFlatThinNormBrick_Solid;
	G4RotationMatrix *rot;

	G4double FlatFlatThinNormBrick_medium;
	G4double FlatFlatThinNormBrick_short;
	G4double FlatFlatThinNormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	FlatFlatThinNormBrick(G4LogicalVolume *world_Logical) {
		L = 200. * mm;
		M = 50. * mm;
		S = 12.5 * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		FlatFlatThinNormBrick_Solid =
		    new G4Box("FlatFlatThinNormBrick_Solid", M / 2., S / 2., L / 2.);
		FlatFlatThinNormBrick_Logical =
		    new G4LogicalVolume(FlatFlatThinNormBrick_Solid, Pb,
		                        "FlatFlatThinNormBrick_Logical", 0, 0, 0);

		FlatFlatThinNormBrick_Logical->SetVisAttributes(
		    new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~FlatFlatThinNormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z),
		                  FlatFlatThinNormBrick_Logical,
		                  "FlatFlatThinNormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  FlatFlatThinNormBrick_Logical,
		                  "FlatFlatThinNormBrick", World_Logical, false, 0);
	}
};

class ThreeQuarterShortNormBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *ThreeQuarterShortNormBrick_Logical;
	G4Box *ThreeQuarterShortNormBrick_Solid;
	G4RotationMatrix *rot;

	G4double ThreeQuarterShortNormBrick_medium;
	G4double ThreeQuarterShortNormBrick_short;
	G4double ThreeQuarterShortNormBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	ThreeQuarterShortNormBrick(G4LogicalVolume *world_Logical) {
		L = 150. * mm;
		M = 100. * mm;
		S = 50. * mm;

		G4Colour green(0., 1., 0.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		ThreeQuarterShortNormBrick_Solid = new G4Box(
		    "ThreeQuarterShortNormBrick_Solid", M / 2., S / 2., L / 2.);
		ThreeQuarterShortNormBrick_Logical =
		    new G4LogicalVolume(ThreeQuarterShortNormBrick_Solid, Pb,
		                        "ThreeQuarterShortNormBrick_Logical", 0, 0, 0);

		ThreeQuarterShortNormBrick_Logical->SetVisAttributes(
		    new G4VisAttributes(green));

		rot = new G4RotationMatrix();
	}

	~ThreeQuarterShortNormBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(
		    0, G4ThreeVector(x, y, z), ThreeQuarterShortNormBrick_Logical,
		    "ThreeQuarterShortNormBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(
		    rot, G4ThreeVector(x, y, z), ThreeQuarterShortNormBrick_Logical,
		    "ThreeQuarterShortNormBrick", World_Logical, false, 0);
	}
};

class FlatConcreteBrick {
  private:
	G4LogicalVolume *World_Logical;

	G4LogicalVolume *FlatConcreteBrick_Logical;
	G4Box *FlatConcreteBrick_Solid;
	G4RotationMatrix *rot;

	G4double FlatConcreteBrick_medium;
	G4double FlatConcreteBrick_short;
	G4double FlatConcreteBrick_long;

  public:
	G4double L;
	G4double M;
	G4double S;

	FlatConcreteBrick(G4LogicalVolume *world_Logical) {
		L = 390. * mm;
		M = 190. * mm;
		S = 95. * mm;

		G4Colour white(1., 1., 1.);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *FlatConcrete = nist->FindOrBuildMaterial("G4_CONCRETE");

		World_Logical = world_Logical;

		FlatConcreteBrick_Solid =
		    new G4Box("FlatConcreteBrick_Solid", M / 2., S / 2., L / 2.);
		FlatConcreteBrick_Logical =
		    new G4LogicalVolume(FlatConcreteBrick_Solid, FlatConcrete,
		                        "FlatConcreteBrick_Logical", 0, 0, 0);

		FlatConcreteBrick_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~FlatConcreteBrick(){};

	void Put(G4double x, G4double y, G4double z) {
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FlatConcreteBrick_Logical,
		                  "FlatConcreteBrick", World_Logical, false, 0);
	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x,
	         G4double angle_y, G4double angle_z) {

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z),
		                  FlatConcreteBrick_Logical, "FlatConcreteBrick",
		                  World_Logical, false, 0);
	}
};
