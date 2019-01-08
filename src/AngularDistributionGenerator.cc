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

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4TransportationManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "AngularDistributionGenerator.hh"
#include "AngularDistributionMessenger.hh"

AngularDistributionGenerator::AngularDistributionGenerator()
    : G4VUserPrimaryGeneratorAction(), particleGun(0),
      angdist(0),checked_position_generator(false) {

	angDistMessenger = new AngularDistributionMessenger(this);
	angdist = new AngularDistribution();

	// Set the limit for the number of Monte-Carlo iterations to find a starting
	// point / initial momentum vector
	MAX_TRIES_POSITION = 1e4;
	MAX_TRIES_MOMENTUM = 1e4;

	particleGun = new G4ParticleGun(1);

	navi = G4TransportationManager::GetTransportationManager()
	           ->GetNavigatorForTracking();
}

AngularDistributionGenerator::~AngularDistributionGenerator() {
	delete particleGun;
}

void AngularDistributionGenerator::GeneratePrimaries(G4Event *anEvent) {
	particleGun->SetParticleDefinition(particleDefinition);
	particleGun->SetParticleEnergy(particleEnergy);

	G4ThreeVector randomOrigin = G4ThreeVector(0., 0., 0.);
	G4ThreeVector randomDirection = G4ThreeVector(0., 0., 1.);

	for(G4int i = 0; i < 4; ++i)
		alt_states[i] = states[i];
	alt_states[1] = -states[1];

	G4double p, pnot;

#ifdef CHECK_POSITION_GENERATOR

	G4int position_success = 0;

	for (unsigned int j = 0; j < source_PV_names.size(); ++j) {
		if (!checked_position_generator) {

			position_success = 0;

			G4cout << "========================================================"
			          "===="
			          "============"
			       << G4endl;
			G4cout << "Checking Monte-Carlo position generator with "
			       << MAX_TRIES_POSITION << " 3D points for volume "
			       << source_PV_names[j] << " ..." << G4endl;

			for (int i = 0; i < MAX_TRIES_POSITION; i++) {
				random_x = (G4UniformRand() - 0.5) * range_x + source_x;
				random_y = (G4UniformRand() - 0.5) * range_y + source_y;
				random_z = (G4UniformRand() - 0.5) * range_z + source_z;

				pv = navi->LocateGlobalPointAndSetup(
				             G4ThreeVector(random_x, random_y, random_z))
				         ->GetName();

				if (pv == source_PV_names[j]) {
					++position_success;
				}
			}

			p = (double)position_success / MAX_TRIES_POSITION;
			pnot = (double)1. - p;

			G4cout << "Check finished. Of " << MAX_TRIES_POSITION
			       << " random 3D points, " << position_success
			       << " were inside volume " << source_PV_names[j] << " ( "
			       << p / perCent << " % )" << G4endl;
			G4cout << "Probability of failure:\tpow( " << pnot << ", "
			       << MAX_TRIES_POSITION
			       << " ) = " << pow(pnot, MAX_TRIES_POSITION) / perCent << " %"
			       << G4endl;
			G4cout << "========================================================"
			          "===="
			          "============"
			       << G4endl << G4endl;
		}
	}
	checked_position_generator = true;

#endif

#ifdef CHECK_MOMENTUM_GENERATOR
	unsigned int max_w = 0;
	double p_max_w = 0.;

	if (!checked_momentum_generator) {
		G4int momentum_success = 0;

		G4cout << "============================================================"
		          "============"
		       << G4endl;
		G4cout << "Checking Monte-Carlo momentum generator with "
		       << MAX_TRIES_MOMENTUM << " 3D vectors ..." << G4endl;

		for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
			random_theta = acos(2.*G4UniformRand() - 1.);
			random_phi = twopi*G4UniformRand();
			random_w = G4UniformRand() * MAX_W;

			if (random_w <= angdist->AngDist(random_theta, random_phi, states,
			                        nstates, mixing_ratios)) {
				++momentum_success;
			} else if(!is_polarized){
				if (random_w <= angdist->AngDist(random_theta, random_phi,
						alt_states, nstates, mixing_ratios)) {
					++momentum_success;
				}
			}
			if (MAX_W <= angdist->AngDist(random_theta, random_phi, states,
			                        nstates, mixing_ratios)) {
				++max_w;
			} else if(!is_polarized){
				if (MAX_W < angdist->AngDist(random_theta, random_phi,
						alt_states, nstates, mixing_ratios)) {
					++momentum_success;
				}
			}
		}

		p = (double)momentum_success / MAX_TRIES_MOMENTUM;
		pnot = (double)1. - p;

		G4cout << "Check finished. Of " << MAX_TRIES_MOMENTUM
		       << " random 3D momentum vectors, " << momentum_success
		       << " were valid ( " << p / perCent << " % )" << G4endl;
		G4cout << "Probability of failure:\tpow( " << pnot << ", "
		       << MAX_TRIES_MOMENTUM
		       << " ) = " << pow(pnot, MAX_TRIES_MOMENTUM) / perCent << " %"
		       << G4endl;
		if(max_w == 0){
			G4cout << "MAX_W == " << MAX_W << " seems to be high enough." << G4endl;
		} else{
			p_max_w = (double) max_w/MAX_TRIES_MOMENTUM; 
			G4cout << G4endl;
			G4cout << "In " << max_w << " out of " << MAX_TRIES_MOMENTUM << " cases (" << p_max_w / perCent << " % ) W(random_theta, random_phi) == MAX_W == " << MAX_W << " was valid. This may mean that MAX_W is set too low and the angular distribution is truncated." << G4endl;
		}
		G4cout << "============================================================"
		          "============"
		       << G4endl << G4endl;

		checked_momentum_generator = true;
	}

