/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "B11_Absorber.hh"

B11_Absorber::B11_Absorber():
World_Logical(nullptr)
{}

B11_Absorber::B11_Absorber(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void B11_Absorber::Construct(G4ThreeVector global_coordinates){

	G4Colour  orange (1.0, 0.5, 0.0) ;
	G4Colour  cyan    (0.0, 1.0, 1.0) ;
	G4Colour grey(0.5, 0.5, 0.5);

	// Create Materials
	G4double Target_Density = 2.460*g/cm3; // Wikipedia	
	G4double Target_Mass = 8.650*g;
	G4double Target_Radius = 9.25*mm;
	G4double Target_Length = Target_Mass/(Target_Density*pi*Target_Radius*Target_Radius);
	printf("The Target Length is: %f\n",Target_Length);

	G4Isotope *B11 = new G4Isotope("B11", 5, 11, 11.0093054*g/mole);
	G4Isotope *B10 = new G4Isotope("B10", 5, 10, 10.0129370*g/mole);

	G4Element *Target_Element = new G4Element("11B_Element", "B", 2);

	Target_Element->AddIsotope(B11, 99.79*perCent);
	Target_Element->AddIsotope(B10, 0.21*perCent);

	G4Material *Target_Material = new G4Material("B11", Target_Density, 1);
	Target_Material->AddElement(Target_Element, 1);

	// Create physical target
	
	G4Tubs *Target_Solid = new G4Tubs("B11_Solid", 0., Target_Radius, Target_Length*0.5, 0., twopi);
	G4LogicalVolume *Target_Logical = new G4LogicalVolume(Target_Solid, Target_Material, "B11_Logical");
	Target_Logical->SetVisAttributes(cyan);

	G4NistManager *man = G4NistManager::Instance();

	G4Material *nat_Cu = man->FindOrBuildMaterial("G4_Cu");

	G4double LidLength=2*mm;
	G4Tubs *Cu_ContainerLid_Solid = new G4Tubs("Cu_ContainerLid_Solid",0,Target_Radius,LidLength*0.5,0,twopi);
	G4LogicalVolume *Cu_ContainerLid_Logical = new G4LogicalVolume(Cu_ContainerLid_Solid, nat_Cu, "Cu_ContainerLid_Logical");
	Cu_ContainerLid_Logical->SetVisAttributes(grey);
	
	// 1mm wall thickness
	G4Tubs *Cu_Container_Solid = new G4Tubs("Cu_Container_Solid", Target_Radius, Target_Radius+1.*mm, Target_Length*0.5+LidLength, 0., twopi);
	G4LogicalVolume *Cu_Container_Logical = new G4LogicalVolume(Cu_Container_Solid, nat_Cu, "Cu_Container_Logical");
	Cu_Container_Logical->SetVisAttributes(orange);
	
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(), Target_Logical, "B11_Absorber", World_Logical, false, 0);
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(), Cu_Container_Logical, "Cu_Container", World_Logical, false, 0);
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0,0,-Target_Length*0.5-LidLength*0.5), Cu_ContainerLid_Logical, "Cu_ContainerLid_Upstream", World_Logical, false, 0);
	new G4PVPlacement(0, global_coordinates+G4ThreeVector(0,0,+Target_Length*0.5+LidLength*0.5), Cu_ContainerLid_Logical, "Cu_ContainerLid_Downstream", World_Logical, false, 0);


}