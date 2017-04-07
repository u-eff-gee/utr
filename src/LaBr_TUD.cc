//**************************************************************//
//	LaBr Crystal (Saint Gobain BrilLanCe 380) 3x3" (TUD)
//**************************************************************//

#include "LaBr_TUD.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "Materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

LaBr_TUD::LaBr_TUD(G4String Detector_Name)
    : lengthCrystal(76.2 * mm), radiusCrystal(76.2 / 2. * mm),
      thicknessHousing(0.5 * mm), radiusHousing(85.2 / 2. * mm),
      thicknessVacuumFront(2. * mm), thicknessVacuumBack(2. * mm) {
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
	Materials *mat = Materials::Instance();

	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
	G4Material *LaBr3Ce = mat->Get_LaBr3Ce();

	/*********************** LaBr mother volume *********************/

	G4double Thickness_Vacuum_front = thicknessVacuumFront;
	G4double Thickness_Vacuum_back = thicknessVacuumBack;
	G4double AluCase_Radius = radiusHousing;
	G4double AluCase_Thickness = thicknessHousing;
	G4double AluCase_Length = 2 * AluCase_Thickness + lengthCrystal +
	                          Thickness_Vacuum_front + Thickness_Vacuum_back;

	Length = AluCase_Length;
	Radius = AluCase_Radius;

	G4Tubs *AluCase_Solid =
	    new G4Tubs("AluCase1_Solid", 0, AluCase_Radius, AluCase_Length / 2,
	               0. * deg, 360. * deg);
	detectorLogical =
	    new G4LogicalVolume(AluCase_Solid, Al, "AluCase1_Logical", 0, 0, 0);
	G4VisAttributes *LaBr_TUDVisAtt = new G4VisAttributes(cyan);
	detectorLogical->SetVisAttributes(LaBr_TUDVisAtt);

	/************************ Vacuum Layer *****************************/

	G4double Radius_Vacuum = radiusHousing - AluCase_Thickness;
	G4double Length_Vacuum =
	    Thickness_Vacuum_front + lengthCrystal + Thickness_Vacuum_back;

	G4Tubs *Vacuum_Solid = new G4Tubs("Vacuum1_Solid", 0, Radius_Vacuum,
	                                  Length_Vacuum / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *Vacuum_Logical =
	    new G4LogicalVolume(Vacuum_Solid, vacuum, "Vacuum1_Logical", 0, 0, 0);
	G4VisAttributes *VacuumVisAtt = new G4VisAttributes(white);
	Vacuum_Logical->SetVisAttributes(VacuumVisAtt);

	new G4PVPlacement(0, G4ThreeVector(), Vacuum_Logical, "Vacuum1",
	                  detectorLogical, false, 0);

	/************************ Centerline *****************************/

	//	  G4double Radius_Center = 1 * mm;
	//	  G4double Length_Center = 40 * cm;
	//
	//	  G4Tubs* Center_Solid = new G4Tubs("Center1_Solid", 0, Radius_Center,
	// Length_Center/2, 0.*deg, 360.*deg);
	//	  G4LogicalVolume* Center_Logical = new G4LogicalVolume(Center_Solid,
	// vacuum, "Center1_Logical", 0, 0, 0);
	//	  G4VisAttributes* CenterVisAtt=new G4VisAttributes(vacuum);
	//	  Center_Logical->SetVisAttributes(CenterVisAtt);
	//
	//	  new G4PVPlacement(0,G4ThreeVector(), Center_Logical, "Center1",
	// detectorLogical, false,0);

	/************************LaBr Crystal*************************/

	G4double Radius_Crystal = radiusCrystal;
	G4double Length_Crystal = lengthCrystal;

	G4Tubs *Crystal_Solid =
	    new G4Tubs("Crystal1_Solid", 0. * cm, Radius_Crystal,
	               Length_Crystal / 2, 0. * deg, 360. * deg);
	G4LogicalVolume *Crystal1_Logical =
	    new G4LogicalVolume(Crystal_Solid, LaBr3Ce, Detector_Name, 0, 0, 0);

	G4VisAttributes *LaBr_TUDCrystalVisAtt = new G4VisAttributes(green);
	Crystal1_Logical->SetVisAttributes(LaBr_TUDCrystalVisAtt);

	new G4PVPlacement(0, G4ThreeVector(), Crystal1_Logical, "Crystal1",
	                  Vacuum_Logical, false, 0);
}

LaBr_TUD::~LaBr_TUD() {}
