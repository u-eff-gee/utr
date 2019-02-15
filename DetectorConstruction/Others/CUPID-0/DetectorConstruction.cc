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
	// In Fig. 3, a cylindrical crystal is shown with a length of 57.3 mm and a
	// mass of 504.70 g.
	// However, this seems to be an extraordinarily big specimen, 
	// since all crystals shown in Fig. 4 are smaller.
	// Therefore an estimated mean of
	// the enriched crystals shown in Fig. 4 was taken.
	// Explicitly, assume a mean mass of 430 g, and a pure 82Se mass of 250 g.
	// To estimate the radius of the crystal, the density of natural ZnSe of
	// 5.27 g/cm^3 from Wikipedia is converted to Zn^{82}Se.
	
	G4double se82_enrichment = 250./430.; // From [1], estimated from Fig. 4
	G4double crystal_Length = 57.3*mm; // From [1], shown in Fig. 3
	G4double crystal_Mass = 430.*g; // From [1], estimated from Fig. 4
	G4double Zn_natSe_Density = 5.27*g/cm3; // From Wikipedia
	G4double Zn_82Se_Density = ((1. - se82_enrichment)*(65.38 + 78.971)+ // The natural part
			se82_enrichment*(65.38 + 81.916699))/ // The enriched part
		(65.38 + 78.971)*Zn_natSe_Density; // Average atomic masses from Wikipedia
						// 82Se atomic mass from Atomic Mass Evaluation (AME).
	G4double crystal_Radius = sqrt(crystal_Mass/(Zn_82Se_Density*crystal_Length*pi));

	// ****** Build the Zn^{82}Se crystal material ******
	// Selenium material, natural and enriched in 82Se	
	// Isotopic abundances from Wikipedia
	G4Isotope *se74 = new G4Isotope("74Se_Isotope", 34, 74);
	G4Isotope *se76 = new G4Isotope("76Se_Isotope", 34, 76);
	G4Isotope *se77 = new G4Isotope("77Se_Isotope", 34, 77);
	G4Isotope *se78 = new G4Isotope("78Se_Isotope", 34, 78);
	G4Isotope *se80 = new G4Isotope("80Se_Isotope", 34, 80);
	G4Isotope *se82 = new G4Isotope("82Se_Isotope", 34, 82);

	G4Element *se_nat_Element = new G4Element("Se_nat_Element", "Se_nat", 6);
	se_nat_Element->AddIsotope(se74,  0.86*perCent);
	se_nat_Element->AddIsotope(se76,  9.23*perCent);
	se_nat_Element->AddIsotope(se77,  7.60*perCent);
	se_nat_Element->AddIsotope(se78, 23.69*perCent);
	se_nat_Element->AddIsotope(se80, 49.80*perCent);
	se_nat_Element->AddIsotope(se82,  8.82*perCent);

	G4Element *se82_Element = new G4Element("82Se_Element", "82Se", 1);
	se82_Element->AddIsotope(se82, 100.*perCent);

	// Natural zinc, isotopic abundances from Wikipedia
	G4Isotope *zn64 = new G4Isotope("64Zn_Isotope", 30, 64);
	G4Isotope *zn66 = new G4Isotope("66Zn_Isotope", 30, 66);
	G4Isotope *zn67 = new G4Isotope("67Zn_Isotope", 30, 67);
	G4Isotope *zn68 = new G4Isotope("68Zn_Isotope", 30, 68);
	G4Isotope *zn70 = new G4Isotope("70Zn_Isotope", 30, 70);

	G4Element *zn_Element = new G4Element("Zn_Element", "Zn", 5);
	zn_Element->AddIsotope(zn64, 49.2*perCent);
	zn_Element->AddIsotope(zn66, 27.7*perCent);
	zn_Element->AddIsotope(zn67,  4.0*perCent);
	zn_Element->AddIsotope(zn68, 18.5*perCent);
	zn_Element->AddIsotope(zn70,  0.6*perCent);

	G4Material *Zn_82Se = new G4Material("Zn_82Se", Zn_82Se_Density, 3);
	Zn_82Se->AddElement(  se82_Element, 50.*se82_enrichment*perCent);
	Zn_82Se->AddElement(se_nat_Element, 50.*(1.-se82_enrichment)*perCent);
	Zn_82Se->AddElement(    zn_Element, 50.*perCent);
	
	// ****** Construct the cylindrical Zn^{82}Se crystal ******
	
	G4Tubs *crystal_Solid = new G4Tubs("crystal_Solid", 0., crystal_Radius, crystal_Length*0.5, 0., twopi);
	G4LogicalVolume *crystal_Logical = new G4LogicalVolume(crystal_Solid, Zn_82Se, "crystal_Logical");

	crystal_Logical->SetVisAttributes(orange);

	G4cout << G4endl;
	G4cout << "=============================================" << G4endl;
	G4cout << "Zn82Se crystal information" << G4endl;
	G4cout << "Radius     : " << crystal_Solid->GetRMax()/cm << " cm" << G4endl;
	G4cout << "Length     : " << 2.*crystal_Solid->GetDz()/cm << " cm" << G4endl;
	G4cout << "Surface    : " << crystal_Solid->GetSurfaceArea()/cm2 << " cm^2" << G4endl;
	G4cout << "Volume     : " << crystal_Solid->GetCubicVolume()/cm3 << " cm^3" << G4endl;
	G4cout << "Density    : " << Zn_82Se->GetDensity()/g*cm3 << " g * cm^-3" << G4endl;
	G4cout << "Mass       : " << Zn_82Se->GetDensity()*crystal_Solid->GetCubicVolume() / g << " g" << G4endl;
	const G4ElementVector *elementVector = Zn_82Se->GetElementVector();
	const G4double *fractionVector = Zn_82Se->GetFractionVector();
	G4cout << "Components : " << G4endl;
	for(long unsigned int i = 0; i < elementVector->size(); ++i){
		G4cout << "\t" << (*elementVector)[i]->GetName() << " ( "
		<< fractionVector[i]/perCent << " % )" << G4endl;
	}
	G4cout << "=============================================" << G4endl;
	G4cout << G4endl;

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), crystal_Logical, "crystal", world_log, false, 0, false);

	return world_phys;
}

void DetectorConstruction::ConstructSDandField() {

	EnergyDepositionSD *crystal_EnergyDepositionSD = new EnergyDepositionSD("crystal_edep", "crystal_edep");
	G4SDManager::GetSDMpointer()->AddNewDetector(crystal_EnergyDepositionSD);
	crystal_EnergyDepositionSD->SetDetectorID(0);
	SetSensitiveDetector("crystal_Logical", crystal_EnergyDepositionSD, true);

//	ParticleSD *crystal_ParticleSD = new ParticleSD("crystal_pid", "crystal_pid");
//	G4SDManager::GetSDMpointer()->AddNewDetector(crystal_ParticleSD);
//	crystal_ParticleSD->SetDetectorID(1);
//	SetSensitiveDetector("crystal_Logical", crystal_ParticleSD, true);
}
