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
#include "Units.hh"

#include "Gd154_Target.hh"

Gd154_Target::Gd154_Target():
World_Logical(nullptr)
{}

Gd154_Target::Gd154_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Gd154_Target::Construct(G4ThreeVector global_coordinates){

	G4Colour yellow(1.0, 1.0, 0.0);

	// g/mole https://www.degruyter.com/view/j/pac.2016.88.issue-3/pac-2015-0305/pac-2015-0305.xml
	G4Isotope *Gd152 = new G4Isotope("152Gd", 64, 152-64, 157.25*g/mole);
	G4Isotope *Gd154 = new G4Isotope("154Gd", 64, 154-64, 157.25*g/mole);
	G4Isotope *Gd155 = new G4Isotope("155Gd", 64, 155-64, 157.25*g/mole);
	G4Isotope *Gd156 = new G4Isotope("156Gd", 64, 156-64, 157.25*g/mole);
	G4Isotope *Gd157 = new G4Isotope("157Gd", 64, 157-64, 157.25*g/mole);
	G4Isotope *Gd158 = new G4Isotope("158Gd", 64, 158-64, 157.25*g/mole);
	G4Isotope *Gd160 = new G4Isotope("160Gd", 64, 160-64, 157.25*g/mole);

	G4Element *Target_Element = new G4Element("154Gd_Element", "Gd", 7);

	// Assumed same isotope abundances as in Sobotka target
	Target_Element->AddIsotope(Gd152, 0.05*perCent);
	Target_Element->AddIsotope(Gd154, 66.78*perCent);
	Target_Element->AddIsotope(Gd155, 17.52*perCent);
	Target_Element->AddIsotope(Gd156, 7.23*perCent);
	Target_Element->AddIsotope(Gd157, 3.16*perCent);
	Target_Element->AddIsotope(Gd158, 3.44*perCent);
	Target_Element->AddIsotope(Gd160, 1.87*perCent);
	
	G4NistManager *nist = G4NistManager::Instance();
	G4Element *nat_O = nist->FindOrBuildElement("O");

	G4double Gd154_Mass = 5711.1 * mg;

	G4double Gd154_Container_OuterHeight = 7.5 * mm; // According to Oak Ridge
	G4double Gd154_Container_InnerHeight = Gd154_Container_OuterHeight - 1. * mm; // Estimated
	G4double Gd154_Container_OuterRadius = 0.5 * inch; // According to Oak Ridge
	G4double Gd154_Container_InnerRadius = Gd154_Container_OuterRadius - 1. * mm; // Estimated
    G4double Gd154_Container_CapThickness = ( Gd154_Container_OuterHeight - Gd154_Container_InnerHeight )/2;

	G4double Gd154_Density = Gd154_Mass / (pi * pow(Gd154_Container_InnerRadius,2) *
	                                 Gd154_Container_InnerHeight);


	G4Material *target_Gd154 = new G4Material("154Gd2O3", Gd154_Density, 2);
	target_Gd154->AddElement(Target_Element, 2);
	target_Gd154->AddElement(nat_O, 3);

	// Target Container Barrel

	G4Tubs *Gd154_Container_Solid =
	    new G4Tubs("Gd154_Container_Solid", Gd154_Container_InnerRadius,
		       Gd154_Container_OuterRadius,
		       Gd154_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);

	//HDPE (High-density polyethylene) used for container according to Oak Ridge, G4_POLYETHYLENE matches the definition of HDPE in density
	G4LogicalVolume *Gd154_Container_Logical = new G4LogicalVolume(
	    Gd154_Container_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Gd154_Container_Logical");
	Gd154_Container_Logical->SetVisAttributes(G4Color::Grey());

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Gd154_Container_Logical,
			  "Gd154_Container", World_Logical, false, 0);

	// Target Container Caps
	G4Tubs *Gd154_ContainerCap_Solid = new G4Tubs(
	    "Gd154_ContainerCap_Solid", 0. * mm, Gd154_Container_InnerRadius,
	    Gd154_Container_CapThickness * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Gd154_ContainerCap_Logical = new G4LogicalVolume(
	    Gd154_ContainerCap_Solid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "Gd154_ContainerCap_Logical");
	Gd154_ContainerCap_Logical->SetVisAttributes(G4Color::Grey());


        //Target Container Bottom
        new G4PVPlacement(
		    0, global_coordinates + G4ThreeVector(0., 0.,
		                     (Gd154_Container_OuterHeight -
		                         Gd154_Container_CapThickness) * 0.5),
		    Gd154_ContainerCap_Logical, "Gd154_ContainerBottom",
		    World_Logical, false, 0);

        // Target Container Top
        new G4PVPlacement(
		    0, global_coordinates + G4ThreeVector(0., 0.,
		                     -(Gd154_Container_OuterHeight -
		                         Gd154_Container_CapThickness) * 0.5),
		    Gd154_ContainerCap_Logical, "Gd154_ContainerTop",
		    World_Logical, false, 0);

	// Gd154 Target Material
	G4Tubs *Gd154_Solid =
	    new G4Tubs("Gd154_Solid", 0. * mm, Gd154_Container_InnerRadius,
		       Gd154_Container_InnerHeight * 0.5, 0. * deg, 360. * deg);

	G4LogicalVolume *Gd154_Logical = new G4LogicalVolume(
	    Gd154_Solid, target_Gd154, "Gd154_Logical");
	Gd154_Logical->SetVisAttributes(G4Color::Yellow());

	new G4PVPlacement(
	    0, global_coordinates + G4ThreeVector(), Gd154_Logical, "Gd154_Target", World_Logical, false, 0);
}
