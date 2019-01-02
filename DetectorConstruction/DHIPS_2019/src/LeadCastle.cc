
#include <iostream>
#include <sstream>
#include <vector>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"

#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4GenericTrap.hh"

#include "Vacuum.hh"

#include "LeadCastle.hh"
using std::cout;
using std::endl;
using std::stringstream;
using std::vector;

// LeadCastle
LeadCastle::LeadCastle(G4LogicalVolume *World_Log):World_Logical(World_Log){}


void LeadCastle::Construct(G4ThreeVector global_coordinates)
{
	LeadCastle_Mother_Solid = new G4Box("LeadCastle_Solid", 2*m,2*m,2*m);
	LeadCastle_Mother_Logical = new G4LogicalVolume(LeadCastle_Mother_Solid, air, "LeadCastle_Logical");
	LeadCastle_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

	HPGe1 = new Germanium1_TUD("Germanium1_TUD");
	HPGe2 = new Germanium2_TUD("Germanium2_TUD");
	HPGePol = new Polarimeter_TUD("Polarimeter_TUD");

	BGO1 = new BGO("BGO1");
	BGO2 = new BGO("BGO2");
	BGOPol = new BGO("BGOP");

	ConsturctCollimator(global_coordinates+G4ThreeVector(0,0,-(10 * block_z)*0.5-distcollimatortotarget));
	ConsturctIronShield(global_coordinates+G4ThreeVector(0,0,-(10 * block_z+48*cm)*0.5-distcollimatortotarget));
	ConsturctLeadShield(global_coordinates+G4ThreeVector(0,0,-distcollimatortotarget));

	new G4PVPlacement(0, global_coordinates, LeadCastle_Mother_Logical, "Lead_Castle", World_Logical, false, 0);
}

void LeadCastle::ConsturctIronShield(G4ThreeVector local_coordinates)
{	
	/************************* Iron Shielding *****************/
	// Iron Shielding around the Copper Collimator
	// 20 cm thick on the sides. 10cm thick on top. 40 cm thick at the bottom
	// Length unclear. Assumption: All iron until it reaches 9.5cm before the target chamber.
	// Box 21,23 are the Boxes on the left and rigth side and they are reaching 
	// all the way to top and bottom.
	// Box 22,24 are the Boxes at the top and at the bottom.
	
	// The G4Box needs half the length of a side as an input argument!
	
	G4VisAttributes *IronShieldvis = new G4VisAttributes(blue);
	// Beginning IronShield1-----------------------------------

	G4LogicalVolume *IronShield1_Logical;
	G4Box *IronShield1_Solid; 

	IronShield1_Solid = new G4Box("IronShield1_Solid",10*cm,25*cm, ((-280 * mm)+10 * block_z )*0.5);
	IronShield1_Logical = new G4LogicalVolume(IronShield1_Solid, Fe, "IronShield1_Logical", 0, 0, 0);
	IronShield1_Logical->SetVisAttributes(IronShieldvis);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(25.*cm , 0,0), IronShield1_Logical, "IronShield1", World_Logical, 0, 0);
	//End of IronShield1--------------------------------------------------------

	// //Beginning IronShield2-----------------------------------------
	G4LogicalVolume *IronShield2_Logical;
	G4Box *IronShield2_Solid; 

	IronShield2_Solid = new G4Box("IronShield2_Solid",15*cm,5*cm, ((-280  * mm)+10 * block_z )*0.5);
	IronShield2_Logical = new G4LogicalVolume(IronShield2_Solid, Fe, "IronShield2_Logical", 0, 0, 0);
	IronShield2_Logical->SetVisAttributes(IronShieldvis);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(0., 20.*cm,0 ), IronShield2_Logical, "IronShield2", World_Logical, 0, 0);
	//End IronShield2----------------------------------------

	//Beginning IronShield3-------------------------------------
	G4LogicalVolume *IronShield3_Logical;
	G4Box *IronShield3_Solid; 

	IronShield3_Solid = new G4Box("IronShield3_Solid",10*cm,25*cm, ((-280  * mm)+10 * block_z )*0.5);
	IronShield3_Logical = new G4LogicalVolume(IronShield3_Solid, Fe, "IronShield3_Logical", 0, 0, 0);
	IronShield3_Logical->SetVisAttributes(IronShieldvis);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(-25.*cm , 0.,0 ), IronShield3_Logical, "IronShield3", World_Logical, 0, 0);
	//End IronShield3---------------------------------------

	//Beginning IronShield4-------------------------------------
	G4LogicalVolume *IronShield4_Logical;
	G4Box *IronShield4_Solid; 

	IronShield4_Solid = new G4Box("IronShield4_Solid",15*cm,5*cm, ((-280 * mm)+10 * block_z )*0.5);
	IronShield4_Logical = new G4LogicalVolume(IronShield4_Solid, Fe, "block24_Logical", 0, 0, 0);
	IronShield4_Logical->SetVisAttributes(IronShieldvis);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(0., -20.*cm,0), IronShield4_Logical, "IronShield4", World_Logical, 0, 0);
	//End Block 24---------------------------------------
}

