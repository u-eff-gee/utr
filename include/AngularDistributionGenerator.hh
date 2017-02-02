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
	G4ThreeVector AngularDistributionRandomDirection(G4double *states,
	                                                 G4int nstates,
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

	G4Navigator *navi;

	G4bool checked;
};

#endif
