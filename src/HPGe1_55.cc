//**************************************************************//
//	55%  HPGe Detector 1 @ HIGS Serial No. 41-TN31524A
//**************************************************************//

#include "HPGe1_55.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

HPGe1_55::HPGe1_55(G4String Detector_Name) {

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

	G4NistManager *man = G4NistManager::Instance();

	G4Material *Cu = man->FindOrBuildMaterial("G4_Cu");
	G4Material *Al = man->FindOrBuildMaterial("G4_Al");
	G4Material *Ge = man->FindOrBuildMaterial("G4_Ge");
	G4Material *vacuum = man->FindOrBuildMaterial("G4_Galactic");
	//  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
	G4Material *Be = man->FindOrBuildMaterial("G4_Be");

	// Detector dimensions and materials as given in the ORTEC data sheet.

	G4double Detector_Radius = 67.1 * mm / 2.;
	G4double Detector_Length = 63.1 * mm;
	G4double Detector_End_Radius = 8. * mm; // (J)
	G4double Hole_Radius = 8.7 * mm / 2.;
	G4double Hole_Depth = 54.3 * mm;
	G4double Hole_Bottom_Radius = Hole_Radius;

	G4double MountCup_Length = 105. * mm;      // (A)
	G4double End_Cap_To_Crystal_Gap = 4. * mm; // (B)
	G4double MountCup_Base = 3.2 * mm;         // (C)
	G4double EndCap_Window = 0.5 * mm;         // (D)
	G4double MountCup_Wall = 0.76 * mm;        // (H)
	G4double EndCap_Wall = 1. * mm;            // (I)

	G4double ColdFinger_Radius =
	    4. * mm / 2; // Brent Fallin's suggestion, not in the ORTEC data sheet.
	G4double ColdFinger_Length =
	    MountCup_Base +
	    (MountCup_Length - MountCup_Wall - MountCup_Base - Detector_Length) +
	    Hole_Depth - 5. * mm; // 5mm less than tip of the hole.

	G4Material *Mother_Material = vacuum;
	G4Material *MountCup_Material = Al;
	G4Material *EndCap_Material = Al;
	G4Material *EndCap_Window_Material = Be;
	G4Material *ColdFinger_Material = Cu;
	G4Material *Crystal_Material = Ge;

	// Mother Volume (tub of air)

	G4double Mother_Radius =
	    Detector_Radius + MountCup_Wall + End_Cap_To_Crystal_Gap + EndCap_Wall;
	G4double Mother_Length =
	    EndCap_Window + End_Cap_To_Crystal_Gap + MountCup_Length;

	Length = Mother_Length;
	Radius = Mother_Radius;

	G4Tubs *Mother_Solid = new G4Tubs("Mother_Solid", 0., Mother_Radius,
	                                  Mother_Length / 2, 0. * deg, 360. * deg);
	HPGe1_55_Logical = new G4LogicalVolume(Mother_Solid, Mother_Material,
	                                       "Mother_Logical", 0, 0, 0);

	HPGe1_55_Logical->SetVisAttributes(new G4VisAttributes(red));

	// End Cap

	G4double EndCap_innerRadius =
	    Detector_Radius + MountCup_Wall + End_Cap_To_Crystal_Gap;
	G4double EndCap_outerRadius =
	    Detector_Radius + MountCup_Wall + End_Cap_To_Crystal_Gap + EndCap_Wall;

	G4double EndCap_Length = MountCup_Length + End_Cap_To_Crystal_Gap;

	G4Tubs *EndCap_Solid =
	    new G4Tubs("EndCap_Solid", EndCap_innerRadius, EndCap_outerRadius,
	               EndCap_Length / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *EndCap_Logical = new G4LogicalVolume(
	    EndCap_Solid, EndCap_Material, "EndCap_Logical", 0, 0, 0);

	EndCap_Logical->SetVisAttributes(new G4VisAttributes(gray));

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length / 2 - EndCap_Length / 2 -
	                                               EndCap_Window),
	                  EndCap_Logical, "EndCap", HPGe1_55_Logical, false, 0);

	// End Cap Window

	G4double EndCap_Window_Radius = EndCap_outerRadius;
	//  G4double EndCap_Window_Length = 0.5*mm;

	G4Tubs *EndCap_Window_Solid =
	    new G4Tubs("EndCap_Window_Solid", 0., EndCap_Window_Radius,
	               EndCap_Window / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *EndCap_Window_Logical =
	    new G4LogicalVolume(EndCap_Window_Solid, EndCap_Window_Material,
	                        "EndCap_Window_Logical", 0, 0, 0);

	EndCap_Window_Logical->SetVisAttributes(new G4VisAttributes(gray));

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length / 2 - EndCap_Window / 2),
	                  EndCap_Window_Logical, "EndCap_Window", HPGe1_55_Logical,
	                  false, 0);

	// Mount Cup Wall

	G4double MountCup_innerRadius = Detector_Radius;
	G4double MountCup_outerRadius = Detector_Radius + MountCup_Wall;
	G4double MountCup_Wall_Length =
	    MountCup_Length - MountCup_Wall - MountCup_Base;

	G4Tubs *MountCup_Solid =
	    new G4Tubs("MountCup_Solid", MountCup_innerRadius, MountCup_outerRadius,
	               MountCup_Wall_Length / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *MountCup_Logical = new G4LogicalVolume(
	    MountCup_Solid, MountCup_Material, "MountCup_Logical", 0, 0, 0);

	MountCup_Logical->SetVisAttributes(new G4VisAttributes(cyan));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., Length / 2 - EndCap_Window -
	                                 End_Cap_To_Crystal_Gap - MountCup_Wall -
	                                 MountCup_Wall_Length / 2),
	    MountCup_Logical, "MountCup_Wall", HPGe1_55_Logical, false, 0);

	// Mount Cup Base

	G4double MountCup_Base_outerRadius = Detector_Radius + MountCup_Wall;
	G4double MountCup_Base_innerRadius = Hole_Bottom_Radius;

	G4Tubs *MountCup_Base_Solid = new G4Tubs(
	    "MountCup_Base_Solid", MountCup_Base_innerRadius,
	    MountCup_Base_outerRadius, MountCup_Base / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *MountCup_Base_Logical =
	    new G4LogicalVolume(MountCup_Base_Solid, MountCup_Material,
	                        "MountCup_Base_Logical", 0, 0, 0);

	MountCup_Base_Logical->SetVisAttributes(new G4VisAttributes(cyan));

	new G4PVPlacement(0, G4ThreeVector(0., 0., -Length / 2 + MountCup_Base / 2),
	                  MountCup_Base_Logical, "MountCupBase", HPGe1_55_Logical,
	                  false, 0);

	// Mount Cup Face

	G4double MountCup_Face_Radius = Detector_Radius + MountCup_Wall;

	G4Tubs *MountCup_Face_Solid =
	    new G4Tubs("MountCup_Face_Solid", 0., MountCup_Face_Radius,
	               MountCup_Wall / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *MountCup_Face_Logical =
	    new G4LogicalVolume(MountCup_Face_Solid, MountCup_Material,
	                        "MountCup_Face_Logical", 0, 0, 0);

	MountCup_Face_Logical->SetVisAttributes(new G4VisAttributes(cyan));

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length / 2 - EndCap_Window -
	                                               End_Cap_To_Crystal_Gap -
	                                               MountCup_Wall / 2),
	                  MountCup_Face_Logical, "MountCup_Face", HPGe1_55_Logical,
	                  false, 0);

	// Cold finger

	G4Tubs *ColdFinger_Tub =
	    new G4Tubs("ColdFinger_Tub", 0., ColdFinger_Radius,
	               ColdFinger_Length / 2, 0. * deg, 360. * deg);
	G4Sphere *ColdFinger_Tip =
	    new G4Sphere("ColdFinger_Tip", 0., ColdFinger_Radius, 0. * deg,
	                 360. * deg, 0. * deg, 180. * deg);

	G4UnionSolid *ColdFinger_Solid =
	    new G4UnionSolid("ColdFinger_Solid", ColdFinger_Tub, ColdFinger_Tip, 0,
	                     G4ThreeVector(0., 0., ColdFinger_Length / 2));

	G4LogicalVolume *ColdFinger_Logical = new G4LogicalVolume(
	    ColdFinger_Solid, ColdFinger_Material, "ColdFinger_Logical", 0, 0, 0);

	ColdFinger_Logical->SetVisAttributes(new G4VisAttributes(orange));

	new G4PVPlacement(
	    0, G4ThreeVector(0., 0., -Length / 2 + ColdFinger_Length / 2),
	    ColdFinger_Logical, "ColdFinger", HPGe1_55_Logical, false, 0);

	// Germanium Detector Crystal

	G4Tubs *Crystal_Body = new G4Tubs(
	    "Crystal_Body", 0., Detector_Radius,
	    (Detector_Length - Detector_End_Radius) / 2, 0. * deg, 360. * deg);
	G4Tubs *Crystal_Face =
	    new G4Tubs("Crystal_Face", 0., Detector_Radius - Detector_End_Radius,
	               Detector_End_Radius, 0. * deg, 360. * deg);
	G4Torus *Crystal_Edge = new G4Torus("Crystal_Edge", 0., Detector_End_Radius,
	                                    Detector_Radius - Detector_End_Radius,
	                                    0. * deg, 360. * deg);

	G4UnionSolid *Crystal_Tub1 = new G4UnionSolid(
	    "Crystal_Tub1", Crystal_Body, Crystal_Edge, 0,
	    G4ThreeVector(0., 0., (Detector_Length - Detector_End_Radius) / 2));
	G4UnionSolid *Crystal_Tub = new G4UnionSolid(
	    "Crystal_Tub", Crystal_Tub1, Crystal_Face, 0,
	    G4ThreeVector(0., 0., (Detector_Length - Detector_End_Radius) / 2));

	G4Tubs *Hole_Tub = new G4Tubs("Hole_Tub", 0., Hole_Radius, Hole_Depth / 2,
	                              0. * deg, 360. * deg);
	G4Sphere *Hole_Tip = new G4Sphere("Hole_Tip", 0., Hole_Radius, 0.,
	                                  360. * deg, 0., 90. * deg);

	G4UnionSolid *Crystal_Hole =
	    new G4UnionSolid("Crystal_Hole", Hole_Tub, Hole_Tip, 0,
	                     G4ThreeVector(0., 0., Hole_Depth / 2));

	G4SubtractionSolid *crystal_Solid = new G4SubtractionSolid(
	    "crystal_Solid", Crystal_Tub, Crystal_Hole, 0,
	    G4ThreeVector(0., 0., -(Detector_Length - Hole_Depth) / 2));
	G4LogicalVolume *Crystal_Logical = new G4LogicalVolume(
	    crystal_Solid, Crystal_Material, Detector_Name, 0, 0, 0);

	Crystal_Logical->SetVisAttributes(new G4VisAttributes(green));

	new G4PVPlacement(0, G4ThreeVector(0., 0., Length / 2 - EndCap_Window -
	                                               End_Cap_To_Crystal_Gap -
	                                               MountCup_Wall -
	                                               Detector_Length / 2 -
	                                               Detector_End_Radius / 2),
	                  Crystal_Logical, "Crystal", HPGe1_55_Logical, false, 0);
}
