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

Detectors::Detectors(G4LogicalVolume *World_Log)
	: BGO1(BGO("BGO1", World_Log))
	, BGO2(BGO("BGO2", World_Log))
	, BGOPol(BGO("BGOPol", World_Log))
	, World_Logical(World_Log) {}

void Detectors::Construct(G4ThreeVector global_coordinates)
{
	HPGe_Collection hpge_Collection;

	// HPGe1
	HPGe_Coaxial HPGe1(World_Logical, "HPGe1");
	HPGe1.setProperties(hpge_Collection.HPGe_100_TUD_73760);
	HPGe1.useDewar();

	// HPGe2
	HPGe_Coaxial HPGe2(World_Logical, "HPGe2");
	HPGe2.setProperties(hpge_Collection.HPGe_100_TUD_72902);
	//HPGe2.useDewar();  // Dewar does not fit into implemented Lead Castle
	
	// Polarimeter 
	HPGe_Coaxial HPGePol(World_Logical, "HPGePol");
	HPGePol.setProperties(hpge_Collection.HPGe_100_TUD_72930);
	HPGePol.useDewar();

	G4double BGO1_Dist = -(detectordistance1 + BGO1.Get_Length() * 0.5);

	BGO1.Construct(global_coordinates, g1_theta, g1_phi, BGO1_Dist);
	HPGe1.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - g1_phi, 0., detectordistance1 + (BGO1.Get_Length() - BGO1.Get_Max_Penetration_Depth()));

	G4double BGO2_Dist = -(detectordistance2 + BGO2.Get_Length() * 0.5);

	BGO2.Construct(global_coordinates, g2_theta, g2_phi, BGO2_Dist);
	HPGe2.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - g2_phi, 0., detectordistance2 + (BGO2.Get_Length() - BGO2.Get_Max_Penetration_Depth()));

	G4double BGOPol_Dist = -(detectordistancePol + BGOPol.Get_Length() * 0.5);

	BGOPol.Construct(global_coordinates, gPol_theta, gPol_phi, BGOPol_Dist);
	HPGePol.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
			-90.*deg - gPol_phi, 0., detectordistancePol + (BGOPol.Get_Length() - BGOPol.Get_Max_Penetration_Depth()));
}
void Detectors::ConstructDetectorFilter(G4ThreeVector global_coordinates, std::string det, G4double CuLength, G4double PbLength)
{
	bool detcheck = false;
	G4double det_phi = 0;
	G4double det_theta = 0;
	G4double det_dist = 0;
	if (det == "HPGe1")
	{
		det_phi = g1_phi;
		det_theta = g1_theta;
		det_dist = detectordistance1;
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
		printf("No Proper detector name used. Please use: HPGe1, HPGe2 or HPGePol in function ConstructDetectorFilter\n");
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
