/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Setup for runs DHIPS Beamtime2019
*/

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"
Materials *materials = Materials::Instance();

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Sensitive Detectors
#include "G4SDManager.hh"
#include "EnergyDepositionSD.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

//Detectors
#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "BGO.hh"

#include "RadiatorTarget.hh"
#include "BeamPipe_Upstream.hh"
#include "BeamPipe_Downstream.hh"
#include "Collimator.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4TwoVector.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Sphere.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
#include <vector>
#include <iostream>

using std::stringstream;
using std::vector;
using std::cout;
using std::endl;

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct()
{

	G4Colour white(1.0, 1.0, 1.0);
	G4Colour grey(0.5, 0.5, 0.5);
	G4Colour black(0.0, 0.0, 0.0);
	G4Colour red(1.0, 0.0, 0.0);
	G4Colour green(0.0, 1.0, 0.0);
	G4Colour blue(0.0, 0.0, 1.0);
	G4Colour cyan(0.0, 1.0, 1.0);
	G4Colour magenta(1.0, 0.0, 1.0);
	G4Colour yellow(1.0, 1.0, 0.0);
	G4Colour orange(1.0, 0.5, 0.0);
	G4Colour light_orange(1.0, 0.82, 0.36);

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
	/************************* World volume *****************/

	/***************** WORLD *****************/

	World_x = 6000. * mm;
	World_y = 6000. * mm;
	World_z = 6000. * mm;

	G4Box *World_dim =
	    new G4Box("World_Solid", World_x * 0.5, World_y * 0.5, World_z * 0.5);

	G4LogicalVolume *World_Logical =
	    new G4LogicalVolume(World_dim, air, "World_Logical", 0, 0, 0);

	//World_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
    	G4VisAttributes* world_vis = new G4VisAttributes(true, red);
    	world_vis->SetForceWireframe(true);

	World_Logical->SetVisAttributes(world_vis);

	G4VPhysicalVolume *World_Physical =
	    new G4PVPlacement(0, G4ThreeVector(), World_Logical, "World", 0, false, 0);

	/***************************************************/
	/***************** INITIALIZATIONS *****************/
	/***************************************************/

	BeamPipe_Upstream BeamPipe_Upstream(World_Logical);
	RadiatorTarget RadiatorTarget(World_Logical);
	Collimator Collimator(World_Logical);
	BeamPipe_Downstream BeamPipe_Downstream(World_Logical);


	/***************************************************/
	/*****************  CONSTRUCTION  *****************/
	/***************************************************/

	BeamPipe_Upstream.Construct(G4ThreeVector(0,0,-2500*mm),0.1);


	RadiatorTarget.Construct(G4ThreeVector(0,0,BeamPipe_Upstream.GetEnd().getZ()+(200+15*0.5)*mm),"Radiator1","Au",3*mm,"Air",0.); 
	RadiatorTarget.Construct(G4ThreeVector(0,0,RadiatorTarget.GetEnd().getZ()+30*mm),"Radiator2","Au",3*mm,"Air",0.);
	Collimator.Construct(G4ThreeVector(0,0,RadiatorTarget.GetEnd().getZ()+0.5*Collimator.GetLength().getZ()+50*mm));
	BeamPipe_Downstream.Construct(G4ThreeVector(0.,0.,0.),0.1);
	// print_info();
	return World_Physical;

}


// 	 G4double disttoendblock32= -40*cm+trans_x ;
	 
// 	 //detectordistance1 should be divided by cosine. For 5 deg the difference is less than 1 in 100000
// 	 //G4double detectordistance1 = abs(disttoendblock32)-(192)*mm;//this is the length of bgo
// 	 G4double detectordistance1 = 225.*mm;//this is the length of bgo
// 	 G4double detectordistance2 = 225.*mm;
// 	 G4double poldistance = 225.*mm;
	 


// /************************* Beam pipe Upstream *****************/
// G4double beam_pipe_collimator_backside_wall = distcollimatortotarget-50.*mm;

// 	G4double beamPipe_NRF_Window_Thickness = 3*mm; // Estimated
// 	G4double beamPipe_NRF_Inner_Radius = 0.5*54.*mm; // Estimated
// 	G4double beamPipe_NRF_Outer_Radius = 0.5*60.*mm;
// 	G4double beamPipe_NRF_Lenght       = (810.-180.)*mm;

// 	G4double Chamber_Outer_Length = 180.*mm;
// 	G4double Chamber_Inner_Length = 150.*mm;

// ////////////////////////
// 	// BeamTube
// ////////////////////////// 
// 	G4Tubs *beamPipe_NRF_Solid = new G4Tubs("beamPipe_NRF_Solid", beamPipe_Inner_Radius, beamPipe_NRF_Outer_Radius, beamPipe_NRF_Lenght*0.5, 0., twopi);

// 	G4LogicalVolume *beamPipe_NRF_Logical = new G4LogicalVolume(beamPipe_NRF_Solid, PE, "beamPipe_NRF_Logical");
// 	beamPipe_NRF_Logical->SetVisAttributes(green);

// 	// new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght*0.5), beamPipe_NRF_Logical, "beamPipe_NRF_Logical", world_log, false, 0);

// ////////////////////////
// 	// Vacuum in Tube
// ////////////////////////// 
// 	G4Tubs *beamPipe_Vacuum_NRF_Solid = new G4Tubs("beamPipe_Vacuum_NRF_Solid", 0,beamPipe_NRF_Inner_Radius, beamPipe_NRF_Lenght*0.5, 0., twopi);

// 	G4LogicalVolume *beamPipe_Vacuum_NRF_Logical = new G4LogicalVolume(beamPipe_Vacuum_NRF_Solid, VACUUM, "beamPipe_Vacuum_NRF_Logical");
// 	beamPipe_Vacuum_NRF_Logical->SetVisAttributes(white);

// 	// new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght*0.5), beamPipe_Vacuum_NRF_Logical, "beamPipe_Vacuum_NRF_Logical", beamPipe_NRF_Logical, false, 0);

// ////////////////////////
// 	// Lid of Tube
// ////////////////////////// 
// 	G4Tubs *beamPipe_NRF_Lid_Solid = new G4Tubs("beamPipe_NRF_Lid_Solid", 0., beamPipe_NRF_Inner_Radius, beamPipe_NRF_Window_Thickness*0.5, 0., twopi);

// 	G4LogicalVolume *beamPipe_NRF_Lid_Logical = new G4LogicalVolume(beamPipe_NRF_Lid_Solid, PE, "beamPipe_NRF_Lid_Logical");
// 	beamPipe_NRF_Lid_Logical->SetVisAttributes(green);

// 	// new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 +trans_z+beamPipe_NRF_Window_Thickness*0.5), beamPipe_NRF_Lid_Logical, "beamPipe_NRF_Lid_Logical", world_log, false, 0);

// ////////////////////////
// 	// Chamber 
// ////////////////////////// 
// 	G4Box *Chamber_Block_Solid=new G4Box("Chamber_Block_Solid",Chamber_Outer_Length*0.5,Chamber_Outer_Length*0.5, Chamber_Outer_Length*0.5);
//   	G4LogicalVolume* Chamber_Block_Logical = new G4LogicalVolume(Chamber_Block_Solid, VACUUM, "Chamber_Block_Logical", 0, 0, 0);
// 	Chamber_Block_Logical->SetVisAttributes(green);

// 	// new G4PVPlacement(0, G4ThreeVector(0 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), Chamber_Block_Logical, "Chamber_Block", world_log, 0, 0);

// //////////////////////////
// 	// Vacuum in Chamber
// ////////////////////////// 
// 	G4Box *Chamber_Vacuum_Block_Solid=new G4Box("Chamber_Vacuum_Block_Solid",Chamber_Inner_Length*0.5,Chamber_Inner_Length*0.5, Chamber_Inner_Length*0.5);
//   	G4LogicalVolume* Chamber_Vacuum_Block_Logical = new G4LogicalVolume(Chamber_Vacuum_Block_Solid, VACUUM, "Chamber_Vacuum_Block_Logical", 0, 0, 0);
// 	Chamber_Vacuum_Block_Logical->SetVisAttributes(white);

// 	// new G4PVPlacement(0, G4ThreeVector(0 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), Chamber_Vacuum_Block_Logical, "Chamber_Vacuum_Block", world_log, 0, 0);

// ////////////////////////
// 	// Deuterium Target
// ////////////////////////// 
// 	G4double D_Thickness = 10*mm;
// 	G4double D_Radius    = 20*mm;

// 	G4Tubs *D_Target_Solid= new G4Tubs("D_Target", 0, D_Radius, D_Thickness*0.5, 0., twopi);

// 	// G4LogicalVolume *D_Target_Logical = new G4LogicalVolume(D_Target_Solid, mat->Get_D_Polystyrol(), "D_Target_Logical");
// 	G4LogicalVolume *D_Target_Logical = new G4LogicalVolume(D_Target_Solid, Pb, "D_Target_Logical");
// 	D_Target_Logical->SetVisAttributes(red);

// 	G4RotationMatrix* D_Rotate=new G4RotationMatrix();
// 	D_Rotate->rotateX(-45*deg);
// 	D_Rotate->rotateY(45*deg);

// 	// new G4PVPlacement(D_Rotate, G4ThreeVector(0 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), D_Target_Logical, "D_Target", Chamber_Vacuum_Block_Logical, 0, 0);

// //////////////////////////
// // 
// // Si-Detector
// //
// //////////////////////////

// 	G4double Si_Radius = 13.81*mm; //coresponding to 600mm^2 active area
// 	G4double Si_Thickness=10*mm;

// 	G4double Si_Radius_Shield = 36.1*mm; 
// 	G4double Si_Thickness_Shield=36*mm;

// 	G4Tubs* Si_Radius_Shield_Solid=new G4Tubs("Si_Radius_Shield_Solid", 0., Si_Radius_Shield, Si_Thickness_Shield*0.5, 0., twopi);
// 	G4LogicalVolume *Si_Radius_Shield_Logical = new G4LogicalVolume(Si_Radius_Shield_Solid, Al, "Si_Radius_Shield_Logical");
// 	Si_Radius_Shield_Logical->SetVisAttributes(blue);

// 	G4RotationMatrix* Si_Rotate=new G4RotationMatrix();
// 	Si_Rotate->rotateY(90*deg);

// 	// new G4PVPlacement(Si_Rotate, G4ThreeVector(40.5*mm+Si_Thickness_Shield*0.5 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), Si_Radius_Shield_Logical, "Si_Radius_Shield", world_log, 0, 0);

