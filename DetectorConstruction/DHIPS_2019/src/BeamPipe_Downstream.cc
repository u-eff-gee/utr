
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

#include "BeamPipe_Downstream.hh"
using std::cout;
using std::endl;
using std::stringstream;

BeamPipe_Downstream::BeamPipe_Downstream(G4LogicalVolume *World_Log):World_Logical(World_Log){}


void BeamPipe_Downstream::Construct(G4ThreeVector global_coordinates,G4double relative_density)
{

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

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
	G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	G4Material *Si = nist->FindOrBuildMaterial("G4_Si");

	Vacuum vac(relative_density, "beampipe_downstream_vacuum");
	G4Material *vacuum = vac.Get_Material();

	G4double diff=0.5*(Chamber_Outer_Length-Chamber_Inner_Length);
//////////////////////
	// BeamTube
////////////////////////// 
	G4Tubs *beamPipe_NRF_Solid = new G4Tubs("beamPipe_NRF_Solid", beamPipe_NRF_Inner_Radius, beamPipe_NRF_Outer_Radius, beamPipe_NRF_Lenght*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_NRF_Logical = new G4LogicalVolume(beamPipe_NRF_Solid, PE, "beamPipe_NRF_Logical");
	beamPipe_NRF_Logical->SetVisAttributes(green);

////////////////////////
	// Vacuum in Tube
////////////////////////// 
	G4Tubs *beamPipe_Vacuum_NRF_Solid = new G4Tubs("beamPipe_Vacuum_NRF_Solid", 0,beamPipe_NRF_Inner_Radius, -beamPipe_NRF_Window_Thickness*0.5+beamPipe_NRF_Lenght*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_Vacuum_NRF_Logical = new G4LogicalVolume(beamPipe_Vacuum_NRF_Solid, vacuum, "beamPipe_Vacuum_NRF_Logical");
	beamPipe_Vacuum_NRF_Logical->SetVisAttributes(white);

////////////////////////
	// Lid of Tube
////////////////////////// 
	G4Tubs *beamPipe_NRF_Lid_Solid = new G4Tubs("beamPipe_NRF_Lid_Solid", 0., beamPipe_NRF_Inner_Radius, beamPipe_NRF_Window_Thickness*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_NRF_Lid_Logical = new G4LogicalVolume(beamPipe_NRF_Lid_Solid, PE, "beamPipe_NRF_Lid_Logical");
	beamPipe_NRF_Lid_Logical->SetVisAttributes(green);

////////////////////////
	// Chamber 
////////////////////////// 
	G4Box *Chamber_Block_Solid=new G4Box("Chamber_Block_Solid",Chamber_Outer_Length*0.5,Chamber_Outer_Length*0.5, Chamber_Outer_Length*0.5);
  	G4LogicalVolume* Chamber_Block_Logical = new G4LogicalVolume(Chamber_Block_Solid, vacuum, "Chamber_Block_Logical", 0, 0, 0);
	Chamber_Block_Logical->SetVisAttributes(green);

//////////////////////////
	// Vacuum in Chamber
////////////////////////// 
	G4Box *Chamber_Vacuum_Block_Solid=new G4Box("Chamber_Vacuum_Block_Solid",Chamber_Inner_Length*0.5,Chamber_Inner_Length*0.5, Chamber_Inner_Length*0.5);
  	G4LogicalVolume* Chamber_Vacuum_Block_Logical = new G4LogicalVolume(Chamber_Vacuum_Block_Solid, vacuum, "Chamber_Vacuum_Block_Logical", 0, 0, 0);
	Chamber_Vacuum_Block_Logical->SetVisAttributes(white);

////////////////////////
	// Deuterium Target
////////////////////////// 
	G4double D_Thickness = 10*mm;
	G4double D_Radius    = 20*mm;

	G4Tubs *D_Target_Solid= new G4Tubs("D_Target", 0, D_Radius, D_Thickness*0.5, 0., twopi);

	G4LogicalVolume *D_Target_Logical = new G4LogicalVolume(D_Target_Solid, Pb, "D_Target_Logical");
	D_Target_Logical->SetVisAttributes(red);

	G4RotationMatrix* D_Rotate=new G4RotationMatrix();
	D_Rotate->rotateX(-45*deg);
	D_Rotate->rotateY(45*deg);

//////////////////////////
	// Si-Detector
//////////////////////////

	G4double Si_Radius = 13.81*mm; //coresponding to 600mm^2 active area
	G4double Si_Thickness=10*mm;

	G4double Si_Radius_Shield = 36.1*mm; 
	G4double Si_Thickness_Shield=36*mm;

	G4Tubs* Si_Radius_Shield_Solid=new G4Tubs("Si_Radius_Shield_Solid", 0., Si_Radius_Shield, Si_Thickness_Shield*0.5, 0., twopi);
	G4LogicalVolume *Si_Radius_Shield_Logical = new G4LogicalVolume(Si_Radius_Shield_Solid, Al, "Si_Radius_Shield_Logical");
	Si_Radius_Shield_Logical->SetVisAttributes(blue);

	G4RotationMatrix* Si_Rotate1=new G4RotationMatrix();
	Si_Rotate1->rotateY(90*deg);

	G4RotationMatrix* Si_Rotate2=new G4RotationMatrix();
	Si_Rotate2->rotateY(-90*deg);

	G4Tubs* Si_Detector_Solid=new G4Tubs("Si_Detector_Solid", 0., Si_Radius, Si_Thickness*0.5, 0., twopi);
	G4LogicalVolume *Si_Detector_Logical = new G4LogicalVolume(Si_Detector_Solid, Si, "Si_Detector_Logical");
	Si_Detector_Logical->SetVisAttributes(orange);

/*Pipe*/	    	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., (-Chamber_Outer_Length+diff)*0.5), beamPipe_NRF_Logical, "beamPipe_NRF_Logical", World_Logical, false, 0);
/*Pipe Vacuum*/		new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0., beamPipe_NRF_Window_Thickness*0.5+(-Chamber_Outer_Length+diff)*0.5), beamPipe_Vacuum_NRF_Logical, "beamPipe_Vacuum_NRF_Logical", World_Logical, false, 0);
/*Lid*/     		new G4PVPlacement(0, global_coordinates+G4ThreeVector(0., 0.,-beamPipe_NRF_Lenght*0.5+beamPipe_NRF_Window_Thickness*0.5+(-Chamber_Outer_Length+diff)*0.5), beamPipe_NRF_Lid_Logical, "beamPipe_NRF_Lid_Logical", World_Logical, false, 0);
/*Chamber*/ 		new G4PVPlacement(0, global_coordinates+G4ThreeVector(0 , 0,  (-Chamber_Outer_Length+beamPipe_NRF_Lenght+Chamber_Inner_Length+diff)*0.5), Chamber_Block_Logical, "Chamber_Block", World_Logical, 0, 0);
/*Chamber Vaccum*/	new G4PVPlacement(0, G4ThreeVector(0 , 0, 0), Chamber_Vacuum_Block_Logical, "Chamber_Vacuum_Block", Chamber_Block_Logical, 0, 0);
/*D_Target*/		new G4PVPlacement(D_Rotate, G4ThreeVector(0 , 0, 0), D_Target_Logical, "D_Target", Chamber_Vacuum_Block_Logical, 0, 0);
/*Si Shield*/		new G4PVPlacement(Si_Rotate1, G4ThreeVector(SiDistanceToTarget+Si_Thickness_Shield*0.5 , 0, 0), Si_Radius_Shield_Logical, "Si_Radius_Shield1", Chamber_Vacuum_Block_Logical, 0, 0);
/*Si Shield*/		new G4PVPlacement(Si_Rotate2, G4ThreeVector(-SiDistanceToTarget-Si_Thickness_Shield*0.5 , 0, 0), Si_Radius_Shield_Logical, "Si_Radius_Shield2", Chamber_Vacuum_Block_Logical, 0, 0);
/*Si*/	        	new G4PVPlacement(Si_Rotate1, G4ThreeVector(SiDistanceToTarget+Si_Thickness*0.5 , 0, 0), Si_Detector_Logical, "Si_Detector1", Chamber_Vacuum_Block_Logical, 0, 0);
/*Si*/	        	new G4PVPlacement(Si_Rotate2, G4ThreeVector(-SiDistanceToTarget-Si_Thickness*0.5 , 0, 0), Si_Detector_Logical, "Si_Detector2", Chamber_Vacuum_Block_Logical, 0, 0);


}