void LeadCastle::ConsturctCollimator(G4ThreeVector local_coordinates)
{
	const int NBlocks = 10;
	//*************************************************
	// Colimator volume
	//*************************************************
	G4Box *big_block = new G4Box("blockwithouthole", block_x *0.5, block_y *0.5, block_z *0.5);
	G4Box *small_block = new G4Box("blockwithouthole", block_small_x *0.5, block_y *0.5, block_z *0.5); //next to target

	G4double colholeradius_min=6. * mm;
	G4double colholeradius_max=10. * mm;

	G4double hole_radius;
	G4Tubs *hole;
	G4SubtractionSolid *Collimator_block; 
	G4LogicalVolume *Collimator_blocks_Logical[NBlocks];
	
	// _________________________ Block Loop _________________________
	for (G4int i=0; i<NBlocks;++i)
	{
		hole_radius = (colholeradius_min+(colholeradius_max-colholeradius_min)/NBlocks*i);
		hole = new G4Tubs(("hole"+std::to_string(i)).c_str(), 0., hole_radius, block_z*0.5, 0., s);
		if (i<4)
		{
			Collimator_block = new G4SubtractionSolid(("Collimator_Block"+std::to_string(i)).c_str(),small_block, hole);
		}
		else
		{
			Collimator_block = new G4SubtractionSolid( ("Collimator_Block"+std::to_string(i)).c_str(),big_block, hole);
		}
		Collimator_blocks_Logical[i] = new G4LogicalVolume(Collimator_block, Cu, ("Collimator_Block"+std::to_string(i)).c_str(), 0, 0, 0);
		Collimator_blocks_Logical[i]->SetVisAttributes(light_orange);

		new G4PVPlacement(0, local_coordinates+G4ThreeVector(0., 0., (NBlocks*0.5-i-0.5)* block_z), Collimator_blocks_Logical[i],("Collimator_Block"+std::to_string(i)).c_str(),World_Logical, 0, 0);
	}
}

