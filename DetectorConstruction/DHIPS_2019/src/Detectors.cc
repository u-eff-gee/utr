#include "Detectors.hh"

#include <iostream>
#include <sstream>
#include <string>

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

  	G4RotationMatrix* RotDet1 = new G4RotationMatrix();
	RotDet1->rotateY(g1_phi-90*deg);

    G4double BGO1_Dist = -(detectordistance1 + BGO1->Get_Length()*0.5);
    G4ThreeVector BGO1_Pos(BGO1_Dist*sin(g1_theta)*cos(g1_phi), BGO1_Dist*cos(g1_theta), BGO1_Dist*sin(g1_theta)*sin(g1_phi));

	new G4PVPlacement(RotDet1, global_coordinates+BGO1_Pos,BGO1_Logical,"BGO1",World_Logical,0,0);   
	new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*BGO1->Get_Length()+BGO1->Get_Max_Penetration_Depth()-0.5*HPGe1->Get_Length()),HPGe1_Logical,"HPGe1",BGO1_Logical,0,0); 


  	G4RotationMatrix* RotDet2 = new G4RotationMatrix();
	RotDet2->rotateY(g2_phi-90*deg);

    G4double BGO2_Dist = -(detectordistance2 + BGO2->Get_Length()*0.5);
    G4ThreeVector BGO2_Pos(BGO2_Dist*sin(g2_theta)*cos(g2_phi), BGO2_Dist*cos(g2_theta), BGO2_Dist*sin(g2_theta)*sin(g2_phi));

	new G4PVPlacement(RotDet2,global_coordinates+BGO2_Pos,BGO2_Logical,"BGO2",World_Logical,0,0); 
	new G4PVPlacement(0,G4ThreeVector(0,0,-0.5*BGO1->Get_Length()+BGO2->Get_Max_Penetration_Depth()-0.5*HPGe2->Get_Length()),HPGe2_Logical,"HPGe2",BGO2_Logical,0,0); 
	

	G4RotationMatrix* RotDetPol = new G4RotationMatrix();
	RotDetPol->rotateY(gPol_phi-90*deg);

    G4double BGOPol_Dist = -(detectordistancePol + BGOPol->Get_Length()*0.5);
    G4ThreeVector BGOPol_Pos(BGOPol_Dist*sin(gPol_theta)*cos(gPol_phi), BGOPol_Dist*cos(gPol_theta), BGOPol_Dist*sin(gPol_theta)*sin(gPol_phi));

	new G4PVPlacement(RotDetPol,global_coordinates+ BGOPol_Pos,BGOPol_Logical,"BGOPol",World_Logical,0,0); 
	new G4PVPlacement(0,G4ThreeVector(0,0,-0.5*BGO1->Get_Length()+BGOPol->Get_Max_Penetration_Depth()-0.5*HPGePol->Get_Length()),HPGePol_Logical,"HPGePol",BGOPol_Logical,0,0); 
	
}

void Detectors::ConstructDetectorFilter(G4ThreeVector global_coordinates,std::string det, G4double CuLength, G4double PbLength)
{
	bool detcheck = false;
	G4double det_phi=0;
	G4double det_theta=0;
	G4double det_dist=0;
	if(det=="Det1")
		{
			det_phi=g1_phi;
			det_theta=g1_theta;
			det_dist=detectordistance1;
			detcheck=true;
		}
		else if(det=="Det2")
			{
				det_phi=g2_phi;
				det_theta=g2_theta;
				det_dist=detectordistance2;
				detcheck=true;
			}
			else if(det=="DetPol")
				{
					det_phi=gPol_phi;
					det_theta=gPol_theta;
					det_dist=detectordistancePol;
					detcheck=true;
				}
				else
				{
					printf("\n");
					printf("No Proper detector name used. Please use: Det1, Det2 or DetPol in function ConstructDetectorFilter\n");
					printf("No filters were used in ConstructDetectorFilter(%s,%f,%f)",det.c_str(),CuLength,PbLength);
					printf("\n");
					abort();
				}
	if(detcheck)
	{
		G4Colour  orange (1.0, 0.5, 0.0) ;
		G4Colour  grey   (0.5, 0.5, 0.5) ;
		G4RotationMatrix* RotDet= new G4RotationMatrix();
		RotDet->rotateY(det_phi-90*deg);
		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
		G4Material *Pb= nist->FindOrBuildMaterial("G4_Pb");
		G4Tubs* Cu_Filter_Solid= new G4Tubs("Cu_Filter_Solid", 0, 25*mm, CuLength*0.5, 0. * deg, 360. * deg);
		G4Tubs* Pb_Filter_Solid= new G4Tubs("Pb_Filter_Solid", 0, 25*mm, PbLength*0.5, 0. * deg, 360. * deg);

		G4LogicalVolume* Cu_Filter_Logical = new G4LogicalVolume(Cu_Filter_Solid, Cu, "Cu_Filter_Logical", 0, 0, 0);
		G4LogicalVolume* Pb_Filter_Logical = new G4LogicalVolume(Pb_Filter_Solid, Pb, "Pb_Filter_Solid", 0, 0, 0);

		Cu_Filter_Logical->SetVisAttributes(orange);
		Pb_Filter_Logical->SetVisAttributes(grey);

		G4double Cu_dist=-det_dist+0.5*CuLength;
		G4double Pb_dist=-det_dist+0.5*PbLength+CuLength;


		G4ThreeVector Pb_Filter_Dist(Pb_dist*sin(det_theta)*cos(det_phi), Pb_dist*cos(det_theta), Pb_dist*sin(det_theta)*sin(det_phi));
		G4ThreeVector Cu_Filter_Dist(Cu_dist*sin(det_theta)*cos(det_phi), Cu_dist*cos(det_theta), Cu_dist*sin(det_theta)*sin(det_phi));

		new G4PVPlacement(RotDet,global_coordinates+Cu_Filter_Dist,Cu_Filter_Logical,(det+"_Cu_Filter").c_str(),World_Logical,0,0); 
		new G4PVPlacement(RotDet,global_coordinates+Pb_Filter_Dist,Pb_Filter_Logical,(det+"_Pb_Filter").c_str(),World_Logical,0,0); 
	


	}


}