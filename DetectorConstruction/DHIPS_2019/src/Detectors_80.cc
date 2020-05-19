#include "Detectors_80.hh"

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

Detectors_80::Detectors_80(G4LogicalVolume *World_Log)
	: BGO1(BGO("BGO1"))
	, BGO2(BGO("BGO2"))
	, BGOPol(BGO("BGOPol"))
	, World_Logical(World_Log) {}

void Detectors_80::Construct(G4ThreeVector global_coordinates)
{
	HPGe_Collection hpge_Collection;

	// HPGe80
	HPGe_Coaxial HPGe80(World_Logical, "HPGe80");
	HPGe80.setProperties(hpge_Collection.HPGe_80_TUD_90006);
	HPGe80.useDewar();

	// HPGe2
	HPGe_Coaxial HPGe2(World_Logical, "HPGe2");
	HPGe2.setProperties(hpge_Collection.HPGe_100_TUD_72902);
	//HPGe2.useDewar();  // Dewar does not fit into implemented Lead Castle
	
	// Polarimeter 
	HPGe_Coaxial HPGePol(World_Logical, "HPGePol");
	HPGePol.setProperties(hpge_Collection.HPGe_100_TUD_72930);
	HPGePol.useDewar();

	G4LogicalVolume *BGO1_Logical = BGO1.Get_Logical();
	G4LogicalVolume *BGO2_Logical = BGO2.Get_Logical();
	G4LogicalVolume *BGOPol_Logical = BGOPol.Get_Logical();

	G4RotationMatrix *RotDet1 = new G4RotationMatrix();
	RotDet1->rotateY(g80_phi - 90 * deg);

	G4double BGO1_Dist = -(detectordistance80 + BGO1.Get_Length() * 0.5);
	G4ThreeVector BGO1_Pos(BGO1_Dist * sin(g80_theta) * cos(g80_phi), BGO1_Dist * cos(g80_theta), BGO1_Dist * sin(g80_theta) * sin(g80_phi));

	new G4PVPlacement(RotDet1, global_coordinates + BGO1_Pos, BGO1_Logical, "BGO1", World_Logical, 0, 0);
	HPGe80.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - g80_phi, 0., detectordistance80 + (BGO1.Get_Length() - BGO1.Get_Max_Penetration_Depth()));

	G4RotationMatrix *RotDet2 = new G4RotationMatrix();
	RotDet2->rotateY(g2_phi - 90 * deg);

	G4double BGO2_Dist = -(detectordistance2 + BGO2.Get_Length() * 0.5);
	G4ThreeVector BGO2_Pos(BGO2_Dist * sin(g2_theta) * cos(g2_phi), BGO2_Dist * cos(g2_theta), BGO2_Dist * sin(g2_theta) * sin(g2_phi));

	new G4PVPlacement(RotDet2, global_coordinates + BGO2_Pos, BGO2_Logical, "BGO2", World_Logical, 0, 0);
	HPGe2.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - g2_phi, 0., detectordistance2 + (BGO2.Get_Length() - BGO2.Get_Max_Penetration_Depth()));

	G4RotationMatrix *RotDetPol = new G4RotationMatrix();
	RotDetPol->rotateY(gPol_phi - 90 * deg);

	G4double BGOPol_Dist = -(detectordistancePol + BGOPol.Get_Length() * 0.5);
	G4ThreeVector BGOPol_Pos(BGOPol_Dist * sin(gPol_theta) * cos(gPol_phi), BGOPol_Dist * cos(gPol_theta), BGOPol_Dist * sin(gPol_theta) * sin(gPol_phi));

	new G4PVPlacement(RotDetPol, global_coordinates + BGOPol_Pos, BGOPol_Logical, "BGOPol", World_Logical, 0, 0);
	HPGePol.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - gPol_phi, 0., detectordistancePol + (BGOPol.Get_Length() - BGOPol.Get_Max_Penetration_Depth()));
}
void Detectors_80::ConstructDetectorFilter(G4ThreeVector global_coordinates, std::string det, G4double CuLength, G4double PbLength)
{
	bool detcheck = false;
	G4double det_phi = 0;
	G4double det_theta = 0;
	G4double det_dist = 0;
	if (det == "HPGe80")
	{
		det_phi = g80_phi;
		det_theta = g80_theta;
		det_dist = detectordistance80;
		detcheck = true;
	}
	else if (det == "HPGe2")
	{
		det_phi = g2_phi;
		det_theta = g2_theta;
		det_dist = detectordistance2;
		detcheck = true;
	}
	else if (det == "HPGePol")
	{
		det_phi = gPol_phi;
		det_theta = gPol_theta;
		det_dist = detectordistancePol;
		detcheck = true;
	}
	else
	{
		printf("\n");
		printf("No Proper detector name used. Please use: HPGe80, HPGe2 or HPGePol in function ConstructDetectorFilter\n");
		printf("No filters were used in ConstructDetectorFilter(%s,%f,%f)", det.c_str(), CuLength, PbLength);
		printf("\n");
		abort();
	}
	if (detcheck)
	{
		G4RotationMatrix *RotDet = new G4RotationMatrix();
		RotDet->rotateY(det_phi - 90 * deg);
		G4NistManager *nist = G4NistManager::Instance();
		G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu");
		G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");
		G4Tubs *Cu_Filter_Solid = new G4Tubs("Cu_Filter_Solid", 0, 25 * mm, CuLength * 0.5, 0. * deg, 360. * deg);
		G4Tubs *Pb_Filter_Solid = new G4Tubs("Pb_Filter_Solid", 0, 25 * mm, PbLength * 0.5, 0. * deg, 360. * deg);

		G4LogicalVolume *Cu_Filter_Logical = new G4LogicalVolume(Cu_Filter_Solid, Cu, "Cu_Filter_Logical", 0, 0, 0);
		G4LogicalVolume *Pb_Filter_Logical = new G4LogicalVolume(Pb_Filter_Solid, Pb, "Pb_Filter_Logical", 0, 0, 0);

		Cu_Filter_Logical->SetVisAttributes(G4Color::Brown());
		Pb_Filter_Logical->SetVisAttributes(G4Color::Grey());

		G4double Cu_dist = -det_dist + 0.5 * CuLength;
		G4double Pb_dist = -det_dist + 0.5 * PbLength + CuLength;

		G4ThreeVector Pb_Filter_Dist(Pb_dist * sin(det_theta) * cos(det_phi), Pb_dist * cos(det_theta), Pb_dist * sin(det_theta) * sin(det_phi));
		G4ThreeVector Cu_Filter_Dist(Cu_dist * sin(det_theta) * cos(det_phi), Cu_dist * cos(det_theta), Cu_dist * sin(det_theta) * sin(det_phi));

		new G4PVPlacement(RotDet, global_coordinates + Cu_Filter_Dist, Cu_Filter_Logical, (det + "_Cu_Filter").c_str(), World_Logical, 0, 0);
		new G4PVPlacement(RotDet, global_coordinates + Pb_Filter_Dist, Pb_Filter_Logical, (det + "_Pb_Filter").c_str(), World_Logical, 0, 0);
	}
}
