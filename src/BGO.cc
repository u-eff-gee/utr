#include "BGO.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
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

#include <sstream>
#include <cstdint>
#include <map>

using std::stringstream;


G4Cons *
createCons(const std::string &prefix, const ConsDims &dims)
{
	return new G4Cons(prefix + "_Solid", dims.Rmin1, dims.Rmax1, dims.Rmin2, dims.Rmax2, dims.length * 0.5, 0. * deg, 360. * deg);
}

std::tuple<G4Cons *, G4LogicalVolume *>
createConsLogical(const std::string &prefix, const ConsDims &dims, G4Material *mat, G4Color color)
{
	G4Cons *solid = createCons(prefix, dims);
	G4LogicalVolume *logical = new G4LogicalVolume(solid, mat, prefix + "_Logical");
	logical->SetVisAttributes(color);

	return std::make_tuple(solid, logical);
}

BGO::BGO(G4LogicalVolume *World_Log, G4String name)
{
	World_Logical = World_Log;

	bgo_Name = name;
	
	/* General dimensions */
	length = 202. * mm;
	radius = 115. * mm;

	/* Outer Aluminum Case dimensions */
	al_Case[1] = ConsDims::angled(8. * mm, 16.5 * deg, 21. * deg, 25. * mm, 62. * mm);
	al_Case[2] = ConsDims::angled(78. * mm, 21. * deg, 21. * deg, al_Case[1].Rmax2 - 3. * mm, al_Case[1].Rmax2);
	al_Case[3] = ConsDims::straight(86. * mm, al_Case[2].Rmin2, al_Case[2].Rmax2);
	al_Case[4] = ConsDims::straight(20. * mm, al_Case[3].Rmin2, al_Case[3].Rmin2 + 23. * mm);
	al_Case[5] = ConsDims(11. * mm, al_Case[4].Rmin2, al_Case[4].Rmin2, 51.5 * mm, al_Case[4].Rmin2);
	al_Case[6] = ConsDims::straight(3. * mm, al_Case[5].Rmin2, al_Case[5].Rmax2);
	al_Case[7] = ConsDims(9. * mm, al_Case[6].Rmin2, al_Case[6].Rmax2, al_Case[6].Rmin2, al_Case[6].Rmin2 + 7. * mm);

	/* BGO crystal dimensions */
	// Not sure if Crystal1 can be further simplified
	auto bgo_Crystal1_length = 30. * mm;
	bgo_Crystal[1] = ConsDims(
		bgo_Crystal1_length, al_Case[1].inner_angle, al_Case[1].outer_angle,
		al_Case[1].Rmin1 + (al_Case[1].length + 2. * mm) * tan(al_Case[1].inner_angle),
		al_Case[1].Rmax2 - (3. * mm + 2. * mm) * tan(al_Case[1].outer_angle),
		al_Case[1].Rmin1 + (al_Case[1].length + 2. * mm + bgo_Crystal1_length) * tan(al_Case[1].inner_angle),
		al_Case[1].Rmax2 - (3. * mm + 2. * mm) * tan(al_Case[1].outer_angle) + bgo_Crystal1_length * tan(al_Case[1].outer_angle));
	bgo_Crystal[2] = ConsDims::angled(30. * mm, 0., al_Case[1].outer_angle, 52.3 * mm, bgo_Crystal[1].Rmax2);
	bgo_Crystal[3] = ConsDims::straight(100. * mm, 52.3 * mm, bgo_Crystal[2].Rmax2);
	bgo_Crystal[4] = ConsDims(14. * mm, bgo_Crystal[3].Rmin1, bgo_Crystal[3].Rmax1, bgo_Crystal[3].Rmin1, bgo_Crystal[3].Rmin1);
	
	/* Inner Aluminum Case dimensions */
	al_Case[11] = ConsDims::angled(43. * mm, al_Case[1].inner_angle, al_Case[1].inner_angle, al_Case[1].Rmin1 - 1. * mm * tan(al_Case[1].inner_angle), al_Case[1].Rmin1);
	al_Case[12] = ConsDims::straight(1. * mm, al_Case[11].Rmax2, bgo_Crystal[2].Rmin1 - 2. * mm);

	// TODO: Exact dimensions are unclear
	al_Case[13] = ConsDims(160. * mm, al_Case[12].Rmax1, al_Case[12].Rmax1 + 1. * mm, al_Case[12].Rmax1, al_Case[12].Rmax1 + 1. * mm);
	max_penetration_depth = al_Case[13].length;

	/* Solid Aluminum Case (for collimator hole carving) */
	G4double al_Case_Solid_z[] = {
		0., 0., al_Case[1].length, al_Case[1].length + al_Case[2].length,
		al_Case[1].length + al_Case[2].length + al_Case[3].length * 1.1,
		al_Case[1].length + al_Case[2].length + al_Case[3].length * 1.1
	};
	G4double al_Case_Solid_r[] = {0., al_Case[1].Rmax1, al_Case[1].Rmax2, al_Case[2].Rmax2, al_Case[3].Rmax2, 0.};
	al_Case_Solid = new G4Polycone("al_Case_Solid", 0., 360.* deg, 6, al_Case_Solid_r, al_Case_Solid_z);

	G4NistManager *man = G4NistManager::Instance();

	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *bgo = man->FindOrBuildMaterial("G4_BGO");
	G4Material *AIR = man->FindOrBuildMaterial("G4_AIR");

	for (auto &[i, al_Case_Part] : al_Case) {
		std::stringstream prefix;
		prefix << "al_Case" << i;
		std::tie(al_Case_Solids[i], al_Case_Logical[i]) = createConsLogical(prefix.str(), al_Case_Part, Al, magenta);
	}
	
	for (auto &[i, bgo_Crystal_Part] : bgo_Crystal) {
		std::stringstream prefix;
		prefix << bgo_Name << '_' << i;
		bgo_Crystal_name[i] = prefix.str();
		std::tie(bgo_Crystal_Solids[i], bgo_Crystal_Logical[i]) = createConsLogical(bgo_Crystal_name[i], bgo_Crystal_Part, bgo, blue);
	}

	//*************************************************
	// BGO mother volume: a tub filled with air that encloses the whole BGO
	//*************************************************

	G4Tubs *bgo_Mother_Solid = new G4Tubs("bgo_Mother_Solid", 0., radius, length * 0.5, 0. * deg, 360. * deg);

	bgo_Mother_Logical = new G4LogicalVolume(bgo_Mother_Solid, AIR, "bgo_Mother_Logical");
	bgo_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
}

