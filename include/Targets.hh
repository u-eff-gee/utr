#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class TargetContainer {
  private:
	G4LogicalVolume *TargetContainer_Logical;

	const G4double TargetContainer_Length = 20. * mm;
	const G4double TargetContainer_Wall_Thickness = 1. * mm;
	const G4double TargetContainer_OuterRadius = 11. * mm;
	const G4double TargetContainer_InnerRadius = 10. * mm;

  public:
	TargetContainer() {

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

		G4Tubs *TargetContainer_Solid =
		    new G4Tubs("TargetContainer_Solid", TargetContainer_InnerRadius,
		               TargetContainer_OuterRadius,
		               TargetContainer_Length * 0.5, 0. * deg, 360. * deg);

		TargetContainer_Logical = new G4LogicalVolume(
		    TargetContainer_Solid, PVC, "TargetContainer_Logical");
		TargetContainer_Logical->SetVisAttributes(new G4VisAttributes(grey));
	};

	~TargetContainer(){};

	G4double Get_Length() { return TargetContainer_Length; };
	G4double Get_Inner_Radius() { return TargetContainer_InnerRadius; };
	G4double Get_Outer_Radius() { return TargetContainer_OuterRadius; };
	G4LogicalVolume *Get_Logical() { return TargetContainer_Logical; };
};

class TargetContainerCap {
  private:
	G4LogicalVolume *TargetContainerCap_Logical;

	const G4double TargetContainerCap_Thickness = 2. * mm;
	const G4double TargetContainerCap_Radius = 10. * mm;

  public:
	TargetContainerCap() {

		G4Colour grey(.5, .5, .5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

		G4Tubs *TargetContainerCap_Solid = new G4Tubs(
		    "TargetContainerCap_Solid", 0. * mm, TargetContainerCap_Radius,
		    TargetContainerCap_Thickness * 0.5, 0. * deg, 360. * deg);

		TargetContainerCap_Logical = new G4LogicalVolume(
		    TargetContainerCap_Solid, PVC, "TargetContainerCap_Logical");
		TargetContainerCap_Logical->SetVisAttributes(new G4VisAttributes(grey));
	};

	~TargetContainerCap(){};

	G4double Get_Thickness() { return TargetContainerCap_Thickness; };
	G4double Get_Radius() { return TargetContainerCap_Radius; };
	G4LogicalVolume *Get_Logical() { return TargetContainerCap_Logical; };
};

class Se82_Target {
  private:
	G4LogicalVolume *Se82_Target_Logical;

	G4double Length;
	G4double Radius;
	G4double Target_Center;

  public:
	Se82_Target() {

		G4Colour yellow(1.0, 1.0, 0.0);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
		Materials *mat = Materials::Instance();

		// Mother volume
		TargetContainer *targetContainer = new TargetContainer();
		TargetContainerCap *targetContainerCap = new TargetContainerCap();
		TargetContainerCap *targetContainerBottom = new TargetContainerCap();

		Length = targetContainer->Get_Length();
		Radius = targetContainer->Get_Outer_Radius();
		G4double Se82_Target_Thickness = 4. * mm;
		G4double Se82_Target_Radius = targetContainer->Get_Inner_Radius();

		G4Tubs *Se82_Target_Solid =
		    new G4Tubs("Se82_Target_Solid", 0. * mm, Radius, Length * 0.5,
		               0. * deg, 360. * deg);

		Se82_Target_Logical =
		    new G4LogicalVolume(Se82_Target_Solid, air, "Se82_Target_Logical");
		Se82_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

		// Target Container

		G4LogicalVolume *TargetContainer_Logical =
		    targetContainer->Get_Logical();

		new G4PVPlacement(0, G4ThreeVector(), TargetContainer_Logical,
		                  "TargetContainer", Se82_Target_Logical, false, 0);

		G4LogicalVolume *TargetContainerBottom_Logical =
		    targetContainerBottom->Get_Logical();

		new G4PVPlacement(
		    0, G4ThreeVector(0., 0.,
		                     targetContainer->Get_Length() * 0.5 -
		                         targetContainerBottom->Get_Thickness() * 0.5),
		    TargetContainerBottom_Logical, "TargetContainerBottom",
		    Se82_Target_Logical, false, 0);

		G4LogicalVolume *TargetContainerCap_Logical =
		    targetContainerCap->Get_Logical();

		new G4PVPlacement(
		    0, G4ThreeVector(0., 0.,
		                     targetContainer->Get_Length() * 0.5 -
		                         targetContainerCap->Get_Thickness() -
		                         Se82_Target_Thickness -
		                         targetContainerBottom->Get_Thickness() * 0.5),
		    TargetContainerCap_Logical, "TargetContainerCap",
		    Se82_Target_Logical, false, 0);

		// Se82 Target Material

		G4Tubs *Se82_Solid =
		    new G4Tubs("Se82_Solid", 0. * mm, Se82_Target_Radius,
		               Se82_Target_Thickness * 0.5, 0. * deg, 360. * deg);

		G4LogicalVolume *Se82_Logical = new G4LogicalVolume(
		    Se82_Solid, mat->Get_target_Se(), "Se82_Logical");
		Se82_Logical->SetVisAttributes(new G4VisAttributes(yellow));

		new G4PVPlacement(
		    0,
		    G4ThreeVector(0., 0., Length * 0.5 -
		                              targetContainerBottom->Get_Thickness() -
		                              Se82_Target_Thickness * 0.5),
		    Se82_Logical, "Se82_Target", Se82_Target_Logical, false, 0);

		Target_Center = Length * 0.5 - targetContainerBottom->Get_Thickness() -
		                Se82_Target_Thickness * 0.5;
	};

