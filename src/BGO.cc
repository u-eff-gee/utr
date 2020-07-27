#include "BGO.hh"

#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4GenericPolycone.hh"
#include "G4Polyhedra.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4IntersectionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

#include "NamedColors.hh"

BGO::BGO(G4LogicalVolume *World_Log, G4String name)
{
	World_Logical = World_Log;
	bgo_name = name;

	G4double caseOut_z[] = {0., 0., 86.*mm, 172.*mm, 172.*mm, 192.*mm, 192.*mm, 193.*mm, 202.*mm, 202.*mm, 42.*mm, 42.*mm};
	G4double caseOut_r[] = {25.*mm, 62.*mm, 95.*mm, 95.*mm, 115.*mm, 115.*mm, 92.*mm, 92.*mm, 58.5*mm, 50.5*mm, 50.5*mm, 37.44*mm};
	G4double caseFilled_z[] = {0., 0., 86.*mm, 202.*mm, 202.*mm};
	G4double caseFilled_r[] = {0., 62.*mm, 95.*mm, 95.*mm, 0.};
	G4double caseIn_z[] = {8.*mm, 8.*mm, 86.*mm, 179.*mm, 190.*mm, 41.2*mm, 41.2*mm};
	G4double caseIn_r[] = {27.5*mm, 62.45*mm, 92.*mm, 92.*mm,51.5*mm, 51.3*mm, 37.28*mm};
	G4double bgo_z[] = {10.*mm, 10.*mm, 70.*mm, 170.*mm, 185.*mm, 40.*mm, 40.*mm};
	G4double bgo_r[] = {29.2*mm, 60.4*mm, 82.3*mm, 82.3*mm, 52.3*mm, 52.3*mm, 37.*mm};

	// Convert BGO radius from midpoint to corner
	for (size_t i = 0; i < sizeof(bgo_r)/sizeof(bgo_r[0]); ++i) {
		bgo_r[i] = bgo_r[i] / cos(22.5 * deg);
	}
	
	// FIXME: The BGO is further abraded on its edges, but the schematics
	// don’t indicate how much abrasion there is. The current implementiation
	// should be sufficient for simulating the HPGe detector (the center of
	// the tip of the BGO is correct), but the efficiency of the BGO cannot
	// be determined correctly. The following is a best effort that is better
	// than doing nothing but probably does not reflect the reality.
	G4double bgo_abrased_z[] = {10.*mm, 10.*mm, 185.*mm, 185.*mm};
	G4double bgo_abrased_r[] = {0.*mm, 61.17*mm, 128.34*mm, 0.*mm};

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

	auto* bgo_continuous_Solid = new G4Polyhedra(bgo_name + "_continuous_Solid", 0., 360. * deg, 8, sizeof(bgo_z)/sizeof(bgo_z[0]), bgo_r, bgo_z);
	auto* bgo_abrased_Solid = new G4Polyhedra(bgo_name + "_abrased_Solid", 22.5*deg, 360. * deg, 8, sizeof(bgo_abrased_z)/sizeof(bgo_abrased_z[0]), bgo_abrased_r, bgo_abrased_z);
	auto* bgo_cut = new G4Tubs("bgo_cut_Solid", 0, radius, length, 0., 45.*deg);
	auto* bgo_crystal_unfit_Solid = new G4IntersectionSolid(bgo_name + "_crystal_unfit_Solid", bgo_continuous_Solid, bgo_cut, 0, G4ThreeVector(.5*cos(22.5*deg), 0.5*sin(22.5*deg), 0.));
	auto* bgo_crystal_Solid = new G4IntersectionSolid(bgo_name + "_crystal_Solid", bgo_crystal_unfit_Solid, bgo_abrased_Solid, 0, G4ThreeVector());

	auto* bgo_Solid = new G4MultiUnion(bgo_name + "_Solid");

	for (int i = 0; i < 8; ++i) {
		G4Transform3D* rot = new G4RotateZ3D(45.*deg*i);
		bgo_Solid->AddNode(*bgo_crystal_Solid, *rot);
	}
	bgo_Solid->Voxelize();
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
	new G4PVPlacement(rot, pos, bgo_Logical, bgo_name, World_Logical, false, 0);
}
