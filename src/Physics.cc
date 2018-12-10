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

// Electromagnetic modular physics lists
#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

// Hadronics modular physics lists
#include "G4HadronPhysicsShielding.hh"
#include "G4HadronElasticPhysicsLEND.hh"
#include "G4ThermalNeutrons.hh"

#include "Physics.hh"

Physics::Physics(){
	RegisterPhysics( new G4EmStandardPhysics_option4() );
	RegisterPhysics( new G4EmLivermorePhysics());

	
//	RegisterPhysics( new G4HadronElasticPhysics() );
//	RegisterPhysics( new G4HadronPhysicsShielding() );
//	RegisterPhysics( new G4ThermalNeutrons(1) );
}