// 	G4Tubs* Si_Detector_Solid=new G4Tubs("Si_Detector_Solid", 0., Si_Radius, Si_Thickness*0.5, 0., twopi);
// 	G4LogicalVolume *Si_Detector_Logical = new G4LogicalVolume(Si_Detector_Solid, VACUUM, "Si_Detector_Logical");
// 	Si_Detector_Logical->SetVisAttributes(orange);

// 	// new G4PVPlacement(Si_Rotate, G4ThreeVector(40.5*mm+Si_Thickness*0.5 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), Si_Detector_Logical, "Si_Detector", world_log, 0, 0);


// /*Pipe*/	    	new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght*0.5), beamPipe_NRF_Logical, "beamPipe_NRF_Logical", world_log, false, 0);
// /*Pipe Vacuum*/		new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght*0.5), beamPipe_Vacuum_NRF_Logical, "beamPipe_Vacuum_NRF_Logical", world_log, false, 0);
// /*Lid*/     		new G4PVPlacement(0, G4ThreeVector(0., 0., -distcollimatortotarget+50 +trans_z+beamPipe_NRF_Window_Thickness*0.5), beamPipe_NRF_Lid_Logical, "beamPipe_NRF_Lid_Logical", world_log, false, 0);
// /*Chamber*/ 		new G4PVPlacement(0, G4ThreeVector(0 , 0, -distcollimatortotarget+50 + trans_z + beamPipe_NRF_Lenght+Chamber_Inner_Length*0.5), Chamber_Block_Logical, "Chamber_Block", world_log, 0, 0);
// /*Chamber Vaccum*/	new G4PVPlacement(0, G4ThreeVector(0 , 0, 0), Chamber_Vacuum_Block_Logical, "Chamber_Vacuum_Block", Chamber_Block_Logical, 0, 0);
// /*D_Target*/		new G4PVPlacement(D_Rotate, G4ThreeVector(0 , 0, 0), D_Target_Logical, "D_Target", Chamber_Vacuum_Block_Logical, 0, 0);
// /*Si Shield*/		new G4PVPlacement(Si_Rotate, G4ThreeVector(40.5*mm+Si_Thickness_Shield*0.5 , 0, 0), Si_Radius_Shield_Logical, "Si_Radius_Shield", Chamber_Vacuum_Block_Logical, 0, 0);
// /*Si*/				new G4PVPlacement(Si_Rotate, G4ThreeVector(40.5*mm+Si_Thickness*0.5 , 0, 0), Si_Detector_Logical, "Si_Detector", Chamber_Vacuum_Block_Logical, 0, 0);

// /************************* Radiator targets *****************/

// 	// Distance from the entrance of the collimator to the side of the radiator target holder that faces the collimator
	
// 	G4double radiator_holder1_to_collimator = 150.*mm;
// 	G4double radiator_holder2_to_collimator = 120.*mm;

// 	//G4double collimator_entrance_z = -distcollimatortotarget + trans_z - 11.*block_z;

// 	RadiatorTarget *radiatorTarget1 = new RadiatorTarget(3*mm, "Pb", "Target_1", 0.*mm, "AIR");
// 	G4LogicalVolume *radiator_Holder1_Logical = radiatorTarget1->Get_Logical();

// 	new G4PVPlacement(0, G4ThreeVector(0., radiatorTarget1->Get_Window_Position(), -distcollimatortotarget + trans_z -11.*block_z - radiator_holder1_to_collimator - radiatorTarget1->Get_Z()), radiator_Holder1_Logical, "radiator_Holder1", world_log, false, 0);

// 	RadiatorTarget *radiatorTarget2 = new RadiatorTarget(3*mm, "Pb", "Target_2", 14.*mm, "Pb");
// 	G4LogicalVolume *radiator_Holder2_Logical = radiatorTarget2->Get_Logical();

// 	new G4PVPlacement(0, G4ThreeVector(0., radiatorTarget2->Get_Window_Position(), -distcollimatortotarget + trans_z -11.*block_z - radiator_holder2_to_collimator - radiatorTarget2->Get_Z()), radiator_Holder2_Logical, "radiator_Holder2", world_log, false, 0);

// /************************* Target holder *****************/

// 	G4double nrf_Target_Holder_Pole_Radius = 3.*mm;
// 	G4double nrf_Target_Holder_Pole_Length = 104.*mm;

// 	G4double nrf_Target_Holder_Inner_Radius = 35.*mm;
// 	G4double nrf_Target_Holder_Outer_Radius = 39.*mm;
// 	G4double nrf_Target_Holder_Thickness = 3.*mm;
// 	G4double nrf_Target_Holder_Angle_Min1 = 0.*deg;
// 	G4double nrf_Target_Holder_Angle_Max1 = 137.*deg;
// 	G4double nrf_Target_Holder_Angle_Min2 = 0.*deg;
// 	G4double nrf_Target_Holder_Angle_Max2 = 137.*deg;

// 	G4Tubs *nrf_Target_Holder_Solid1 = new G4Tubs("nrf_Target_Holder_Solid1", nrf_Target_Holder_Inner_Radius, nrf_Target_Holder_Outer_Radius, nrf_Target_Holder_Thickness*0.5, nrf_Target_Holder_Angle_Min1, nrf_Target_Holder_Angle_Max1);

// 	G4LogicalVolume *nrf_Target_Holder_Logical1 = new G4LogicalVolume(nrf_Target_Holder_Solid1, Al, "nrf_Target_Holder_Logical1");
// 	nrf_Target_Holder_Logical1->SetVisAttributes(grey);

// 	G4RotationMatrix *rotNRFTargetHolder1 = new G4RotationMatrix();
// 	rotNRFTargetHolder1->rotateY(180.*deg);

// 	// new G4PVPlacement(rotNRFTargetHolder1, G4ThreeVector(0., 0., 0.), nrf_Target_Holder_Logical1, "nrf_Target_Holder1", world_log, false, 0);

// 	G4Tubs *nrf_Target_Holder_Solid2 = new G4Tubs("nrf_Target_Holder_Solid2", nrf_Target_Holder_Inner_Radius, nrf_Target_Holder_Outer_Radius, nrf_Target_Holder_Thickness*0.5, nrf_Target_Holder_Angle_Min2, nrf_Target_Holder_Angle_Max2);

// 	G4LogicalVolume *nrf_Target_Holder_Logical2 = new G4LogicalVolume(nrf_Target_Holder_Solid2, Al, "nrf_Target_Holder_Logical2");
// 	nrf_Target_Holder_Logical2->SetVisAttributes(grey);

// 	G4RotationMatrix *rotNRFTargetHolder2 = new G4RotationMatrix();
// 	rotNRFTargetHolder2->rotateX(180.*deg);
// 	rotNRFTargetHolder2->rotateY(180.*deg);

// 	// new G4PVPlacement(rotNRFTargetHolder2, G4ThreeVector(0., 0., 0.), nrf_Target_Holder_Logical2, "nrf_Target_Holder2", world_log, false, 0);

// 	G4RotationMatrix *xRotPole = new G4RotationMatrix();
// 	xRotPole->rotateX(90.*deg);

// 	G4Tubs *nrf_Target_Holder_Pole_Solid = new G4Tubs("nrf_Target_Holder_Pole_Solid", 0., nrf_Target_Holder_Pole_Radius, nrf_Target_Holder_Pole_Length*0.5, 0., twopi);

// 	G4LogicalVolume *nrf_Target_Holder_Pole_Logical = new G4LogicalVolume(nrf_Target_Holder_Pole_Solid, Al, "nrf_Target_Holder_Pole_Logical");
// 	nrf_Target_Holder_Pole_Logical->SetVisAttributes(grey);

// 	// new G4PVPlacement(xRotPole, G4ThreeVector(0., -nrf_Target_Holder_Outer_Radius - nrf_Target_Holder_Pole_Length*0.5, 0.), nrf_Target_Holder_Pole_Logical, "nrf_Target_Holder_Pole", world_log, false, 0);

// /************************* Red Boxes for Orientation *****************/

// // These boxes are purely for orientation and should be commented out
// // before running a simulation

// //Block around target
// //Beginning Block 026-------------------------------------
// //G4LogicalVolume *block26_Logical;
// //G4Box *block26; 
// //G4VisAttributes *block26vis;

// //block26 =
// 	    //new G4Box("block26",1*cm,1*cm, 1*cm);

//   //block26_Logical =
// 	    //new G4LogicalVolume(block26, Fe, "block26_Logical", 0, 0, 0);
// 	//block26vis = new G4VisAttributes(red);
// 	//block26_Logical->SetVisAttributes(block26vis);

// 	//new G4PVPlacement(0, G4ThreeVector(0, 0,0 ), block26_Logical, "block", world_log, 0, 0);
// //End Block 026---------------------------------------

// //Orientation Sphere
// //G4LogicalVolume *Sphere_Logical;
// //G4Sphere *Sphere; 
// //G4VisAttributes *Spherevis;

// //Sphere =
// //new G4Sphere("Sphere",
//               //0.*cm,
//              //12.5*cm,
//               //0*deg,
//              //360*deg,
//              //0*deg,
//        //180*deg ) ;
             
//              //Sphere_Logical =
// 	    //new G4LogicalVolume(Sphere, Air, "Sphere_Logical", 0, 0, 0);
// 	//Spherevis = new G4VisAttributes(red);
// 	//Sphere_Logical->SetVisAttributes(Spherevis);

// 	//new G4PVPlacement(0, G4ThreeVector(0, 0,0 ), Sphere_Logical, 
// 	//"Sphere", world_log, 0, 0);


// /************************* Important Paramters*****************/

// //Angles and Distances for Detector1
//  Germanium1_TUD* germaniumDetector1=new Germanium1_TUD("Germanium1_TUD");

// 	G4String bgo1_Name = "BGO1";
//   BGO* bgo1 = new BGO(bgo1_Name); 

//   G4double g1_Distance = -(detectordistance1 + germaniumDetector1->Get_Length()*0.5 + bgo1->Get_Length()) + bgo1->Get_Max_Penetration_Depth();
  
//   G4double g1_theta=90.*deg;
//   G4double g1_phi=5.*deg;
//   G4double g1_Angle=g1_phi-90.*deg;
  
  
//    G4double bgo1_Distance = -(detectordistance1 + bgo1->Get_Length()/2);
//   G4ThreeVector bgo1_Position = G4ThreeVector
//   (bgo1_Distance*sin(g1_theta)*cos(g1_phi), bgo1_Distance*cos(g1_theta), bgo1_Distance*sin(g1_theta)*sin(g1_phi));
  
