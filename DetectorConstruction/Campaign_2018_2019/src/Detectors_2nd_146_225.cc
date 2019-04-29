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
#include "Detectors_2nd_146_225.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"

Detectors_2nd_146_225::Detectors_2nd_146_225(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Detectors_2nd_146_225::Construct(G4ThreeVector global_coordinates){

	HPGe_Collection hpge_Collection;

	/**************** HPGE10 HPGe_Cologne *******************/

	G4double hpge10_rt = 47.4 * mm; 
	G4double hpge10_dy = 0. * mm; 
	G4double hpge10_dz = 0. * mm; 
	G4double hpge10_phi = 270. * deg;
	G4double hpge10_theta = 90. * deg;

	HPGe_Coaxial hpge10(World_Logical, "HPGe10");
	hpge10.setProperties(hpge_Collection.HPGe_100_Cologne_73954);
	hpge10.useDewar();
	hpge10.Add_Filter("G4_Cu", 1.*1.15*mm, 50.*mm);
	hpge10.Add_Filter("G4_Pb", 1.*2.*mm, 50.*mm);
	hpge10.Add_Wrap("G4_Pb", 2.*1.2*mm);
	hpge10.Construct(global_coordinates + G4ThreeVector(0., hpge10_dy, hpge10_dz),
			hpge10_theta, hpge10_phi, hpge10_rt, 0.*deg);

	/**************** HPGE11 HPGe_Stuttgart *******************/

	G4double hpge11_rt = 84.4 * mm; 
	G4double hpge11_dy = 0. * mm; 
	G4double hpge11_dz = 0. * mm; 
	G4double hpge11_phi = 0. * deg;
	G4double hpge11_theta = 90. * deg;

	HPGe_Coaxial hpge11(World_Logical, "HPGe11");
	hpge11.setProperties(hpge_Collection.HPGe_86_Stuttgart_31120);
	hpge11.useDewar();
	hpge11.Add_Filter("G4_Cu", 1.*1.15*mm, 50.*mm);
	hpge11.Add_Filter("G4_Pb", 1.*2.*mm, 50.*mm);
	hpge11.Add_Wrap("G4_Pb", 1.*1.2*mm);
	hpge11.Construct(global_coordinates + G4ThreeVector(0., hpge11_dy, hpge11_dz),
			hpge11_theta, hpge11_phi, hpge11_rt, 0.*deg);

	/**************** HPGE12 HPGe_55_TUNL_31524 *******************/

	G4double hpge12_rt = 45.4 * mm; 
	G4double hpge12_dy = 0. * mm; 
	G4double hpge12_dz = 0. * mm; 
	G4double hpge12_phi = 90. * deg;
	G4double hpge12_theta = 90. * deg;

	HPGe_Coaxial hpge12(World_Logical, "HPGe12");
	hpge12.setProperties(hpge_Collection.HPGe_55_TUNL_31524);
	hpge12.useDewar();
	hpge12.Add_Filter("G4_Cu", 1.*1.15*mm, 50.*mm);
	hpge12.Add_Filter("G4_Pb", (1.*1.2 + 1.*2.)*mm, 50.*mm);
	hpge12.Add_Wrap("G4_Pb", 2.*1.2*mm);
	hpge12.Construct(global_coordinates + G4ThreeVector(0., hpge12_dy, hpge12_dz),
			hpge12_theta, hpge12_phi, hpge12_rt, 0.*deg);
}