#endif
	position_found = false;
	momentum_found = false;

	for (int i = 0; i < MAX_TRIES_POSITION; i++) {
		random_x = (G4UniformRand() - 0.5) * range_x + source_x;
		random_y = (G4UniformRand() - 0.5) * range_y + source_y;
		random_z = (G4UniformRand() - 0.5) * range_z + source_z;

		pv = navi->LocateGlobalPointAndSetup(
		             G4ThreeVector(random_x, random_y, random_z))
		         ->GetName();

		for (unsigned int j = 0; j < source_PV_names.size(); ++j) {
			if (pv == source_PV_names[j]) {
				randomOrigin = G4ThreeVector(random_x, random_y, random_z);
				particleGun->SetParticlePosition(randomOrigin);
				position_found = true;
				break;
			}
		}

		if (position_found) {
			break;
		}
	}

	for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
		random_theta = acos(2.*G4UniformRand() - 1.);
		random_phi = twopi*G4UniformRand();
		random_w = G4UniformRand() * MAX_W;

		if (!is_polarized){
			if (random_w <= angdist->AngDist(random_theta, random_phi, states,
		            nstates, mixing_ratios) + 
			 	angdist->AngDist(random_theta,
			    random_phi, alt_states, nstates, mixing_ratios))
				momentum_found = true;
		} else{
			if (random_w <= angdist->AngDist(random_theta, random_phi, states,
			    nstates, mixing_ratios))
				momentum_found = true;

		}
		if (momentum_found) {
			randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi),
			                                sin(random_theta) * sin(random_phi),
			                                cos(random_theta));
			particleGun->SetParticleMomentumDirection(randomDirection);
			break;
		}
	}

	if (!position_found)
		G4cout << "Warning: AngularDistributionGenerator: Monte-Carlo method "
		          "could not determine a starting point after "
		       << MAX_TRIES_POSITION << " iterations" << G4endl;
	if (!momentum_found)
		G4cout << "Warning: AngularDistributionGenerator: Monte-Carlo method "
		          "could not determine a starting velocity vector after "
		       << MAX_TRIES_MOMENTUM << " iterations" << G4endl;

	particleGun->GeneratePrimaryVertex(anEvent);
}