//   G4double copperfilter1_Distance = -(-17*cm+detectordistance1 + germaniumDetector1->Get_Length());
//   G4double leadfilter1_Distance = -(-18*cm+detectordistance1 + germaniumDetector1->Get_Length());
// //Angles and Distances for Detector2
//  Germanium2_TUD* germaniumDetector2=new Germanium2_TUD("Germanium2_TUD");
  
//  	G4String bgo2_Name = "BGO2";
//     BGO* bgo2 = new BGO(bgo2_Name);

//   //G4double g2_Distance = -(detectordistance2 + germaniumDetector2->Get_Length());
//   G4double g2_Distance = -(detectordistance2 + germaniumDetector2->Get_Length()*0.5 + bgo2->Get_Length()) + bgo2->Get_Max_Penetration_Depth();
//   G4double g2_theta=90.*deg;
//   G4double g2_phi= 140.*deg;
//   G4double g2_Angle= g2_phi-90*deg;
  
  
//   G4double bgo2_Distance = -(detectordistance2 + bgo2->Get_Length()/2);
//   G4ThreeVector bgo2_Position = G4ThreeVector
//   (bgo2_Distance*sin(g2_theta)*cos(g2_phi), bgo2_Distance*cos(g2_theta), bgo2_Distance*sin(g2_theta)*sin(g2_phi));



// G4double copperfilter2_Distance = -(-17*cm+detectordistance1 + germaniumDetector1->Get_Length());
// G4double leadfilter2_Distance = -(-18*cm+detectordistance1 + germaniumDetector1->Get_Length());
// //Angles and Distances for Polarimeter
//   Polarimeter_TUD* polarimeterDetector=new Polarimeter_TUD("Polarimeter_TUD");

// 	G4String bgop_Name = "BGOP";
// BGO* bgop = new BGO(bgop_Name);

//   G4double pol_Distance = -(poldistance + polarimeterDetector->Get_Length()*0.5 + bgop->Get_Length()) + bgop->Get_Max_Penetration_Depth();
  
//   G4double pol_theta=90.*deg;
//   G4double pol_phi=180.*deg;
//   G4double pol_Angle=pol_phi-90.*deg;

  
//   G4double bgop_Distance = -(poldistance + bgop->Get_Length()/2);
//   G4ThreeVector bgop_Position = G4ThreeVector
//   (bgop_Distance*sin(pol_theta)*cos(pol_phi), bgop_Distance*cos(pol_theta), bgop_Distance*sin(pol_theta)*sin(pol_phi));


// G4double copperfilterpol_Distance = -(-17*cm+detectordistance1 + germaniumDetector1->Get_Length());
// G4double leadfilterpol_Distance = -(-18*cm+detectordistance1 + germaniumDetector1->Get_Length());

// /************************* Iron Shielding *****************/
// // Iron Shielding around the Copper Collimator
// // 20 cm thick on the sides. 10cm thick on top. 40 cm thick at the bottom
// // Length unclear. Assumption: All iron until it reaches 9.5cm before the target chamber.
// // Box 21,23 are the Boxes on the left and rigth side and they are reaching 
// // all the way to top and bottom.
// // Box 22,24 are the Boxes at the top and at the bottom.

// // The G4Box needs half the length of a side as an input argument!


// // Beginning Block 21-----------------------------------

// G4LogicalVolume *block21_Logical;
// G4Box *block21; 
// G4VisAttributes *block21vis;

// block21 =
// 	    new G4Box("block21",10*cm,25*cm, ((-280 * mm)+10 * block_z )*0.5);

//   block21_Logical =
// 	    new G4LogicalVolume(block21, Fe, "block21_Logical", 0, 0, 0);
// 	block21vis = new G4VisAttributes(blue);
// 	block21_Logical->SetVisAttributes(block21vis);

// 	new G4PVPlacement(0, G4ThreeVector(25.*cm+trans_x , trans_y,-distcollimatortotarget-
// 	(10 * block_z+48*cm)*0.5+trans_z ), block21_Logical, "block", world_log, 0, 0);
// //End of Block21--------------------------------------------------------

// //Beginning Block 22-----------------------------------------
// G4LogicalVolume *block22_Logical;
// G4Box *block22; 
// G4VisAttributes *block22vis;

// block22 =
// 	    new G4Box("block22",15*cm,5*cm, ((-280  * mm)+10 * block_z )*0.5);

//   block22_Logical =
// 	    new G4LogicalVolume(block22, Fe, "block22_Logical", 0, 0, 0);
// 	block22vis = new G4VisAttributes(blue);
// 	block22_Logical->SetVisAttributes(block22vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(trans_x , 20.*cm+trans_y,-distcollimatortotarget-
// 	(10 * block_z+48*cm)*0.5+trans_z ), block22_Logical, "block", world_log, 0, 0);
// //End Block 22----------------------------------------

// //Beginning Block 23-------------------------------------
// G4LogicalVolume *block23_Logical;
// G4Box *block23; 
// G4VisAttributes *block23vis;

// block23 =
// 	    new G4Box("block22",10*cm,25*cm, ((-280  * mm)+10 * block_z )*0.5);

//   block23_Logical =
// 	    new G4LogicalVolume(block23, Fe, "block23_Logical", 0, 0, 0);
// 	block23vis = new G4VisAttributes(blue);
// 	block23_Logical->SetVisAttributes(block23vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(-25.*cm+trans_x , trans_y,-distcollimatortotarget-
// 	(10 * block_z+48*cm)*0.5+trans_z ), block23_Logical, "block", world_log, 0, 0);
// //End Block 23---------------------------------------

// //Beginning Block 24-------------------------------------
// G4LogicalVolume *block24_Logical;
// G4Box *block24; 
// G4VisAttributes *block24vis;

// block24 =
// 	    new G4Box("block24",15*cm,5*cm, ((-280 * mm)+10 * block_z )*0.5);

//   block24_Logical =
// 	    new G4LogicalVolume(block24, Fe, "block24_Logical", 0, 0, 0);
// 	block24vis = new G4VisAttributes(blue);
// 	block24_Logical->SetVisAttributes(block24vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(trans_x , -20.*cm+trans_y,-distcollimatortotarget-
// 	(10 * block_z+48*cm)*0.5 +trans_z), block24_Logical, "block", world_log, 0, 0);
// //End Block 24---------------------------------------


// /************************* Lead Shielding *****************/

// //Right Leadwall (From the perspective of the wall)
// // The Wall stars at the same z-coordinate as the collimator and is 20cm long for 1m
// // in y-direction it starts 10cm below the collimator and ends 10cm above it.
// //Beginning Block 31-------------------------------------
// G4LogicalVolume *block31_Logical;
// G4Box *block31; 
// G4VisAttributes *block31vis;

// block31 =
// 	    new G4Box("block31",10*cm,25*cm, 28.5*cm);

//   block31_Logical =
// 	    new G4LogicalVolume(block31, Pb, "block31_Logical", 0, 0, 0);
// 	block31vis = new G4VisAttributes(leadcolor);
// 	block31_Logical->SetVisAttributes(block31vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(-45*cm+trans_x , trans_y,-distcollimatortotarget-
// 	97.5*cm+31.0*cm+trans_z), block31_Logical, "collimator_Lead_right_front", world_log, 0, 0);
// //End Block 31---------------------------------------



 
// //Second part of the wall is only 5cm thick and continues for 81cm until the back wall.

// //Beginning Block 32-------------------------------------
// G4LogicalVolume *block32_Logical;
// G4Box *block32; 
// G4VisAttributes *block32vis;
// G4UnionSolid *subcase_Solid11;
// G4Tubs *subcase_Solid12;


// //BGO POSITION
// //G4ThreeVector bgo1_Position = G4ThreeVector
// //(bgo1_Distance*sin(g1_theta)*cos(g1_phi),
//  //bgo1_Distance*cos(g1_theta),
//   //bgo1_Distance*sin(g1_theta)*sin(g1_phi));

// //Center of Block in WorldCoordinates (-22,5cm,0,-10,7cm)
// G4double bgo11_Distance=bgo1_Distance+10*cm;

// G4RotationMatrix yRot45degblock32;
// yRot45degblock32.rotateY(M_PI*95/180.*rad);
// G4ThreeVector translationblock32(
// -(disttoendblock32+13.75*cm +trans_x)+bgo11_Distance*sin(g1_theta)*cos(g1_phi),
// -trans_y+bgo11_Distance*cos(g1_theta),
// -(-distcollimatortotarget-
// 	95*cm+12*cm+90.5*cm+trans_z)+bgo11_Distance*sin(g1_theta)*sin(g1_phi));



// G4SubtractionSolid *block32minusDetector1;	
// G4SubtractionSolid *block32minusDetector1andhole;	




// G4UnionSolid* Al_Solid1 = bgo1->Get_Al_Case_Solid();
// subcase_Solid11= Al_Solid1;
// block32 =
// 	    new G4Box("block32",137.5*mm,25*cm, 40.5*cm);
// block32minusDetector1=
// //new G4SubtractionSolid("block32-subcase_Solid11",block32,subcase_Solid11,
// //&yRot45degblock32,translationblock32);
// new G4SubtractionSolid("block32-subcase_Solid11",block32,subcase_Solid11, &yRot45degblock32, translationblock32);

// subcase_Solid12= new G4Tubs("subcase_Solid12",
//  0, 25*mm, 1000*mm/2, 0. * deg, 360. * deg);


// block32minusDetector1andhole=
// new G4SubtractionSolid("block32minusDetector1-subcase_Solid12",block32minusDetector1,subcase_Solid12,
// &yRot45degblock32,translationblock32);



//  block32_Logical =
//     new G4LogicalVolume(block32minusDetector1andhole, Pb, "block32_Logical", 0, 0, 0);
//  // block32_Logical =
// 	//    new G4LogicalVolume(block32minusDetector1, Pb, "block32_Logical", 0, 0, 0);
	
// 	block32vis = new G4VisAttributes(leadcolor);
// 	block32_Logical->SetVisAttributes(block32vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(disttoendblock32+13.75*cm +trans_x, trans_y,-distcollimatortotarget-
// 	95*cm+12*cm+90.5*cm+trans_z), block32_Logical, "collimator_Lead_right_back", world_log, 0, 0);


// //End Block 32---------------------------------------

// //Small intermediate Block to fill a Gap between block32 and block31
// //Beginning Block 33-------------------------------------
// G4LogicalVolume *block33_Logical;
// G4Box *block33; 
// G4VisAttributes *block33vis;

