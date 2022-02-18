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
#include "G4IntersectionSolid.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "G4NistManager.hh"

#include "NamedColors.hh"

BGO::BGO(G4LogicalVolume *World_Log, G4String name)
{
	World_Logical = World_Log;
	bgo_name = name;

	// FIXME: The BGO is further abraded on its edges, but the schematics
	// don’t indicate how much abrasion there is. The current implementiation
	// should be sufficient for simulating the HPGe detector (the center of
	// the tip of the BGO is correct), but the efficiency of the BGO cannot
	// be determined correctly. The following is a best effort that is better
	// than doing nothing but probably does not reflect the reality.

	G4NistManager *man = G4NistManager::Instance();
	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *bgo = man->FindOrBuildMaterial("G4_BGO");

	caseFilled_Solid = new G4Polycone(bgo_name + "_caseFilled_Solid", 0., 360.* deg, caseFilled_z.size(), caseFilled_r.data(), caseFilled_z.data());
	auto* caseOut_Solid = new G4GenericPolycone(bgo_name + "_caseOut_Solid", 0., 360. * deg, caseOut_z.size(), caseOut_r.data(), caseOut_z.data());
	auto* caseIn_Solid = new G4GenericPolycone(bgo_name + "_caseIn_Solid", 0., 360. * deg, caseIn_z.size(), caseIn_r.data(), caseIn_z.data());
	auto* caseGap_Solid = new G4GenericPolycone(bgo_name + "_caseGap_Solid", 0., 360. * deg, caseGap_z.size(), caseGap_r.data(), caseGap_z.data());
	auto* bgo_case_Solid = new G4SubtractionSolid(bgo_name + "_case_Solid", caseOut_Solid, caseIn_Solid, 0, G4ThreeVector(0, 0, 0));
	bgo_case_Logical = new G4LogicalVolume(bgo_case_Solid, Al, bgo_name + "_case_Logical");
	bgo_case_Logical->SetVisAttributes(magenta);

	auto* bgo_continuous_Solid = new G4Polyhedra(bgo_name + "_continuous_Solid", 0., 360. * deg, 8, bgo_z.size(), bgo_r.data(), bgo_z.data());
	auto* bgo_abrased_Solid = new G4Polyhedra(bgo_name + "_abrased_Solid", 22.5*deg, 360. * deg, 8, bgo_abrased_z.size(), bgo_abrased_r.data(), bgo_abrased_z.data());
	auto* bgo_cut = new G4Tubs("bgo_cut_Solid", 0, Get_Radius(), Get_Length(), 0., 45.*deg);
	auto* bgo_crystal_unfit1_Solid = new G4IntersectionSolid(bgo_name + "_crystal_unfit1_Solid", bgo_continuous_Solid, bgo_cut, 0, G4ThreeVector(.5*cos(22.5*deg), 0.5*sin(22.5*deg), 0.));
	auto* bgo_crystal_unfit2_Solid = new G4IntersectionSolid(bgo_name + "_crystal_unfit2_Solid", bgo_crystal_unfit1_Solid, bgo_abrased_Solid, 0, G4ThreeVector());
	auto* bgo_crystal_Solid = new G4IntersectionSolid(bgo_name + "_crystal_Solid", bgo_crystal_unfit2_Solid, caseGap_Solid, 0, G4ThreeVector());

	char segment_name[1024];
	for (int i = 0; i < 8; ++i) {
		snprintf(segment_name, sizeof(segment_name), "%s_%d_Logical", bgo_name.c_str(), i);
		auto* bgo_segment_Logical = new G4LogicalVolume(bgo_crystal_Solid, bgo, segment_name);
		bgo_segment_Logical->SetVisAttributes(blue);
		bgo_Logical.push_back(bgo_segment_Logical);
	}
}

G4ThreeVector BGO::GetPos(G4double theta, G4double phi, G4double dist_from_center) const
{
	auto dist = dist_from_center + Get_Length() * 0.5;
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

	char segment_name[1024];
	for (int i = 0; i < 8; ++i) {
		auto* rots = new G4RotationMatrix();
		rots->rotateY(90. * deg + phi);
		rots->rotateZ(i * 45. * deg);
		snprintf(segment_name, sizeof(segment_name), "%s_%d", bgo_name.c_str(), i);
		new G4PVPlacement(rots, pos, bgo_Logical[i], segment_name, World_Logical, false, 0);
	}
}
