#ifndef ANGULARDISTRIBUTIONGENERATOR_HH
#define ANGULARDISTRIBUTIONGENERATOR_HH

#include "G4Navigator.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class AngularDistributionGenerator : public G4VUserPrimaryGeneratorAction {
  public:
	AngularDistributionGenerator();
	~AngularDistributionGenerator();

	void GeneratePrimaries(G4Event *anEvent);
	G4bool AngularDistribution(G4double rand_theta, G4double rand_phi,
	                           G4double rand_w, G4double *states, G4int nstates,
	                           G4double *mixing_ratios);

  private:
	G4ParticleGun *particleGun;

	G4String particleName;
	G4String Source_PV_Name;
	G4double particleEnergy;

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
