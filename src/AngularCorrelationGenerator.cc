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
      angdist(0),
      MAX_TRIES_POSITION(1e4),
      MAX_TRIES_MOMENTUM(1e4),
      direction_given(false),
      checked_momentum_generator(false),
      checked_position_generator(false) {
  angCorrMessenger = new AngularCorrelationMessenger(this);
  angdist = new AngularDistribution();

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

#ifdef CHECK_POSITION_GENERATOR
  check_position_generator();
#endif

#ifdef CHECK_MOMENTUM_GENERATOR
  check_momentum_generator();
#endif

  G4ThreeVector randomPosition = G4ThreeVector(0., 0., 0.);
  randomPosition = generate_position();

  G4ThreeVector randomDirection = G4ThreeVector(0., 0., 1.);
  G4ThreeVector referenceDirection = randomDirection;

  G4ThreeVector randomPolarization = G4ThreeVector(1., 0., 0.);
  G4ThreeVector referencePolarization = randomPolarization;

  G4ThreeVector eulerAngles(0., 0., 0.);

  for (unsigned long n_particle = 0; n_particle < particles.size(); ++n_particle) {
    randomDirection = generate_direction(n_particle);
    randomPolarization = generate_polarization(n_particle);

    if (n_particle > 0) {
      eulerAngles = get_euler_angles(referenceDirection, referencePolarization);
      randomDirection = randomDirection.rotateX(eulerAngles.y()).rotateZ(eulerAngles.x());
      randomPolarization = randomPolarization.rotateZ(
                                                 eulerAngles.z())
                               .rotateX(eulerAngles.y())
                               .rotateZ(eulerAngles.x());
    }

    particleGun->SetParticleDefinition(particles[n_particle]);
    particleGun->SetParticleEnergy(particleEnergies[n_particle]);
    particleGun->SetParticlePosition(randomPosition);
    particleGun->SetParticleMomentumDirection(randomDirection);
    particleGun->SetParticlePolarization(randomPolarization);
    particleGun->GeneratePrimaryVertex(anEvent);

    referenceDirection = randomDirection;
    referencePolarization = randomPolarization;
  }
}

G4ThreeVector AngularCorrelationGenerator::generate_position() {

  for (int i = 0; i < MAX_TRIES_POSITION; i++) {
    random_x = (G4UniformRand() - 0.5) * range_x + source_x;
    random_y = (G4UniformRand() - 0.5) * range_y + source_y;
    random_z = (G4UniformRand() - 0.5) * range_z + source_z;

    pv = navi->LocateGlobalPointAndSetup(
                 G4ThreeVector(random_x, random_y, random_z))
             ->GetName();

    for (unsigned int j = 0; j < source_PV_names.size(); ++j) {
      if (pv == source_PV_names[j]) {
        return G4ThreeVector(random_x, random_y, random_z);
      }
    }
  }

  G4cout << "Warning: AngularCorrelationGenerator: Monte-Carlo method "
            "could not determine a starting point after "
         << MAX_TRIES_POSITION << " iterations" << G4endl;
  return G4ThreeVector();
}

