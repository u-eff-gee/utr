#include "AngularDistributionGenerator.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4TransportationManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#define MAX_TRIES 1000
#define CHECK_MC_GENERATOR 1

AngularDistributionGenerator::AngularDistributionGenerator()
    : G4VUserPrimaryGeneratorAction(), particleGun(0), checked(false) {
	particleGun = new G4ParticleGun(1);

	// Set particle type and energy

	particleName = "gamma";
	particleEnergy = 3. * MeV;

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
	particleGun->SetParticleDefinition(particle);

	particleGun->SetParticleEnergy(particleEnergy);

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

	// Enter identifiers for the angular distribution to be simulated
	const G4int nstates = 3;
	G4double states[nstates] = {0., 1., 0.};
	G4double mixing_ratios[nstates - 1] = {0., 0.};

	G4ThreeVector randomOrigin = G4ThreeVector(0., 0., 0.);
	G4ThreeVector randomDirection = G4ThreeVector(0., 0., 1.);

#ifdef CHECK_MC_GENERATOR
	if (!checked) {

		G4int nhit = 0;

		G4cout << "Checking Monte-Carlo position generator with " << MAX_TRIES
		       << " 3D points ..." << G4endl;

		for (int i = 0; i < MAX_TRIES; i++) {
			random_x = (G4UniformRand() - 0.5) * range_x + source_x;
			random_y = (G4UniformRand() - 0.5) * range_y + source_y;
			random_z = (G4UniformRand() - 0.5) * range_z + source_z;

			pv = navi->LocateGlobalPointAndSetup(
			             G4ThreeVector(random_x, random_y, random_z))
			         ->GetName();

			if (pv == Source_PV_Name) {
				nhit++;
			}
		}

		G4cout << "Check finished." << G4endl;
		G4cout << "Of " << MAX_TRIES << " random 3D points, " << nhit
		       << " were inside volume '" << Source_PV_Name << "' ( "
		       << (double)nhit / MAX_TRIES * 100. << " percent )" << G4endl;

		checked = true;
	}
#endif

	for (int i = 0; i < MAX_TRIES; i++) {
		random_x = (G4UniformRand() - 0.5) * range_x + source_x;
		random_y = (G4UniformRand() - 0.5) * range_y + source_y;
		random_z = (G4UniformRand() - 0.5) * range_z + source_z;

		pv = navi->LocateGlobalPointAndSetup(
		             G4ThreeVector(random_x, random_y, random_z))
		         ->GetName();

		if (pv == Source_PV_Name) {
			randomOrigin = G4ThreeVector(random_x, random_y, random_z);
			randomDirection = AngularDistributionRandomDirection(
			    states, nstates, mixing_ratios);

			particleGun->SetParticlePosition(randomOrigin);
			particleGun->SetParticleMomentumDirection(randomDirection);

			particleGun->GeneratePrimaryVertex(anEvent);
			return;
		}
	}

	particleGun->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector AngularDistributionGenerator::AngularDistributionRandomDirection(
    G4double *states, G4int nstates, G4double *mixing_ratios) {

	G4double random_theta, random_phi, random_w;

	if (nstates == 3) {
		// 0^+ -> 1^+ -> 0^+
		if (states[0] == 0. && states[1] == 1. && states[2] == 0.) {
			for (int i = 0; i < MAX_TRIES; i++) {
				random_theta = G4UniformRand() * 180. * deg;
				random_phi = G4UniformRand() * 360. * deg;
				random_w = G4UniformRand() * 1.5;

				if (random_w <=
				    0.75 * (1 + pow(cos(random_theta), 2) +
				            pow(sin(random_theta), 2) * cos(2 * random_phi))) {
					break;
				}
			}

			return G4ThreeVector(cos(random_phi) * sin(random_theta),
			                     sin(random_phi) * sin(random_theta),
			                     cos(random_theta));
		}

		// 0^+ -> 1^+ -> 0^+
		if (states[0] == 0. && states[1] == -1. && states[2] == 0.) {
			for (int i = 0; i < MAX_TRIES; i++) {
				random_theta = G4UniformRand() * 180. * deg;
				random_phi = G4UniformRand() * 360. * deg;
				random_w = G4UniformRand() * 1.5;

				if (random_w <=
				    0.75 * (1 + pow(cos(random_theta), 2) -
				            pow(sin(random_theta), 2) * cos(2 * random_phi))) {
					break;
				}
			}

			return G4ThreeVector(cos(random_phi) * sin(random_theta),
			                     sin(random_phi) * sin(random_theta),
			                     cos(random_theta));
		}

		// 0^+ -> 2^+ -> 0^+
		if (states[0] == 0. && states[1] == 2. && states[2] == 0.) {
			for (int i = 0; i < MAX_TRIES; i++) {
				random_theta = G4UniformRand() * 180. * deg;
				random_phi = G4UniformRand() * 360. * deg;
				random_w = G4UniformRand() * 2.5;

				if (random_w <=
				    0.625 * (2 + cos(2 * random_phi) + cos(4 * random_phi) -
				             2 * cos(2 * random_theta) *
				                 (1 + 2 * cos(2 * random_phi)) *
				                 pow(sin(random_phi), 2))) {
					break;
				}
			}

			return G4ThreeVector(cos(random_phi) * sin(random_theta),
			                     sin(random_phi) * sin(random_theta),
			                     cos(random_theta));
		}

		// 0^+ -> 1^- -> 2^+
		if (states[0] == 0. && states[1] == -1. && states[2] == 2.) {
			for (int i = 0; i < MAX_TRIES; i++) {
				random_theta = G4UniformRand() * 180. * deg;
				random_phi = G4UniformRand() * 360. * deg;
				random_w = G4UniformRand() * 1.05;

				if (random_w <=
				    ((-2. + 3. * sqrt(5.)) *
				     (-1 + 3 * pow(cos(random_phi), 2) -
				      3 * cos(2 * random_theta) * pow(sin(random_phi), 2))) /
				            (20. * (1 + pow(mixing_ratios[1], 2))) +
				        0.125 * (7 + 3 * pow(cos(random_phi), 2) -
				                 3 * cos(2 * random_theta) *
				                     pow(sin(random_theta), 2))) {
					break;
				}
			}

			return G4ThreeVector(cos(random_phi) * sin(random_theta),
			                     sin(random_phi) * sin(random_theta),
			                     cos(random_theta));
		}

		// 0^+ -> 1^+ -> 2^+
		if (states[0] == 0. && states[1] == 1. && states[2] == 2.) {
			for (int i = 0; i < MAX_TRIES; i++) {
				random_theta = G4UniformRand() * 180. * deg;
				random_phi = G4UniformRand() * 360. * deg;
				random_w = G4UniformRand() * 1.05;

				if (random_w <=
				    ((-2. + 3. * sqrt(5.)) *
				     (-1 + 3 * pow(cos(random_phi), 2) +
				      3 * cos(2 * random_theta) * pow(sin(random_phi), 2))) /
				            (20. * (1 + pow(mixing_ratios[1], 2))) +
				        0.125 * (7 + 3 * pow(cos(random_phi), 2) +
				                 3 * cos(2 * random_theta) *
				                     pow(sin(random_theta), 2))) {
					break;
				}
			}

			return G4ThreeVector(cos(random_phi) * sin(random_theta),
			                     sin(random_phi) * sin(random_theta),
			                     cos(random_theta));
		}

	} else if (nstates == 4) {
		G4cout << "Warning: AngularDistributionGenera:: Required spin sequence "
		          "not found."
		       << G4endl;
		return G4ThreeVector();

	} else {
		G4cout << "Warning: AngularDistributionGenera:: Required spin sequence "
		          "not found."
		       << G4endl;
		return G4ThreeVector();
	}

	return G4ThreeVector();
}
