#include "BGO.hh"

#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4GenericPolycone.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

#include "NamedColors.hh"

BGO::BGO(G4LogicalVolume *World_Log, G4String name)
{
	World_Logical = World_Log;
	bgo_name = name;

	G4double caseOut_z[] = {0., 0., 86.*mm, 172.*mm, 172.*mm, 192.*mm, 192.*mm, 193.*mm, 202.*mm, 202.*mm, 44.*mm, 44.*mm};
	G4double caseOut_r[] = {25.*mm, 62.*mm, 95.*mm, 95.*mm, 115.*mm, 115.*mm, 92.*mm, 92.*mm, 58.5*mm, 50.5*mm, 50.5*mm, 37.5*mm};
	G4double caseFilled_z[] = {0., 0., 86.*mm, 202.*mm, 202.*mm};
	G4double caseFilled_r[] = {0., 62.*mm, 95.*mm, 95.*mm, 0.};
	G4double caseIn_z[] = {8.*mm, 8.*mm, 86.*mm, 179.*mm, 190.*mm, 43.*mm, 43.*mm};
	G4double caseIn_r[] = {27.37*mm, 61.37*mm, 92.*mm, 92.*mm,51.5*mm, 51.5*mm, 37.74*mm};
	G4double bgo_z[] = {10.*mm, 10.*mm, 70.*mm, 170.*mm, 184.*mm, 40.*mm, 40.*mm};
	G4double bgo_r[] = {28.*mm, 63.*mm, 86.*mm, 86.*mm, 52.*mm3, 52.*mm3, 37.*mm};

	length = caseOut_z[9];
	radius = caseOut_z[5];
	max_penetration_depth = length - caseOut_z[11];

	G4NistManager *man = G4NistManager::Instance();
	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *bgo = man->FindOrBuildMaterial("G4_BGO");

	caseFilled_Solid = new G4Polycone(bgo_name + "_caseFilled_Solid", 0., 360.* deg, sizeof(caseFilled_z)/sizeof(caseFilled_z[0]), caseFilled_r, caseFilled_z);
	auto* caseOut_Solid = new G4GenericPolycone(bgo_name + "_caseOut_Solid", 0., 360. * deg, sizeof(caseOut_z)/sizeof(caseOut_z[0]), caseOut_r, caseOut_z);
	auto* caseIn_Solid = new G4GenericPolycone(bgo_name + "_caseIn_Solid", 0., 360. * deg, sizeof(caseIn_z)/sizeof(caseIn_z[0]), caseIn_r, caseIn_z);
	auto* bgo_case_Solid = new G4SubtractionSolid(bgo_name + "_case_Solid", caseOut_Solid, caseIn_Solid, 0, G4ThreeVector(0, 0, 0));
	bgo_case_Logical = new G4LogicalVolume(bgo_case_Solid, Al, bgo_name + "_case_Logical");
	bgo_case_Logical->SetVisAttributes(magenta);
	auto* bgo_Solid = new G4GenericPolycone(bgo_name + "_Solid", 0., 360. * deg, sizeof(bgo_z)/sizeof(bgo_z[0]), bgo_r, bgo_z);
	bgo_Logical = new G4LogicalVolume(bgo_Solid, bgo, bgo_name + "_Logical");
	bgo_Logical->SetVisAttributes(blue);
}

G4ThreeVector BGO::GetPos(G4double theta, G4double phi, G4double dist_from_center) const
{
	auto dist = dist_from_center + length * 0.5;
	return G4ThreeVector(
		dist * sin(theta) * cos(phi),
		dist * cos(theta),
		dist * sin(theta) * sin(phi));
}

void BGO::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi, G4double dist_from_center)
{
	auto pos = global_coordinates + GetPos(theta, phi, dist_from_center);
	auto* rot = new G4RotationMatrix();
	rot->rotateY(90. * deg + phi);

	new G4PVPlacement(rot, pos, bgo_case_Logical, bgo_name + "_case", World_Logical, false, 0);
	new G4PVPlacement(rot, pos, bgo_Logical, bgo_name , World_Logical, false, 0);
}