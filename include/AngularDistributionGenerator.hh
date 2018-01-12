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

#ifndef ANGULARDISTRIBUTIONGENERATOR_HH
#define ANGULARDISTRIBUTIONGENERATOR_HH 1

#include "G4Navigator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include <vector>

// 360 deg / 2*pi
#define RAD_TO_DEG 57.29577951308232
// Maximum value for the sampled w
#define MAX_W 3.

using std::vector;

class AngularDistributionMessenger;

class AngularDistributionGenerator : public G4VUserPrimaryGeneratorAction {
  public:
	AngularDistributionGenerator();
	~AngularDistributionGenerator();

	void GeneratePrimaries(G4Event *anEvent);
	G4bool AngularDistribution(G4double rand_theta, G4double rand_phi,
	                           G4double rand_w, G4double *states, G4int nstates,
	                           G4double *mixing_ratios);

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

  private:
	G4ParticleGun *particleGun;
	AngularDistributionMessenger *angDistMessenger;

	G4ParticleDefinition *particleDefinition;
	vector<G4String> source_PV_names;
	G4double particleEnergy;

	G4int nstates;
	G4double states[4];
	G4double mixing_ratios[3];

	G4double source_x;
	G4double source_y;
	G4double source_z;

	G4double range_x;
	G4double range_y;
	G4double range_z;

	G4String pv;
	G4double random_x;
	G4double random_y;
	G4double random_z;

	G4double random_theta;
	G4double random_phi;
	G4double random_w;

	G4Navigator *navi;

	G4double MAX_TRIES_POSITION;
	G4bool position_found;
	G4double MAX_TRIES_MOMENTUM;
	G4bool momentum_found;

	G4bool checked_position_generator;
	G4bool checked_momentum_generator;
};

#endif
