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

#include "Materials.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#define PI 3.141592

Materials::Materials() { ConstructMaterials(); }

Materials::~Materials() {}

void Materials::ConstructMaterials() {

	// Implement materials which are not in the Geant4 materials database
	// http://geant4.cern.ch/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/apas08.html
	// for example isotopically enriched materials or materials with a
	// non-standard density.

	G4NistManager *nist = G4NistManager::Instance();

	G4Element *nat_La = nist->FindOrBuildElement("La");
	G4Element *nat_Br = nist->FindOrBuildElement("Br");
	G4Element *nat_Ce = nist->FindOrBuildElement("Ce");
	G4Element *nat_Cl = nist->FindOrBuildElement("Cl");

	G4double z, a;
	G4double density, abundance, fractionmass;
	G4String name, symbol;
	G4int ncomponents, natoms;

	/******************************************************/
	// Enriched Ti target from 2014/2015 NRF experiments
	/******************************************************/

	/*G4Isotope *Ti50 =
	    new G4Isotope(name = "50Ti", z = 22, n = 28, a = 49.945791 * g / mole);
	G4Isotope *Ti49 =
	    new G4Isotope(name = "49Ti", z = 22, n = 27, a = 48.947870 * g / mole);
	G4Isotope *Ti48 =
	    new G4Isotope(name = "48Ti", z = 22, n = 26, a = 47.947946 * g / mole);
	G4Isotope *Ti47 =
	    new G4Isotope(name = "47Ti", z = 22, n = 25, a = 46.951763 * g / mole);
	G4Isotope *Ti46 =
	    new G4Isotope(name = "46Ti", z = 22, n = 24, a = 45.952631 * g /
	mole);*/

	G4Isotope *Ti50 = new G4Isotope(name = "50Ti", z = 22, a = 50);
	G4Isotope *Ti49 = new G4Isotope(name = "49Ti", z = 22, a = 49);
	G4Isotope *Ti48 = new G4Isotope(name = "48Ti", z = 22, a = 48);
	G4Isotope *Ti47 = new G4Isotope(name = "47Ti", z = 22, a = 47);
	G4Isotope *Ti46 = new G4Isotope(name = "46Ti", z = 22, a = 46);

	G4Element *enriched_Ti =
	    new G4Element(name = "enriched Ti", symbol = "Ti", ncomponents = 5);

	enriched_Ti->AddIsotope(Ti50, abundance = 67.62 * perCent);
	enriched_Ti->AddIsotope(Ti49, abundance = 3.01 * perCent);
	enriched_Ti->AddIsotope(Ti48, abundance = 24.06 * perCent);
	enriched_Ti->AddIsotope(Ti47, abundance = 2.53 * perCent);
	enriched_Ti->AddIsotope(Ti46, abundance = 2.78 * perCent);
	G4Element *nat_O = nist->FindOrBuildElement("O");

	target_TiO2 = new G4Material(name = "enriched Ti", density = 4. * g / cm3,
	                             ncomponents = 2);
	// Density is a mean value of different TiO2 crystal lattice configurations.
	target_TiO2->AddElement(enriched_Ti, natoms = 1);
	target_TiO2->AddElement(nat_O, natoms = 2);

	/*************************************************/
	// Enriched Kr gas from 2016/2017 NRF experiments
	/*************************************************/

	/*G4Isotope *Kr78 = new G4Isotope(name = "78Kr", z = 36, n = 42,
	                                a = 77.920364783 * g / mole);
	G4Isotope *Kr80 = new G4Isotope(name = "80Kr", z = 36, n = 44,
	                                a = 79.916378965 * g / mole);
	G4Isotope *Kr82 =
	    new G4Isotope(name = "82Kr", z = 36, n = 46, a = 81.9134836 * g / mole);
	G4Isotope *Kr83 = new G4Isotope(name = "83Kr", z = 36, n = 47,
	                                a = 82.914136099 * g / mole);
	G4Isotope *Kr84 = new G4Isotope(name = "84Kr", z = 36, n = 48,
	                                a = 83.811506687 * g / mole);
	G4Isotope *Kr86 = new G4Isotope(name = "86Kr", z = 36, n = 50,
	                                a = 85.910610729 * g / mole);*/

	G4Isotope *Kr78 = new G4Isotope(name = "78Kr", z = 36, a = 78);
	G4Isotope *Kr80 = new G4Isotope(name = "80Kr", z = 36, a = 80);
	G4Isotope *Kr82 = new G4Isotope(name = "82Kr", z = 36, a = 82);
	G4Isotope *Kr83 = new G4Isotope(name = "83Kr", z = 36, a = 83);
	G4Isotope *Kr84 = new G4Isotope(name = "84Kr", z = 36, a = 84);
	G4Isotope *Kr86 = new G4Isotope(name = "86Kr", z = 36, a = 86);

	G4Element *enriched_Kr =
	    new G4Element(name = "enriched Kr", symbol = "Kr", ncomponents = 6);

	enriched_Kr->AddIsotope(Kr78, abundance = 0.003 * perCent);
	enriched_Kr->AddIsotope(Kr80, abundance = 0.005 * perCent);
	enriched_Kr->AddIsotope(Kr82, abundance = 99.945 * perCent);
	enriched_Kr->AddIsotope(Kr83, abundance = 0.041 * perCent);
	enriched_Kr->AddIsotope(Kr84, abundance = 0.003 * perCent);
	enriched_Kr->AddIsotope(Kr86, abundance = 0.003 * perCent);

	G4double Kr_Mass = 1.50218 * g;
	G4double GasSphere_Inner_Radius = 9. * mm; // Estimated

	G4double Kr_Density =
	    Kr_Mass / (4. / 3. * PI * pow(GasSphere_Inner_Radius, 3));

	target_Kr = new G4Material(name = "target_Kr", Kr_Density, ncomponents = 1);
	target_Kr->AddElement(enriched_Kr, natoms = 1);

	/*************************************************/
	//             "Stainless steel"
	// Approximate alloy: 85% Fe
	//                    15% Cr
	// Density is the weighted mean of pure Fe and Cr
	/*************************************************/

	G4Element *nat_Fe = new G4Element(name = "natural Fe", symbol = "nat_Fe",
	                                  z = 26, a = 55.845 * g / mole);
	G4Element *nat_Cr = new G4Element(name = "natural Cr", symbol = "nat_Cr",
	                                  z = 24, a = 51.9961 * g / mole);

	G4double Fe_Density = 7.874 * g / cm3;
	G4double Cr_Density = 7.19 * g / cm3;

	G4double Fe_Percent = 0.85;
	G4double Cr_Percent = 0.15;

	G4double StainlessSteel_Density =
	    Fe_Percent * Fe_Density + Cr_Percent * Cr_Density;

	stainlessSteel = new G4Material(name = "Stainless_Steel",
	                                StainlessSteel_Density, ncomponents = 2);

	stainlessSteel->AddElement(nat_Fe, natoms = 1);
	stainlessSteel->AddElement(nat_Cr, natoms = 1);

	/*************************************************/
	// Enriched Se target from 2016 NRF experiment
	/*************************************************/

	/*G4Isotope *Se74 = new G4Isotope(name = "74Se", z = 34, n = 40,
	                                a = 73.922476436 * g / mole);
	G4Isotope *Se76 = new G4Isotope(name = "76Se", z = 34, n = 42,
	                                a = 75.919213597 * g / mole);
	G4Isotope *Se77 = new G4Isotope(name = "77Se", z = 34, n = 43,
	                                a = 76.919914038 * g / mole);
	G4Isotope *Se78 = new G4Isotope(name = "78Se", z = 34, n = 44,
	                                a = 77.91730909 * g / mole);
	G4Isotope *Se80 = new G4Isotope(name = "80Se", z = 34, n = 46,
	                                a = 79.916521271 * g / mole);
	G4Isotope *Se82 = new G4Isotope(name = "82Se", z = 34, n = 48,
	                                a = 81.916699401 * g / mole);*/

	G4Isotope *Se74 = new G4Isotope(name = "74Se", z = 34, a = 74);
	G4Isotope *Se76 = new G4Isotope(name = "76Se", z = 34, a = 76);
	G4Isotope *Se77 = new G4Isotope(name = "77Se", z = 34, a = 77);
	G4Isotope *Se78 = new G4Isotope(name = "78Se", z = 34, a = 78);
	G4Isotope *Se80 = new G4Isotope(name = "80Se", z = 34, a = 80);
	G4Isotope *Se82 = new G4Isotope(name = "82Se", z = 34, a = 83);
	;

	G4Element *enriched_Se =
	    new G4Element(name = "enriched Se", symbol = "Se", ncomponents = 6);

	enriched_Se->AddIsotope(Se74, abundance = 0.01 * perCent);
	enriched_Se->AddIsotope(Se76, abundance = 0.01 * perCent);
	enriched_Se->AddIsotope(Se77, abundance = 0.01 * perCent);
	enriched_Se->AddIsotope(Se78, abundance = 0.02 * perCent);
	enriched_Se->AddIsotope(Se80, abundance = 0.02 * perCent);
	enriched_Se->AddIsotope(Se82, abundance = 99.93 * perCent);

	G4double Se_Mass = 1.99845 * g;
	G4double TargetContainer_Inner_Radius = 10. * mm;
	G4double TargetContainer_Inner_Length = 4. * mm; // Estimated

	G4double Se_Density = Se_Mass / (2. * PI * TargetContainer_Inner_Length *
	                                 TargetContainer_Inner_Radius);

	target_Se = new G4Material(name = "target_Se", Se_Density, ncomponents = 1);
	target_Se->AddElement(enriched_Se, natoms = 1);

	/************************************************************/
	// Enriched Sn target from 2016 NRF experiment (preliminary)
	/************************************************************/

	// G4Isotope *Sn120 = new G4Isotope(name = "120Sn", z = 50, n = 70,
	//                                 a = 119.902201634 * g / mole);
	G4Isotope *Sn120 = new G4Isotope(name = "120Sn", z = 50, a = 120);

	G4Element *enriched_Sn =
	    new G4Element(name = "enriched Sn", symbol = "Sn", ncomponents = 1);

	enriched_Sn->AddIsotope(Sn120, abundance = 100. * perCent);

	G4double Sn_Mass = 5. * g;     // Estimated;
	G4double Sn_Length = 3. * mm;  // Estimated;
	G4double Sn_Radius = 10. * mm; // Estimated;

	G4double Sn_Density = Sn_Mass / (2. * PI * Sn_Length * Sn_Radius);

	target_Sn = new G4Material(name = "target_Sn", Sn_Density, ncomponents = 1);

	target_Sn->AddElement(enriched_Sn, natoms = 1);

	/***********************************/
	//       Lanthanum bromide
	/***********************************/

	// Standard LaBr
	density = 5.06 * g / cm3;
	LaBr3 = new G4Material(name = "LaBr3", density, ncomponents = 2);

	LaBr3->AddElement(nat_La, 1);
	LaBr3->AddElement(nat_Br, 3);

	// Brillance 380 from Enrique Nacher (Santiago)
	LaBr3Ce = new G4Material("Brillance380", density = 5.08 * g / cm3,
	                         ncomponents = 3);
	LaBr3Ce->AddElement(nat_La, fractionmass = 34.855 * perCent);
	LaBr3Ce->AddElement(nat_Br, fractionmass = 60.145 * perCent);
	LaBr3Ce->AddElement(nat_Ce, fractionmass = 5.0 * perCent);

	// Brillance350 from Enrique Nacher (Santiago)
	LaCl3Ce = new G4Material("Brillance350", density = 3.85 * g / cm3,
	                         ncomponents = 3);
	LaCl3Ce->AddElement(nat_La, fractionmass = 53.804 * perCent);
	LaCl3Ce->AddElement(nat_Cl, fractionmass = 41.196 * perCent);
	LaCl3Ce->AddElement(nat_Ce, fractionmass = 5.0 * perCent);

	/***********************************/
	//       Beam tube vacuum
	/***********************************/

	G4Element *nat_N = nist->FindOrBuildElement("N");
	G4Element *nat_C = nist->FindOrBuildElement("C");
	G4Element *nat_Ar = nist->FindOrBuildElement("Ar");

	density = 0.00120479 * g / cm3 * 1.e-3; // Estimated as one thousandth of
	                                        // the normal density of air as
	                                        // given in the Geant4 material
	                                        // database
	pump_vacuum =
	    new G4Material(name = "pump_vacuum", density, ncomponents = 4);
	pump_vacuum->AddElement(nat_O, fractionmass = 23.1781 * perCent);
	pump_vacuum->AddElement(nat_N, fractionmass = 75.5268 * perCent);
	pump_vacuum->AddElement(nat_C, fractionmass = 0.0124 * perCent);
	pump_vacuum->AddElement(nat_Ar, fractionmass = 1.2827 * perCent);
}
