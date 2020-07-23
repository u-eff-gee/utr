#pragma once

#include "G4LogicalVolume.hh"

#include "BGO.hh"

class LeadCastle
{
public:
	LeadCastle(G4LogicalVolume *World_Log);
	void Construct(G4ThreeVector global_coordinates, BGO *, BGO *, BGO *);

private:
	G4LogicalVolume *World_Logical;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *Fe = nist->FindOrBuildMaterial("G4_Fe");
	G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4Colour yellow = G4Colour(1.0, 1.0, 0.0);
	G4Colour blue = G4Colour(0.0, 0.0, 1.0);
	G4Colour grey = G4Colour(0.5, 0.5, 0.5);
	G4Colour light_orange = G4Colour(1.0, 0.82, 0.36);

	G4double block_x = 300. * mm;		//x increases to the left from the perpective of the beam
	G4double block_small_x = 100. * mm; //x increases to the left from the perpective of the beam
	G4double block_y = 300. * mm;		//y increases towards the top
	G4double block_z = 95. * mm;		//z lies in the direction of the beam

	G4double detectordistance1 = 225. * mm;
	G4double detectordistance2 = 225.*mm;
	G4double detectordistancepol = 225.*mm;
	G4double distcollimatortotarget = 162 * mm;

	G4double g1_theta = 90. * deg;
	G4double g1_phi = 0. * deg;
	G4double g2_theta = 90 * deg;
	G4double g2_phi = 140 * deg;
	G4double gPol_theta = 90. * deg;
	G4double gPol_phi = 180. * deg;

	void ConstructCollimator(G4ThreeVector local_coordinates);
	void ConstructIronShield(G4ThreeVector local_coordinates);
	void ConstructLeadShield(G4ThreeVector local_coordinates);
	void ConstructLeadCollimator(G4ThreeVector local_coordinates, BGO *, BGO *, BGO *);
};