// block33 =
// 	    new G4Box("block33",17.5*cm,25*cm, 2.5*cm);

//   block33_Logical =
// 	    new G4LogicalVolume(block33, Pb, "block33_Logical", 0, 0, 0);
// 	block33vis = new G4VisAttributes(leadcolor);
// 	block33_Logical->SetVisAttributes(block33vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(-30*cm+trans_x , 0+trans_y,
// 		-38*cm+2.5*cm-distcollimatortotarget+trans_z), block33_Logical, "collomator_Lead_right_back", world_log, 0, 0);
// //End Block 33---------------------------------------


// //Above the iron shielding sits a layer of lead that is 30cm thick
// //Beginning Block 41-------------------------------------
// G4LogicalVolume *block41_Logical;
// G4Box *block41; 
// G4VisAttributes *block41vis;

// block41 =
// 	    new G4Box("block41",180*cm,10*cm,120*cm);

//   block41_Logical =
// 	    new G4LogicalVolume(block41, Pb, "block41_Logical", 0, 0, 0);
// 	block41vis = new G4VisAttributes(grey);
// 	block41_Logical->SetVisAttributes(block41vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(trans_x ,+35.*cm+trans_y,-distcollimatortotarget-95*cm+90.5*cm+trans_z), block41_Logical, "ceiling_Lead", world_log, 0, 0);
// //End Block 41---------------------------------------


// //All the ground below the level of the lowest iron shielding is assumed to be lead.
// //Beginning Block 51-------------------------------------
// G4LogicalVolume *block51_Logical;
// G4Box *block51; 
// G4VisAttributes *block51vis;

// block51 =
// 	    new G4Box("block51",180*cm,10*cm, 120*cm);

//   block51_Logical =
// 	    new G4LogicalVolume(block51, Pb, "block51_Logical", 0, 0, 0);
// 	block51vis = new G4VisAttributes(grey);
// 	block51_Logical->SetVisAttributes(block51vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(trans_x , -35.*cm+trans_y,-distcollimatortotarget-95*cm+90.5*cm+trans_z), block51_Logical, "floor_Lead", world_log, 0, 0);
// //End Block 51---------------------------------------

// //Chamber on the right side
// // The first wall. It is 6cm in front of the Cone, 10cm deep and 25cm high. It stars
// // directly at the thinner wall Element from Block32 
// //Beginning Block 61-------------------------------------
// G4LogicalVolume *block61_Logical;
// G4Box *block61; 
// G4VisAttributes *block61vis;

// block61 =
// 	    new G4Box("block61",40*cm,12.5*cm, 5*cm);

//   block61_Logical =
// 	    new G4LogicalVolume(block61, Pb, "block61_Logical", 0, 0, 0);
// 	block61vis = new G4VisAttributes(leadcolor);
// 	block61_Logical->SetVisAttributes(block61vis);
	
// 		// new G4PVPlacement(0, G4ThreeVector(-40.*cm-26*cm-trans_x ,-25*cm+12.5*cm+trans_y,-distcollimatortotarget-
// 	// 95*cm-6*cm+90.5*cm+trans_z), block61_Logical, "Det1_Lead_downstream", world_log, 0, 0);
// //End Block 61---------------------------------------

// // The second wall. It is 6 behind the Cone, 10cm deep and 25cm high. It stars
// // directly at the thinner wall Element from Block32 
// //Beginning Block 62-------------------------------------
// G4LogicalVolume *block62_Logical;
// G4Box *block62; 
// G4VisAttributes *block62vis;

// block62 =
// 	    new G4Box("block62",40*cm,12.5*cm, 5*cm);

//   block62_Logical =
// 	    new G4LogicalVolume(block62, Pb, "block62_Logical", 0, 0, 0);
// 	block62vis = new G4VisAttributes(leadcolor);
// 	block62_Logical->SetVisAttributes(block62vis);
	
// 		// new G4PVPlacement(0, G4ThreeVector(-40.*cm-26*cm+trans_x , -25*cm+12.5*cm+trans_y,-distcollimatortotarget-
// 	// 95*cm-6*cm+90.5*cm+43*cm+trans_z), block62_Logical, "Det1_Lead_upstream", world_log, 0, 0);
// //End Block 62---------------------------------------


// //Chamber on the left side
// // Right Wall leading into the chamber. It is 40cm high, 10cm thick 
// //and as long as the collimator (95cm) minus 28.5cm for a total of 66.5 cm
// //Beginning Block 71-------------------------------------
// G4LogicalVolume *block71_Logical;
// G4Box *block71; 
// G4VisAttributes *block71vis;

// block71 =
// 	    new G4Box("block71",10*cm,25*cm, 28.5*cm);

//   block71_Logical =
// 	    new G4LogicalVolume(block71, Pb, "block71_Logical", 0, 0, 0);
// 	block71vis = new G4VisAttributes(leadcolor);
// 	block71_Logical->SetVisAttributes(block71vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(45.*cm+trans_x , trans_y,-distcollimatortotarget-
// 	95*cm+28.5*cm+trans_z), block71_Logical, "collimator_Lead_left_front", world_log, 0, 0);
// //End Block 71---------------------------------------

// //Left Wall closing the chamber off. It starts 37cm behind the right wall and runs
// // at a constant thickness of 10cm towards the end of the chamber.

// //Beginning Block 72-------------------------------------
// G4LogicalVolume *block72_Logical;
// G4Box *block72; 
// G4VisAttributes *block72vis;

// block72 =
// 	    new G4Box("block72",10*cm,25*cm, 72*cm);

//   block72_Logical =
// 	    new G4LogicalVolume(block72, Pb, "block72_Logical", 0, 0, 0);
// 	block72vis = new G4VisAttributes(leadcolor);
// 	block72_Logical->SetVisAttributes(block72vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(165.*cm+trans_x , trans_y,-distcollimatortotarget-
// 	95*cm+37*cm+72*cm+trans_z), block72_Logical, "casle_left_wall_Lead", world_log, 0, 0);
// //End Block 72---------------------------------------

// // A Block auf Lead sitting in front of the entrance of the chamber.
// // It is a meter Wide, 30cm deep and 5cm high
// //Beginning Block 73-------------------------------------
// G4LogicalVolume *block73_Logical;
// G4Box *block73; 
// G4VisAttributes *block73vis;

// block73 =
// 	    new G4Box("block73",50*cm,2.5*cm, 15*cm);

//   block73_Logical =
// 	    new G4LogicalVolume(block73, Pb, "block73_Logical", 0, 0, 0);
// 	block73vis = new G4VisAttributes(leadcolor);
// 	block73_Logical->SetVisAttributes(block73vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(55.*cm+50*cm+trans_x , -25.*cm+2.5*cm+trans_y,-distcollimatortotarget-
// 	95*cm+37*cm+15*cm+trans_z), block73_Logical, "castle_front_wall_Lead", world_log, 0, 0);
// //End Block 73---------------------------------------

// // Lead behind the cones.It is is 19cm deep and has a 90 degree angle with the wall behind it.


// //Beginning Block 74-------------------------------------
// G4LogicalVolume *block74_Logical;
// G4Box *block74; 
// G4VisAttributes *block74vis;

// block74 =
// 	    new G4Box("block74",27*cm,25*cm, 9.5*cm); 

//   block74_Logical =
// 	    new G4LogicalVolume(block74, Pb, "block74_Logical", 0, 0, 0);
// 	block74vis = new G4VisAttributes(leadcolor);
// 	block74_Logical->SetVisAttributes(block74vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(26*cm+trans_x + 27*cm*0.5,trans_y, 41.5*cm-distcollimatortotarget
// +trans_z), block74_Logical, "pol_Lead_upstream", world_log, 0, 0);
// //End Block 74---------------------------------------



// // Block in front of and enclosing the Detector and Polarimeter
// //Beginning Block 75-------------------------------------
// G4LogicalVolume *block75_Logical;
// G4Box *block751; 
// G4Box *block752; 
// G4UnionSolid *block75;
// // DREIECK! G4Box *block753; 
// G4VisAttributes *block75vis;

// G4UnionSolid* blocksum_751_752;
// G4GenericTrap* itsatrap;
// block751 =
// 	    new G4Box("block751",5.*cm,25*cm, (14.55/2)*cm+3*cm); 
// block752 =
// 	    new G4Box("block752",14.*cm,25*cm, 15.6*cm); //1mm Overlap between the blocks
	    
// blocksum_751_752= new G4UnionSolid("blocksum_751_752", block751, block752,0, G4ThreeVector
// (9*cm, 0.,-(14.55/2)*cm+15.5*cm+36*cm-0.1*cm));


// vector <G4TwoVector> vertices1;

// vertices1.push_back (G4TwoVector(5.*cm,25*cm));
// vertices1.push_back (G4TwoVector(5.*cm,-25*cm));
// vertices1.push_back (G4TwoVector(-5.*cm,-25*cm));
// vertices1.push_back (G4TwoVector(-5.*cm,25*cm));
// vertices1.push_back (G4TwoVector(23.*cm,25*cm));
// vertices1.push_back (G4TwoVector(23.*cm,-25*cm));
// vertices1.push_back (G4TwoVector(-5.*cm,-25*cm));
// vertices1.push_back (G4TwoVector(-5.*cm,25*cm));


// itsatrap = new G4GenericTrap("itsatrap",   (21.55/2)*cm,
//          vertices1);



// block75= new G4UnionSolid("blocksum_751_752", blocksum_751_752, itsatrap,0, G4ThreeVector
// (0, 0.,(36/2)*cm));



// G4UnionSolid *subcase_Solid21;
// G4Tubs *subcase_Solid22;



// G4double bgo21_Distance=bgo2_Distance+10*cm;

// G4RotationMatrix yRot45degblock75;
// yRot45degblock75.rotateY(M_PI*230/180.*rad);
// G4ThreeVector translationblock75(
// -(17.5*cm+trans_x) +bgo21_Distance*sin(g2_theta)*cos(g2_phi),
// -trans_y+bgo21_Distance*cos(g2_theta),
// -(-distcollimatortotarget+14.55/2*cm-35*cm	+trans_z)+bgo21_Distance*sin(g2_theta)*sin(g2_phi)
// );



// G4SubtractionSolid *block75minusDetector2;	
// G4SubtractionSolid *block75minusDetector2andhole;	





// subcase_Solid21= Al_Solid1;
// block75minusDetector2=
// new G4SubtractionSolid("block75-subcase_Solid21",block75,subcase_Solid21, &yRot45degblock75, translationblock75);

