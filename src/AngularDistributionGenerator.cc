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

#define CHECK_POSITION_GENERATOR 1
#define CHECK_MOMENTUM_GENERATOR 1

AngularDistributionGenerator::AngularDistributionGenerator()
    : G4VUserPrimaryGeneratorAction(), particleGun(0),
      checked_position_generator(false) {

	angDistMessenger = new AngularDistributionMessenger(this);

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

	G4double p, pnot;

#ifdef CHECK_POSITION_GENERATOR

	G4int position_success = 0;

	for(unsigned int j = 0; j < source_PV_names.size(); ++j){
		if (!checked_position_generator) {

			position_success = 0;

			G4cout << "============================================================"
				  "============"
			       << G4endl;
			G4cout << "Checking Monte-Carlo position generator with "
			       << MAX_TRIES_POSITION << " 3D points for volume " << source_PV_names[j] << " ..." << G4endl;

			for (int i = 0; i < MAX_TRIES_POSITION; i++) {
				random_x = (G4UniformRand() - 0.5) * range_x + source_x;
				random_y = (G4UniformRand() - 0.5) * range_y + source_y;
				random_z = (G4UniformRand() - 0.5) * range_z + source_z;

				pv = navi->LocateGlobalPointAndSetup(
					     G4ThreeVector(random_x, random_y, random_z))
					 ->GetName();

				if (pv == source_PV_names[j]) {
					position_success++;
				}
			}

			p = (double)position_success / MAX_TRIES_POSITION;
			pnot = (double) 1. - p;

			G4cout << "Check finished. Of " << MAX_TRIES_POSITION
			       << " random 3D points, " << position_success
			       << " were inside volume " << source_PV_names[j] << " ( " <<  p / perCent << " % )" << G4endl;
			G4cout << "Probability of failure:\tpow( " << pnot << ", "
			       << MAX_TRIES_POSITION
			       << " ) = " << pow(pnot, MAX_TRIES_POSITION) / perCent << " %"
			       << G4endl;
			G4cout << "============================================================"
				  "============"
			       << G4endl << G4endl;

		}
	}
	checked_position_generator = true;

#endif

#ifdef CHECK_MOMENTUM_GENERATOR

	if (!checked_momentum_generator) {
		G4int momentum_success = 0;

		G4cout << "============================================================"
		          "============"
		       << G4endl;
		G4cout << "Checking Monte-Carlo momentum generator with "
		       << MAX_TRIES_MOMENTUM << " 3D vectors ..." << G4endl;

		for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
			random_theta = G4UniformRand() * 180. * deg;
			random_phi = G4UniformRand() * 360. * deg;
			random_w = G4UniformRand() * 1.5;

			if (AngularDistribution(random_theta, random_phi, random_w, states,
			                        nstates, mixing_ratios)) {
				momentum_success++;
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

		for(unsigned int j = 0; j < source_PV_names.size(); ++j){
			if (pv == source_PV_names[j]) {
				randomOrigin = G4ThreeVector(random_x, random_y, random_z);
				particleGun->SetParticlePosition(randomOrigin);
				position_found = true;
				break;
			}
		}

		if(position_found){
			break;
		}
	}

	for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
		random_theta = G4UniformRand() * 180. * deg;
		random_phi = G4UniformRand() * 360. * deg;
		random_w = G4UniformRand() * 1.5;

		if (AngularDistribution(random_theta, random_phi, random_w, states,
		                        nstates, mixing_ratios)) {
			randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi),
			                                sin(random_theta) * sin(random_phi),
			                                cos(random_theta));
			particleGun->SetParticleMomentumDirection(randomDirection);
			momentum_found = true;
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

G4bool AngularDistributionGenerator::AngularDistribution(
    G4double rand_theta, G4double rand_phi, G4double rand_w, G4double *st,
    G4int nst, G4double *mix) {

	if (nst == 3) {
		// 0.1^+ -> 0.1^+ -> 0.1^+
		// Wildcard for test distributions
		if (st[0] == 0.1 && st[1] == 0.1 && st[2] == 0.1) {
			if (rand_theta >= 85. * deg && rand_theta <= 95. * deg &&
			    ((rand_phi >= 355. * deg && rand_phi <= 360. * deg) ||
			     (rand_phi >= 0. * deg && rand_phi <= 5. * deg))) {
				return true;
			}

			return false;
		}

		// 0^+ -> 0^+ -> 0^+
		// Isotropic distribution
		if ((st[0] == 0. && st[1] == 0. && st[2] == 0.) || (st[0] == -0.1 && st[1] == -0.1 && st[2] == -0.1)) {
			return true;
		}

		// 0^+ -> 1^+ -> 0^+ or 0^- -> 1^- -> 0^-
		if ((st[0] == 0. && st[1] == 1. && st[2] == 0.) || (st[0] == -0.1 && st[1] == -1. && st[2] == -2.)) {
			if (rand_w <=
			    0.75 * (1 + pow(cos(rand_theta), 2) +
			            pow(sin(rand_theta), 2) * cos(2 * rand_phi))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 1^- -> 0^+ or 0^- -> 1^- -> 0^-
		if ((st[0] == 0. && st[1] == -1. && st[2] == 0.) || (st[0] == -0.1 && st[1] == 1. && st[2] == -0.1)) {
			if (rand_w <=
			    0.75 * (1 + pow(cos(rand_theta), 2) -
			            pow(sin(rand_theta), 2) * cos(2 * rand_phi))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 2^+ -> 0^+ or 0^- -> 2^- -> 0^-
		if ((st[0] == 0. && st[1] == 2. && st[2] == 0.) || (st[0] == -0.1 && st[1] == -2. && st[2] == -0.1)) {
			if (rand_w <=
			    0.625 * (2 + cos(2 * rand_phi) + cos(4 * rand_phi) -
			             2 * cos(2 * rand_theta) * (1 + 2 * cos(2 * rand_phi)) *
			                 pow(sin(rand_phi), 2))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 2^+ -> 2^+ or 0^- -> 2^- -> 2^-
		if ((st[0] == 0. && st[1] == 2. && st[2] == 2.) || (st[0] == -0.1 && st[1] == -2. && st[2] == -2.)) {
			if (rand_w <= 1./((1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]))*(
			1.16071 + 
			mix[1]*(0.298807 + mix[1]) + 
			(0.0892857 + (2.09165 + 0.25*mix[1])*mix[1])*cos(2.*rand_phi) +
			pow(cos(rand_theta), 2)*(-1.33929 + (-5.37853 - 0.375*mix[1])*mix[1] + (-1.51786 + (-9.56183 - 0.875*mix[1])*mix[1]*cos(2.*rand_phi))) +
			pow(cos(rand_theta), 4)*(1.42857 + (7.47018 + 0.625*mix[1])*mix[1] + (1.42857 + (7.47018 + 0.625*mix[1])*mix[1])*cos(2.*rand_phi))
		)  

			) {
				return true;
			}
			return false;
		}


		// 0^+ -> 1^- -> 2^+ or 0^- -> 1^+ -> 2^-
		if ((st[0] == 0. && st[1] == -1. && st[2] == 2.) || (st[0] == -0.1 && st[1] == 1. && st[2] == -2.)) {

			if (rand_w <=
			    1. / (1. + pow(mix[1], 2)) *
			        (0.975 + (-0.33541 + 0.875 * mix[1]) * mix[1] +
			         (-0.075 + (-1.00623 - 0.375 * mix[1]) * mix[1]) *
			             cos(2. * rand_phi) +
			         pow(cos(rand_theta), 2) *
			             (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1] +
			              (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1]) *
			                  cos(2. * rand_phi)))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 1^+ -> 2^+ or 0^- -> 1^- -> 2^-
		if ((st[0] == 0. && st[1] == 1. && st[2] == 2.) || (st[0] == -0.1 && st[1] == -1. && st[2] == -2.)) {

			if (rand_w <=
			    1. / (1. + pow(mix[1], 2)) *
			        (0.975 + (-0.33541 + 0.875 * mix[1]) * mix[1] +
			         (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1]) *
			             cos(2. * rand_phi) +
			         pow(cos(rand_theta), 2) *
			             (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1] +
			              (-0.075 + (-1.00623 - 0.375 * mix[1]) * mix[1]) *
			                  cos(2. * rand_phi)))) {
				return true;
			}
			return false;
		}

		// 1.5^+ -> 2.5^- -> 1.5^+ or 1.5^- -> 2.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -2.5 && st[2] == 1.5) || (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			if (rand_w <=
			    1. / ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2))) *
			        ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2)) -
			         0.5 * (0.374166 - (1.89737 + 0.190901 * mix[1]) * mix[1]) *
			             ((0.374166 + (1.89737 - 0.190901 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(rand_theta), 2)) +
			              0.572703 * (-1.96 + mix[0] * (3.313 + mix[0])) *
			                  (pow(cos(rand_theta), 2) - 1.) *
			                  cos(2 * rand_phi)) +
			         0.0621963 * pow(mix[0], 2) * pow(mix[1], 2) *
			             (3. +
			              pow(cos(rand_theta), 4) *
			                  (35. - 35. * cos(2 * rand_phi)) -
			              5. * cos(2 * rand_phi) +
			              pow(cos(rand_theta), 2) *
			                  (-30. + 40. * cos(2 * rand_phi))))) {
				return true;
			}
			return false;
		}

		// 1.5^+ -> 2.5^+ -> 1.5^+ or 1.5^- -> 2.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 2.5 && st[2] == 1.5) || (st[0] == -1.5 && st[1] == -2.5 && st[2] == -1.5)) {

			if (rand_w <=
			    1. / ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2))) *
			        ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2)) -
			         0.5 * (0.374166 - (1.89737 + 0.190901 * mix[1]) * mix[1]) *
			             ((0.374166 + (1.89737 - 0.190901 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(rand_theta), 2)) -
			              0.572703 * (-1.96 + mix[0] * (3.313 + mix[0])) *
			                  (pow(cos(rand_theta), 2) - 1.) *
			                  cos(2 * rand_phi)) +
			         0.0621963 * pow(mix[0], 2) * pow(mix[1], 2) *
			             (3. +
			              pow(cos(rand_theta), 2) *
			                  (-30. - 40. * cos(2 * rand_phi)) +
			              5. * cos(2 * rand_phi) +
			              pow(cos(rand_theta), 4) *
			                  (35. + 35. * cos(2 * rand_phi))))) {
				return true;
			}
			return false;
		}
		
		// 1.5^+ -> 1.5^+ -> 1.5^+ or 1.5^- -> 1.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 1.5 && st[2] == 1.5) || (st[0] == -1.5 && st[1] == -1.5 && st[2] == -1.5)) {

			if (rand_w <= 1./(2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]))*
				(
					2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]) - 
					(0.4 + 1.54919*mix[1])*
					(
						(0.4 - 1.54919*mix[0])*(1. - 3.*pow(cos(rand_theta), 2)) + 
						(-1.2 - 1.54919*mix[0])*cos(2.*rand_phi)*pow(sin(rand_theta), 2)		
					)
				)
			                  ) {
				return true;
			}
			return false;
		}
		
		// 1.5^+ -> 1.5^- -> 1.5^+ or 1.5^- -> 1.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -1.5 && st[2] == 1.5) || (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			if (rand_w <= 1./(2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]))*
				(
					2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]) - 
					(0.4 + 1.54919*mix[1])*
					(
						(0.4 - 1.54919*mix[0])*(1. - 3.*pow(cos(rand_theta), 2)) + 
						(1.2 + 1.54919*mix[0])*cos(2.*rand_phi)*pow(sin(rand_theta), 2)		
					)
				)
			                  ) {
				return true;
			}
			return false;
		}
		
		// 0.5^- -> 1.5^- -> 0.5^- or 0.5^+ -> 1.5^+ -> 0.5^+
		if ((st[0] == -0.5 && st[1] == -1.5 && st[2] == -0.5) || (st[0] == 0.5 && st[1] == 1.5 && st[2] == 0.5)) {

			if (rand_w <= 1./(2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]))*
				(
					2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]) - (0.5 + (-1.73205 - 0.5*mix[1])*mix[1])*(
							(0.5 + (1.73205 - 0.5*mix[0])*mix[0])*(1.-3.*pow(cos(rand_theta), 2)) + 1.5*(-1. + mix[0]*(1.1547 + mix[0]))*(-1. + pow(cos(rand_theta),2))*cos(2.*rand_phi)
					)
				)
			        ) {
				return true;
			}
			return false;
		}
		
		// 0.5^- -> 1.5^+ -> 0.5^- or 0.5^+ -> 1.5^- -> 0.5^+
		if ((st[0] == -0.5 && st[1] == 1.5 && st[2] == -0.5) || (st[0] == 0.5 && st[1] == -1.5 && st[2] == 0.5) ) {

			if (rand_w <= 1./(2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]))*
				(
					2.*(1. + mix[0]*mix[0])*(1. + mix[1]*mix[1]) - (0.5 + (-1.73205 - 0.5*mix[1])*mix[1])*(
							(0.5 + (1.73205 - 0.5*mix[0])*mix[0])*(1.-3.*pow(cos(rand_theta), 2)) - 1.5*(-1. + mix[0]*(1.1547 + mix[0]))*(-1. + pow(cos(rand_theta),2))*cos(2.*rand_phi)
					)
				)
			        ) {
				return true;
			}
			return false;
		}

	} else if (nst == 4) {
		G4cout
		    << "Warning: AngularDistributionGenerator:: Required spin sequence "
		       "not found."
		    << G4endl;
		return false;

	} else {
		G4cout
		    << "Warning: AngularDistributionGenerator:: Required spin sequence "
		       "not found."
		    << G4endl;
		return false;
	}

	return false;
}
