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

#include "Physics.hh"

// Electromagnetic modular physics lists
#ifdef EM_FAST
#include "G4EmStandardPhysics_option1.hh"
#endif

#ifdef EM_LIVERMORE
#include "G4EmLivermorePhysics.hh"
#endif

#ifdef EM_LIVERMORE_POLARIZED
#include "G4EmLivermorePolarizedPhysics.hh"
#endif

#ifdef EM_LIVERMORE_POLARIZED_JAEA
#include "EMPhysicsPolarizedJAEA.hh"
#endif

#ifdef EM_STANDARD
#include "G4EmStandardPhysics_option4.hh"
#endif

// Hadronic elastic modular physics lists
#ifdef HADRON_ELASTIC_STANDARD
#include "G4HadronElasticPhysics.hh"
#endif
#ifdef HADRON_ELASTIC_HP
#include "G4HadronElasticPhysicsHP.hh"
#endif
#ifdef HADRON_ELASTIC_LEND
#include "G4HadronElasticPhysicsLEND.hh"
#endif

// Hadronic elastic modular physics lists
#ifdef HADRON_INELASTIC_STANDARD
#include "G4HadronPhysicsFTFP_BERT.hh"
#endif
#ifdef HADRON_INELASTIC_HP
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#endif
#ifdef HADRON_INELASTIC_LEND
#include "G4HadronPhysicsShieldingLEND.hh"
#endif

Physics::Physics(){
	G4cout << "================================================================"
	          "================" << G4endl;
	G4cout << "Using the following physics lists:" << G4endl;

// Electromagnetic modular physics lists
#ifdef EM_FAST 
	G4cout << "\tG4EmStandardPhysics_option1 ..." << G4endl;
	RegisterPhysics( new G4EmStandardPhysics_option1() );
#endif
#ifdef EM_LIVERMORE
	G4cout << "\tG4EmLivermorePhysics ..." << G4endl;
	RegisterPhysics( new G4EmLivermorePhysics() );
#endif
#ifdef EM_LIVERMORE_POLARIZED
	G4cout << "\tG4EmLivermorePolarizedPhysics ..." << G4endl;
	RegisterPhysics( new G4EmLivermorePolarizedPhysics() );
#endif
#ifdef EM_LIVERMORE_POLARIZED_JAEA
	G4cout << "\tG4EmLivermorePolarizedPhysics with JAEA elastic processes ..." << G4endl;
	RegisterPhysics( new EMPhysicsPolarizedJAEA() );
#endif
#ifdef EM_STANDARD 
	G4cout << "\tG4EmStandardPhysics_option4 ..." << G4endl;
	RegisterPhysics( new G4EmStandardPhysics_option4() );
#endif

// Hadronic elastic modular physics lists
#ifdef HADRON_ELASTIC_STANDARD
	G4cout << "\tG4HadronElasticPhysics ..." << G4endl;
	RegisterPhysics( new G4HadronElasticPhysics() );
#endif

#ifdef HADRON_ELASTIC_HP
	G4cout << "\tG4HadronElasticPhysicsHP ..." << G4endl;
	RegisterPhysics( new G4HadronElasticPhysicsHP() );
#endif

#ifdef HADRON_ELASTIC_LEND
	G4cout << "\tG4HadronElasticPhysicsLEND ..." << G4endl;
	RegisterPhysics( new G4HadronElasticPhysicsLEND() );
#endif

// Hadronic inelastic modular physics lists
#ifdef HADRON_INELASTIC_STANDARD
	G4cout << "\tG4HadronPhysicsFTFP_BERT ..." << G4endl;
	RegisterPhysics( new G4HadronPhysicsFTFP_BERT() );
#endif

#ifdef HADRON_INELASTIC_HP
	G4cout << "\tG4HadronPhysicsFTFP_BERT_HP ..." << G4endl;
	RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP() );
#endif

#ifdef HADRON_INELASTIC_LEND
	G4cout << "\tG4HadronPhysicsShieldingLEND ..." << G4endl;
	RegisterPhysics( new G4HadronPhysicsShieldingLEND() );
#endif

	G4cout << "================================================================"
	          "================" << G4endl;
}
