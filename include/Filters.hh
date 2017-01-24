#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "DetectorConstruction.hh"

class FilterCase{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* FilterCase_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double FilterCase_Length = 80.*mm; // Estimated
	const G4double FilterCase_Inner_Radius = 45.*mm; // Estimated
	const G4double FilterCase_Wall_Thickness = 2.*mm; // Estimated
	const G4double FilterCaseBottomHole_Radius = 40.*mm; // Estimated
	const G4double FilterCaseBottom_Thickness = 2.*mm; // Estimated

	G4double Length = FilterCase_Length;
	G4double Radius = FilterCase_Inner_Radius + FilterCase_Wall_Thickness;

	FilterCase(G4LogicalVolume* world_Logical){
		G4Colour white (1., 1., 1.);
		//G4Colour yellow(1., 1., 0.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

		World_Logical = world_Logical;

		G4Tubs* FilterCase_Solid = new G4Tubs("FilterCase_Solid", FilterCase_Inner_Radius, FilterCase_Inner_Radius + FilterCase_Wall_Thickness, FilterCase_Length*0.5, 0.*deg, 360.*deg);
		FilterCase_Logical = new G4LogicalVolume(FilterCase_Solid, PE, "FilterCase_Logical", 0, 0, 0);

		FilterCase_Logical->SetVisAttributes(new G4VisAttributes(white));

		G4Tubs* FilterCaseBottom_Solid = new G4Tubs("FilterCaseBottom_Solid", FilterCaseBottomHole_Radius, FilterCase_Inner_Radius, FilterCaseBottom_Thickness*0.5, 0.*deg, 360.*deg);
		G4LogicalVolume* FilterCaseBottom_Logical = new G4LogicalVolume(FilterCaseBottom_Solid, PE, "FilterCaseBottom_Logical", 0, 0, 0);

		FilterCaseBottom_Logical->SetVisAttributes(new G4VisAttributes(white));

		new G4PVPlacement(0, G4ThreeVector(0., 0., FilterCase_Length*0.5 - FilterCase_Wall_Thickness*0.5), FilterCaseBottom_Logical, "FilterCaseBottom", FilterCase_Logical, false, 0);

		rot = new G4RotationMatrix();
	}

	~FilterCase(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FilterCase_Logical, "FilterCase", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), FilterCase_Logical, "FilterCase", World_Logical, false, 0);	

	}

};

class FilterCaseRing{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* FilterCaseRing_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double FilterCaseRing_Thickness = 2.*mm; // Estimated
	const G4double FilterCaseRing_Radius = 45.*mm; // Estimated
	const G4double FilterCaseRing_Inner_Radius = 40.*mm; // Estimated

	G4double Thickness = FilterCaseRing_Thickness;
	G4double Radius = FilterCaseRing_Radius;

	FilterCaseRing(G4LogicalVolume* world_Logical){
		G4Colour white (1., 1., 1.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

		World_Logical = world_Logical;

		G4Tubs* FilterCaseRing_Solid = new G4Tubs("FilterCaseRing_Solid", FilterCaseRing_Inner_Radius, FilterCaseRing_Radius, FilterCaseRing_Thickness*0.5, 0.*deg, 360.*deg);
		FilterCaseRing_Logical = new G4LogicalVolume(FilterCaseRing_Solid, PE, "FilterCaseRing_Logical", 0, 0, 0);

		FilterCaseRing_Logical->SetVisAttributes(new G4VisAttributes(white));

		rot = new G4RotationMatrix();
	}

	~FilterCaseRing(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), FilterCaseRing_Logical, "FilterCaseRing", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), FilterCaseRing_Logical, "FilterCaseRing", World_Logical, false, 0);	

	}

};

