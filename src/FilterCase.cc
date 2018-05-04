#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "FilterCase.hh"
#include "Units.hh"

FilterCase::FilterCase(G4double Filter_Thickness, G4bool Use_Ring){
	
	// Dimensions read off from a technical drawing by B. Loeher and J. Isaak
	G4double FilterCase_Length = 2. * inch;
	G4double FilterCase_Inner_Radius = 1.78 * inch;
	G4double FilterCase_Wall_Thickness = 0.07 * inch;

	G4double FilterCaseBottom_Thickness = 0.1 * inch;
	G4double FilterCaseBottom_Radius = 1.78 *inch;
	G4double FilterCaseBottom_Inner_Radius = 1.5 * inch;

	FilterCaseRing_Thickness = 0.1 * inch;	// Estimated, same thickness as bottom
	G4double FilterCaseRing_Radius = 1.78*inch;
	G4double FilterCaseRing_Inner_Radius = 1.5 * inch;


	Length = FilterCase_Length;
	Radius = FilterCase_Inner_Radius + FilterCase_Wall_Thickness;

	G4Colour white(1., 1., 1.);

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

	// Mother volume
	
	G4Tubs *FilterCase_Solid = new G4Tubs("FilterCase_Solid", 0., FilterCase_Inner_Radius + FilterCase_Wall_Thickness, FilterCase_Length*0.5, 0., twopi);

	FilterCase_Logical = new G4LogicalVolume(FilterCase_Solid, PE, "FilterCase_Logical");

	// Wall

	G4Tubs *FilterCaseWall_Solid =
	    new G4Tubs("FilterCaseWall_Solid", FilterCase_Inner_Radius,
		       FilterCase_Inner_Radius + FilterCase_Wall_Thickness,
		       FilterCase_Length * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *FilterCaseWall_Logical = new G4LogicalVolume(
	    FilterCaseWall_Solid, PE, "FilterCaseWall_Logical", 0, 0, 0);

	FilterCaseWall_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(0, G4ThreeVector(), FilterCaseWall_Logical, "FilterCaseWall", FilterCase_Logical, false, 0, false);

	// Bottom
	
	G4Tubs *FilterCaseBottom_Solid =
	    new G4Tubs("FilterCaseBottom_Solid", FilterCaseBottom_Inner_Radius,
		       FilterCaseBottom_Radius,
		       FilterCaseBottom_Thickness * 0.5, 0. * deg, 360. * deg);
	G4LogicalVolume* FilterCaseBottom_Logical = new G4LogicalVolume(
	    FilterCaseBottom_Solid, PE, "FilterCaseBottom_Logical", 0, 0, 0);

	FilterCaseBottom_Logical->SetVisAttributes(new G4VisAttributes(white));

	new G4PVPlacement(0, G4ThreeVector(0., 0., FilterCase_Length*0.5 - FilterCaseBottom_Thickness*0.5), FilterCaseBottom_Logical, "FilterCaseBottom", FilterCase_Logical, false, 0, false);
	
	// Ring

	if(Use_Ring){
		G4Tubs *FilterCaseRing_Solid =
		    new G4Tubs("FilterCaseRing_Solid", FilterCaseRing_Inner_Radius,
			       FilterCaseRing_Radius, FilterCaseRing_Thickness * 0.5,
			       0. * deg, 360. * deg);
		G4LogicalVolume *FilterCaseRing_Logical = new G4LogicalVolume(
		    FilterCaseRing_Solid, PE, "FilterCaseRing_Logical", 0, 0, 0);

		FilterCaseRing_Logical->SetVisAttributes(new G4VisAttributes(white));

		new G4PVPlacement(0, G4ThreeVector(0., 0., 0.5*FilterCase_Length - FilterCaseBottom_Thickness - Filter_Thickness - FilterCaseRing_Thickness*0.5), FilterCaseRing_Logical, "FilterCaseRing", FilterCase_Logical, false, 0, false);

		Radial_Offset = (FilterCaseBottom_Thickness + Filter_Thickness + FilterCaseRing_Thickness) - FilterCase_Length*0.5;
	} else{
		Radial_Offset = (FilterCaseBottom_Thickness + Filter_Thickness) - FilterCase_Length*0.5;
	}
}
