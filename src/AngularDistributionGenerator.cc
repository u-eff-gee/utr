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
	MAX_TRIES_POSITION = 1000;
	MAX_TRIES_MOMENTUM = 1000;

	particleGun = new G4ParticleGun(1);

	// Random point will be sampled inside a box located at (source_x, source_y,
	// source_z). The box has the dimensions [ -range_xyz*0.5 , range_xyz*0.5 ].
	// Check with the auxiliary volume AuxBox in DetectorConstruction.cc whether
	// this box completely contains the desired source.
	source_x = 0. * mm;
	source_y = 0. * mm;
	source_z = 0. * mm;

	range_x = 20. * mm;
	range_y = 20. * mm;
	range_z = 4. * mm;

	// Enter name of the G4PhysicalVolume from which the radiation should be
	// emitted
	Source_PV_Name = "Se82_Target";

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

	if (!checked_position_generator) {

		G4int position_success = 0;

		G4cout << "============================================================"
		          "============"
		       << G4endl;
		G4cout << "Checking Monte-Carlo position generator with "
		       << MAX_TRIES_POSITION << " 3D points ..." << G4endl;

		for (int i = 0; i < MAX_TRIES_POSITION; i++) {
			random_x = (G4UniformRand() - 0.5) * range_x + source_x;
			random_y = (G4UniformRand() - 0.5) * range_y + source_y;
			random_z = (G4UniformRand() - 0.5) * range_z + source_z;

			pv = navi->LocateGlobalPointAndSetup(
			             G4ThreeVector(random_x, random_y, random_z))
			         ->GetName();

			if (pv == Source_PV_Name) {
				position_success++;
			}
		}

		p = (double)position_success / MAX_TRIES_POSITION;
		pnot = (double)1. - p;

		G4cout << "Check finished. Of " << MAX_TRIES_POSITION
		       << " random 3D points, " << position_success
		       << " were inside volume '" << Source_PV_Name << "' ( "
		       << p / perCent << " % )" << G4endl;
		G4cout << "Probability of failure:\tpow( " << pnot << ", "
		       << MAX_TRIES_POSITION
		       << " ) = " << pow(pnot, MAX_TRIES_POSITION) / perCent << " %"
		       << G4endl;
		G4cout << "============================================================"
		          "============"
		       << G4endl << G4endl;

		checked_position_generator = true;
	}

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
			random_w = G4UniformRand()*1.5;

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

		if (pv == Source_PV_Name) {
			randomOrigin = G4ThreeVector(random_x, random_y, random_z);
			particleGun->SetParticlePosition(randomOrigin);
			position_found = true;
			break;
		}
	}

	for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
		random_theta = G4UniformRand() * 180. * deg;
		random_phi = G4UniformRand() * 360. * deg;
		random_w = G4UniformRand()*1.5;

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
		          "could not determine a starting vector after "
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
		if (st[0] == 0. && st[1] == 0. && st[2] == 0.) {
			return true;
		}

		// 0^+ -> 1^+ -> 0^+
		if (st[0] == 0. && st[1] == 1. && st[2] == 0.) {
			if (rand_w <=
			    0.75 * (1 + pow(cos(rand_theta), 2) +
			            pow(sin(rand_theta), 2) * cos(2 * rand_phi))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 1^- -> 0^+
		if (st[0] == 0. && st[1] == -1. && st[2] == 0.) {
			if (rand_w <=
			    0.75 * (1 + pow(cos(rand_theta), 2) -
			            pow(sin(rand_theta), 2) * cos(2 * rand_phi))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 2^+ -> 0^+
		if (st[0] == 0. && st[1] == 2. && st[2] == 0.) {
			if (rand_w <=
			    0.625 * (2 + cos(2 * rand_phi) + cos(4 * rand_phi) -
			             2 * cos(2 * rand_theta) * (1 + 2 * cos(2 * rand_phi)) *
			                 pow(sin(rand_phi), 2))) {
				return true;
			}
			return false;
		}

		// 0^+ -> 1^- -> 2^+
		if (st[0] == 0. && st[1] == -1. && st[2] == 2.) {

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

		// 0^+ -> 1^+ -> 2^+
		if (st[0] == 0. && st[1] == 1. && st[2] == 2.) {

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

		// 1.5^+ -> 2.5^- -> 1.5^+
		if (st[0] == 1.5 && st[1] == -2.5 && st[2] == 1.5) {

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