// subcase_Solid22= new G4Tubs("subcase_Solid22",
//  0, 25*mm, 1000*mm/2, 0. * deg, 360. * deg);


// block75minusDetector2andhole=
// new G4SubtractionSolid("block75minusDetector2-subcase_Solid22",block75minusDetector2,subcase_Solid22,
// &yRot45degblock75,translationblock75);


// G4UnionSolid *subcase_Solidpol1;
// G4Tubs *subcase_Solidpol2;

// //BGO POSITION
// //G4ThreeVector bgop_Position = G4ThreeVector
// //(bgop_Distance*sin(pol_theta)*cos(pol_phi),
//  //bgop_Distance*cos(pol_theta),
//   //bgop_Distance*sin(pol_theta)*sin(pol_phi));

// //Center of Block in WorldCoordinates (15*cm+trans_x ,trans_y,		 -distcollimatortotarget+14.55/2*cm	+trans_z)

// G4double bgop1_Distance=bgop_Distance+10*cm;

// G4RotationMatrix yRot45degblock75pol;
// yRot45degblock75pol.rotateY(M_PI*270/180.*rad);
// G4ThreeVector translationblock75pol(
// -(17.5*cm+trans_x) +bgop1_Distance*sin(pol_theta)*cos(pol_phi),
// -trans_y+bgop1_Distance*cos(pol_theta),
// -(-distcollimatortotarget+14.55/2*cm-35*cm	+trans_z)+bgop1_Distance*sin(pol_theta)*sin(pol_phi)
// );



// G4SubtractionSolid *block75minusDetector2pol;	
// G4SubtractionSolid *block75minusDetector2andholepol;	





// subcase_Solidpol1= Al_Solid1;
// block75minusDetector2pol=
// new G4SubtractionSolid("block75minusDetector2andhole-subcase_Solidpol1",block75minusDetector2andhole,subcase_Solidpol1, &yRot45degblock75pol, translationblock75pol);

// subcase_Solidpol2= new G4Tubs("subcase_Solidpol2",
//  0, 25*mm, 1000*mm/2, 0. * deg, 360. * deg);


// block75minusDetector2andholepol=
// new G4SubtractionSolid("block75minusDetector2pol-subcase_Solidpol2",block75minusDetector2pol,subcase_Solidpol2,
// &yRot45degblock75pol,translationblock75pol);









	
//  block75_Logical =
// 	    new G4LogicalVolume(block75minusDetector2andholepol, Pb, "block75_Logical", 0, 0, 0);
// 	block75vis = new G4VisAttributes(leadcolor);
// 	block75_Logical->SetVisAttributes(block75vis);
	
// 		new G4PVPlacement(0, G4ThreeVector(17.5*cm+trans_x ,trans_y,
// 		 -distcollimatortotarget+14.55/2*cm-35*cm
// 	+trans_z), block75_Logical, "pol_and_det2_collimator", world_log, 0, 0);



// //Wall behind target
// //Beginning Block 81-------------------------------------
// // G4LogicalVolume *block81_Logical;
// // G4Box *block81; 
// // G4VisAttributes *block81vis;

// // block81 =
// // 	    new G4Box("block81",160*cm,25*cm,20*cm);

// //   block81_Logical =
// // 	    new G4LogicalVolume(block81, Pb, "block81_Logical", 0, 0, 0);
// // 	block81vis = new G4VisAttributes(grey);
// // 	block81_Logical->SetVisAttributes(block81vis);
	
// // 		new G4PVPlacement(0, G4ThreeVector(trans_x , trans_y,71*cm-distcollimatortotarget
// // 	+trans_z ), block81_Logical, "block", world_log, 0, 0);
// //End Block 81---------------------------------------

// /************************* Detector 1*****************/


// //First Detector. Sole one on the right side. Angle is 95 degrees relativ to the beam.

 
  
//   G4RotationMatrix* rm1=new G4RotationMatrix();
//   rm1->rotateY(g1_Angle);

//   G4ThreeVector germaniumDetector1_Position=G4ThreeVector(g1_Distance*sin(g1_theta)*cos(g1_phi), g1_Distance*cos(g1_theta), g1_Distance*sin(g1_theta)*sin(g1_phi));

//   G4LogicalVolume* germaniumDetector1_Logical=germaniumDetector1->Get_Logical();
// //Unterscheidet sich vom Source Code aber kompeliert nur so...
// //(G4LogicalVolume* vorangestellt)  und Get_Crystal2_Logical musste durch
// //Get_Logical ersetzt werden.
//  //G4LogicalVolume* crystal1_Logical = germaniumDetector1->Get_Logical();

//   // new G4PVPlacement(rm1, germaniumDetector1_Position, germaniumDetector1_Logical, "Germanium1_TUD", world_log, false, 0);




//  //BGO of Germanium1_TUD
  
//   // Is already used in Block 32
//   //BGO* bgo1 = new BGO();
  
 
  
//   G4LogicalVolume* bgo1_Logical = bgo1->Get_Logical();
  
//  // G4LogicalVolume* bgoCrystal1_Logical = bgo1->Get_Crystal_Logical();
  
//   // new G4PVPlacement(rm1, bgo1_Position, bgo1_Logical, "bgo1", world_log, false, 0);






// /***************************** Detector 2 **************************/
//   // contains Germanium2 (Serial Number 72902)
//   /*******************************************************************/

// //
// // Second Detector 130 Degree Angle
// //


//   G4RotationMatrix* rm2=new G4RotationMatrix();
//   rm2->rotateY(g2_Angle);

//   G4ThreeVector germaniumDetector2_Position=G4ThreeVector(g2_Distance*sin(g2_theta)*cos(g2_phi), g2_Distance*cos(g2_theta), g2_Distance*sin(g2_theta)*sin(g2_phi));
  
//   G4LogicalVolume* germaniumDetector2_Logical=germaniumDetector2->Get_Logical();
// // Auch hier musste crystal2_Logical mit Pointer  gemacht werden.
//    //G4LogicalVolume* crystal2_Logical = germaniumDetector2->Get_Logical();
  
//   new G4PVPlacement(rm2, germaniumDetector2_Position, germaniumDetector2_Logical, "Germanium2", world_log, false, 0);

//  // BGO of Germanium2
  

  
//   G4LogicalVolume* bgo2_Logical = bgo2->Get_Logical();
  
//   //G4LogicalVolume* bgoCrystal2_Logical = bgo2->Get_Crystal_Logical();
  
//   new G4PVPlacement(rm2, bgo2_Position, bgo2_Logical, "bgo2", world_log, false, 0);

 
  
//    /***************************** Polarimeter **************************/
//   // contains Polarimeter Crystal (Serial Number 72930)
//   /********************************************************************/
// // At 90 Degree angle relativ to the beam.

//   G4RotationMatrix* rmpol=new G4RotationMatrix();
//   rmpol->rotateY(pol_Angle);

//   G4ThreeVector polarimeterDetector_Position=G4ThreeVector(pol_Distance*sin(pol_theta)*cos(pol_phi), pol_Distance*cos(pol_theta), pol_Distance*sin(pol_theta)*sin(pol_phi));
  
//   G4LogicalVolume* polarimeterDetector_Logical=polarimeterDetector->Get_Logical();
// //Pointer benutzt
//   //G4LogicalVolume* crystalp_Logical = polarimeterDetector->Get_Logical();

//   new G4PVPlacement(rmpol, polarimeterDetector_Position, polarimeterDetector_Logical, "Polarimeter", world_log, false, 0);

//   // BGO of Polarimeter
  
  
  
//   G4LogicalVolume* bgop_Logical = bgop->Get_Logical();
//   //Auch hier wurde G4LogicalVolume* hinzugefügt
//   //G4LogicalVolume* bgoCrystalp_Logical = bgop->Get_Crystal_Logical();
  
//   new G4PVPlacement(rmpol, bgop_Position, bgop_Logical, "bgop", world_log, false, 0);

// //Additional Blocks to make the target chamber more realistic

// //Block on the right side of the chamber
// //Beginning Block 91


// G4LogicalVolume *block91_Logical;
// G4Box *block91; 
// G4VisAttributes *block91vis;

// block91 =
// 	    new G4Box("block91",3.75*cm,25*cm,19*cm);

//   block91_Logical =
// 	    new G4LogicalVolume(block91, Pb, "block91_Logical", 0, 0, 0);
// 	block91vis = new G4VisAttributes(leadcolor);
// 	block91_Logical->SetVisAttributes(block91vis);
	
// 	new G4PVPlacement(0, G4ThreeVector(-8.75*cm+trans_x , trans_y,-distcollimatortotarget-
// 	+19*cm+trans_z ), block91_Logical, "block", world_log, 0, 0);




// //End Block 91
// //  Block on the left side of the chamber
// //Beginning Block 92


// G4LogicalVolume *block92_Logical;
// G4Box *block92; 
// G4VisAttributes *block92vis;

// block92 =
// 	    new G4Box("block92",3.75*cm,25*cm,19*cm+1.131*cm);

//   block92_Logical =
// 	    new G4LogicalVolume(block92, Pb, "block92_Logical", 0, 0, 0);
// 	block92vis = new G4VisAttributes(leadcolor);
// 	block92_Logical->SetVisAttributes(block92vis);
	
// 	new G4PVPlacement(0, G4ThreeVector(+8.75*cm+trans_x , trans_y,-distcollimatortotarget-
// 	+19*cm+1.131*cm+trans_z ), block92_Logical, "collimator_Lead_left_back", world_log, 0, 0);




// //End Block 92
// //Triangle through which the hole in front of Detector 2 exits.
// //Beginning Block 93
// G4LogicalVolume *block93_Logical;
// G4GenericTrap* itsatrap2;
// G4VisAttributes *block93vis;

// vector <G4TwoVector> vertices2;

// vertices2.push_back (G4TwoVector(-7.5*cm,25*cm));
// vertices2.push_back (G4TwoVector(-7.5*cm,-25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,-25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,-25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,-25*cm));
// vertices2.push_back (G4TwoVector(0.*cm,25*cm));


// itsatrap2 = new G4GenericTrap("itsatrap2",   (8.938/2)*cm,
//          vertices2);
         
//          ///////////////////////////////


// G4ThreeVector translationblock93(
// -(+12.5*cm+trans_x ) +bgo21_Distance*sin(g2_theta)*cos(g2_phi),
// -trans_y+bgo21_Distance*cos(g2_theta),
// -(-distcollimatortotarget
// 	+2.262*cm+(8.938/2)*cm+trans_z)+bgo21_Distance*sin(g2_theta)*sin(g2_phi)
// );

         

