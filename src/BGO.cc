#include "BGO.hh"

#include "BGO.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

#include "G4Color.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

#include <sstream>

using std::stringstream;

BGO::BGO(G4String name){

	bgo_Name = name;

	G4Colour  white   (1.0, 1.0, 1.0) ;
	G4Colour  grey    (0.5, 0.5, 0.5) ;
	G4Colour  black   (0.0, 0.0, 0.0) ;
	G4Colour  red     (1.0, 0.0, 0.0) ;
	G4Colour  green   (0.0, 1.0, 0.0) ;
	G4Colour  blue    (0.0, 0.0, 1.0) ;
	G4Colour  cyan    (0.0, 1.0, 1.0) ;
	G4Colour  magenta (1.0, 0.0, 1.0) ;
	G4Colour  yellow  (1.0, 1.0, 0.0) ;
	G4Colour  orange (1.0, 0.5, 0.0) ;
	G4Colour  light_blue  (1.0, 0.82, 0.36) ;

	G4NistManager* man = G4NistManager::Instance();

	G4Material* Al = man->FindOrBuildMaterial("G4_Al");
	G4Material* bgo = man->FindOrBuildMaterial("G4_BGO");
	G4Material* AIR = man->FindOrBuildMaterial("G4_AIR");

	bgo_Mother_Length = 202.*mm;
	bgo_Mother_Radius = 115.*mm;

	//*************************************************
	// BGO mother volume: a tub filled with air that encloses the whole BGO
	//*************************************************
	
	G4Tubs *bgo_Mother_Solid = new G4Tubs("bgo_Mother_Solid", 0., bgo_Mother_Radius, bgo_Mother_Length*0.5, 0.*deg, 360.*deg);

	bgo_Mother_Logical = new G4LogicalVolume(bgo_Mother_Solid, AIR, "bgo_Mother_Logical");
	bgo_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

	//*************************************************
	// BGO aluminium case
	// Consists of seven parts with labels 1-7 from the target-facing side (1) to the backside (7)
	//*************************************************
	
	// Part 1
	
	G4double	al_Case1_Length = 8.*mm;
	G4double	al_Case1_inner_angle = 16.5*deg;
	G4double	al_Case1_outer_angle = 21.*deg;
	G4double	al_Case1_Rmin1 = 25.*mm;
	G4double	al_Case1_Rmax1 = 62.*mm;
	G4double	al_Case1_Rmin2 = al_Case1_Rmin1 + al_Case1_Length*tan(al_Case1_inner_angle);
	G4double	al_Case1_Rmax2 = al_Case1_Rmax1 + al_Case1_Length*tan(al_Case1_outer_angle);
	
	G4Cons* al_Case1_Solid = new G4Cons("al_Case1_Solid", al_Case1_Rmin1, al_Case1_Rmax1, al_Case1_Rmin2, al_Case1_Rmax2, al_Case1_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case1_Logical = new G4LogicalVolume(al_Case1_Solid, Al, "al_Case1_Logical");
	G4RotationMatrix *rot = new G4RotationMatrix();
	rot->rotateX(180.*deg);
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length*0.5), al_Case1_Logical, "al_Case1", bgo_Mother_Logical, false, 0);
	al_Case1_Logical->SetVisAttributes(magenta);

	// Part 2
	
	G4double	al_Case2_Length = 78.*mm;
	G4double	al_Case2_inner_angle = 21.*deg;
	G4double	al_Case2_outer_angle = 21.*deg;
	G4double	al_Case2_Rmin1 = al_Case1_Rmax2 - 3.*mm;
	G4double	al_Case2_Rmax1 = al_Case1_Rmax2;
	G4double	al_Case2_Rmin2 = al_Case2_Rmin1 + al_Case2_Length*tan(al_Case2_inner_angle);
	G4double	al_Case2_Rmax2 = al_Case2_Rmax1 + al_Case2_Length*tan(al_Case2_outer_angle);
	
	G4Cons* al_Case2_Solid = new G4Cons("al_Case2_Solid", al_Case2_Rmin1, al_Case2_Rmax1, al_Case2_Rmin2, al_Case2_Rmax2, al_Case2_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case2_Logical = new G4LogicalVolume(al_Case2_Solid, Al, "al_Case2_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length*0.5), al_Case2_Logical, "al_Case2", bgo_Mother_Logical, false, 0);
	al_Case2_Logical->SetVisAttributes(magenta);
	
	// Part 3
	
	G4double	al_Case3_Length = 86.*mm;
	G4double	al_Case3_Rmin1 = al_Case2_Rmin2;
	G4double	al_Case3_Rmax1 = al_Case2_Rmax2;
	G4double	al_Case3_Rmin2 = al_Case3_Rmin1; 
	G4double	al_Case3_Rmax2 = al_Case3_Rmax1;	
	G4Cons* al_Case3_Solid = new G4Cons("al_Case3_Solid", al_Case3_Rmin1, al_Case3_Rmax1, al_Case3_Rmin2, al_Case3_Rmax2, al_Case3_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case3_Logical = new G4LogicalVolume(al_Case3_Solid, Al, "al_Case3_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length*0.5), al_Case3_Logical, "al_Case3", bgo_Mother_Logical, false, 0);

	al_Case3_Logical->SetVisAttributes(magenta);
	
	// Part 4
	
	G4double	al_Case4_Length = 20.*mm;
	G4double	al_Case4_Rmin1 = al_Case3_Rmin2;
	G4double	al_Case4_Rmax1 = al_Case4_Rmin1 + 23.*mm;
	G4double	al_Case4_Rmin2 = al_Case4_Rmin1; 
	G4double	al_Case4_Rmax2 = al_Case4_Rmax1;	
	G4Cons* al_Case4_Solid = new G4Cons("al_Case4_Solid", al_Case4_Rmin1, al_Case4_Rmax1, al_Case4_Rmin2, al_Case4_Rmax2, al_Case4_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case4_Logical = new G4LogicalVolume(al_Case4_Solid, Al, "al_Case4_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - al_Case4_Length*0.5), al_Case4_Logical, "al_Case4", bgo_Mother_Logical, false, 0);

	al_Case4_Logical->SetVisAttributes(magenta);
	
	// Part 5
	
	G4double	al_Case5_Length = 11.*mm;
	G4double	al_Case5_Rmin1 = al_Case4_Rmin2;
	G4double	al_Case5_Rmax1 = al_Case4_Rmin2;
	G4double	al_Case5_Rmin2 = 51.5*mm; 
	G4double	al_Case5_Rmax2 = al_Case5_Rmax1;	
	G4Cons* al_Case5_Solid = new G4Cons("al_Case5_Solid", al_Case5_Rmin1, al_Case5_Rmax1, al_Case5_Rmin2, al_Case5_Rmax2, al_Case5_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case5_Logical = new G4LogicalVolume(al_Case5_Solid, Al, "al_Case5_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm - al_Case5_Length*0.5), al_Case5_Logical, "al_Case5", bgo_Mother_Logical, false, 0);

	al_Case5_Logical->SetVisAttributes(magenta);
	
	// Part 6
	
	G4double	al_Case6_Length = 3.*mm;
	G4double	al_Case6_Rmin1 = al_Case5_Rmin2;
	G4double	al_Case6_Rmax1 = al_Case5_Rmax2;
	G4double	al_Case6_Rmin2 = al_Case6_Rmin1; 
	G4double	al_Case6_Rmax2 = al_Case6_Rmax1;	
	G4Cons* al_Case6_Solid = new G4Cons("al_Case6_Solid", al_Case6_Rmin1, al_Case6_Rmax1, al_Case6_Rmin2, al_Case6_Rmax2, al_Case6_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case6_Logical = new G4LogicalVolume(al_Case6_Solid, Al, "al_Case6_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm  - al_Case5_Length - al_Case6_Length*0.5), al_Case6_Logical, "al_Case6", bgo_Mother_Logical, false, 0);

	al_Case6_Logical->SetVisAttributes(magenta);
	
	// Part 7
	
	G4double	al_Case7_Length = 9.*mm;
	G4double	al_Case7_Rmin1 = al_Case6_Rmin2;
	G4double	al_Case7_Rmax1 = al_Case6_Rmax2;
	G4double	al_Case7_Rmin2 = al_Case6_Rmin2; 
	G4double	al_Case7_Rmax2 = al_Case6_Rmin2 + 7.*mm;

	G4Cons* al_Case7_Solid = new G4Cons("al_Case7_Solid", al_Case7_Rmin1, al_Case7_Rmax1, al_Case7_Rmin2, al_Case7_Rmax2, al_Case7_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Case7_Logical = new G4LogicalVolume(al_Case7_Solid, Al, "al_Case7_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - al_Case2_Length - al_Case3_Length - 7.*mm  - al_Case5_Length - al_Case6_Length - al_Case7_Length*0.5), al_Case7_Logical, "al_Case7", bgo_Mother_Logical, false, 0);

	al_Case7_Logical->SetVisAttributes(magenta);
	
	//*************************************************
	// BGO aluminium case (solid part for use in G4SubtractionSolids)
	// Consists of the first three parts of the BGO aluminium case as a solid (Rmin = 0). By defining this separate G4UnionSolid al_Case_Solid, it is easier to carve the collimator holes.
	//*************************************************
	
	// Part 1
	
	G4double	al_SolidCase1_Length = 8.*mm;
	//G4double	al_SolidCase1_inner_angle = 16.5*deg;
	G4double	al_SolidCase1_outer_angle = 21.*deg;
	G4double	al_SolidCase1_Rmin1 = 0.*mm;
	G4double	al_SolidCase1_Rmax1 = 62.*mm;
	G4double	al_SolidCase1_Rmin2 = 0.*mm;
	G4double	al_SolidCase1_Rmax2 = al_SolidCase1_Rmax1 + al_SolidCase1_Length*tan(al_SolidCase1_outer_angle);
	
	G4Cons* al_SolidCase1_Solid = new G4Cons("al_SolidCase1_Solid", al_SolidCase1_Rmin1, al_SolidCase1_Rmax1, al_SolidCase1_Rmin2, al_SolidCase1_Rmax2, al_SolidCase1_Length*0.5, 0.*deg, 360.*deg);

	// Part 2
	
	G4double	al_SolidCase2_Length = 78.*mm;
	//G4double	al_SolidCase2_inner_angle = 21.*deg;
	G4double	al_SolidCase2_outer_angle = 21.*deg;
	G4double	al_SolidCase2_Rmin1 = 0.*mm;
	G4double	al_SolidCase2_Rmax1 = al_Case1_Rmax2;
	G4double	al_SolidCase2_Rmin2 = 0.*mm;
	G4double	al_SolidCase2_Rmax2 = al_SolidCase2_Rmax1 + al_SolidCase2_Length*tan(al_SolidCase2_outer_angle);
		
	G4Cons* al_SolidCase2_Solid = new G4Cons("al_SolidCase2_Solid", al_SolidCase2_Rmin1, al_SolidCase2_Rmax1, al_SolidCase2_Rmin2, al_SolidCase2_Rmax2, al_SolidCase2_Length*0.5, 0.*deg, 360.*deg);
	
	// Part 3
	
	G4double	al_SolidCase3_Length = 86.*mm;
	G4double	al_SolidCase3_Rmin1 = 0.*mm;
	G4double	al_SolidCase3_Rmax1 = al_Case2_Rmax2;
	G4double	al_SolidCase3_Rmin2 = 0.*mm;
	G4double	al_SolidCase3_Rmax2 = al_SolidCase3_Rmax1;	
	G4Cons* al_SolidCase3_Solid = new G4Cons("al_SolidCase3_Solid", al_SolidCase3_Rmin1, al_SolidCase3_Rmax1, al_SolidCase3_Rmin2, al_SolidCase3_Rmax2, al_SolidCase3_Length*0.5, 0.*deg, 360.*deg);

	// Merge the three parts into G4UnionSolid
	
	G4UnionSolid *al_Case_12_Solid = new G4UnionSolid("al_Case_12_Solid", al_SolidCase1_Solid, al_SolidCase2_Solid, 0, G4ThreeVector(0.*mm, 0.*mm, al_SolidCase1_Length*0.5 + al_SolidCase2_Length*0.5));
	
	al_Case_Solid = new G4UnionSolid("al_Case_Solid", al_Case_12_Solid, al_SolidCase3_Solid, 0, G4ThreeVector(0.*mm, 0.*mm, al_SolidCase1_Length*0.5 + al_SolidCase2_Length+ al_SolidCase3_Length*0.5));
	
	//*************************************************
	// BGO crystal
	// Consists of four parts with labels 1-4 from the target-facing side (1) to the backside (4)
	//*************************************************

	// Part 1
	
	G4double	bgo_Crystal1_Length = 30.*mm;
	//G4double	bgo_Crystal1_inner_angle = al_Case1_inner_angle;
	//G4double	bgo_Crystal1_outer_angle = al_Case1_outer_angle;
	G4double	bgo_Crystal1_Rmin1 = al_Case1_Rmin1 + (al_Case1_Length + 2.*mm)*tan(al_Case1_inner_angle);
	G4double	bgo_Crystal1_Rmax1 = al_Case1_Rmax2 - (3.*mm + 2.*mm)*tan(al_Case1_outer_angle);

	G4double	bgo_Crystal1_Rmin2 = al_Case1_Rmin1 + (al_Case1_Length + 2.*mm + bgo_Crystal1_Length)*tan(al_Case1_inner_angle) ; 
	G4double	bgo_Crystal1_Rmax2 = al_Case1_Rmax2 - (3.*mm + 2.*mm)*tan(al_Case1_outer_angle) + bgo_Crystal1_Length*tan(al_Case1_outer_angle);

	G4Cons* bgo_Crystal1_Solid = new G4Cons("bgo_Crystal1_Solid", bgo_Crystal1_Rmin1, bgo_Crystal1_Rmax1, bgo_Crystal1_Rmin2, bgo_Crystal1_Rmax2, bgo_Crystal1_Length*0.5, 0.*deg, 360.*deg);

	stringstream logicalVolume_name;
	logicalVolume_name << bgo_Name << "_1";

	G4LogicalVolume* bgo_Crystal1_Logical = new G4LogicalVolume(bgo_Crystal1_Solid, bgo, logicalVolume_name.str());
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - 2.*mm - bgo_Crystal1_Length*0.5), bgo_Crystal1_Logical, "bgo_Crystal1", bgo_Mother_Logical, false, 0);

	bgo_Crystal1_Logical->SetVisAttributes(blue);
	
	// Part 2
		
	G4double	bgo_Crystal2_Length = 30.*mm;
	//G4double	bgo_Crystal2_outer_angle = al_Case1_outer_angle;
	G4double	bgo_Crystal2_Rmin1 = 52.3*mm;
	G4double	bgo_Crystal2_Rmax1 = bgo_Crystal1_Rmax2;

	G4double	bgo_Crystal2_Rmin2 = bgo_Crystal2_Rmin1;
	G4double	bgo_Crystal2_Rmax2 = bgo_Crystal2_Rmax1 + bgo_Crystal2_Length*tan(al_Case1_outer_angle);

	G4Cons* bgo_Crystal2_Solid = new G4Cons("bgo_Crystal2_Solid", bgo_Crystal2_Rmin1, bgo_Crystal2_Rmax1, bgo_Crystal2_Rmin2, bgo_Crystal2_Rmax2, bgo_Crystal2_Length*0.5, 0.*deg, 360.*deg);

	logicalVolume_name.str("");
	logicalVolume_name.clear();
	logicalVolume_name << bgo_Name << "_2";

	G4LogicalVolume* bgo_Crystal2_Logical = new G4LogicalVolume(bgo_Crystal2_Solid, bgo, logicalVolume_name.str());
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - 2.*mm - bgo_Crystal1_Length - bgo_Crystal2_Length*0.5), bgo_Crystal2_Logical, "bgo_Crystal2", bgo_Mother_Logical, false, 0);

	bgo_Crystal2_Logical->SetVisAttributes(blue);
	
	// Part 3
		
	G4double	bgo_Crystal3_Length = 100.*mm;
	G4double	bgo_Crystal3_Rmin1 = 52.3*mm;
	G4double	bgo_Crystal3_Rmax1 = bgo_Crystal2_Rmax2;

	G4double	bgo_Crystal3_Rmin2 = bgo_Crystal3_Rmin1;
	G4double	bgo_Crystal3_Rmax2 = bgo_Crystal3_Rmax1;

	G4Cons* bgo_Crystal3_Solid = new G4Cons("bgo_Crystal3_Solid", bgo_Crystal3_Rmin1, bgo_Crystal3_Rmax1, bgo_Crystal3_Rmin2, bgo_Crystal3_Rmax2, bgo_Crystal3_Length*0.5, 0.*deg, 360.*deg);

	logicalVolume_name.str("");
	logicalVolume_name.clear();
	logicalVolume_name << bgo_Name << "_3";

	G4LogicalVolume* bgo_Crystal3_Logical = new G4LogicalVolume(bgo_Crystal3_Solid, bgo, logicalVolume_name.str());
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - 2.*mm - bgo_Crystal1_Length - bgo_Crystal2_Length - bgo_Crystal3_Length*0.5), bgo_Crystal3_Logical, "bgo_Crystal3", bgo_Mother_Logical, false, 0);

	bgo_Crystal3_Logical->SetVisAttributes(blue);
	
	// Part 4
		
	G4double	bgo_Crystal4_Length = 14.*mm;
	G4double	bgo_Crystal4_Rmin1 = bgo_Crystal3_Rmin1;
	G4double	bgo_Crystal4_Rmax1 = bgo_Crystal3_Rmax1;

	G4double	bgo_Crystal4_Rmin2 = bgo_Crystal4_Rmin1;
	G4double	bgo_Crystal4_Rmax2 = bgo_Crystal4_Rmin1;

	G4Cons* bgo_Crystal4_Solid = new G4Cons("bgo_Crystal4_Solid", bgo_Crystal4_Rmin1, bgo_Crystal4_Rmax1, bgo_Crystal4_Rmin2, bgo_Crystal4_Rmax2, bgo_Crystal4_Length*0.5, 0.*deg, 360.*deg);

	logicalVolume_name.str("");
	logicalVolume_name.clear();
	logicalVolume_name << bgo_Name << "_4";

	G4LogicalVolume* bgo_Crystal4_Logical = new G4LogicalVolume(bgo_Crystal4_Solid, bgo, logicalVolume_name.str());
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Case1_Length - 2.*mm - bgo_Crystal1_Length - bgo_Crystal2_Length - bgo_Crystal3_Length - bgo_Crystal4_Length*0.5), bgo_Crystal4_Logical, "bgo_Crystal4", bgo_Mother_Logical, false, 0);

	bgo_Crystal4_Logical->SetVisAttributes(blue);
	
	//*************************************************
	// BGO inner aluminium case
	// Consists of three parts with labels 1-3 from the target-facing side (1) to the backside (3)
	//*************************************************
	
	// Part 1
	
	G4double	al_Inner_Case1_Length = 43.*mm;
	//G4double	al_Inner_Case1_inner_angle = al_Case1_inner_angle;
	//G4double	al_Inner_Case1_outer_angle = al_Case1_outer_angle;
	G4double	al_Inner_Case1_Rmin1 = al_Case1_Rmin1 - 1.*mm*tan(al_Case1_inner_angle);
	G4double	al_Inner_Case1_Rmax1 = al_Case1_Rmin1;

	G4double	al_Inner_Case1_Rmin2 = al_Inner_Case1_Rmin1 + al_Inner_Case1_Length*tan(al_Case1_inner_angle);
	G4double	al_Inner_Case1_Rmax2 = al_Inner_Case1_Rmax1 + al_Inner_Case1_Length*tan(al_Case1_inner_angle);


	G4Cons* al_Inner_Case1_Solid = new G4Cons("al_Inner_Case1_Solid", al_Inner_Case1_Rmin1, al_Inner_Case1_Rmax1, al_Inner_Case1_Rmin2, al_Inner_Case1_Rmax2, al_Inner_Case1_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Inner_Case1_Logical = new G4LogicalVolume(al_Inner_Case1_Solid, Al, "al_Inner_Case1_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Inner_Case1_Length * 0.5), al_Inner_Case1_Logical, "al_Inner_Case1", bgo_Mother_Logical, false, 0);

	al_Inner_Case1_Logical->SetVisAttributes(magenta);
	
	// Part 2
	
	G4double	al_Inner_Case2_Length = 1.*mm;
	G4double	al_Inner_Case2_Rmin1 = al_Inner_Case1_Rmax2;
	G4double	al_Inner_Case2_Rmax1 = bgo_Crystal2_Rmin1 - 2.*mm;

	G4double	al_Inner_Case2_Rmin2 = al_Inner_Case2_Rmin1;
	G4double	al_Inner_Case2_Rmax2 = al_Inner_Case2_Rmax1;


	G4Cons* al_Inner_Case2_Solid = new G4Cons("al_Inner_Case2_Solid", al_Inner_Case2_Rmin1, al_Inner_Case2_Rmax1, al_Inner_Case2_Rmin2, al_Inner_Case2_Rmax2, al_Inner_Case2_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Inner_Case2_Logical = new G4LogicalVolume(al_Inner_Case2_Solid, Al, "al_Inner_Case2_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Inner_Case1_Length + al_Inner_Case2_Length * 0.5), al_Inner_Case2_Logical, "al_Inner_Case2", bgo_Mother_Logical, false, 0);

	al_Inner_Case2_Logical->SetVisAttributes(magenta);

	// Part 3
	
	G4double	al_Inner_Case3_Length = 160.*mm;
	max_penetration_depth = al_Inner_Case3_Length;
	G4double	al_Inner_Case3_Rmin1 = al_Inner_Case2_Rmax1;
	G4double	al_Inner_Case3_Rmax1 = al_Inner_Case2_Rmax1 + 1.*mm;

	G4double	al_Inner_Case3_Rmin2 = al_Inner_Case3_Rmin1;
	G4double	al_Inner_Case3_Rmax2 = al_Inner_Case2_Rmax1;


	G4Cons* al_Inner_Case3_Solid = new G4Cons("al_Inner_Case3_Solid", al_Inner_Case3_Rmin1, al_Inner_Case3_Rmax1, al_Inner_Case3_Rmin2, al_Inner_Case3_Rmax2, al_Inner_Case3_Length*0.5, 0.*deg, 360.*deg);
	G4LogicalVolume* al_Inner_Case3_Logical = new G4LogicalVolume(al_Inner_Case3_Solid, Al, "al_Inner_Case3_Logical");
	new G4PVPlacement(rot, G4ThreeVector(0., 0., bgo_Mother_Length*0.5 - al_Inner_Case1_Length + 1.*mm - al_Inner_Case3_Length * 0.5), al_Inner_Case3_Logical, "al_Inner_Case3", bgo_Mother_Logical, false, 0);

	al_Inner_Case3_Logical->SetVisAttributes(magenta);
}

BGO::~BGO(){}
