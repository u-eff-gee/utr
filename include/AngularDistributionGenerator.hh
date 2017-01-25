#ifndef ANGULARDISTRIBUTIONGENERATOR_HH
#define ANGULARDISTRIBUTIONGENERATOR_HH

#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class AngularDistributionGenerator : public G4VUserPrimaryGeneratorAction {
  public:
	AngularDistributionGenerator();
	~AngularDistributionGenerator();

	void GeneratePrimaries(G4Event *anEvent);

  private:
	G4ParticleGun *particleGun;
};

#endif
