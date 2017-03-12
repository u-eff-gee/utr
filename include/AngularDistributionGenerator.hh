#ifndef ANGULARDISTRIBUTIONGENERATOR_HH
#define ANGULARDISTRIBUTIONGENERATOR_HH 1

#include "G4Navigator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

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

	G4ParticleDefinition *GetParticleDefinition() {
		return particleDefinition;
	};
	G4double GetParticleEnergy() { return particleEnergy; };

	G4int GetNStates() { return nstates; };

	G4double GetState(int statenumber) { return states[statenumber]; };
	G4double GetDelta(int deltanumber) { return mixing_ratios[deltanumber]; };

  private:
	G4ParticleGun *particleGun;
	AngularDistributionMessenger *angDistMessenger;

	G4ParticleDefinition *particleDefinition;
	G4String Source_PV_Name;
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