class Pb_45mm_1_4in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Pb_45mm_1_4in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 0.25*inch; // Measured
	const G4double Radius = 45.*mm; // Measured

	Pb_45mm_1_4in(G4LogicalVolume* world_Logical){
		G4Colour grey (.5, .5, .5);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs* Pb_45mm_1_4in_Solid = new G4Tubs("Pb_45mm_1_4in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Pb_45mm_1_4in_Logical = new G4LogicalVolume(Pb_45mm_1_4in_Solid, Pb, "Pb_45mm_1_4in_Logical", 0, 0, 0);

		Pb_45mm_1_4in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_1_4in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_1_4in_Logical, "Pb_45mm_1_4in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_1_4in_Logical, "Pb_45mm_1_4in", World_Logical, false, 0);	

	}

};

class Pb_45mm_5_64in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Pb_45mm_5_64in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 5./64.*inch; // Measured
	const G4double Radius = 45.*mm; // Measured

	Pb_45mm_5_64in(G4LogicalVolume* world_Logical){
		G4Colour grey (.5, .5, .5);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs* Pb_45mm_5_64in_Solid = new G4Tubs("Pb_45mm_5_64in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Pb_45mm_5_64in_Logical = new G4LogicalVolume(Pb_45mm_5_64in_Solid, Pb, "Pb_45mm_5_64in_Logical", 0, 0, 0);

		Pb_45mm_5_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_5_64in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_5_64in_Logical, "Pb_45mm_5_64in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_5_64in_Logical, "Pb_45mm_5_64in", World_Logical, false, 0);	

	}

};


class Pb_45mm_3_64in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Pb_45mm_3_64in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 3./64.*inch; // Measured
	const G4double Radius = 45.*mm; // Measured

	Pb_45mm_3_64in(G4LogicalVolume* world_Logical){
		G4Colour grey (.5, .5, .5);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs* Pb_45mm_3_64in_Solid = new G4Tubs("Pb_45mm_3_64in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Pb_45mm_3_64in_Logical = new G4LogicalVolume(Pb_45mm_3_64in_Solid, Pb, "Pb_45mm_3_64in_Logical", 0, 0, 0);

		Pb_45mm_3_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_45mm_3_64in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_45mm_3_64in_Logical, "Pb_45mm_3_64in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_45mm_3_64in_Logical, "Pb_45mm_3_64in", World_Logical, false, 0);	

	}

};


class Pb_50mm_5_64in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Pb_50mm_5_64in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 5./64.*inch; // Measured
	const G4double Radius = 50.*mm; // Measured

	Pb_50mm_5_64in(G4LogicalVolume* world_Logical){
		G4Colour grey (.5, .5, .5);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs* Pb_50mm_5_64in_Solid = new G4Tubs("Pb_50mm_5_64in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Pb_50mm_5_64in_Logical = new G4LogicalVolume(Pb_50mm_5_64in_Solid, Pb, "Pb_50mm_5_64in_Logical", 0, 0, 0);

		Pb_50mm_5_64in_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_50mm_5_64in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_50mm_5_64in_Logical, "Pb_50mm_5_64in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_50mm_5_64in_Logical, "Pb_50mm_5_64in", World_Logical, false, 0);	

	}

};


class Pb_70mm_5mm{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Pb_70mm_5mm_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 5.*mm; // Measured
	const G4double Radius = 70.*mm; // Measured

	Pb_70mm_5mm(G4LogicalVolume* world_Logical){
		G4Colour grey (.5, .5, .5);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

		World_Logical = world_Logical;

		G4Tubs* Pb_70mm_5mm_Solid = new G4Tubs("Pb_70mm_5mm_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Pb_70mm_5mm_Logical = new G4LogicalVolume(Pb_70mm_5mm_Solid, Pb, "Pb_70mm_5mm_Logical", 0, 0, 0);

		Pb_70mm_5mm_Logical->SetVisAttributes(new G4VisAttributes(grey));

		rot = new G4RotationMatrix();
	}

	~Pb_70mm_5mm(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Pb_70mm_5mm_Logical, "Pb_70mm_5mm", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Pb_70mm_5mm_Logical, "Pb_70mm_5mm", World_Logical, false, 0);	

	}

};