// G4SubtractionSolid *itsatrap2withhole;



// itsatrap2withhole = 
// new G4SubtractionSolid("block75minusDetector2andhole-subcase_Solidpol1",
// itsatrap2,subcase_Solid22, &yRot45degblock75, translationblock93);
// //Rotation can stay the same as in Block 75 as we are describing the same hole.


// /////////////////////////////



// block93_Logical =
// 	    new G4LogicalVolume(itsatrap2withhole, Pb, "block93_Logical", 0, 0, 0);
// 	block93vis = new G4VisAttributes(leadcolor);
// 	block93_Logical->SetVisAttributes(block93vis);
	
// 	new G4PVPlacement(0, G4ThreeVector(+12.5*cm+trans_x , trans_y,-distcollimatortotarget
// 	+2.262*cm+(8.938/2)*cm+trans_z ), block93_Logical, "block", world_log, 0, 0);

// //End Block93


// /*****************************Filter************************+*****/
// //Beginning Copper-Filter for Det1

// 	G4double copperfilter1_Radius =2.5*cm;
// 	G4double copperfilter1_Length =1*cm;
// G4ThreeVector copperfilter1_Position = G4ThreeVector(
// copperfilter1_Distance*sin(g1_theta)*cos(g1_phi),
//  copperfilter1_Distance*cos(g1_theta),
//   copperfilter1_Distance*sin(g1_theta)*sin(g1_phi));
  
  
//   G4Tubs *copperfilter1_Solid =
// 	    new G4Tubs("copperfilter1_Solid", 0. * cm, copperfilter1_Radius,
// 	               copperfilter1_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *copperfilter1_Logical;
// G4VisAttributes *copperfilter1vis;

//   copperfilter1_Logical =
// 	    new G4LogicalVolume(copperfilter1_Solid, Cu, "copperfilter1_Logical", 0, 0, 0);
// 	copperfilter1vis = new G4VisAttributes(orange);
// 	copperfilter1_Logical->SetVisAttributes(copperfilter1vis);
  
//   // new G4PVPlacement(rm1, copperfilter1_Position, copperfilter1_Logical, "copperfilter1", world_log, false, 0);
// //End of Copper-Filter for Det1
// //Beginning Copper-Filter for Det2

// 	G4double copperfilter2_Radius =2.5*cm;
// 	G4double copperfilter2_Length =1*cm;
// G4ThreeVector copperfilter2_Position = G4ThreeVector(
// copperfilter2_Distance*sin(g2_theta)*cos(g2_phi),
//  copperfilter2_Distance*cos(g2_theta),
//   copperfilter2_Distance*sin(g2_theta)*sin(g2_phi));
  
  
//   G4Tubs *copperfilter2_Solid =
// 	    new G4Tubs("copperfilter2_Solid", 0. * cm, copperfilter2_Radius,
// 	               copperfilter2_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *copperfilter2_Logical;
// G4VisAttributes *copperfilter2vis;

//   copperfilter2_Logical =
// 	    new G4LogicalVolume(copperfilter2_Solid, Cu, "copperfilter2_Logical", 0, 0, 0);
// 	copperfilter2vis = new G4VisAttributes(orange);
// 	copperfilter2_Logical->SetVisAttributes(copperfilter2vis);
  
//   new G4PVPlacement(rm2, copperfilter2_Position, copperfilter2_Logical, "copperfilter2", world_log, false, 0);
// //End of Copper-Filter for Det2
// //Beginning Copper-Filter for Pol

// 	G4double copperfilterpol_Radius =2.5*cm;
// 	G4double copperfilterpol_Length =1*cm;
// G4ThreeVector copperfilterpol_Position = G4ThreeVector(
// copperfilterpol_Distance*sin(pol_theta)*cos(pol_phi),
//  copperfilterpol_Distance*cos(pol_theta),
//   copperfilterpol_Distance*sin(pol_theta)*sin(pol_phi));
  
  
//   G4Tubs *copperfilterpol_Solid =
// 	    new G4Tubs("copperfilterpol_Solid", 0. * cm, copperfilterpol_Radius,
// 	               copperfilterpol_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *copperfilterpol_Logical;
// G4VisAttributes *copperfilterpolvis;

//   copperfilterpol_Logical =
// 	    new G4LogicalVolume(copperfilterpol_Solid, Cu, "copperfilterpol_Logical", 0, 0, 0);
// 	copperfilterpolvis = new G4VisAttributes(orange);
// 	copperfilterpol_Logical->SetVisAttributes(copperfilterpolvis);
  
//   new G4PVPlacement(rmpol, copperfilterpol_Position, copperfilterpol_Logical, "copperfilterpol", world_log, false, 0);
// //End of Copper-Filter for Pol
// //Beginning Lead-Filter for Det1

// 	G4double leadfilter1_Radius =2.5*cm;
// 	G4double leadfilter1_Length =1*cm;
// G4ThreeVector leadfilter1_Position = G4ThreeVector(
// leadfilter1_Distance*sin(g1_theta)*cos(g1_phi),
//  leadfilter1_Distance*cos(g1_theta),
//   leadfilter1_Distance*sin(g1_theta)*sin(g1_phi));
  
  
//   G4Tubs *leadfilter1_Solid =
// 	    new G4Tubs("leadfilter1_Solid", 0. * cm, leadfilter1_Radius,
// 	               leadfilter1_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *leadfilter1_Logical;
// G4VisAttributes *leadfilter1vis;

//   leadfilter1_Logical =
// 	    new G4LogicalVolume(leadfilter1_Solid, Pb, "leadfilter1_Logical", 0, 0, 0);
// 	leadfilter1vis = new G4VisAttributes(leadcolor);
// 	leadfilter1_Logical->SetVisAttributes(leadfilter1vis);
  
//   // new G4PVPlacement(rm1, leadfilter1_Position, leadfilter1_Logical, "leadfilter1", world_log, false, 0);
// //End of Lead-Filter for Det1
// //Beginning Lead-Filter for Det2

// 	G4double leadfilter2_Radius =2.5*cm;
// 	G4double leadfilter2_Length =1*cm;
// G4ThreeVector leadfilter2_Position = G4ThreeVector(
// leadfilter2_Distance*sin(g2_theta)*cos(g2_phi),
//  leadfilter2_Distance*cos(g2_theta),
//   leadfilter2_Distance*sin(g2_theta)*sin(g2_phi));
  
  
//   G4Tubs *leadfilter2_Solid =
// 	    new G4Tubs("leadfilter2_Solid", 0. * cm, leadfilter2_Radius,
// 	               leadfilter2_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *leadfilter2_Logical;
// G4VisAttributes *leadfilter2vis;

//   leadfilter2_Logical =
// 	    new G4LogicalVolume(leadfilter2_Solid, Pb, "leadfilter2_Logical", 0, 0, 0);
// 	leadfilter2vis = new G4VisAttributes(leadcolor);
// 	leadfilter2_Logical->SetVisAttributes(leadfilter2vis);
  
//   new G4PVPlacement(rm2, leadfilter2_Position, leadfilter2_Logical, "leadfilter2", world_log, false, 0);
// //End of Lead-Filter for Det2
// //Beginning Lead-Filter for Pol

// 	G4double leadfilterpol_Radius =2.5*cm;
// 	G4double leadfilterpol_Length =1*cm;
// G4ThreeVector leadfilterpol_Position = G4ThreeVector(
// leadfilterpol_Distance*sin(pol_theta)*cos(pol_phi),
//  leadfilterpol_Distance*cos(pol_theta),
//   leadfilterpol_Distance*sin(pol_theta)*sin(pol_phi));
  
  
//   G4Tubs *leadfilterpol_Solid =
// 	    new G4Tubs("leadfilterpol_Solid", 0. * cm, leadfilterpol_Radius,
// 	               leadfilterpol_Length / 2, 0. * deg, 360. * deg);
	               
// G4LogicalVolume *leadfilterpol_Logical;
// G4VisAttributes *leadfilterpolvis;

//   leadfilterpol_Logical =
// 	    new G4LogicalVolume(leadfilterpol_Solid, Pb, "leadfilterpol_Logical", 0, 0, 0);
// 	leadfilterpolvis = new G4VisAttributes(leadcolor);
// 	leadfilterpol_Logical->SetVisAttributes(leadfilterpolvis);
  
//   new G4PVPlacement(rmpol, leadfilterpol_Position, leadfilterpol_Logical, "leadfilterpol", world_log, false, 0);
// //End of Lead-Filter for Pol

// 	/************************* NRF Target *****************/

//   	//// Distance of the NRF target in z-direction to the origin
// 	//G4double target_distance = 0. * mm;

// 	//G4double target_radius = 12. * mm;
// 	//G4double target_area = pi * target_radius * target_radius;

// 	//// Masses of target constituents, measured by M. Schilling, M. Zweidinger
// 	//// (and P. Ries ?)
// 	//// Neglected the UV hardened glue
// 	//G4double Fe1_mass = 0.4948 * g;
// 	//G4double Co1_mass = 0.5153 * g;
// 	//G4double Al1_mass = 0.610 * g;
// 	//G4double Al2_mass = 0.6074 * g;
// 	//G4double Cr2O3_1_mass = 0.9574 * g; // "Kleber II" is missing here
// 	//G4double Sn_mass = 9.1516 * g;
// 	//G4double Cr2O3_2_mass = 0.9372 * g; // Subtracted "Dose (ohne Deckel) + ...
// 	                                    //// + 116Sn" from "Dose (ohne Deckel) +
// 	                                    //// ... + 52Cr2O3II"
// 	//G4double Al3_mass = 0.6113 * g;
// 	//G4double Al4_mass = 0.6111 * g;
// 	//G4double Fe2_mass = 0.5790 * g;
// 	//G4double Co2_mass = 0.5392 * g;

// 	//// Calculate length of cylindric target constituents from their known mass,
// 	//// density and radius
// 	//G4double Fe1_thickness = Fe1_mass / Fe_density / target_area;
// 	//G4double Co1_thickness = Co1_mass / Co_density / target_area;
// 	//G4double Al1_thickness = Al1_mass / Al_density / target_area;
// 	//G4double Al2_thickness = Al2_mass / Al_density / target_area;
// 	//G4double Cr2O3_1_thickness =
// 	    //Cr2O3_1_mass / mat->Get_Cr2O3_density() / target_area;
// 	//G4double Sn_thickness = Sn_mass / mat->Get_Sn_density() / target_area;
// 	//G4double Cr2O3_2_thickness =
// 	    //Cr2O3_2_mass / mat->Get_Cr2O3_density() / target_area;
// 	//G4double Al3_thickness = Al3_mass / Al_density / target_area;
// 	//G4double Al4_thickness = Al4_mass / Al_density / target_area;
// 	//G4double Fe2_thickness = Fe2_mass / Fe_density / target_area;
// 	//G4double Co2_thickness = Co2_mass / Co_density / target_area;