void AngularCorrelationGenerator::check_position_generator() {

  if (!checked_position_generator) {
    G4int position_success = 0;

    G4int max_x = 0;
    G4int max_y = 0;
    G4int max_z = 0;

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
        pvx = navi->LocateGlobalPointAndSetup(
                      G4ThreeVector(0.5 * range_x + source_x, random_y, random_z))
                  ->GetName();
        pvmx = navi->LocateGlobalPointAndSetup(
                       G4ThreeVector(-0.5 * range_x + source_x, random_y, random_z))
                   ->GetName();
        pvy = navi->LocateGlobalPointAndSetup(
                      G4ThreeVector(random_x, 0.5 * range_y + source_y, random_z))
                  ->GetName();
        pvmy = navi->LocateGlobalPointAndSetup(
                       G4ThreeVector(random_x, -0.5 * range_y + source_y, random_z))
                   ->GetName();
        pvz = navi->LocateGlobalPointAndSetup(
                      G4ThreeVector(random_x, random_y, 0.5 * range_z + source_z))
                  ->GetName();
        pvmz = navi->LocateGlobalPointAndSetup(
                       G4ThreeVector(random_x, random_y, -0.5 * range_z + source_z))
                   ->GetName();

        if (pv == source_PV_names[j]) {
          ++position_success;
        }
        if (pvx == source_PV_names[j] || pvmx == source_PV_names[j]) {
          ++max_x;
        }
        if (pvy == source_PV_names[j] || pvmy == source_PV_names[j]) {
          ++max_y;
        }
        if (pvz == source_PV_names[j] || pvmz == source_PV_names[j]) {
          ++max_z;
        }
      }

      G4double p = (G4double)position_success / MAX_TRIES_POSITION;
      G4double pnot = 1. - p;

      G4cout << "Check finished. Of " << MAX_TRIES_POSITION
             << " random 3D points, " << position_success
             << " were inside volume " << source_PV_names[j] << " ( "
             << p / perCent << " % )" << G4endl;
      G4cout << "Probability of failure:\tpow( " << pnot << ", "
             << MAX_TRIES_POSITION
             << " ) = " << pow(pnot, MAX_TRIES_POSITION) / perCent << " %"
             << G4endl;

      if (max_x == 0) {
        G4cout << "X range " << source_x << " +- " << 0.5 * range_x << " seems to be large enough." << G4endl;
      } else {
        G4double p_max_x = (double)max_x / MAX_TRIES_POSITION;
        G4cout << G4endl;
        G4cout << "In " << max_x << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_x / perCent << " % ) the randomly sampled point (sourceX +- sourceDX, sourceY + randomY, sourceZ + randomZ) was still inside the source volume. This may mean that the x range does not encompass the whole source volume." << G4endl;
      }

      if (max_y == 0) {
        G4cout << "Y range " << source_y << " +- " << 0.5 * range_y << " seems to be large enough." << G4endl;
      } else {
        G4double p_max_y = (double)max_y / MAX_TRIES_POSITION;
        G4cout << G4endl;
        G4cout << "In " << max_y << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_y / perCent << " % ) the randomly sampled point (sourceX + randomX, sourceY +- sourceDY, sourceZ + randomZ) was still inside the source volume. This may mean that the y range does not encompass the whole source volume." << G4endl;
      }

      if (max_z == 0) {
        G4cout << "Z range " << source_z << " +- " << 0.5 * range_z << " seems to be large enough." << G4endl;
      } else {
        G4double p_max_z = (double)max_z / MAX_TRIES_POSITION;
        G4cout << G4endl;
        G4cout << "In " << max_z << " out of " << MAX_TRIES_POSITION << " cases (" << p_max_z / perCent << " % ) the randomly sampled point (sourceX + randomX, sourceY + randomY, sourceZ +- sourceDZ) was still inside the source volume. This may mean that the z range does not encompass the whole source volume." << G4endl;
      }

      G4cout << "========================================================"
                "===="
                "============"
             << G4endl << G4endl;
    }
  }
  checked_position_generator = true;
}

G4ThreeVector AngularCorrelationGenerator::generate_direction(unsigned long n_particle) {

  if (n_particle == 0 && direction_given) {

    return direction / direction.mag();

  } else if (n_particle > 0 && relative_angle_given[n_particle]) {
    random_phi = twopi * G4UniformRand();

    G4ThreeVector randomDirection(0., 0., 1.);
    randomDirection.setMag(1.);
    randomDirection.setTheta(relative_angle[n_particle]);
    randomDirection.setPhi(random_phi);

    return randomDirection / randomDirection.mag();

  } else {
    G4ThreeVector randomDirection(0., 0., 1.);

    for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
      random_theta = acos(2. * G4UniformRand() - 1.);
      random_phi = twopi * G4UniformRand();
      random_w = G4UniformRand() * MAX_W;

      if (!is_polarized[n_particle]) {
        if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                         nstates[n_particle], &mixing_ratios[n_particle][0]) +
                            angdist->AngDist(random_theta,
                                             random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0])) {
          randomDirection.setTheta(random_theta);
          randomDirection.setPhi(random_phi);
          return randomDirection;
        }
      } else {
        if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                         nstates[n_particle], &mixing_ratios[n_particle][0])) {
          randomDirection.setTheta(random_theta);
          randomDirection.setPhi(random_phi);
          return randomDirection;
        }
      }
    }
  }
  return G4ThreeVector();
}

