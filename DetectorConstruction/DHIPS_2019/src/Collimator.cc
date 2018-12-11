
#include <iostream>
#include <sstream>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"

#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "Vacuum.hh"

#include "Collimator.hh"
using std::cout;
using std::endl;
using std::stringstream;

// Collimator
Collimator::Collimator(G4LogicalVolume *World_Log):World_Logical(World_Log){}


void Collimator::Construct(G4ThreeVector global_coordinates)
{

	X_begin+=global_coordinates.getX();
	Y_begin+=global_coordinates.getY();
	Z_begin+=global_coordinates.getZ();

	X_end+=global_coordinates.getX();
	Y_end+=global_coordinates.getY();
	Z_end+=global_coordinates.getZ();

	G4Colour yellow  (1.0, 1.0, 0.0) ;
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour light_orange(1.0, 0.82, 0.36);

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

	// Variable for collimator hole radius
	G4double hole_radius;
	G4Tubs *hole;
	G4SubtractionSolid *Collimator_block; 
	G4LogicalVolume *Collimator_blocks_Logical[NBlocks];
	G4VisAttributes *blockvis;


	
	//*************************************************
	// Mother volume
	//*************************************************
	
	G4Box *Collimator_Mother_Solid = new G4Box("Collimator_Solid", block_x*0.5, block_y*0.5, block_z *0.5*NBlocks);

	G4LogicalVolume *Collimator_Mother_Logical = new G4LogicalVolume(Collimator_Mother_Solid, air, "Collimator_Mother_Logical");
	Collimator_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	Collimator_Mother_Logical->SetVisAttributes(yellow);
	
	G4Box *big_block = new G4Box("blockwithouthole", block_x *0.5, block_y *0.5, block_z *0.5);
	G4Box *small_block = new G4Box("blockwithouthole", block_small_x *0.5, block_y *0.5, block_z *0.5); //next to target

	G4Box *lead_box= new G4Box("Collomator_lead",block_small_x*0.5,block_y*0.5,block_z *0.5*4);
	G4LogicalVolume *lead_box_Logical = new G4LogicalVolume(lead_box, Pb, "Collimator_Lead_Block_Logical", 0, 0, 0);
	blockvis = new G4VisAttributes(grey);
	lead_box_Logical->SetVisAttributes(blockvis);
		
	
	// _________________________ Block Loop _________________________
	for (G4int i=0; i<NBlocks;++i)
	{
		 hole_radius = (colholeradius_min+(colholeradius_max-colholeradius_min)/NBlocks*i);
		 hole = new G4Tubs(("hole"+std::to_string(i)).c_str(), 0., hole_radius, block_z*0.5, 0., twopi);
		if (i<4)
		{
			Collimator_block = new G4SubtractionSolid(("Collimator_Block"+std::to_string(i)).c_str(),small_block, hole);
		}
		else
		{
			Collimator_block = new G4SubtractionSolid( ("Collimator_Block"+std::to_string(i)).c_str(),big_block, hole);
		}
	  	Collimator_blocks_Logical[i] = new G4LogicalVolume(Collimator_block, Cu, ("Collimator_Block"+std::to_string(i)).c_str(), 0, 0, 0);
		blockvis = new G4VisAttributes(light_orange);
		Collimator_blocks_Logical[i]->SetVisAttributes(blockvis);
	
		new G4PVPlacement(0, G4ThreeVector(0., 0., (NBlocks*0.5-i-0.5)* block_z), Collimator_blocks_Logical[i],("Collimator_Block"+std::to_string(i)).c_str(),Collimator_Mother_Logical, 0, 0);
		// delete Collimator_block;
	}
	new G4PVPlacement(0, G4ThreeVector(block_small_x, 0., (+0.5*5+0.5)* block_z), lead_box_Logical,"Collimator_Lead_Block",Collimator_Mother_Logical, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(-block_small_x, 0., (+0.5*5+0.5)* block_z), lead_box_Logical,"Collimator_Lead_Block",Collimator_Mother_Logical, 0, 0);
		

	new G4PVPlacement(0, global_coordinates, Collimator_Mother_Logical, "Collimator",World_Logical, 0, 0);




}