// 	//// Dimensions of the target container. Calculate its length from the length
// 	//// of all targets
// 	//G4double TargetContainerCap_Thickness = 2. * mm;  // Estimated
// 	//G4double TargetContainerWall_Thickness = 1. * mm; // Estimated
// 	//G4double TargetContainerWall_Length =
// 	    //Fe1_thickness + Co1_thickness + Al1_thickness + Al2_thickness +
// 	    //Cr2O3_1_thickness + Sn_thickness + Cr2O3_2_thickness + Al3_thickness +
// 	    //Al4_thickness + Co2_thickness + Fe2_thickness;

// 	//G4Tubs *TargetContainerCap1_Solid =
// 	    //new G4Tubs("TargetContainerCap1_Solid", 0. * mm,
// 	               //target_radius + TargetContainerWall_Thickness,
// 	               //TargetContainerCap_Thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *TargetContainerCap1_Logical = new G4LogicalVolume(
// 	    //TargetContainerCap1_Solid, PE, "TargetContainerCap1_Logical");
// 	//G4VisAttributes *TargetContainerCap1_vis = new G4VisAttributes(cyan);
// 	//TargetContainerCap1_Logical->SetVisAttributes(TargetContainerCap1_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //0.5 * TargetContainerCap_Thickness),
// 	    //TargetContainerCap1_Logical, "TargetContainerCap1", world_log, 0, 0);

// 	//G4Tubs *TargetContainerWall_Solid =
// 	    //new G4Tubs("TargetContainerWall_Solid", target_radius,
// 	               //target_radius + TargetContainerWall_Thickness,
// 	               //TargetContainerWall_Length * 0.5, 0., twopi);
// 	//G4LogicalVolume *TargetContainerWall_Logical = new G4LogicalVolume(
// 	    //TargetContainerWall_Solid, PE, "TargetContainerWall_Logical");
// 	//G4VisAttributes *TargetContainerWall_vis = new G4VisAttributes(cyan);
// 	//TargetContainerWall_Logical->SetVisAttributes(TargetContainerWall_vis);
// ////	new G4PVPlacement(
// ////	    0, G4ThreeVector(0., 0.,  target_distance +
// ////	                                 TargetContainerCap_Thickness +
// ////	                                 0.5 * TargetContainerWall_Length),
// ////	    TargetContainerWall_Logical, "TargetContainerWall", world_log, 0, 0);

// 	//// Place all the target material inside the TargetContainerWall to avoid
// 	//// overlaps
// 	//G4Tubs *Fe1_Solid = new G4Tubs("Fe1_Solid", 0. * mm, target_radius,
// 	                               //Fe1_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Fe1_Logical =
// 	    //new G4LogicalVolume(Fe1_Solid, Fe, "Fe1_Logical");
// 	//G4VisAttributes *Fe1_vis = new G4VisAttributes(red);
// 	//Fe1_Logical->SetVisAttributes(Fe1_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //TargetContainerCap_Thickness +
// 	                                 //Fe1_thickness * 0.5),
// 	    //Fe1_Logical, "Fe1", world_log, 0, 0);

// 	//G4Tubs *Co1_Solid = new G4Tubs("Co1_Solid", 0. * mm, target_radius,
// 	                               //Co1_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Co1_Logical =
// 	    //new G4LogicalVolume(Co1_Solid, Co, "Co1_Logical");
// 	//G4VisAttributes *Co1_vis = new G4VisAttributes(white);
// 	//Co1_Logical->SetVisAttributes(Co1_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //TargetContainerCap_Thickness +
// 	                                 //Fe1_thickness + 0.5 * Co1_thickness),
// 	    //Co1_Logical, "Co1", world_log, 0, 0);

// 	//G4Tubs *Al1_Solid = new G4Tubs("Al1_Solid", 0. * mm, target_radius,
// 	                               //Al1_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Al1_Logical =
// 	    //new G4LogicalVolume(Al1_Solid, Al, "Al1_Logical");
// 	//G4VisAttributes *Al1_vis = new G4VisAttributes(grey);
// 	//Al1_Logical->SetVisAttributes(Al1_vis);
// 	//new G4PVPlacement(
// 	    //0,
// 	    //G4ThreeVector(0., 0., target_distance +
// 	                              //TargetContainerCap_Thickness + Co1_thickness +
// 	                              //Fe1_thickness + 0.5 * Al1_thickness),
// 	    //Al1_Logical, "Al1", world_log, 0, 0);

// 	//G4Tubs *Al2_Solid = new G4Tubs("Al2_Solid", 0. * mm, target_radius,
// 	                               //Al2_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Al2_Logical =
// 	    //new G4LogicalVolume(Al2_Solid, Al, "Al2_Logical");
// 	//G4VisAttributes *Al2_vis = new G4VisAttributes(grey);
// 	//Al2_Logical->SetVisAttributes(Al2_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //TargetContainerCap_Thickness +
// 	                                 //Al1_thickness + Co1_thickness +
// 	                                 //Fe1_thickness + 0.5 * Al2_thickness),
// 	    //Al2_Logical, "Al2", world_log, 0, 0);

// 	//G4Tubs *Cr2O3_1_Solid = new G4Tubs("Cr2O3_1_Solid", 0. * mm, target_radius,
// 	                                   //Cr2O3_1_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Cr2O3_1_Logical = new G4LogicalVolume(
// 	    //Cr2O3_1_Solid, mat->Get_target_Cr2O3(), "Cr2O3_1_Logical");
// 	//G4VisAttributes *Cr2O3_1_vis = new G4VisAttributes(green);
// 	//Cr2O3_1_Logical->SetVisAttributes(Cr2O3_1_vis);
// 	//new G4PVPlacement(
// 	    //0,
// 	    //G4ThreeVector(0., 0., target_distance +
// 	                              //TargetContainerCap_Thickness + Al1_thickness +
// 	                              //Co1_thickness + Fe1_thickness +
// 	                              //Al2_thickness + 0.5 * Cr2O3_1_thickness),
// 	    //Cr2O3_1_Logical, "Cr2O3_1", world_log, 0, 0);

// 	//G4Tubs *Sn_Solid = new G4Tubs("Sn_Solid", 0. * mm, target_radius,
// 	                              //Sn_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Sn_Logical =
// 	    //new G4LogicalVolume(Sn_Solid, mat->Get_target_Sn(), "Sn_Logical");
// 	//G4VisAttributes *Sn_vis = new G4VisAttributes(yellow);
// 	//Sn_Logical->SetVisAttributes(Sn_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //TargetContainerCap_Thickness +
// 	                                 //Al1_thickness + Co1_thickness +
// 	                                 //Fe1_thickness + Al2_thickness +
// 	                                 //Cr2O3_1_thickness + 0.5 * Sn_thickness),
// 	    //Sn_Logical, "Sn", world_log, 0, 0);

// 	//G4Tubs *Cr2O3_2_Solid = new G4Tubs("Cr2O3_2_Solid", 0. * mm, target_radius,
// 	                                   //Cr2O3_2_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Cr2O3_2_Logical = new G4LogicalVolume(
// 	    //Cr2O3_2_Solid, mat->Get_target_Cr2O3(), "Cr2O3_2_Logical");
// 	//G4VisAttributes *Cr2O3_2_vis = new G4VisAttributes(green);
// 	//Cr2O3_2_Logical->SetVisAttributes(Cr2O3_2_vis);
// 	//new G4PVPlacement(
// 	    //0,
// 	    //G4ThreeVector(0., 0., target_distance +
// 	                              //TargetContainerCap_Thickness + Al1_thickness +
// 	                              //Co1_thickness + Fe1_thickness +
// 	                              //Al2_thickness + Cr2O3_1_thickness +
// 	                              //Sn_thickness + 0.5 * Cr2O3_2_thickness),
// 	    //Cr2O3_2_Logical, "Cr2O3_2", world_log, 0, 0);

// 	//G4Tubs *Al3_Solid = new G4Tubs("Al3_Solid", 0. * mm, target_radius,
// 	                               //Al3_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Al3_Logical =
// 	    //new G4LogicalVolume(Al3_Solid, Al, "Al3_Logical");
// 	//G4VisAttributes *Al3_vis = new G4VisAttributes(grey);
// 	//Al3_Logical->SetVisAttributes(Al3_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0.,target_distance +
// 	                                 //TargetContainerCap_Thickness +
// 	                                 //Al1_thickness + Co1_thickness +
// 	                                 //Fe1_thickness + Al2_thickness +
// 	                                 //Cr2O3_1_thickness + Sn_thickness +
// 	                                 //Cr2O3_2_thickness + 0.5 * Al3_thickness),
// 	    //Al3_Logical, "Al3", world_log, 0, 0);

// 	//G4Tubs *Al4_Solid = new G4Tubs("Al4_Solid", 0. * mm, target_radius,
// 	                               //Al4_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Al4_Logical =
// 	    //new G4LogicalVolume(Al4_Solid, Al, "Al4_Logical");
// 	//G4VisAttributes *Al4_vis = new G4VisAttributes(grey);
// 	//Al4_Logical->SetVisAttributes(Al4_vis);
// 	//new G4PVPlacement(
// 	    //0,
// 	    //G4ThreeVector(0., 0., target_distance +
// 	                              //TargetContainerCap_Thickness + Al1_thickness +
// 	                              //Co1_thickness + Fe1_thickness +
// 	                              //Al2_thickness + Cr2O3_1_thickness +
// 	                              //Sn_thickness + Cr2O3_2_thickness +
// 	                              //Al3_thickness + 0.5 * Al4_thickness),
// 	    //Al4_Logical, "Al4", world_log, 0, 0);