void AngularCorrelationGenerator::check_momentum_generator() {

  if (!checked_momentum_generator) {

    G4int momentum_success = 0;
    unsigned int max_w = 0;
    double p_max_w = 0.;

    for (unsigned long n_particle = 0; n_particle < particles.size(); ++n_particle) {

      momentum_success = 0;
      max_w = 0;
      p_max_w = 0.;

      G4cout << "============================================================"
                "============"
             << G4endl;
      G4cout << "Monte-Carlo momentum generator with "
             << MAX_TRIES_MOMENTUM << " 3D vectors for" << G4endl;
      G4cout << "Cascade step #" << n_particle + 1 << " ( Particle: " << particles[n_particle]->GetParticleName() << " ) " << G4endl;

      if (!momentum_generator_check_unnecessary(n_particle)) {
        for (int i = 0; i < MAX_TRIES_MOMENTUM; i++) {
          random_theta = acos(2. * G4UniformRand() - 1.);
          random_phi = twopi * G4UniformRand();
          random_w = G4UniformRand() * MAX_W;

          if (!is_polarized[n_particle]) {
            if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                             nstates[n_particle], &mixing_ratios[n_particle][0]) +
                                angdist->AngDist(random_theta,
                                                 random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0]))
              ++momentum_success;
          } else {
            if (random_w <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                             nstates[n_particle], &mixing_ratios[n_particle][0]))
              ++momentum_success;
          }
          if (!is_polarized[n_particle]) {
            if (MAX_W <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                          nstates[n_particle], &mixing_ratios[n_particle][0]) +
                             angdist->AngDist(random_theta,
                                              random_phi, &alt_states[n_particle][0], nstates[n_particle], &mixing_ratios[n_particle][0]))
              ++max_w;
          } else {
            if (MAX_W <= angdist->AngDist(random_theta, random_phi, &states[n_particle][0],
                                          nstates[n_particle], &mixing_ratios[n_particle][0]))
              ++max_w;
          }
        }

        G4double p = (double)momentum_success / MAX_TRIES_MOMENTUM;
        G4double pnot = (double)1. - p;

        G4cout << "Check finished. Of " << MAX_TRIES_MOMENTUM
               << " random 3D momentum vectors, " << momentum_success
               << " were valid ( " << p / perCent << " % )" << G4endl;
        G4cout << "Probability of failure:\tpow( " << pnot << ", "
               << MAX_TRIES_MOMENTUM
               << " ) = " << pow(pnot, MAX_TRIES_MOMENTUM) / perCent << " %"
               << G4endl;
        if (max_w == 0) {
          G4cout << "MAX_W == " << MAX_W << " seems to be high enough." << G4endl;
        } else {
          p_max_w = (double)max_w / MAX_TRIES_MOMENTUM;
          G4cout << G4endl;
          G4cout << "In " << max_w << " out of " << MAX_TRIES_MOMENTUM << " cases (" << p_max_w / perCent << " % ) W(random_theta, random_phi) == MAX_W == " << MAX_W << " was valid. This may mean that MAX_W is set too low and the angular distribution is truncated." << G4endl;
        }
        G4cout << "============================================================"
                  "============"
               << G4endl << G4endl;

      } else {
        G4cout << "Check not necessary." << G4endl;
        G4cout << "============================================================"
                  "============"
               << G4endl;
      }
    }
    checked_momentum_generator = true;
  }
}

bool AngularCorrelationGenerator::momentum_generator_check_unnecessary(unsigned long n_particle) {

  G4bool unnecessary = false;

  if (n_particle == 0 && direction_given) {
    G4cout << "Direction            : ( " << direction.x() << ", " << direction.y() << ", " << direction.z() << " )" << G4endl;
    unnecessary = true;

  } else if (n_particle > 0 && relative_angle_given[n_particle]) {
    G4cout << "Relative angle       : " << relative_angle[n_particle] / deg << " degree(s)" << G4endl;
    unnecessary = true;
  } else {
    G4cout << "Angular distribution : ";
    for (int n_state = 0; n_state < nstates[n_particle]; ++n_state) {
      G4cout << states[n_particle][n_state] << " ";
      if (n_state < nstates[n_particle] - 1)
        G4cout << "-> ";
    }
    G4cout << G4endl;
  }

  if (!is_polarized[n_particle]) {
    G4cout << "Polarization         : unpolarized" << G4endl;
  } else {
    G4cout << "Polarization         : ( " << polarization[n_particle].x() << ", " << polarization[n_particle].y() << ", " << polarization[n_particle].z() << " )" << G4endl;
  }

  return unnecessary;
}

G4ThreeVector AngularCorrelationGenerator::generate_polarization(unsigned long n_particle) {
  if (!is_polarized[n_particle]) {
    random_theta = acos(2. * G4UniformRand() - 1.);
    random_phi = twopi * G4UniformRand();

    G4ThreeVector random_polarization(1., 0., 0.);
    random_polarization.setMag(1.);
    random_polarization.setTheta(random_theta);
    random_polarization.setPhi(random_phi);

    return random_polarization;
  } else {
    return polarization[n_particle] / polarization[n_particle].mag();
  }
}

G4ThreeVector AngularCorrelationGenerator::get_euler_angles(G4ThreeVector reference_direction, G4ThreeVector reference_polarization) {

  G4double gamma = 0.;
  if (reference_polarization.mag() == 0.) {
    gamma = twopi * G4UniformRand();
  } else {
    gamma = reference_polarization.getPhi();
  }

  G4double beta = acos(reference_direction.z());
  if (reference_direction.y() > 0.)
    beta = -beta;

  G4double alpha = 0.;

  if (beta == 0.) {
    alpha = 0.;
  } else {
    alpha = asin(reference_direction.x() / sin(beta));
  }

  return G4ThreeVector(alpha, beta, gamma);
}