class Cu_45mm_1_8in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Cu_45mm_1_8in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 0.125*inch; // Measured
	const G4double Radius = 45.*mm; // Measured

	Cu_45mm_1_8in(G4LogicalVolume* world_Logical){
		G4Colour orange (1., .5, 0.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs* Cu_45mm_1_8in_Solid = new G4Tubs("Cu_45mm_1_8in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Cu_45mm_1_8in_Logical = new G4LogicalVolume(Cu_45mm_1_8in_Solid, Cu, "Cu_45mm_1_8in_Logical", 0, 0, 0);

		Cu_45mm_1_8in_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_45mm_1_8in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_45mm_1_8in_Logical, "Cu_45mm_1_8in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_45mm_1_8in_Logical, "Cu_45mm_1_8in", World_Logical, false, 0);	

	}

};


class Cu_45mm_1mm{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Cu_45mm_1mm_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 1.*mm; // Measured
	const G4double Radius = 45.*mm; // Measured

	Cu_45mm_1mm(G4LogicalVolume* world_Logical){
		G4Colour orange (1., .5, 0.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs* Cu_45mm_1mm_Solid = new G4Tubs("Cu_45mm_1mm_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Cu_45mm_1mm_Logical = new G4LogicalVolume(Cu_45mm_1mm_Solid, Cu, "Cu_45mm_1mm_Logical", 0, 0, 0);

		Cu_45mm_1mm_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_45mm_1mm(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_45mm_1mm_Logical, "Cu_45mm_1mm", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_45mm_1mm_Logical, "Cu_45mm_1mm", World_Logical, false, 0);	

	}

};


class Cu_50mm_1mm{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Cu_50mm_1mm_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 1.*mm; // Measured
	const G4double Radius = 50.*mm; // Measured

	Cu_50mm_1mm(G4LogicalVolume* world_Logical){
		G4Colour orange (1., .5, 0.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs* Cu_50mm_1mm_Solid = new G4Tubs("Cu_50mm_1mm_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Cu_50mm_1mm_Logical = new G4LogicalVolume(Cu_50mm_1mm_Solid, Cu, "Cu_50mm_1mm_Logical", 0, 0, 0);

		Cu_50mm_1mm_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_50mm_1mm(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_50mm_1mm_Logical, "Cu_50mm_1mm", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_50mm_1mm_Logical, "Cu_50mm_1mm", World_Logical, false, 0);	

	}

};


class Cu_73mm_1_20in{
private:
	G4LogicalVolume* World_Logical;

	G4LogicalVolume* Cu_73mm_1_20in_Logical;
	G4RotationMatrix* rot;
	
public:
	const G4double Thickness = 0.05*inch; // Measured
	const G4double Radius = 73.5*mm; // Measured

	Cu_73mm_1_20in(G4LogicalVolume* world_Logical){
		G4Colour orange (1., .5, 0.);

		G4NistManager* nist = G4NistManager::Instance();
		G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");

		World_Logical = world_Logical;

		G4Tubs* Cu_73mm_1_20in_Solid = new G4Tubs("Cu_73mm_1_20in_Solid", 0., Radius, Thickness*0.5, 0.*deg, 360.*deg);
		Cu_73mm_1_20in_Logical = new G4LogicalVolume(Cu_73mm_1_20in_Solid, Cu, "Cu_73mm_1_20in_Logical", 0, 0, 0);

		Cu_73mm_1_20in_Logical->SetVisAttributes(new G4VisAttributes(orange));

		rot = new G4RotationMatrix();
	}

	~Cu_73mm_1_20in(){};

	void Put(G4double x, G4double y, G4double z){
		new G4PVPlacement(0, G4ThreeVector(x, y, z), Cu_73mm_1_20in_Logical, "Cu_73mm_1_20in", World_Logical, false, 0);	

	}

	void Put(G4double x, G4double y, G4double z, G4double angle_x, G4double angle_y, G4double angle_z){

		rot = new G4RotationMatrix();
		rot->rotateX(angle_x);
		rot->rotateY(angle_y);
		rot->rotateZ(angle_z);

		new G4PVPlacement(rot, G4ThreeVector(x, y, z), Cu_73mm_1_20in_Logical, "Cu_73mm_1_20in", World_Logical, false, 0);	

	}

};
