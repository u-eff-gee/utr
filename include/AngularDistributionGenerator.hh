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
#pragma once

#include "G4Navigator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include <vector>

#include "AngularDistribution.hh"

#define CHECK_POSITION_GENERATOR 1
#define CHECK_MOMENTUM_GENERATOR 1
// Maximum value for the sampled w
#define MAX_W 3.

using std::vector;

class AngularDistributionMessenger;

class AngularDistributionGenerator : public G4VUserPrimaryGeneratorAction {
  public:
  AngularDistributionGenerator();
  ~AngularDistributionGenerator();

  void GeneratePrimaries(G4Event *anEvent);

  // Self-checks
  void check_position_generator();
  void check_momentum_generator();

  // Set- and Get- methods to use with the AngularDistributionMessenger

  void SetNStates(G4int nst) { nstates = nst; };
  void SetState(G4int statenumber, G4double st) { states[statenumber] = st; };
  void SetDelta(G4int deltanumber, G4double delta) {
    mixing_ratios[deltanumber] = delta;
  };

  void SetParticleEnergy(G4double en) { particleEnergy = en; };
  void SetParticleDefinition(G4ParticleDefinition *pd) {
    particleDefinition = pd;
  };
  void SetSourceX(G4double x) { source_x = x; };
  void SetSourceY(G4double y) { source_y = y; };
  void SetSourceZ(G4double z) { source_z = z; };

  void SetSourceDX(G4double dx) { range_x = dx; };
  void SetSourceDY(G4double dy) { range_y = dy; };
  void SetSourceDZ(G4double dz) { range_z = dz; };

  void AddSourcePV(G4String physvol) { source_PV_names.push_back(physvol); };

  void SetPolarized(G4bool pol) { is_polarized = pol; };

  G4ParticleDefinition *GetParticleDefinition() {
    return particleDefinition;
  };
  G4double GetParticleEnergy() { return particleEnergy; };

  G4int GetNStates() { return nstates; };

  G4double GetState(int statenumber) { return states[statenumber]; };
  G4double GetDelta(int deltanumber) { return mixing_ratios[deltanumber]; };

  G4double GetSourceX() { return source_x; };
  G4double GetSourceY() { return source_y; };
  G4double GetSourceZ() { return source_z; };

  G4double GetSourceDX() { return range_x; };
  G4double GetSourceDY() { return range_y; };
  G4double GetSourceDZ() { return range_z; };

  G4String GetSourcePV(int i) { return source_PV_names[i]; };

  G4bool IsPolarized() { return is_polarized; };

  private:
  G4ParticleGun *particleGun;
  AngularDistributionMessenger *angDistMessenger;
  AngularDistribution *angdist;

  G4ParticleDefinition *particleDefinition;
  vector<G4String> source_PV_names;
  G4double particleEnergy;

  G4int nstates;
  G4double states[4];
  // The alternative states are used for the simulation of unpolarized gamma-ray excitations,
  // since the angular distribution of the transition is the sum of the two possible
  // polarizations, for example E1 and M1. The most simple way to switch between these two
  // possibilities is to switch the parity of the first excited state in the cascade.
  G4double alt_states[4];
  G4double mixing_ratios[3];

  G4double source_x;
  G4double source_y;
  G4double source_z;

  G4double range_x;
  G4double range_y;
  G4double range_z;

  G4bool is_polarized;

  G4Navigator *navi;

  G4double MAX_TRIES_POSITION;
  G4double MAX_TRIES_MOMENTUM;

  G4bool checked_position_generator;
  G4bool checked_momentum_generator;
};
