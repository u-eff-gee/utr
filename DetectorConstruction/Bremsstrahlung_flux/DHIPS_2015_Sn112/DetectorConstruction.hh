/*
utr - Geant4 simulation of DHIPS at TU Darmstadt
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

#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
	G4VPhysicalVolume *Construct() override;
	void ConstructSDandField() override;

	void print_info() const;

private:
	double targetposition_z;
	double target_length;
	double total_collimator_length;
	double collimator_to_bremstarget;
	double collimator_to_target;
	double bremstarget_thickness1;
	double bremstarget_thickness2;
	double distance_bremstargets;
	G4double block_x;
	G4double block_y;
	G4double World_x;
	G4double World_y;
	G4double World_z;
	G4LogicalVolume *World_logical;

	void ConstructCollimatorBlocks(G4String &name, G4ThreeVector local_coordinates, G4double block_z, G4double holeradius);


};