// 	//G4Tubs *Co2_Solid = new G4Tubs("Co2_Solid", 0. * mm, target_radius,
// 	                               //Co2_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Co2_Logical =
// 	    //new G4LogicalVolume(Co2_Solid, Co, "Co2_Logical");
// 	//G4VisAttributes *Co2_vis = new G4VisAttributes(white);
// 	//Co2_Logical->SetVisAttributes(Co2_vis);
// 	//new G4PVPlacement(
// 	    //0,
// 	    //G4ThreeVector(0., 0.,
// 	                  //target_distance +
// 	                      //TargetContainerCap_Thickness + Al1_thickness +
// 	                      //Co1_thickness + Fe1_thickness + Al2_thickness +
// 	                      //Cr2O3_1_thickness + Sn_thickness + Cr2O3_2_thickness +
// 	                      //Al3_thickness + Al4_thickness + 0.5 * Co2_thickness),
// 	    //Co2_Logical, "Co2", world_log, 0, 0);

// 	//G4Tubs *Fe2_Solid = new G4Tubs("Fe2_Solid", 0. * mm, target_radius,
// 	                               //Fe2_thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *Fe2_Logical =
// 	    //new G4LogicalVolume(Fe2_Solid, Fe, "Fe2_Logical");
// 	//G4VisAttributes *Fe2_vis = new G4VisAttributes(red);
// 	//Fe2_Logical->SetVisAttributes(Fe2_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0.,
// 	                     //target_distance +
// 	                         //TargetContainerCap_Thickness + Al1_thickness +
// 	                         //Co1_thickness + Fe1_thickness + Al2_thickness +
// 	                         //Cr2O3_1_thickness + Sn_thickness +
// 	                         //Cr2O3_2_thickness + Al3_thickness + Al4_thickness +
// 	                         //Co2_thickness + 0.5 * Fe2_thickness),
// 	    //Fe2_Logical, "Fe2", world_log, 0, 0);

// 	//G4Tubs *TargetContainerCap2_Solid =
// 	    //new G4Tubs("TargetContainerCap2_Solid", 0. * mm,
// 	               //target_radius + TargetContainerWall_Thickness,
// 	               //TargetContainerCap_Thickness * 0.5, 0., twopi);
// 	//G4LogicalVolume *TargetContainerCap2_Logical = new G4LogicalVolume(
// 	    //TargetContainerCap2_Solid, PE, "TargetContainerCap2_Logical");
// 	//G4VisAttributes *TargetContainerCap2_vis = new G4VisAttributes(cyan);
// 	//TargetContainerCap2_Logical->SetVisAttributes(TargetContainerCap2_vis);
// 	//new G4PVPlacement(
// 	    //0, G4ThreeVector(0., 0., target_distance +
// 	                                 //1.5 * TargetContainerCap_Thickness +
// 	                                 //TargetContainerWall_Length),
// 	    //TargetContainerCap2_Logical, "TargetContainerCap2", world_log, 0, 0);



// /***********************Neutron Moderator*********************/

// // Beginning NeutronMod-----------------------------------

// // G4LogicalVolume *NeutronMod_Logical;
// // G4Box *NeutronMod; 
// // G4VisAttributes *NeutronModvis;




// // NeutronMod=
// // 	    new G4Box("Gadoliniumtar",3*cm,3*cm,3*cm);
// //  NeutronMod_Logical =
// // 	    new G4LogicalVolume(NeutronMod, H2O, "NeutronMod", 0, 0, 0);
// // 	NeutronModvis = new G4VisAttributes(blue);
// // 	NeutronMod_Logical->SetVisAttributes(NeutronModvis);

// // 	new G4PVPlacement(0, G4ThreeVector(trans_x , trans_y,-distcollimatortotarget+
// //  5*cm), NeutronMod_Logical, "block", world_log, 0, 0);
// //End of NeutronMod--------------------------------------------------------
// /************************Gd157-Target********************************/
// //--------------------------------
// G4LogicalVolume *Gadoliniumtar_Logical;
// G4Tubs *Gadoliniumtar; 
// G4VisAttributes *Gadoliniumtarvis;

// Gadoliniumtar=
// 	 new G4Tubs("Fe2_Solid", 0. * mm, 1*cm,
// 	                               0.8*cm * 0.5, 0., twopi);

// Gadoliniumtar_Logical =
// 	    new G4LogicalVolume(Gadoliniumtar, H2O, "Gadoliniumtar", 0, 0, 0);
// Gadoliniumtarvis = new G4VisAttributes(red);
// Gadoliniumtar_Logical->SetVisAttributes(Gadoliniumtarvis);

// 	// new G4PVPlacement(0, G4ThreeVector(trans_x , trans_y,
//  // trans_z), Gadoliniumtar_Logical, "Gd_Target", world_log, 0, 0);
// //------------------------------------
// 	return world_phys;
// }
// void DetectorConstruction::ConstructSDandField() {

// 	// Detector 1
// //	ParticleSD *germanium1_SD = new ParticleSD("germanium1_SD", "germanium1_Hits");
// //	germanium1_SD->SetDetectorID(1);
// //	SetSensitiveDetector("Germanium1_TUD", germanium1_SD, true);

// 	// BGO 1 (4 parts)
// //	ParticleSD *bgo1_1_SD = new ParticleSD("bgo1_1_SD", "bgo1_1_Hits");
// //	bgo1_1_SD->SetDetectorID(11);
// //	SetSensitiveDetector("BGO1_1", bgo1_1_SD, true);
// //
// //	ParticleSD *bgo1_2_SD = new ParticleSD("bgo1_2_SD", "bgo1_2_Hits");
// //	bgo1_2_SD->SetDetectorID(12);
// //	SetSensitiveDetector("BGO1_2", bgo1_2_SD, true);
// //
// //	ParticleSD *bgo1_3_SD = new ParticleSD("bgo1_3_SD", "bgo1_3_Hits");
// //	bgo1_3_SD->SetDetectorID(13);
// //	SetSensitiveDetector("BGO1_3", bgo1_3_SD, true);
// //
// //	ParticleSD *bgo1_4_SD = new ParticleSD("bgo1_4_SD", "bgo1_4_Hits");
// //	bgo1_4_SD->SetDetectorID(14);
// //	SetSensitiveDetector("BGO1_4", bgo1_4_SD, true);
	
// 	// Detector 2
// //	ParticleSD *germanium2_SD = new ParticleSD("germanium2_SD", "germanium2_Hits");
// //	germanium2_SD->SetDetectorID(2);
// //	SetSensitiveDetector("Germanium2_TUD", germanium2_SD, true);

// 	// BGO 2 (4 parts)
// //	ParticleSD *bgo2_1_SD = new ParticleSD("bgo2_1_SD", "bgo2_1_Hits");
// //	bgo2_1_SD->SetDetectorID(21);
// //	SetSensitiveDetector("BGO2_1", bgo2_1_SD, true);
// //
// //	ParticleSD *bgo2_2_SD = new ParticleSD("bgo2_2_SD", "bgo2_2_Hits");
// //	bgo2_2_SD->SetDetectorID(22);
// //	SetSensitiveDetector("BGO2_2", bgo2_2_SD, true);
// //
// //	ParticleSD *bgo2_3_SD = new ParticleSD("bgo2_3_SD", "bgo2_3_Hits");
// //	bgo2_3_SD->SetDetectorID(23);
// //	SetSensitiveDetector("BGO2_3", bgo2_3_SD, true);
// //
// //	ParticleSD *bgo2_4_SD = new ParticleSD("bgo2_4_SD", "bgo2_4_Hits");
// //	bgo2_4_SD->SetDetectorID(24);
// //	SetSensitiveDetector("BGO2_4", bgo2_4_SD, true);
	
// 	// Polarimeter
// //	ParticleSD *polarimeter_SD = new ParticleSD("polarimeter_SD", "polarimeter_Hits");
// //	polarimeter_SD->SetDetectorID(3);
// //	SetSensitiveDetector("Polarimeter_TUD", polarimeter_SD, true);

// 	// BGO Polarimeter (4 parts)
// //	ParticleSD *bgop_1_SD = new ParticleSD("bgop_1_SD", "bgop_1_Hits");
// //	bgop_1_SD->SetDetectorID(31);
// //	SetSensitiveDetector("BGOP_1", bgop_1_SD, true);
// //
// //	ParticleSD *bgop_2_SD = new ParticleSD("bgop_2_SD", "bgop_2_Hits");
// //	bgop_2_SD->SetDetectorID(32);
// //	SetSensitiveDetector("BGOP_2", bgop_2_SD, true);
// //
// //	ParticleSD *bgop_3_SD = new ParticleSD("bgop_3_SD", "bgop_3_Hits");
// //	bgop_3_SD->SetDetectorID(33);
// //	SetSensitiveDetector("BGOP_3", bgop_3_SD, true);
// //
// //	ParticleSD *bgop_4_SD = new ParticleSD("bgop_4_SD", "bgop_4_Hits");
// //	bgop_4_SD->SetDetectorID(34);
// //	SetSensitiveDetector("BGOP_4", bgop_4_SD, true);

// 	// Target container lid which is first hit by the beam 
// 	//ParticleSD *targetContainer_SD = new ParticleSD("targetContainer_SD", "targetContainer_Hits");
// 	//targetContainer_SD->SetDetectorID(0);
// 	//SetSensitiveDetector("TargetContainerCap1_Logical", targetContainer_SD, true);

// // // Gadolinium-Target
// // ParticleSD *GadoliniumTarget_SD = new ParticleSD("GadoliniumTarget_SD", "GadoliniumTarget_Hits");
// // 	GadoliniumTarget_SD->SetDetectorID(0);
// // 	SetSensitiveDetector("Gadoliniumtar", 	GadoliniumTarget_SD, true);
	
	
	
// 	// Radiator targets
// //	ParticleSD *radiatorTarget1_SD = new ParticleSD("radiatorTarget1_SD", "radiatorTarget1_Hits");
// //	radiatorTarget1_SD->SetDetectorID(4);
// //	SetSensitiveDetector("Target_1", radiatorTarget1_SD, true);
// //
// //	ParticleSD *radiatorTarget2_SD = new ParticleSD("radiatorTarget2_SD", "radiatorTarget2_Hits");
// //	radiatorTarget2_SD->SetDetectorID(5);
// //	SetSensitiveDetector("Target_2", radiatorTarget2_SD, true);

// 	ParticleSD *Si_Detector_SD = new ParticleSD("Si_Detector_SD", "Si_Detector_Hits");
// 	Si_Detector_SD->SetDetectorID(1);
// 	SetSensitiveDetector("Si_Detector_Logical", 	Si_Detector_SD, true);

// 	SecondarySD *D_Target_SD = new SecondarySD("D_Target_SD", "D_Target_Hits");
// 	D_Target_SD->SetDetectorID(2);
// 	SetSensitiveDetector("D_Target_Logical", 	D_Target_SD, true);
// }
