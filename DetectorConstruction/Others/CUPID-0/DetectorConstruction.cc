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

#include "DetectorConstruction.hh"

// Materials
#include "G4Material.hh"
#include "G4NistManager.hh"

// Geometry
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "G4SDManager.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

// Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

	G4Colour orange(1.0, 0.5, 0.0);

	G4NistManager *nist = G4NistManager::Instance();

	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	/***************** World Volume *****************/

	G4double world_r = 300. * mm;

	G4Sphere *world_dim =
	    new G4Sphere("world_dim", 0., world_r, 0., twopi, 0., pi);

	G4LogicalVolume *world_log =
	    new G4LogicalVolume(world_dim, vacuum, "world_log", 0, 0, 0);

	//world_log->SetVisAttributes(G4VisAttributes::GetInvisible());

	G4VPhysicalVolume *world_phys =
	    new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

	/***************** Zn^{82}Se crystal *****************/

	// One of 24 Zn^{82}Se crystals used in the CUPID-0 detector array for 0νββ search
	// [1] O. Azzolini et al., Eur. Phys. J. C 78 (2018) 428
	// The dimensions of the crystal were read off from the given reference.
	// Explicitly, the dimensions in Fig. 3 are used.
	// The image shows a length of the cylindrical crystal of 57.3 mm and a
	// mass of 504.70 g.
	// To estimate the radius of the crystal, the density of natural ZnSe of
	// 5.27 g/cm^3 from Wikipedia is converted to Zn^{82}Se.
	
	G4double crystal_Length = 57.3*mm; // From [1]
	G4double crystal_Mass = 504.70*g; // From [1]
	G4double Zn_natSe_Density = 5.27*g/cm3; // From Wikipedia
	G4double Zn_82Se_Density = (65.38 + 81.916699)/(65.38 + 78.971)*Zn_natSe_Density; // Average atomic masses from Wikipedia
									// 82Se atomic mass from Atomic Mass Evaluation (AME).
	G4double crystal_Radius = sqrt(crystal_Mass/(Zn_82Se_Density*crystal_Length*pi));

	// ****** Build the Zn^{82}Se crystal material ******
	// Selenium material enriched in 82Se (assume 82Se is the only isotope)
	G4Isotope *se82 = new G4Isotope("82Se_Isotope", 34, 82);
	G4Element *se82_Element = new G4Element("82Se_Element", "82Se", 1);
	se82_Element->AddIsotope(se82, 100.*perCent);

	// Natural zinc. Isotopic abundances from Wikipedia
	G4Isotope *zn64 = new G4Isotope("64Zn_Isotope", 30, 64);
	G4Isotope *zn66 = new G4Isotope("66Zn_Isotope", 30, 66);
	G4Isotope *zn67 = new G4Isotope("67Zn_Isotope", 30, 67);
	G4Isotope *zn68 = new G4Isotope("68Zn_Isotope", 30, 68);

	G4Element *zn_Element = new G4Element("Zn_Element", "Zn", 4);
	zn_Element->AddIsotope(zn64, 49.2*perCent);
	zn_Element->AddIsotope(zn66, 27.7*perCent);
	zn_Element->AddIsotope(zn67,  4.0*perCent);
	zn_Element->AddIsotope(zn68, 18.5*perCent);

	G4Material *Zn_82Se = new G4Material("Zn_82Se", Zn_82Se_Density, 2);
	Zn_82Se->AddElement(se82_Element, 1);
	Zn_82Se->AddElement(  zn_Element, 1);
	
	// ****** Construct the cylindrical Zn^{82}Se crystal ******
	
	G4Tubs *crystal_Solid = new G4Tubs("crystal_Solid", 0., crystal_Radius, crystal_Length*0.5, 0., twopi);
	G4LogicalVolume *crystal_Logical = new G4LogicalVolume(crystal_Solid, Zn_82Se, "crystal_Logical");

	crystal_Logical->SetVisAttributes(orange);

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), crystal_Logical, "crystal", world_log, false, 0, false);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

	EnergyDepositionSD *crystal_EnergyDepositionSD = new EnergyDepositionSD("crystal_edep", "crystal_edep");
	G4SDManager::GetSDMpointer()->AddNewDetector(crystal_EnergyDepositionSD);
	crystal_EnergyDepositionSD->SetDetectorID(0);
	SetSensitiveDetector("crystal_Logical", crystal_EnergyDepositionSD, true);

	ParticleSD *crystal_ParticleSD = new ParticleSD("crystal_pid", "crystal_pid");
	G4SDManager::GetSDMpointer()->AddNewDetector(crystal_ParticleSD);
	crystal_ParticleSD->SetDetectorID(1);
	SetSensitiveDetector("crystal_Logical", crystal_ParticleSD, true);
}