	~Se82_Target(){};

	G4double Get_Thickness() { return Length; };
	G4double Get_Radius() { return Radius; };
	G4double Get_Target_Center() { return Target_Center; };

	G4LogicalVolume *Get_Logical() { return Se82_Target_Logical; };
};

class Kr82_Target {
  private:
	G4LogicalVolume *Kr82_Target_Logical;

	G4double Length;
	G4double Radius;
	G4double Target_Center;

  public:
	Kr82_Target() {

		G4Colour green(0., 1.0, 0.0);
		G4Colour grey(0.5, 0.5, 0.5);

		G4NistManager *nist = G4NistManager::Instance();
		G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
		Materials *mat = Materials::Instance();

		// Mother volume
		G4double GasSphere_Outer_Radius = 10. * mm; // Measured
		G4double GasSphere_Inner_Radius = 9. * mm;  // Estimated

		G4double Tube_Length = 20. * mm;
		G4double Tube_Inner_Radius = 0.25 * mm; // Estimated
		G4double Tube_Outer_Radius = 1. * mm;   // Measured

		G4double Valve_Length = 20. * mm;       // Measured
		G4double Valve_Outer_Radius = 2.5 * mm; // Measured

		Length = GasSphere_Outer_Radius * 2. + Tube_Length + Valve_Length;
		Radius = GasSphere_Outer_Radius;
		Target_Center =
		    Length * 0.5 - Valve_Length - Tube_Length - GasSphere_Outer_Radius;

		G4Tubs *Kr82_Solid = new G4Tubs("Kr82_Solid", 0. * mm, Radius,
		                                Length * 0.5, 0. * deg, 360. * deg);

		Kr82_Target_Logical =
		    new G4LogicalVolume(Kr82_Solid, vacuum, "Kr82_Logical");
		Kr82_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

		// Krypton gas

		G4Sphere *Krypton_Solid =
		    new G4Sphere("Krypton_Solid", 0., GasSphere_Inner_Radius, 0.,
		                 360. * deg, 0., 180. * deg);

		G4LogicalVolume *Krypton_Logical = new G4LogicalVolume(
		    Krypton_Solid, mat->Get_target_Kr(), "Krypton_Logical");

		Krypton_Logical->SetVisAttributes(new G4VisAttributes(green));

		new G4PVPlacement(
		    0, G4ThreeVector(0., 0., -Length * 0.5 + GasSphere_Outer_Radius),
		    Krypton_Logical, "Krypton", Kr82_Target_Logical, false, 0);

		// Sphere

		G4Sphere *Sphere_Solid = new G4Sphere(
		    "Sphere_Solid", GasSphere_Inner_Radius, GasSphere_Outer_Radius, 0.,
		    360. * deg, 0., 180. * deg);

		G4LogicalVolume *Sphere_Logical = new G4LogicalVolume(
		    Sphere_Solid, mat->Get_Stainless_Steel(), "Sphere_Logical");

		Sphere_Logical->SetVisAttributes(new G4VisAttributes(grey));

		new G4PVPlacement(
		    0, G4ThreeVector(0., 0., -Length * 0.5 + GasSphere_Outer_Radius),
		    Sphere_Logical, "Sphere", Kr82_Target_Logical, false, 0);

		// Tube

		G4Tubs *Tube_Solid =
		    new G4Tubs("Tube_Solid", Tube_Inner_Radius, Tube_Outer_Radius,
		               Tube_Length * 0.5, 0. * deg, 360. * deg);

		G4LogicalVolume *Tube_Logical = new G4LogicalVolume(
		    Tube_Solid, mat->Get_Stainless_Steel(), "Tube_Logical");
		Tube_Logical->SetVisAttributes(new G4VisAttributes(grey));

		new G4PVPlacement(0, G4ThreeVector(0., 0., Length * 0.5 - Valve_Length -
		                                               Tube_Length * 0.5),
		                  Tube_Logical, "Tube", Kr82_Target_Logical, false, 0);

		// Valve

		G4Tubs *Valve_Solid =
		    new G4Tubs("Valve_Solid", 0., Valve_Outer_Radius,
		               Valve_Length * 0.5, 0. * deg, 360. * deg);

		G4LogicalVolume *Valve_Logical = new G4LogicalVolume(
		    Valve_Solid, mat->Get_Stainless_Steel(), "Valve_Logical");
		Valve_Logical->SetVisAttributes(new G4VisAttributes(grey));

		new G4PVPlacement(
		    0, G4ThreeVector(0., 0., Length * 0.5 - Valve_Length * 0.5),
		    Valve_Logical, "Valve", Kr82_Target_Logical, false, 0);
	};

	~Kr82_Target(){};

	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };
	G4double Get_Target_Center() { return Target_Center; };

	G4LogicalVolume *Get_Logical() { return Kr82_Target_Logical; };
};
