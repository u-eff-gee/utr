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
#include "Units.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh" //For definition of inch

#define PI 3.141592

Materials *Materials::instance = nullptr;

Materials::Materials() { 
	ConstructMaterials(); 
}

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
	G4Element *nat_O = nist->FindOrBuildElement("O");

	G4double z, a;
	G4double density, abundance, fractionmass;
	G4String name, symbol;
	G4int ncomponents, natoms;

	/******************************************************/
	// Enriched Dy164 targets from 2013 NRF experiments
	/******************************************************/

	// 164Dy metallic from Cologne
	G4double density164Dy = 8.559 * g / cm3;
	target_Dy164 =
	    new G4Material(name = "164Dy", density164Dy, ncomponents = 1);

	G4Isotope *Dy164 = new G4Isotope(name = "164Dy", z = 66, a = 164);
	G4Isotope *Dy163 = new G4Isotope(name = "163Dy", z = 66, a = 163);
	G4Isotope *Dy162 = new G4Isotope(name = "162Dy", z = 66, a = 162);
	G4Isotope *Dy161 = new G4Isotope(name = "161Dy", z = 66, a = 161);

	G4Element *enriched_Dy =
	    new G4Element(name = "enriched Dy", symbol = "Dy", ncomponents = 4);

	enriched_Dy->AddIsotope(Dy164, abundance = 98 * perCent);
	enriched_Dy->AddIsotope(Dy163, abundance = 2 * 24.9 / (18.9 + 25.5 + 24.9) *
	                                           perCent);
	enriched_Dy->AddIsotope(Dy162, abundance = 2 * 25.5 / (18.9 + 25.5 + 24.9) *
	                                           perCent);
	enriched_Dy->AddIsotope(Dy161, abundance = 2 * 18.9 / (18.9 + 25.5 + 24.9) *
	                                           perCent);

	target_Dy164->AddElement(enriched_Dy, natoms = 1);

	// 164Dy2O3 from Yale
	G4double density164Dy2O3 = 7.8 * g / cm3;
	target_Dy164_2O3 =
	    new G4Material(name = "164Dy2O3", density164Dy2O3, ncomponents = 2);
	target_Dy164_2O3->AddElement(enriched_Dy, natoms = 2);
	target_Dy164_2O3->AddElement(nat_O, natoms = 3);

	/******************************************************/
	// Enriched Cr54 target from 2014/2015 NRF experiments
	/******************************************************/

	G4double density54Cr2O3 = 5.35346 * g / cm3;
	target_Cr54_2O3 =
	    new G4Material(name = "54Cr2O3", density54Cr2O3, ncomponents = 2);

	G4Isotope *Cr54 = new G4Isotope(name = "54Cr", z = 24, a = 54);
	G4Isotope *Cr53 = new G4Isotope(name = "53Cr", z = 24, a = 53);
	G4Isotope *Cr52 = new G4Isotope(name = "52Cr", z = 24, a = 52);
	G4Isotope *Cr50 = new G4Isotope(name = "50Cr", z = 24, a = 50);

	G4Element *enriched_Cr =
	    new G4Element(name = "enriched Cr", symbol = "Cr", ncomponents = 4);

	enriched_Cr->AddIsotope(Cr54, abundance = 95.58 * perCent);
	enriched_Cr->AddIsotope(Cr53, abundance = 1.29 * perCent);
	enriched_Cr->AddIsotope(Cr52, abundance = 3.05 * perCent);
	enriched_Cr->AddIsotope(Cr50, abundance = 0.08 * perCent);

	target_Cr54_2O3->AddElement(enriched_Cr, natoms = 2);
	target_Cr54_2O3->AddElement(nat_O, natoms = 3);

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

	target_TiO2 = new G4Material(name = "enriched Ti", density = 4. * g / cm3,
	                             ncomponents = 2);
	// Density is a mean value of different TiO2 crystal lattice configurations.
	target_TiO2->AddElement(enriched_Ti, natoms = 1);
	target_TiO2->AddElement(nat_O, natoms = 2);

	/*************************************************/
	//             "low-density Aluminium"
	// Since it tedious to construct metal beams of
	// holding structures, simply construct a solid
	// beam made of low-density aluminium
	/*************************************************/

	G4Element *nat_Al = new G4Element(name = "natural Al", symbol = "nat_Al", 
					z = 13, a = 26.9815385 * g / mole);

	G4double Al_Density = 2.70 * g / cm3;

	half_density_Al = new G4Material(name = "half_density_Al",
	                                Al_Density*0.5, ncomponents = 1);

	half_density_Al->AddElement(nat_Al, natoms = 1);

	one_third_density_Al = new G4Material(name = "one_third_density_Al",
	                                Al_Density/3., ncomponents = 1);

	one_third_density_Al->AddElement(nat_Al, natoms = 1);

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
	G4Isotope *Se82 = new G4Isotope(name = "82Se", z = 34, a = 82);
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

	G4double Se_Density = Se_Mass / (PI * pow(TargetContainer_Inner_Radius, 2) *
	                                 TargetContainer_Inner_Length);

	target_Se = new G4Material(name = "target_Se", Se_Density, ncomponents = 1);
	target_Se->AddElement(enriched_Se, natoms = 1);

	/*************************************************/
	// Enriched 150Nd target from 2016 NRF experiment
	/*************************************************/

	G4Isotope *Nd142 = new G4Isotope(name = "142Nd", z = 60, a = 142);
	G4Isotope *Nd143 = new G4Isotope(name = "143Nd", z = 60, a = 143);
	G4Isotope *Nd144 = new G4Isotope(name = "144Nd", z = 60, a = 144);
	G4Isotope *Nd145 = new G4Isotope(name = "145Nd", z = 60, a = 145);
	G4Isotope *Nd146 = new G4Isotope(name = "146Nd", z = 60, a = 146);
	G4Isotope *Nd148 = new G4Isotope(name = "148Nd", z = 60, a = 148);
	G4Isotope *Nd150 = new G4Isotope(name = "150Nd", z = 60, a = 150);

	G4Element *enriched_Nd150 =
	    new G4Element(name = "enriched Nd150", symbol = "Nd-150e", ncomponents = 7);

	enriched_Nd150->AddIsotope(Nd142, abundance = 1.26 * perCent);
	enriched_Nd150->AddIsotope(Nd143, abundance = 0.82 * perCent);
	enriched_Nd150->AddIsotope(Nd144, abundance = 1.34 * perCent);
	enriched_Nd150->AddIsotope(Nd145, abundance = 0.74 * perCent);
	enriched_Nd150->AddIsotope(Nd146, abundance = 1.31 * perCent);
	enriched_Nd150->AddIsotope(Nd148, abundance = 0.94 * perCent);
	enriched_Nd150->AddIsotope(Nd150, abundance = 93.59 * perCent);

	G4double Nd150_Mass = 11582.8 * mg;

	G4double Nd150_Container_OuterHeight = 7.5 * mm; // According to Oak Rich
	G4double Nd150_Container_InnerHeight = Nd150_Container_OuterHeight - 1. * mm; // Estimated
	G4double Nd150_Container_OuterRadius = 0.5 * inch; // According to Oak Rich
	G4double Nd150_Container_InnerRadius = Nd150_Container_OuterRadius - 1. * mm; // Estimated

	G4double Nd150_Density = Nd150_Mass / (PI * pow(Nd150_Container_InnerRadius,2) *
	                                 Nd150_Container_InnerHeight);

	target_Nd150 = new G4Material(name = "target_Nd150", Nd150_Density, ncomponents = 2);
	target_Nd150->AddElement(enriched_Nd150, natoms = 2); //Nd(2)O(3) according to Oak Rich
	target_Nd150->AddElement(nat_O, natoms = 3);

	/*************************************************/
	// Enriched 152Sm target from 2016 NRF experiment
	/*************************************************/

	G4Isotope *Sm144 = new G4Isotope(name = "144Sm", z = 62, a = 144);
	G4Isotope *Sm147 = new G4Isotope(name = "147Sm", z = 62, a = 147);
	G4Isotope *Sm148 = new G4Isotope(name = "148Sm", z = 62, a = 148);
	G4Isotope *Sm149 = new G4Isotope(name = "149Sm", z = 62, a = 149);
	G4Isotope *Sm150 = new G4Isotope(name = "150Sm", z = 62, a = 150);
    G4Isotope *Sm152 = new G4Isotope(name = "152Sm", z = 62, a = 152);
    G4Isotope *Sm154 = new G4Isotope(name = "154Sm", z = 62, a = 154);

	G4Element *enriched_Sm152 =
	    new G4Element(name = "enriched Sm152", symbol = "Sm-152e", ncomponents = 7);
  //TODO: There is currently no data sheet for the target (Have to request it at some point from Isoflex with Ref: Certificate of Analysis No.62-02-152-3090).
  //      Only the share of the isotope of interest (152Sm) was written on the flask (96.10%)
  //      So I just assumed that the remaining natural occuring isotopes of Sm are contained in the target weighted with their respective natural abbundance taken from https://www.isotopes.gov/catalog/product.php?element=Samarium
	enriched_Sm152->AddIsotope(Sm144, abundance = 3.90 * 3.09  / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent); 
	enriched_Sm152->AddIsotope(Sm147, abundance = 3.90 * 14.97 / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent);
	enriched_Sm152->AddIsotope(Sm148, abundance = 3.90 * 11.24 / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent);
	enriched_Sm152->AddIsotope(Sm149, abundance = 3.90 * 13.83 / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent);
	enriched_Sm152->AddIsotope(Sm150, abundance = 3.90 * 7.44  / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent);
    enriched_Sm152->AddIsotope(Sm152, abundance = 96.10 * perCent);
    enriched_Sm152->AddIsotope(Sm154, abundance = 3.90 * 22.71 / (3.09+14.97+11.24+13.83+7.44+22.71) * perCent);

	G4double Sm152_Mass = 1.61397 * g;
	G4double Sm152_TargetContainer_Inner_Radius = 10. * mm;
	G4double Sm152_TargetContainer_Inner_Length = 4. * mm; // Estimated

    // Sm152_Density also just estimated (through the height in the target container) as a material in powder form has some arbitary density which is impossible to look up
	G4double Sm152_Density = Sm152_Mass / (PI * pow(Sm152_TargetContainer_Inner_Radius,2) *
	                                 Sm152_TargetContainer_Inner_Length); 

	target_Sm152 = new G4Material(name = "target_Sm152", Sm152_Density, ncomponents = 2);
	target_Sm152->AddElement(enriched_Sm152, natoms = 2); //Assumed Sm(2)O(3) as the oxide
	target_Sm152->AddElement(nat_O, natoms = 3);

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

	G4double Sn_Density = Sn_Mass / (PI * pow(Sn_Radius,2) * Sn_Length);

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

	/***********************************/
	//      Brass (CuZn30) 
	/***********************************/

	G4Element *nat_Cu = nist->FindOrBuildElement("Cu");
	G4Element *nat_Zn = nist->FindOrBuildElement("Zn");

	density = 8.75 * g / cm3; // Estimated from densities given in Wikipedia

	brass = new G4Material(name = "brass", density, ncomponents = 2);

	brass->AddElement(nat_Cu, fractionmass = 70.*perCent);
	brass->AddElement(nat_Zn, fractionmass = 30.*perCent);

	one_third_density_brass = new G4Material(name = "one_third_density_brass", density/3., ncomponents = 2);

	one_third_density_brass->AddElement(nat_Cu, fractionmass = 70.*perCent);
	one_third_density_brass->AddElement(nat_Zn, fractionmass = 30.*perCent);

	/***********************************/
	//      PEEK (C19H12O3) 
	/***********************************/	

	G4Element *nat_H = nist->FindOrBuildElement("H");

	density = 1.32 * g / cm3; // Densitiy given in Wikipedia (https://de.wikipedia.org/wiki/Polyetheretherketon)

	peek = new G4Material(name = "peek", density, ncomponents = 3);

	peek->AddElement(nat_C, fractionmass = 100*19./24.*perCent);
	peek->AddElement(nat_H, fractionmass = 100*1./24.*perCent);
	peek->AddElement(nat_O, fractionmass = 100*4./24.*perCent);

	/***********************************/
	//      P10 gas (90Ar10CH4) 
	/***********************************/	

	density = 0.00156 * g / cm3;	// Densitiy given by SRIM/TRIM database

	P10 = new G4Material(name = "P10", density, ncomponents = 3);

	P10->AddElement(nat_Ar, fractionmass = 90.*perCent);
	P10->AddElement(nat_C, fractionmass = 2.*perCent);
	P10->AddElement(nat_H, fractionmass = 8.*perCent);

	/***********************************/
	//      CF4 gas 
	/***********************************/	

	G4Element *nat_F = nist->FindOrBuildElement("F");

	density = 0.00125 * g / cm3;	// Densitiy given by SRIM/TRIM database

	CF4 = new G4Material(name = "CF4", density, ncomponents = 2);

	CF4->AddElement(nat_C, fractionmass = 100.*1./7.*perCent);
	CF4->AddElement(nat_F, fractionmass = 100.*6./7.*perCent);

	/***********************************/
	//      PCB_coating (Cu + Ni + Au) 
	/***********************************/	

	G4Element *nat_Ni = nist->FindOrBuildElement("Ni");
	G4Element *nat_Au = nist->FindOrBuildElement("Au");

	density =  8.965 * g / cm3; // calculated from relative volume of Cu, Ni and Au with densities from nist library

	PCB_coating = new G4Material(name = "PCB_coating", density, ncomponents = 3);

	PCB_coating->AddElement(nat_Cu, fractionmass = 87.5*perCent);
	PCB_coating->AddElement(nat_Ni, fractionmass = 12.25*perCent);
	PCB_coating->AddElement(nat_Au, fractionmass = 0.25*perCent);

	/***********************************/
	//      PA2200 material used for 3D printing (variant of Polyamide 12)
	// 	Densities of printed and loose material from
	//      https://www.tobatec.net/pdf/Lasersintern_MaterialData_PA_2200.pdf (accessed on 08/21/2019)
	/***********************************/
	
	density = 0.45 * g / cm3;
	PA2200 = new G4Material(name = "PA2200", density, ncomponents = 4);

	// C12H23NO
	PA2200->AddElement(nat_H, fractionmass = 0.11748294772392213);
	PA2200->AddElement(nat_C, fractionmass = 0.7304491137985716);
	PA2200->AddElement(nat_N, fractionmass = 0.07098632321050559);
	PA2200->AddElement(nat_O, fractionmass = 0.08108161526700071);
	
	density = 0.93 * g / cm3;
	PA2200_printed = new G4Material(name = "PA2200_printed", density, ncomponents = 4);

	// C12H23NO
	PA2200_printed->AddElement(nat_H, fractionmass = 0.11748294772392213);
	PA2200_printed->AddElement(nat_C, fractionmass = 0.7304491137985716);
	PA2200_printed->AddElement(nat_N, fractionmass = 0.07098632321050559);
	PA2200_printed->AddElement(nat_O, fractionmass = 0.08108161526700071);

	/******************************************************/
	// Photo-fission targets for 2019 HIgS experiments
	/******************************************************/

	// 240Pu from TUD
	G4double density240Pu = 19.84 * g / cm3;

	target_Pu240 = new G4Material(name = "240Pu", density240Pu, ncomponents = 1);

	G4Isotope *Pu240 = new G4Isotope(name = "240Pu", z = 94, a = 240);

	G4Element *enriched_Pu =
	    new G4Element(name = "enriched Pu", symbol = "Pu", ncomponents = 1);

	enriched_Pu->AddIsotope(Pu240, abundance = 100. * perCent);

	target_Pu240->AddElement(enriched_Pu, natoms = 1);

	// 239Pu from TUD, not a real target!!! 239Pu(n,f) used to simulate gamma/neutron emission of 240Pu(g,f)
	G4double density239Pu = 19.84 * g / cm3;

	target_Pu239 = new G4Material(name = "239Pu", density239Pu, ncomponents = 1);

	G4Isotope *Pu239 = new G4Isotope(name = "239Pu", z = 94, a = 239);

	G4Element *enriched_Pu2 =
	    new G4Element(name = "enriched Pu2", symbol = "Pu", ncomponents = 1);

	enriched_Pu2->AddIsotope(Pu239, abundance = 100. * perCent);

	target_Pu239->AddElement(enriched_Pu2, natoms = 1);

	// 232Th from TUD, P-1156
	G4double density232Th = 11.72 * g / cm3;

	target_Th232 = new G4Material(name = "232Th", density232Th, ncomponents = 1);

	G4Isotope *Th232 = new G4Isotope(name = "232Th", z = 90, a = 232);

	G4Element *enriched_Th =
	    new G4Element(name = "enriched Th", symbol = "Th", ncomponents = 1);

	enriched_Th->AddIsotope(Th232, abundance = 100. * perCent);

	target_Th232->AddElement(enriched_Th, natoms = 1);

	// 238U from TUD, P-1134
	G4double density238U = 18.95 * g / cm3;

	target_U238 = new G4Material(name = "238U", density238U, ncomponents = 1);

	G4Isotope *U238 = new G4Isotope(name = "238U", z = 92, a = 238);

	G4Element *enriched_U =
	    new G4Element(name = "enriched U", symbol = "U", ncomponents = 1);

	enriched_U->AddIsotope(U238, abundance = 100. * perCent);

	target_U238->AddElement(enriched_U, natoms = 1);

}
