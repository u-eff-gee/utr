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

#include "Units.hh"
#include "Detectors_G3_150Sm.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr_3x3.hh"

Detectors_G3_150Sm::Detectors_G3_150Sm(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Detectors_G3_150Sm::Construct(G4ThreeVector global_coordinates){

	HPGe_Collection hpge_Collection;

	G4double hpgesCuFilterThickness =    3./64.*inch;
	G4double hpgesPbFilterThickness = 3.*3./64.*inch;
	// HPGe distances: 1 inch beam pipe radius, 1 inch distance from beam pipe to filters, filter thicknesses and 2mm "Container Shielding" (Written in J. Kleemann's private paper logbook,
	// it's very likely, that the filter case ring is meant, the filter cases (Containers for the shielding (filters)) were introduced around this beam time according to U. Gayer)
	G4double hpgesDistance = (1.+1.) * inch + hpgesCuFilterThickness + hpgesPbFilterThickness + 2*mm;

	// All filter diameters are unknown (here 45mm arbitrarily chosen), but all their thicknesses are known
	// The LaBr detector distances are unknown, here 100mm are arbitrarily chosen (however, the used filters are known)
	// Whether a lead wrap around the detectors was used is unknown (for both HPGe and LaBr), here it is assumed that this was not the case
	// The assignment of the four LaBr detectors to the four phi angles is unknown, however the angles are basically equivalent anyways
	// Likewise, for HPGe1 and HPGe2 it is not known which was at phi=0° and which at phi=180°, and for HPGe3 and HPGe4 which was at phi=90° and phi=270°


	/**************** HPGE1 *******************/

	G4double hpge1_rt =  hpgesDistance;
	G4double hpge1_dy = 0. * mm;
	G4double hpge1_dz = 0. * mm;
	G4double hpge1_phi = 0. * deg;
	G4double hpge1_theta = 90. * deg;

	HPGe_Coaxial hpge1(World_Logical, "HPGe1");
	hpge1.setProperties(hpge_Collection.HPGe_60_TUNL_31061);
	hpge1.useFilterCase();
	hpge1.useFilterCaseRing();
	hpge1.useDewar();
	hpge1.Add_Filter("G4_Cu", hpgesCuFilterThickness, 45.*mm);
	hpge1.Add_Filter("G4_Pb", hpgesPbFilterThickness, 45.*mm);
	//hpge1.Add_Wrap("G4_Pb", 1.*1.2*mm);
	hpge1.Construct(global_coordinates + G4ThreeVector(0., hpge1_dy, hpge1_dz),
			hpge1_theta, hpge1_phi, hpge1_rt, 0.*deg);

	/**************** HPGE2 *******************/

	G4double hpge2_rt = hpgesDistance; // 1 inch beam pipe radius, 1 inch distance to filters and filter thicknesses
	G4double hpge2_dy = 0. * mm;
	G4double hpge2_dz = 0. * mm;
	G4double hpge2_phi = 180. * deg;
	G4double hpge2_theta = 90. * deg;

	HPGe_Coaxial hpge2(World_Logical, "HPGe2");
	hpge2.setProperties(hpge_Collection.HPGe_60_TUNL_30986);
	hpge2.useFilterCase();
	hpge2.useFilterCaseRing();
	hpge2.useDewar();
	hpge2.Add_Filter("G4_Cu", hpgesCuFilterThickness, 45.*mm);
	hpge2.Add_Filter("G4_Pb", hpgesPbFilterThickness, 45.*mm);
	//hpge2.Add_Wrap("G4_Pb", 1.*1.2*mm);
	hpge2.Construct(global_coordinates + G4ThreeVector(0., hpge2_dy, hpge2_dz),
			hpge2_theta, hpge2_phi, hpge2_rt, 0.*deg);

	/**************** HPGE3 *******************/

	G4double hpge3_rt = hpgesDistance; // 1 inch beam pipe radius, 1 inch distance to filters and filter thicknesses
	G4double hpge3_dy = 0. * mm; 
	G4double hpge3_dz = 0. * mm;
	G4double hpge3_phi = 90. * deg;
	G4double hpge3_theta = 90. * deg;

	HPGe_Coaxial hpge3(World_Logical, "HPGe3");
	hpge3.setProperties(hpge_Collection.HPGe_60_TUNL_40663);
	hpge3.useFilterCase();
	hpge3.useFilterCaseRing();
	hpge3.useDewar();
	hpge3.Add_Filter("G4_Cu", hpgesCuFilterThickness, 45.*mm);
	hpge3.Add_Filter("G4_Pb", hpgesPbFilterThickness, 45.*mm);
	//hpge3.Add_Wrap("G4_Pb", 1.*1.2*mm);
	hpge3.Construct(global_coordinates + G4ThreeVector(0., hpge3_dy, hpge3_dz),
			hpge3_theta, hpge3_phi, hpge3_rt, 0.*deg);

	/**************** HPGE4 *******************/

	G4double hpge4_rt = hpgesDistance; // 1 inch beam pipe radius, 1 inch distance to filters and filter thicknesses 
	G4double hpge4_dy = 0. * mm; 
	G4double hpge4_dz = 0. * mm; 
	G4double hpge4_phi = 270. * deg;
	G4double hpge4_theta = 90. * deg;

	HPGe_Coaxial hpge4(World_Logical, "HPGe4");
	hpge4.setProperties(hpge_Collection.HPGe_60_TUNL_21033);
	hpge4.useFilterCase();
	hpge4.useFilterCaseRing();
	hpge4.useDewar();
	hpge4.Add_Filter("G4_Cu", hpgesCuFilterThickness, 45.*mm);
	hpge4.Add_Filter("G4_Pb", hpgesPbFilterThickness, 45.*mm);
	//hpge4.Add_Wrap("G4_Pb", 1.*1.2*mm);
	hpge4.Construct(global_coordinates + G4ThreeVector(0., hpge4_dy, hpge4_dz),
			hpge4_theta, hpge4_phi, hpge4_rt, 0.*deg);

	/**************** LABR1 *******************/

	G4double labr1_rt = 100 * mm; // The LaBr detector distances are unknown, here 100mm are arbitrarily chosen (however, the used filters are known)
	G4double labr1_dy = 0. * mm; 
	G4double labr1_dz = 0. * mm; 
	G4double labr1_phi = 45. * deg;
	G4double labr1_theta = 135. * deg;

	LaBr_3x3 labr1(World_Logical, "LaBr1");
	labr1.useFilterCase();
	labr1.useFilterCaseRing();
	//labr1.useHousing();
	labr1.Add_Filter("G4_Cu", 1./8.*inch, 45.*mm);
	labr1.Add_Filter("G4_Pb", 3./64.*inch, 45.*mm);
	//labr1.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr1.Construct(global_coordinates + G4ThreeVector(0., labr1_dy, labr1_dz),
			labr1_theta, labr1_phi, labr1_rt);

	/**************** LABR2 *******************/

	G4double labr2_rt = 100 * mm; // The LaBr detector distances are unknown, here 100mm are arbitrarily chosen (however, the used filters are known)
	G4double labr2_dy = 0. * mm; 
	G4double labr2_dz = 0. * mm; 
	G4double labr2_phi = 135. * deg;
	G4double labr2_theta = 135. * deg;

	LaBr_3x3 labr2(World_Logical, "LaBr2");
	labr2.useFilterCase();
	labr2.useFilterCaseRing();
	//labr2.useHousing();
	labr2.Add_Filter("G4_Cu", 1./8.*inch, 45.*mm);
	labr2.Add_Filter("G4_Pb", 3./64.*inch, 45.*mm);
	//labr2.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr2.Construct(global_coordinates + G4ThreeVector(0., labr2_dy, labr2_dz),
			labr2_theta, labr2_phi, labr2_rt);

	/**************** LABR3 *******************/

	G4double labr3_rt = 100. * mm; // The LaBr detector distances are unknown, here 100mm are arbitrarily chosen (however, the used filters are known)
	G4double labr3_dy = 0. * mm; 
	G4double labr3_dz = 0. * mm; 
	G4double labr3_phi = 225. * deg;
	G4double labr3_theta = 135. * deg;

	LaBr_3x3 labr3(World_Logical, "LaBr3");
	labr3.useFilterCase();
	labr3.useFilterCaseRing();
	//labr3.useHousing();
	labr3.Add_Filter("G4_Cu", 1./8.*inch, 45.*mm);
	//labr3.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr3.Construct(global_coordinates + G4ThreeVector(0., labr3_dy, labr3_dz),
			labr3_theta, labr3_phi, labr3_rt);

	/**************** LABR4 *******************/

	G4double labr4_rt = 100. * mm; // The LaBr detector distances are unknown, here 100mm are arbitrarily chosen (however, the used filters are known)
	G4double labr4_dy = 0. * mm;
	G4double labr4_dz = 0. * mm;
	G4double labr4_phi = 315. * deg;
	G4double labr4_theta = 135. * deg;

	LaBr_3x3 labr4(World_Logical, "LaBr4");
	labr4.useFilterCase();
	labr4.useFilterCaseRing();
	//labr4.useHousing();
	labr4.Add_Filter("G4_Cu", 1./8.*inch, 45.*mm);
	//labr4.Add_Wrap("G4_Pb", 1.*1.2*mm);
	labr4.Construct(global_coordinates + G4ThreeVector(0., labr4_dy, labr4_dz),
			labr4_theta, labr4_phi, labr4_rt);
}
