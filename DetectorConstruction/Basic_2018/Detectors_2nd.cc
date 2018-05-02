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

#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"

#include "Units.hh"
#include "Detectors_2nd.hh"
#include "HPGe_Stuttgart.hh"

Detectors_2nd::Detectors_2nd(){
	
	G4NistManager *nist = G4NistManager::Instance();

	G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

	G4double Detectors_2nd_X = 70.*inch; // Dimension of mother volume, arbitrary
	G4double Detectors_2nd_Y = 70.*inch; // Dimension of mother volume, arbitrary
	Detectors_2nd_Length = 70.*inch; // Dimension of mother volume, arbitrary
	
	// Mother volume
	G4Box *Detectors_2nd_Solid = new G4Box("Detectors_2nd_Solid", Detectors_2nd_X*0.5, Detectors_2nd_Y*0.5, Detectors_2nd_Length*0.5);
	Detectors_2nd_Logical = new G4LogicalVolume(Detectors_2nd_Solid, air, "Detectors_2nd_Logical");


	HPGe_Stuttgart hpge_Stuttgart = HPGe_Stuttgart("HPGe_Stuttgart");
	G4LogicalVolume *HPGe_Stuttgart_Logical = hpge_Stuttgart.Get_Logical();

	new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), HPGe_Stuttgart_Logical, "HPGe_Stuttgart", Detectors_2nd_Logical, false, 0, false);
}
