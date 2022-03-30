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

#define MAX_ALLOWED_FAIL_CHANCE 1e-6

AngularDistributionGenerator::AngularDistributionGenerator() : G4VUserPrimaryGeneratorAction(), particleGun(0), angdist(0), checked_position_generator(false) {
  angDistMessenger = new AngularDistributionMessenger(this);
  angdist = new AngularDistribution();

  // Set the limit for the number of Monte-Carlo iterations to find a starting point / initial momentum vector
  MAX_TRIES_POSITION = 1e4;
  MAX_TRIES_MOMENTUM = 1e4;

  particleGun = new G4ParticleGun(1);

  navi = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

AngularDistributionGenerator::~AngularDistributionGenerator() {
  delete particleGun;
}

void AngularDistributionGenerator::GeneratePrimaries(G4Event *anEvent) {
  particleGun->SetParticleDefinition(particleDefinition);
  particleGun->SetParticleEnergy(particleEnergy);

  G4ThreeVector randomOrigin = G4ThreeVector(0., 0., 0.);
  G4ThreeVector randomDirection = G4ThreeVector(0., 0., 1.);

  for (G4int i = 0; i < 4; ++i)
    alt_states[i] = states[i];
  if (states[1] == 0.) {
    alt_states[1] = -0.1;
  } else if (states[1] == -0.1) {
    alt_states[1] = 0.;
  } else {
    alt_states[1] = -states[1];
  }

#ifdef CHECK_POSITION_GENERATOR
  check_position_generator();
#endif
#ifdef CHECK_MOMENTUM_GENERATOR
  check_momentum_generator();
#endif

  G4bool position_found = false;
  G4double random_x;
  G4double random_y;
  G4double random_z;
  G4String pv;

  G4bool momentum_found = false;
  G4double random_theta;
  G4double random_phi;
  G4double random_w;

  for (int i = 0; i < MAX_TRIES_POSITION; i++) {
    random_x = (G4UniformRand() - 0.5) * range_x + source_x;
    random_y = (G4UniformRand() - 0.5) * range_y + source_y;
    random_z = (G4UniformRand() - 0.5) * range_z + source_z;
    randomOrigin = G4ThreeVector(random_x, random_y, random_z);

    pv = navi->LocateGlobalPointAndSetup(randomOrigin)->GetName();

    for (auto source_pv : source_PV_names) {
      if (pv == source_pv) {
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
    random_theta = acos(2. * G4UniformRand() - 1.);
    random_phi = twopi * G4UniformRand();
    random_w = G4UniformRand() * MAX_W;

    if (is_polarized) {
      if (random_w <= angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios))
        momentum_found = true;
    } else {
      if (random_w <= (angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios) + angdist->AngDist(random_theta, random_phi, alt_states, nstates, mixing_ratios)) / 2.)
        momentum_found = true;
    }
    if (momentum_found) {
      randomDirection = G4ThreeVector(sin(random_theta) * cos(random_phi), sin(random_theta) * sin(random_phi), cos(random_theta));
      particleGun->SetParticleMomentumDirection(randomDirection);
      break;
    }
  }

  if (!position_found)
    G4cout << "Warning: AngularDistributionGenerator: Monte-Carlo method could not determine a starting point after " << MAX_TRIES_POSITION << " iterations" << G4endl;
  if (!momentum_found)
    G4cout << "Warning: AngularDistributionGenerator: Monte-Carlo method could not determine a starting velocity vector after " << MAX_TRIES_MOMENTUM << " iterations" << G4endl;

  particleGun->GeneratePrimaryVertex(anEvent);
}

void AngularDistributionGenerator::check_momentum_generator() {
  if (checked_momentum_generator)
    return;

  G4double random_theta;
  G4double random_phi;
  G4double random_w;
  G4int momentum_success = 0;
  unsigned int max_w_overflow_counter = 0;
  G4double occurred_max_w = -1.;
  double p_max_w = 0.;

  G4cout << "========================================================================" << G4endl;
  G4cout << "Checking Monte-Carlo momentum generator with " << MAX_TRIES_MOMENTUM << " 3D vectors..." << G4endl;

  for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
    random_theta = acos(2. * G4UniformRand() - 1.);
    random_phi = twopi * G4UniformRand();
    random_w = G4UniformRand() * MAX_W;

    if (is_polarized) {
      if (random_w <= angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios))
        momentum_success++;
    } else {
      if (random_w <= (angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios) + angdist->AngDist(random_theta, random_phi, alt_states, nstates, mixing_ratios)) / 2.)
        momentum_success++;
    }

    if (is_polarized) {
      if (MAX_W < angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios))
        max_w_overflow_counter++;
    } else {
      if (MAX_W < (angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios) + angdist->AngDist(random_theta, random_phi, alt_states, nstates, mixing_ratios)) / 2.)
        max_w_overflow_counter++;
    }

    if (is_polarized) {
      if (occurred_max_w < angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios))
        occurred_max_w = angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios);
    } else {
      if (occurred_max_w < (angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios) + angdist->AngDist(random_theta, random_phi, alt_states, nstates, mixing_ratios)) / 2.)
        occurred_max_w = (angdist->AngDist(random_theta, random_phi, states, nstates, mixing_ratios) + angdist->AngDist(random_theta, random_phi, alt_states, nstates, mixing_ratios)) / 2.;
    }
  }

  G4double p = (double)momentum_success / MAX_TRIES_MOMENTUM;
  G4double pnot = 1. - p;

  G4cout << "Check finished. Of " << MAX_TRIES_MOMENTUM
         << " random 3D momentum vectors, " << momentum_success
         << " were valid ( " << p / perCent << " % )" << G4endl;
  G4cout << "Probability of failure: pow( " << pnot << ", "
         << MAX_TRIES_MOMENTUM
         << " ) = " << pow(pnot, MAX_TRIES_MOMENTUM) / perCent << " %"
         << G4endl;
  if (pow(pnot, MAX_TRIES_MOMENTUM) > MAX_ALLOWED_FAIL_CHANCE) {
    G4cerr << "ERROR: Probability of failure for Monte-Carlo momentum generation of " << pow(pnot, MAX_TRIES_MOMENTUM) / perCent << " % was deemed to high! Aborting..." << G4endl;
    throw std::exception();
  }
  if (max_w_overflow_counter == 0) {
    G4cout << "MAX_W == " << MAX_W << " seems to be high enough as the maximal occurred value of the angular distribution was " << occurred_max_w << G4endl;
  } else {
    p_max_w = (double)max_w_overflow_counter / MAX_TRIES_MOMENTUM;
    G4cout << G4endl;
    G4cerr << "ERROR: In " << max_w_overflow_counter << " out of " << MAX_TRIES_MOMENTUM << " cases (" << p_max_w / perCent << " % ) W(random_theta, random_phi) > MAX_W == " << MAX_W << " was valid. This means that MAX_W is set too low and the angular distribution is truncated! The maximal occurred value of the angular distribution was " << occurred_max_w << ". Aborting..." << G4endl;
    throw std::exception();
  }
  G4cout << "========================================================================" << G4endl << G4endl;
  checked_momentum_generator = true;
}

