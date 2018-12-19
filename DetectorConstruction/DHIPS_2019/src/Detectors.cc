#include "Detectors.hh"

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

using std::cout;
using std::endl;
using std::stringstream;

Detectors::Detectors(G4LogicalVolume *World_Log):World_Logical(World_Log){}


void Detectors::Construct(G4ThreeVector global_coordinates)
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

	G4NistManager* nist = G4NistManager::Instance();

	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");

	Germanium1_TUD* HPGe1 = new Germanium1_TUD("HPGe1");
	Germanium2_TUD* HPGe2 = new Germanium2_TUD("HPGe2") ;
	Polarimeter_TUD* HPGePol = new Polarimeter_TUD("HPGePol") ;
	BGO* BGO1 = new BGO("BGO1");
	BGO* BGO2 = new BGO("BGO2");
	BGO* BGOPol = new BGO("BGOPol");

	G4LogicalVolume* HPGe1_Logical= HPGe1->Get_Logical();
	G4LogicalVolume* HPGe2_Logical= HPGe2->Get_Logical();
	G4LogicalVolume* HPGePol_Logical= HPGePol->Get_Logical();

	G4LogicalVolume* BGO1_Logical= BGO1->Get_Logical();
	G4LogicalVolume* BGO2_Logical= BGO2->Get_Logical();
	G4LogicalVolume* BGOPol_Logical= BGOPol->Get_Logical();

 	G4double g1_theta=90.*deg;
  	G4double g1_phi=5.*deg;

	G4double g2_phi= 140*deg; 
	G4double g2_theta= 90*deg;

  	G4double gPol_theta=90.*deg;
  	G4double gPol_phi=180.*deg;

	G4double detectordistance1 = 225.*mm;
	G4double detectordistance2 = 225.*mm;
	G4double detectordistancePol = 225.*mm;

	G4RotationMatrix* RotDet1 = new G4RotationMatrix();
	RotDet1->rotateY(g1_phi-90*deg);

	G4double HPGe1_Dist = -(detectordistance1 + HPGe1->Get_Length()*0.5 + BGO1->Get_Length()) + BGO1->Get_Max_Penetration_Depth();
  	G4ThreeVector HPGe1_Pos(HPGe1_Dist*sin(g1_theta)*cos(g1_phi), HPGe1_Dist*cos(g1_theta), HPGe1_Dist*sin(g1_theta)*sin(g1_phi));

    G4double BGO1_Dist = -(detectordistance1 + BGO1->Get_Length()*0.5);
    G4ThreeVector BGO1_Pos(BGO1_Dist*sin(g1_theta)*cos(g1_phi), BGO1_Dist*cos(g1_theta), BGO1_Dist*sin(g1_theta)*sin(g1_phi));
  
	new G4PVPlacement(RotDet1, HPGe1_Pos,HPGe1_Logical,"HPGe1",World_Logical,0,0); 
	new G4PVPlacement(RotDet1, BGO1_Pos,BGO1_Logical,"BGO1",World_Logical,0,0); 

	G4RotationMatrix* RotDet2 = new G4RotationMatrix();
	RotDet2->rotateY(g2_phi-90*deg);

	G4double HPGe2_Dist = -(detectordistance2 + HPGe2->Get_Length()*0.5 + BGO2->Get_Length()) + BGO2->Get_Max_Penetration_Depth();
  	G4ThreeVector HPGe2_Pos(HPGe2_Dist*sin(g2_theta)*cos(g2_phi), HPGe2_Dist*cos(g2_theta), HPGe2_Dist*sin(g2_theta)*sin(g2_phi));

    G4double BGO2_Dist = -(detectordistance2 + BGO2->Get_Length()*0.5);
    G4ThreeVector BGO2_Pos(BGO2_Dist*sin(g2_theta)*cos(g2_phi), BGO2_Dist*cos(g2_theta), BGO2_Dist*sin(g2_theta)*sin(g2_phi));

	new G4PVPlacement(RotDet2,HPGe2_Pos,HPGe2_Logical,"HPGe2",World_Logical,0,0); 
	new G4PVPlacement(RotDet2,BGO2_Pos,BGO2_Logical,"BGO2",World_Logical,0,0); 
	
	G4RotationMatrix* RotDetPol = new G4RotationMatrix();
	RotDetPol->rotateY(gPol_phi-90*deg);

	G4double HPGePol_Dist = -(detectordistancePol + HPGePol->Get_Length()*0.5 + BGOPol->Get_Length()) + BGOPol->Get_Max_Penetration_Depth();
  	G4ThreeVector HPGePol_Pos(HPGePol_Dist*sin(gPol_theta)*cos(gPol_phi), HPGePol_Dist*cos(gPol_theta), HPGePol_Dist*sin(gPol_theta)*sin(gPol_phi));

    G4double BGOPol_Dist = -(detectordistancePol + BGOPol->Get_Length()*0.5);
    G4ThreeVector BGOPol_Pos(BGOPol_Dist*sin(gPol_theta)*cos(gPol_phi), BGOPol_Dist*cos(gPol_theta), BGOPol_Dist*sin(gPol_theta)*sin(gPol_phi));


	new G4PVPlacement(RotDetPol,HPGePol_Pos,HPGePol_Logical,"HPGePol",World_Logical,0,0); 
	new G4PVPlacement(RotDetPol, BGOPol_Pos,BGOPol_Logical,"BGOPol",World_Logical,0,0); 


}