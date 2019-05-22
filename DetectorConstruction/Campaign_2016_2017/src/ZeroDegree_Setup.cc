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

#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "ZeroDegree_Setup.hh"

ZeroDegree_Setup::ZeroDegree_Setup(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void ZeroDegree_Setup::Construct(G4ThreeVector global_coordinates){

	// Zero degree detector
	
	HPGe_Collection hpge_Collection;
	G4double zerodegree_rt = 0. * mm;
	G4double zerodegree_dy = 0. * mm;
	G4double zerodegree_phi = 180. * deg;
	G4double zerodegree_theta = 0. * deg;

	G4RotationMatrix* zerodegree_rotation = new G4RotationMatrix();
	zerodegree_rotation->rotateZ(-zerodegree_phi);
	zerodegree_rotation->rotateY(-zerodegree_theta);

	HPGe_Coaxial zerodegree(World_Logical, "ZeroDegree");
	zerodegree.setProperties(hpge_Collection.HPGe_120_TUNL_40383);
	zerodegree.Construct(global_coordinates + G4ThreeVector(0., zerodegree_dy, 0.),
			zerodegree_theta, zerodegree_phi, zerodegree_rt, 0.*deg);
}
