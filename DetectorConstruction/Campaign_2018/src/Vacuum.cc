/* utr - Geant4 simulation of the UTR at HIGS
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

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Vacuum.hh"

	Vacuum::Vacuum(G4double relative_density, G4String material_name){

	G4NistManager *nist = G4NistManager::Instance();

	G4Element *nat_O = nist->FindOrBuildElement("O");
	G4Element *nat_N = nist->FindOrBuildElement("N");
	G4Element *nat_C = nist->FindOrBuildElement("C");
	G4Element *nat_Ar = nist->FindOrBuildElement("Ar");
	

	G4double density_of_air = 1.225e-3 * g / cm3; // Density of air at sea level and 288K, Wikipedia
	G4double density = relative_density*density_of_air;
	Vacuum_Material = new G4Material(material_name, density, 4); 

	Vacuum_Material->AddElement(nat_O, 23.1781 * perCent);
	Vacuum_Material->AddElement(nat_N, 75.5268 * perCent);
	Vacuum_Material->AddElement(nat_Ar, 1.2827 * perCent);
	Vacuum_Material->AddElement(nat_C,  0.0124 * perCent);
}
