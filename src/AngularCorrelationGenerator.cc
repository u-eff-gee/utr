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

#include "AngularCorrelationGenerator.hh"
#include "AngularCorrelationMessenger.hh"

AngularCorrelationGenerator::AngularCorrelationGenerator()
    : G4VUserPrimaryGeneratorAction(), particleGun(0),
      angdist(0), direction_given(false), checked_momentum_generator(false), checked_position_generator(false) {

	angCorrMessenger = new AngularCorrelationMessenger(this);
	angdist = new AngularDistribution();

	// Set the limit for the number of Monte-Carlo iterations to find a starting
	// point / initial momentum vector
	MAX_TRIES_POSITION = 1e4;
	MAX_TRIES_MOMENTUM = 1e4;

	particleGun = new G4ParticleGun(1);
	particleTable = G4ParticleTable::GetParticleTable();

	navi = G4TransportationManager::GetTransportationManager()
	           ->GetNavigatorForTracking();
}

AngularCorrelationGenerator::~AngularCorrelationGenerator() {
	delete angCorrMessenger;
	delete particleGun;
}

void AngularCorrelationGenerator::GeneratePrimaries(G4Event *anEvent) {

	G4ParticleDefinition *pd = particleTable->FindParticle("geantino");
	particleGun->SetParticleDefinition(pd);
	particleGun->SetParticleEnergy(3.);

	G4ThreeVector randomOrigin = G4ThreeVector(0., 0., 0.);
	G4ThreeVector randomDirection = G4ThreeVector(0., 0., 1.);

	G4double p, pnot;

#ifdef CHECK_POSITION_GENERATOR

	G4int position_success = 0;

	if (!checked_position_generator) {
		for (unsigned int j = 0; j < source_PV_names.size(); ++j) {

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
		for(unsigned long n_particle = 0; n_particle < particles.size(); ++n_particle){

			G4int momentum_success = 0;

			G4cout << "============================================================"
				  "============"
			       << G4endl;
			G4cout << "Checking Monte-Carlo momentum generator with "
			       << MAX_TRIES_MOMENTUM << " 3D vectors for" << G4endl;
			G4cout << "Cascade #" << n_particle << " ( Particle: " << 
				particles[n_particle]->GetParticleName() << " ): ";
			
			if(n_particle == 0 && direction_given){
				G4cout << "( direction: [" << direction.x() << ", " << direction.y() << ", " << direction.z() << " ] )" << G4endl;
				G4cout << "No check necessary." << G4endl;
			G4cout << "============================================================"
				  "============"
			       << G4endl;
			} else if(n_particle > 0 && relative_angle_given[n_particle]){
				G4cout << "( relative angle of " << relative_angle[n_particle]/deg << " degree(s) )" << G4endl;
				G4cout << "No check necessary." << G4endl;
			G4cout << "============================================================"
				  "============"
			       << G4endl;
			} else{
				G4cout << "( angular distribution ";
				for(int n_state = 0; n_state < nstates[n_particle]; ++n_state){
					G4cout << states[n_particle][n_state] << " ";
					if(n_state < nstates[n_particle] - 1)
						G4cout << "-> ";
				}
				G4cout << ") ..." << G4endl;

				for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
					random_theta = acos(2.*G4UniformRand() - 1.);
					random_phi = twopi*G4UniformRand();
					random_w = G4UniformRand() * MAX_W;

					if (!is_polarized[n_particle]){
						if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
						    nstates[n_particle], &mixing_ratios[n_particle][0]) + 
							angdist->AngDist(random_theta,
						    random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0]))
							++momentum_success;
					} else{
						if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
						    nstates[n_particle], &mixing_ratios[n_particle][0]))
							++momentum_success;

					}
					if (!is_polarized[n_particle]){
						if (MAX_W <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
						    nstates[n_particle], &mixing_ratios[n_particle][0]) + 
							angdist->AngDist(random_theta,
						    random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0]))
							++max_w;
					} else{
						if (MAX_W <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
						    nstates[n_particle], &mixing_ratios[n_particle][0]))
							++max_w;

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

			}
		}
		checked_momentum_generator = true;
	}

#endif

	position_found = false;

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

	if (!position_found)
		G4cout << "Warning: AngularCorrelationGenerator: Monte-Carlo method "
		          "could not determine a starting point after "
		       << MAX_TRIES_POSITION << " iterations" << G4endl;

	// Store the emission direction of the previous particle in the cascade
	G4double theta_reference = 0.;
	G4double phi_reference = 0.;

	for(unsigned long n_particle = 0; n_particle < particles.size(); ++n_particle){

		momentum_found = false;

		if(n_particle == 0 && direction_given){
			theta_reference = atan2(direction.z(), sqrt(direction.x()*direction.x() + direction.y()*direction.y()));
			phi_reference = atan2(direction.y(), direction.x());
			particleGun->SetParticleMomentumDirection(direction);
			particleGun->GeneratePrimaryVertex(anEvent);
		} else if(n_particle > 0 && relative_angle_given[n_particle]){
			random_theta = theta_reference + relative_angle[n_particle];
			random_phi = twopi*G4UniformRand();
			theta_reference = random_theta;
			phi_reference = random_phi;

			randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi),
							sin(random_theta) * sin(random_phi),
							cos(random_theta));
			particleGun->SetParticleMomentumDirection(randomDirection);
			particleGun->GeneratePrimaryVertex(anEvent);
		} else {
			for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
				random_theta = acos(2.*G4UniformRand() - 1.);
				random_phi = twopi*G4UniformRand();
				random_w = G4UniformRand() * MAX_W;

				if (!is_polarized[n_particle]){
					if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
					    nstates[n_particle], &mixing_ratios[n_particle][0]) + 
						angdist->AngDist(random_theta,
					    random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0]))
						momentum_found = true;
				} else{
					if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
					    nstates[n_particle], &mixing_ratios[n_particle][0]))
						momentum_found = true;
				}
				if (momentum_found) {
					if(n_particle > 0){
					//	if(random_theta + theta_reference > pi){
					//		random_theta = random_theta + theta_reference - pi;
					//	} else{
					//		random_theta = random_theta + theta_reference;
					//	}
					//	if(random_phi + phi_reference > twopi){
					//		random_phi = random_phi + phi_reference - twopi;
					//	} else{
					//		random_phi = random_phi + phi_reference;
					//	}

						randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi),
										sin(random_theta) * sin(random_phi),
										cos(random_theta));
						particleGun->SetParticleMomentumDirection(randomDirection);
						particleGun->GeneratePrimaryVertex(anEvent);
					} else{
						randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi),
										sin(random_theta) * sin(random_phi),
										cos(random_theta));
						theta_reference = random_theta;
						phi_reference = random_phi;
						particleGun->SetParticleMomentumDirection(randomDirection);
						particleGun->GeneratePrimaryVertex(anEvent);
					}
					break;
				}
			}
		}
	}
}