void AngularDistributionGenerator::check_position_generator() {
  if (checked_position_generator)
    return;

  G4String pv;
  G4String pv_max_x;
  G4String pv_min_x;
  G4String pv_max_y;
  G4String pv_min_y;
  G4String pv_max_z;
  G4String pv_min_z;

  G4double random_x;
  G4double random_y;
  G4double random_z;

  for (auto source_pv : source_PV_names) {
    G4int max_x = 0;
    G4int max_y = 0;
    G4int max_z = 0;
    G4int position_success = 0;

    G4cout << "========================================================================" << G4endl;
    G4cout << "Checking Monte-Carlo position generator with " << MAX_TRIES_POSITION << " 3D points for volume " << source_pv << "..." << G4endl;

    for (int i = 0; i < MAX_TRIES_POSITION; i++) {
      random_x = (G4UniformRand() - 0.5) * range_x + source_x;
      random_y = (G4UniformRand() - 0.5) * range_y + source_y;
      random_z = (G4UniformRand() - 0.5) * range_z + source_z;

      pv = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, random_y, random_z))->GetName();
      pv_max_x = navi->LocateGlobalPointAndSetup(G4ThreeVector(0.5 * range_x + source_x, random_y, random_z))->GetName();
      pv_min_x = navi->LocateGlobalPointAndSetup(G4ThreeVector(-0.5 * range_x + source_x, random_y, random_z))->GetName();
      pv_max_y = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, 0.5 * range_y + source_y, random_z))->GetName();
      pv_min_y = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, -0.5 * range_y + source_y, random_z))->GetName();
      pv_max_z = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, random_y, 0.5 * range_z + source_z))->GetName();
      pv_min_z = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, random_y, -0.5 * range_z + source_z))->GetName();

      if (pv == source_pv) {
        ++position_success;
      }
      if (pv_max_x == source_pv || pv_min_x == source_pv) {
        ++max_x;
      }
      if (pv_max_y == source_pv || pv_min_y == source_pv) {
        ++max_y;
      }
      if (pv_max_z == source_pv || pv_min_z == source_pv) {
        ++max_z;
      }
    }

    G4double p = (G4double)position_success / MAX_TRIES_POSITION;
    G4double pnot = 1. - p;

    G4cout << "Check finished. Of " << MAX_TRIES_POSITION
           << " random 3D points, " << position_success
           << " were inside volume " << source_pv << " ( "
           << p / perCent << " % )" << G4endl;
    G4cout << "Probability of failure: pow( " << pnot << ", "
           << MAX_TRIES_POSITION
           << " ) = " << pow(pnot, MAX_TRIES_POSITION) / perCent << " %"
           << G4endl;
    if (pow(pnot, MAX_TRIES_POSITION) > MAX_ALLOWED_FAIL_CHANCE) {
      G4cerr << "ERROR: Probability of failure for Monte-Carlo position generation of " << pow(pnot, MAX_TRIES_POSITION) / perCent << " % for volume " << source_pv << " was deemed to high! Aborting..." << G4endl;
      throw std::exception();
    }

    if (max_x == 0) {
      G4cout << "X range " << source_x << " +- " << 0.5 * range_x << " seems to be large enough." << G4endl;
    } else {
      G4double p_max_x = (double)max_x / MAX_TRIES_POSITION;
      G4cout << G4endl;
      G4cout << "In " << max_x << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_x / perCent << " % ) randomly sampled points on the x-boundary (sourceX +- sourceDX, sourceY + randomY, sourceZ + randomZ) were still inside the source volume. This may mean that the x range does not encompass the whole source volume." << G4endl;
    }

    if (max_y == 0) {
      G4cout << "Y range " << source_y << " +- " << 0.5 * range_y << " seems to be large enough." << G4endl;
    } else {
      G4double p_max_y = (double)max_y / MAX_TRIES_POSITION;
      G4cout << G4endl;
      G4cout << "In " << max_y << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_y / perCent << " % ) randomly sampled points on the y-boundary (sourceX + randomX, sourceY +- sourceDY, sourceZ + randomZ) were still inside the source volume. This may mean that the y range does not encompass the whole source volume." << G4endl;
    }

    if (max_z == 0) {
      G4cout << "Z range " << source_z << " +- " << 0.5 * range_z << " seems to be large enough." << G4endl;
    } else {
      G4double p_max_z = (double)max_z / MAX_TRIES_POSITION;
      G4cout << G4endl;
      G4cout << "In " << max_z << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_z / perCent << " % ) randomly sampled points on the z-boundary (sourceX + randomX, sourceY + randomY, sourceZ +- sourceDZ) were still inside the source volume. This may mean that the z range does not encompass the whole source volume." << G4endl;
    }

    G4cout << "========================================================================" << G4endl << G4endl;
  }
  checked_position_generator = true;
}
