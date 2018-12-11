
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

#include "BeamPipe_Upstream.hh"
using std::cout;
using std::endl;
using std::stringstream;

BeamPipe_Upstream::BeamPipe_Upstream(G4LogicalVolume *World_Log):World_Logical(World_Log){}


void BeamPipe_Upstream::Construct(G4ThreeVector global_coordinates,G4double relative_density)
{

	X_begin+=global_coordinates.getX();
	Y_begin+=global_coordinates.getY();
	Z_begin+=global_coordinates.getZ();

	X_end+=global_coordinates.getX();
	Y_end+=global_coordinates.getY();
	Z_end+=global_coordinates.getZ();

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
	G4Material *Al= nist->FindOrBuildMaterial("G4_Al");
	G4Material* AIR = nist->FindOrBuildMaterial("G4_AIR");
	
	Vacuum vac(relative_density, "beampipe_upstream_vacuum");
	G4Material *vacuum = vac.Get_Material();

	//*************************************************
	// Mother volume
	//*************************************************
	
	G4Box *beampipe_upstream_Mother_Solid = new G4Box("beampipe_upstream_Solid", beamPipe_Outer_Radius_Large, beamPipe_Outer_Radius_Large, beamPipe_Small_Radius_Length*0.5);

	G4LogicalVolume *beampipe_upstream_Mother_Logical = new G4LogicalVolume(beampipe_upstream_Mother_Solid, AIR, "beampipe_upstream_Mother_Logical");
	// beampipe_upstream_Mother_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	beampipe_upstream_Mother_Logical->SetVisAttributes(yellow);
	
	//*************************************************
	// Pipe volume
	//*************************************************
	
	G4Tubs *beamPipe_Large_Radius_Solid = new G4Tubs("beamPipe_Large_Radius_Solid", beamPipe_Outer_Radius_Small, beamPipe_Outer_Radius_Large, beamPipe_Large_Radius_Length*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_Large_Radius_Logical = new G4LogicalVolume(beamPipe_Large_Radius_Solid, Al, "beamPipe_Large_Radius_Logical");
	beamPipe_Large_Radius_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., (beamPipe_Small_Radius_Length-beamPipe_Large_Radius_Length)*0.5), beamPipe_Large_Radius_Logical, "beamPipe_Large_Radius", beampipe_upstream_Mother_Logical, false, 0);

	G4Tubs *beamPipe_Exit_Window_Solid = new G4Tubs("beamPipe_Exit_Window_Solid", 0., beamPipe_Inner_Radius, exit_Window_Thickness*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_Exit_Window_Logical = new G4LogicalVolume(beamPipe_Exit_Window_Solid, Al, "beamPipe_Exit_Window_Logical");
	beamPipe_Exit_Window_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0.,(beamPipe_Small_Radius_Length-exit_Window_Thickness)*0.5), beamPipe_Exit_Window_Logical, "beamPipe_Exit_Window", beampipe_upstream_Mother_Logical, false, 0);

	G4Tubs *beamPipe_Small_Radius_Solid = new G4Tubs("beamPipe_Small_Radius_Solid", beamPipe_Inner_Radius, beamPipe_Outer_Radius_Small, beamPipe_Small_Radius_Length*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_Small_Radius_Logical = new G4LogicalVolume(beamPipe_Small_Radius_Solid, Al, "beamPipe_Small_Radius_Logical");
	beamPipe_Small_Radius_Logical->SetVisAttributes(grey);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), beamPipe_Small_Radius_Logical, "beamPipe_Small_Radius", beampipe_upstream_Mother_Logical, false, 0);

	// cout << "DetectorConstruction.cc: Construct(): Beam pipe in simulation starts at z = " << 0 beamPipe_Large_Radius_Length - beamPipe_Small_Radius_Length << " mm" << endl;

	G4Tubs *beamPipe_Vacuum_Solid = new G4Tubs("beamPipe_Vacuum_Solid", 0., beamPipe_Inner_Radius, (beamPipe_Small_Radius_Length-exit_Window_Thickness)*0.5, 0., twopi);

	G4LogicalVolume *beamPipe_Vacuum_Logical = new G4LogicalVolume(beamPipe_Vacuum_Solid, vacuum, "beamPipe_Vacuum_Logical");
	beamPipe_Vacuum_Logical->SetVisAttributes(cyan);

	new G4PVPlacement(0, G4ThreeVector(0., 0.,-exit_Window_Thickness), beamPipe_Vacuum_Logical, "beamPipe_Vacuum", beampipe_upstream_Mother_Logical, false, 0);


	new G4PVPlacement(0, global_coordinates, beampipe_upstream_Mother_Logical, "beampipe_upstream", World_Logical, false, 0);


}