void BGO::Construct(G4ThreeVector global_coordinates, G4double theta, G4double phi,
					G4double dist_from_center)
{
	G4RotationMatrix *rot = new G4RotationMatrix();
	rot->rotateX(180. * deg);

	//*************************************************
	// BGO aluminium case
	// Consists of seven parts with labels 1-7 from the target-facing side (1) to the backside (7)
	// and three parts with labels 11-13 for the inside (target-facing side to backside)
	//*************************************************

	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length * 0.5), al_Case_Logical[1], "al_Case1", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length * 0.5), al_Case_Logical[2], "al_Case2", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length - al_Case[3].length * 0.5), al_Case_Logical[3], "al_Case3", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length - al_Case[3].length - al_Case[4].length * 0.5), al_Case_Logical[4], "al_Case4", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length - al_Case[3].length - 7. * mm - al_Case[5].length * 0.5), al_Case_Logical[5], "al_Case5", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length - al_Case[3].length - 7. * mm - al_Case[5].length - al_Case[6].length * 0.5), al_Case_Logical[6], "al_Case", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - al_Case[2].length - al_Case[3].length - 7. * mm - al_Case[5].length - al_Case[6].length - al_Case[7].length * 0.5), al_Case_Logical[7], "al_Case7", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[11].length * 0.5), al_Case_Logical[11], "al_Case_11", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[11].length + al_Case[12].length * 0.5), al_Case_Logical[12], "al_Case12", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[11].length + 1. * mm - al_Case[13].length * 0.5), al_Case_Logical[13], "al_Case13", bgo_Mother_Logical, false, 0);

	//*************************************************
	// BGO crystal
	// Consists of four parts with labels 1-4 from the target-facing side (1) to the backside (4)
	//*************************************************

	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - 2. * mm - bgo_Crystal[1].length * 0.5), bgo_Crystal_Logical[1], "bgo_Crystal1", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - 2. * mm - bgo_Crystal[1].length - bgo_Crystal[2].length * 0.5), bgo_Crystal_Logical[2], "bgo_Crystal2", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - 2. * mm - bgo_Crystal[1].length - bgo_Crystal[2].length - bgo_Crystal[3].length * 0.5), bgo_Crystal_Logical[3], "bgo_Crystal3", bgo_Mother_Logical, false, 0);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., length * 0.5 - al_Case[1].length - 2. * mm - bgo_Crystal[1].length - bgo_Crystal[2].length - bgo_Crystal[3].length - bgo_Crystal[4].length * 0.5), bgo_Crystal_Logical[4], "bgo_Crystal4", bgo_Mother_Logical, false, 0);

	G4RotationMatrix *RotDet = new G4RotationMatrix();
	RotDet->rotateY(-90 * deg + phi);

	G4ThreeVector BGO_Pos(dist_from_center * sin(theta) * cos(phi), dist_from_center * cos(theta), dist_from_center * sin(theta) * sin(phi));
	new G4PVPlacement(RotDet, global_coordinates + BGO_Pos, bgo_Mother_Logical, bgo_Name, World_Logical, 0, 0);
}