void LeadCastle::ConsturctLeadShield(G4ThreeVector local_coordinates)
{
	//Beginning LeadCeiling-------------------------------------
	G4LogicalVolume *LeadCeiling_Logical;
	G4Box *LeadCeiling_Solid; 

	LeadCeiling_Solid = new G4Box("LeadCeiling_Solid",180*cm,10*cm,120*cm);
	LeadCeiling_Logical = new G4LogicalVolume(LeadCeiling_Solid, Pb, "LeadCeiling_Logical", 0, 0, 0);
	LeadCeiling_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(0.,+35.*cm,0.), LeadCeiling_Logical, "LeadCeiling", World_Logical, 0, 0);
	//End LeadCeiling---------------------------------------

	//Beginning LeadFloor-------------------------------------
	G4LogicalVolume *LeadFloor_Logical;
	G4Box *LeadFloor_Solid; 
	
	LeadFloor_Solid = new G4Box("LeadFloor_Solid",180*cm,10*cm, 120*cm);
	LeadFloor_Logical = new G4LogicalVolume(LeadFloor_Solid, Pb, "LeadFloor_Logical", 0, 0, 0);
	LeadFloor_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(0. , -35.*cm,0.), LeadFloor_Logical, "LeadFloor", World_Logical, 0, 0);
	//End LeadFloor---

	//Beginning LeadBackWall-------------------------------------
	G4LogicalVolume *LeadBackWall_Logical;
	G4Box *LeadBackWall_Solid; 
	
	LeadBackWall_Solid = new G4Box("LeadBackWall_Solid",160*cm,25*cm, 10*cm);
	LeadBackWall_Logical = new G4LogicalVolume(LeadBackWall_Solid, Pb, "LeadBackWall_Logical", 0, 0, 0);
	LeadBackWall_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(0.,0.,110.*cm), LeadBackWall_Logical, "LeadBackWall", World_Logical, 0, 0);
	//End LeadBackWall---

	//Beginning LeadRightWall-------------------------------------
	G4LogicalVolume *LeadRightWall_Logical;
	G4Box *LeadRightWall_Solid; 
	
	LeadRightWall_Solid = new G4Box("LeadRightWall_Solid",10*cm,25*cm, 120*cm);
	LeadRightWall_Logical = new G4LogicalVolume(LeadRightWall_Solid, Pb, "LeadRightWall_Logical", 0, 0, 0);
	LeadRightWall_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(-170*cm,0.,0), LeadRightWall_Logical, "LeadRightWall", World_Logical, 0, 0);
	//End LeadRightWall---

	//Beginning LeadLeftCollimator-------------------------------------
	G4LogicalVolume *LeadLeftCollimator_Logical;
	G4Box *LeadLeftCollimator_Solid; 

	LeadLeftCollimator_Solid = new G4Box("LeadLeftCollimator_Solid",10*cm,25*cm, 28.5*cm);
	LeadLeftCollimator_Logical = new G4LogicalVolume(LeadLeftCollimator_Solid, Pb, "LeadLeftCollimator_Logical", 0, 0, 0);
	LeadLeftCollimator_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(45.*cm, 0.,-95*cm+28.5*cm), LeadLeftCollimator_Logical, "LeadLeftCollimator", World_Logical, 0, 0);
	//End LeadLeftCollimator---------------------------------------

	//Beginning LeadRightCollimator-------------------------------------
	G4LogicalVolume *LeadRightCollimator_Logical;
	G4Box *LeadRightCollimator_Solid; 

	LeadRightCollimator_Solid = new G4Box("LeadRightCollimator_Solid",10*cm,25*cm, 28.5*cm);
	LeadRightCollimator_Logical = new G4LogicalVolume(LeadRightCollimator_Solid, Pb, "block31_Logical", 0, 0, 0);
	LeadRightCollimator_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(-45*cm ,0.,-66.5*cm), LeadRightCollimator_Logical, "LeadRightCollimator", World_Logical, 0, 0);
	//End LeadRightCollimator---------------------------------------

	//Beginning LeadCastleGate-------------------------------------
	G4LogicalVolume *LeadCastleGate_Logical;
	G4Box *LeadCastleGate_Solid; 

	LeadCastleGate_Solid = new G4Box("LeadCastleGate_Solid",52.5*cm,25*cm, 15*cm);
	LeadCastleGate_Logical = new G4LogicalVolume(LeadCastleGate_Solid, Pb, "block73_Logical", 0, 0, 0);
	LeadCastleGate_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(107.5*cm, 0,-53*cm), LeadCastleGate_Logical, "LeadCastleGate", World_Logical, 0, 0);
	//End LeadCastleGate---------------------------------------

	//Beginning LeadCastle_LeftWall-------------------------------------
	G4LogicalVolume *LeadCastle_LeftWall_Logical;
	G4Box *LeadCastle_LeftWall_Solid; 

	LeadCastle_LeftWall_Solid = new G4Box("LeadCastle_LeftWall_Solid",10*cm,25*cm, 93.5*cm);
	LeadCastle_LeftWall_Logical = new G4LogicalVolume(LeadCastle_LeftWall_Solid, Pb, "LeadCastle_LeftWall_Logical", 0, 0, 0);
	LeadCastle_LeftWall_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(170.*cm , 0.,26.5*cm), LeadCastle_LeftWall_Logical, "LeadCastle_LeftWall", World_Logical, 0, 0);
	//End LeadCastle_LeftWall---------------------------------------

	//Beginning LeadDetectorCollimator_Pol_Det2-------------------------------------
	G4double g2_theta=90.*deg;
	G4double g2_phi= 140*deg;

	G4double pol_theta=90.*deg;
	G4double pol_phi=180.*deg;

	G4Box* Det2_Box_Solid = new G4Box("Det2_Box_Solid",5.*cm,25*cm, (14.55/2)*cm+3*cm); 
	G4Box* Pol_Box_Solid = new G4Box("Pol_Box_Solid",14.*cm,25*cm, 15.6*cm); //1mm Overlap between the blocks
	G4UnionSolid* Det2_Pol_Box_Solid= new G4UnionSolid("Det2_Pol_Box_Solid", Det2_Box_Solid, Pol_Box_Solid,0, G4ThreeVector(9*cm, 0.,-(14.55/2)*cm+15.5*cm+36*cm-0.1*cm));
	
	vector <G4TwoVector> vertices1{
		G4TwoVector(5.*cm,25*cm),
		G4TwoVector(5.*cm,-25*cm),
		G4TwoVector(-5.*cm,-25*cm),
		G4TwoVector(-5.*cm,25*cm),
		G4TwoVector(23.*cm,25*cm),
		G4TwoVector(23.*cm,-25*cm),
		G4TwoVector(-5.*cm,-25*cm),
		G4TwoVector(-5.*cm,25*cm)};

	G4GenericTrap* Middle_Box_Solid = new G4GenericTrap("Middle_Box_Solid",(21.55/2)*cm, vertices1);
	G4UnionSolid* LeadCollimator_Pol_Det2_Plain_Solid= new G4UnionSolid("LeadCollimator_Pol_Det2_Plain_Solid", Det2_Pol_Box_Solid, Middle_Box_Solid,0, G4ThreeVector(0, 0.,(36/2)*cm));
	
	G4double BGO2_Distance=-(detectordistance2 + BGO2->Get_Length()/2)+block_small_x;
	G4RotationMatrix RotationDet2_Y;
	RotationDet2_Y.rotateY(M_PI*230/180.*rad);
	G4ThreeVector TranslationDet2(-17.5*cm +BGO2_Distance*sin(g2_theta)*cos(g2_phi),BGO2_Distance*cos(g2_theta),-(-distcollimatortotarget+14.55/2*cm-35*cm)+BGO2_Distance*sin(g2_theta)*sin(g2_phi));


	G4UnionSolid* BGO2_AlCase = BGO2->Get_Al_Case_Solid();
	G4SubtractionSolid* LeadCollimator_Pol_Det2BGO_Solid=new G4SubtractionSolid("LeadCollimator_Pol_Det2BGO_Solid",LeadCollimator_Pol_Det2_Plain_Solid,BGO2_AlCase, &RotationDet2_Y, TranslationDet2);
	G4Tubs* FilterHole_Solid= new G4Tubs("FilterHole_Solid", 0, 25*mm, 1000*mm/2, 0. * deg, 360. * deg);
	G4SubtractionSolid* LeadCollimator_Pol_Det2Complete_Solid=new G4SubtractionSolid("LeadCollimator_Pol_Det2Complete_Solid",LeadCollimator_Pol_Det2BGO_Solid,FilterHole_Solid,&RotationDet2_Y,TranslationDet2);

	G4double BGOPol_Distance=-(detectordistancepol + BGOPol->Get_Length()/2)+block_small_x;
	G4RotationMatrix RotationPol_Y;
	RotationPol_Y.rotateY(M_PI*270/180.*rad);
	G4ThreeVector TranslationPol(-17.5*cm +BGOPol_Distance*sin(pol_theta)*cos(pol_phi),BGOPol_Distance*cos(pol_theta),-(-distcollimatortotarget+14.55/2*cm-35*cm)+BGOPol_Distance*sin(pol_theta)*sin(pol_phi));

	G4UnionSolid* BGOPol_AlCase = BGOPol->Get_Al_Case_Solid();
	G4SubtractionSolid* LeadCollimator_PolBGO_Det2Complete_Solid=new G4SubtractionSolid("LeadCollimator_PolBGO_Det2Complete_Solid",LeadCollimator_Pol_Det2Complete_Solid,BGOPol_AlCase, &RotationPol_Y, TranslationPol);
	G4SubtractionSolid* LeadCollimator_Pol_Det2_Solid=new G4SubtractionSolid("LeadCollimator_Pol_Det2_Solid",LeadCollimator_PolBGO_Det2Complete_Solid,FilterHole_Solid,&RotationPol_Y,TranslationPol);


	G4LogicalVolume* LeadCollimator_Pol_Det2_Logical = new G4LogicalVolume(LeadCollimator_Pol_Det2_Solid, Pb, "LeadCollimator_Pol_Det2_Logical", 0, 0, 0);
	LeadCollimator_Pol_Det2_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+G4ThreeVector(17.5*cm ,0,+14.55*0.5*cm-35*cm), LeadCollimator_Pol_Det2_Logical, "LeadCollimator_Pol_Det2", World_Logical, 0, 0);

	//End LeadDetectorCollimator_Pol_Det2-------------------------------------

	// // Beginning LeadDetectorCollimator_Pol_Det2-------------------------------------
	// G4double FilterRadius= 25*mm;
	// G4double X_Box= 15.6*cm;
	// G4double Y_Box= 25.*cm;
	// G4double Z_Box= 14.*cm;

	// G4Tubs* FilterHole_Solid= new G4Tubs("FilterHole_Solid", 0, FilterRadius, 500*mm, 0. * deg, 360. * deg);

	// G4double g2_theta=90.*deg;
	// G4double g2_phi= (130-180)*deg;

	// G4double pol_theta=90.*deg;
	// G4double pol_phi=180.*deg;

	// G4double BGODet2_Distance=-(detectordistance2 + BGO2->Get_Length()/2)+block_small_x;
	// G4RotationMatrix* RotationDet2_Y= new G4RotationMatrix();
	// RotationDet2_Y->rotateY(g2_phi);


	// G4double BGOPol_Distance=-(detectordistancepol + BGOPol->Get_Length()/2)+block_small_x;
	// G4double BGO2_Distance=-(detectordistance2 + BGO2->Get_Length()/2)+10*cm;
	// G4ThreeVector TranslationDet2((FilterRadius+1.8*cm+Z_Box*cos(50*deg)-(+Z_Box-Z_Box*sin(50*deg))*tan(50*deg)),0,0);
	// G4ThreeVector TranslationPol(sin(pol_theta)*cos(pol_phi),cos(pol_theta),sin(pol_theta)*sin(pol_phi));

	// G4Box* Pol_Box_Solid = new G4Box("Pol_Box_Solid",X_Box,Y_Box, Z_Box);
	// G4UnionSolid* BGOPol_AlCase = BGOPol->Get_Al_Case_Solid();

	// G4Box* Det2_Box_Solid = new G4Box("Det2_Box_Solid",20*cm,Y_Box, Z_Box);
	// G4UnionSolid* BGODet2_AlCase = BGO2->Get_Al_Case_Solid();

	// G4UnionSolid* LeadCollimator_Plain_Solid = new G4UnionSolid("Lead_Collimator_Plain_Solid",Pol_Box_Solid,Det2_Box_Solid,RotationDet2_Y,TranslationDet2);
	// G4SubtractionSolid* LeadCollimator_PolFilter_Solid=new G4SubtractionSolid("LeadCollimator_PolFilter_Solid",LeadCollimator_Plain_Solid,FilterHole_Solid, 0, G4ThreeVector());
	// G4SubtractionSolid* LeadCollimator_Det2Filter_Solid=new G4SubtractionSolid("LeadCollimator_Det2Filter_Solid",LeadCollimator_PolFilter_Solid,FilterHole_Solid,RotationDet2_Y,TranslationDet2);
	// G4SubtractionSolid* LeadCollimator_PolBGO_Solid=new G4SubtractionSolid("LeadCollimator_Det2BGO_Det2Complete_Solid",LeadCollimator_Det2Filter_Solid,BGOPol_AlCase, 0, G4ThreeVector());
	// G4SubtractionSolid* LeadCollimator_Det2_Pol_Solid=new G4SubtractionSolid("LeadCollimator_Det2_Det2_Solid",LeadCollimator_PolBGO_Solid,BGODet2_AlCase,RotationDet2_Y,TranslationDet2);

	// G4ThreeVector TranslationLeadCollimator1_Detector(0,0,BGO2_Distance-block_small_x*0.5);
	// G4Box* LeadCollimator_Detector_Block_Solid = new G4Box("LeadCollimator_Detector_Block_Solid",1*m,1.1*Y_Box,block_small_x*0.5);
	// G4SubtractionSolid* LeadCollimator_Cut1_Solid= new G4SubtractionSolid("LeadCollimator_Cut1_Solid",LeadCollimator_Det2_Pol_Solid,LeadCollimator_Detector_Block_Solid,0,TranslationLeadCollimator1_Detector);

	// G4ThreeVector TranslationLeadCollimator_Detector(34*cm,0,-Z_Box-block_small_x);
	// G4SubtractionSolid* LeadCollimator_Complete_Solid= new G4SubtractionSolid("LeadCollimator_Complete_Solid",LeadCollimator_Cut1_Solid,LeadCollimator_Detector_Block_Solid,0,TranslationLeadCollimator_Detector);

	// G4LogicalVolume* LeadCollimator_Det2_Pol_Logical = new G4LogicalVolume(LeadCollimator_PolFilter_Solid, Pb, "LeadCollimator_Det2_Pol_Logical", 0, 0, 0);
	// LeadCollimator_Det2_Pol_Logical->SetVisAttributes(yellow);
	// G4RotationMatrix* RotationDetectorCollimator = new G4RotationMatrix();
	// RotationDetectorCollimator->rotateY(-0*deg);
	// new G4PVPlacement(Rotlocal_coordinates+ationDetectorCollimator, G4ThreeVector(0,0,Z_Box), LeadCollimator_Det2_Pol_Logical, "LeadCollimator_Det2_Pol", World_Logical, 0, 0);
	// // End LeadDetectorCollimator_Pol_Det2-------------------------------------

	
	//Beginning LeadFrontDet2-------------------------------------
	vector <G4TwoVector> vertices2{
		G4TwoVector(-7.5*cm,25*cm),
		G4TwoVector(-7.5*cm,-25*cm),
		G4TwoVector(0.*cm,-25*cm),
		G4TwoVector(0.*cm,25*cm),
		G4TwoVector(0.*cm,25*cm),
		G4TwoVector(0.*cm,-25*cm),
		G4TwoVector(0.*cm,-25*cm),
		G4TwoVector(0.*cm,25*cm)};
	G4GenericTrap* LeadFrontDet2_Triangle = new G4GenericTrap("LeadFrontDet2_Triangle",(8.938/2)*cm,vertices2);
	G4ThreeVector TranslationLeadFrontDet2(-(+12.5*cm) +BGO2_Distance*sin(g2_theta)*cos(g2_phi),BGO2_Distance*cos(g2_theta),-(-distcollimatortotarget+2.262*cm+(8.938/2)*cm)+BGO2_Distance*sin(g2_theta)*sin(g2_phi));
	G4SubtractionSolid* LeadFrontDet2Hole_Solid = new G4SubtractionSolid("LeadFrontDet2Hole_Solid",LeadFrontDet2_Triangle,FilterHole_Solid, &RotationDet2_Y, TranslationLeadFrontDet2);
	G4LogicalVolume* LeadFrontDet2Hole_Logical = new G4LogicalVolume(LeadFrontDet2Hole_Solid, Pb, "LeadFrontDet2Hole_Logical", 0, 0, 0);
	LeadFrontDet2Hole_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(+12.5*cm , 0,2.262*cm+(8.938/2)*cm ), LeadFrontDet2Hole_Logical, "LeadFrontDet2Hole", World_Logical, 0, 0);
	G4Box* LeadCollimatorConector_Solid = new G4Box("LeadCollimatorConector_Solid",3.75*cm,block_y*0.5,block_z *0.5*4+1.131*cm);
	G4LogicalVolume* LeadCollimatorConector_Logical = new G4LogicalVolume(LeadCollimatorConector_Solid, Pb, "LeadCollimatorConector_Logical", 0, 0, 0);
	LeadCollimatorConector_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(8.75*cm , 0,-block_z *0.5*4+1.131*cm),LeadCollimatorConector_Logical,"LeadCollimatorConector",World_Logical,0,0); 
	//End LeadFrontDet2-------------------------------------

	//Begin LeadDownstream Pol
	G4double LeadDownstream_Pol_Z=34*cm;

	G4Box* LeadDownstream_Pol_Solid = new G4Box("LeadDownstream_Pol_Solid",14.*cm,25*cm,LeadDownstream_Pol_Z);
	G4LogicalVolume* LeadDownstream_Pol_Logical = new G4LogicalVolume(LeadDownstream_Pol_Solid, Pb, "LeadDownstream_Pol_Logical", 0, 0, 0);
	LeadDownstream_Pol_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(26.5*cm , 0,120*cm-20*cm-LeadDownstream_Pol_Z),LeadDownstream_Pol_Logical,"LeadDownstream_Pol",World_Logical,0,0); 
	


	//End LeadDownstream Pol


	//Beginning Det1House-------------------------------------
	G4double BGO1_Distance=(-(detectordistance1 + BGO1->Get_Length()*0.5));
	G4Box* LeadDet1House_Solid= new G4Box("LeadDet1House_Solid", 15*cm,25*cm,13.938*cm);
	//Z-Dimension from distcollimatortotarget and Collimator_Lead_Block

	G4double g1_theta=90.*deg;
	G4double g1_phi= 5*deg;

	G4RotationMatrix* RotationDet1_Y= new G4RotationMatrix();
	RotationDet1_Y->rotateY(g1_phi+90*deg);
	G4ThreeVector TranslationDet1(BGO1_Distance*sin(g1_theta)*cos(g1_phi)+10*cm*cos(g1_phi),BGO1_Distance*cos(g1_theta),distcollimatortotarget+BGO1_Distance*sin(g1_theta)*sin(g1_phi)+10*cm*sin(g1_phi));

	

	G4SubtractionSolid* Det1_Box_Filter_Solid = new G4SubtractionSolid("Det1_Box_Filter_Solid",LeadDet1House_Solid,FilterHole_Solid,RotationDet1_Y,G4ThreeVector());
	G4SubtractionSolid* Det1_Complete_Solid = new G4SubtractionSolid("Det1_Complete_Solid",Det1_Box_Filter_Solid,BGO1->Get_Al_Case_Solid(),RotationDet1_Y,G4ThreeVector());
	G4LogicalVolume* Det1_Complete_Logical = new G4LogicalVolume(Det1_Complete_Solid, Pb, "Det1_Complete_Logical", 0, 0, 0);
	Det1_Complete_Logical->SetVisAttributes(grey);
	
	new G4PVPlacement(0, local_coordinates+TranslationDet1,Det1_Complete_Logical,"LeadDet1House",World_Logical,0,0); 

	G4double UpstreamDet1_X= 77.5*cm;//180*cm (Ceiling)-0.5*block_small_x-10*cm(RightWall)
	G4double UpstreamDet1_Z= block_z*4*0.5+10*cm*sin(g1_phi)*0.5;

	G4Box* UpstreamDet1_Solid = new G4Box("UpstreamDet1_Solid",UpstreamDet1_X,25*cm,UpstreamDet1_Z);
	G4LogicalVolume* UpstreamDet1_Logical = new G4LogicalVolume(UpstreamDet1_Solid, Pb, "UpstreamDet1_Logical", 0, 0, 0);
	UpstreamDet1_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(-block_small_x*0.5-UpstreamDet1_X,0,-UpstreamDet1_Z+10*cm*sin(g1_phi)*0.5),UpstreamDet1_Logical,"UpstreamDet1",World_Logical,0,0); 

	G4Box* DownstreamDet1_Solid = new G4Box("DownstreamDet1_Solid",UpstreamDet1_X,25*cm,block_z *0.5*4+1.131*cm);
	G4LogicalVolume* DownstreamDet1_Logical = new G4LogicalVolume(DownstreamDet1_Solid, Pb, "DownstreamDet1_Logical", 0, 0, 0);
	DownstreamDet1_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, local_coordinates+G4ThreeVector(detectordistance1-0.5*block_small_x-UpstreamDet1_X+BGO1_Distance*sin(g1_theta)*cos(g1_phi)+10*cm*cos(g1_phi),0,UpstreamDet1_Z+10*cm*sin(g1_phi)+13.938*cm*2),DownstreamDet1_Logical,"DownstreamDet1",World_Logical,0,0); 

	//End Det1House-------------------------------------
}