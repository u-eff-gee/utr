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

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Si28_Target.hh"

/*
 * In runs 473, 474, 541, 542, 548, 549, 555, 560, 561, 563, 564, 571 and 572,
 * at least two 28Si targets were used, either as the actual target of interest
 * or for calibration purposes. This class implements the 28Si target.
 *
 * At the moment, every piece of information here is sensibly estimated, because
 * there is no information in the logbook about the composition or dimensions
 * of the targets.
 *
 */

Si28_Target::Si28_Target():
World_Logical(nullptr)
{}

Si28_Target::Si28_Target(G4LogicalVolume *World_Log):
World_Logical(World_Log)
{}

void Si28_Target::Construct(G4ThreeVector global_coordinates){

	G4Colour white(1.0, 1.0, 1.0);

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *silicon = nist->FindOrBuildMaterial("G4_Si");

	// Create Materials
	G4double Target_Density = 2.3290*g/cm3; // Wikipedia	
	G4double Target_Mass = 2.*g;
	G4double Target_Radius = 10.*mm;
	G4double Target_Length = Target_Mass/(Target_Density*pi*Target_Radius*Target_Radius);

	// Create physical target
	
	G4Tubs *Target_Solid = new G4Tubs("Si28_Solid", 0., Target_Radius, Target_Length*0.5, 0., twopi);

	G4LogicalVolume *Target_Logical = new G4LogicalVolume(Target_Solid, silicon, "Si28_Logical");
	Target_Logical->SetVisAttributes(white);

	new G4PVPlacement(0, global_coordinates + G4ThreeVector(), Target_Logical, "Si28_Target", World_Logical, false, 0);
}
