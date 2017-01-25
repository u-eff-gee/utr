#ifndef ANGULARDISTRIBUTIONGENERATOR_HH
#define ANGULARDISTRIBUTIONGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"

class AngularDistributionGenerator: public G4VUserPrimaryGeneratorAction{
public:
	AngularDistributionGenerator();
	~AngularDistributionGenerator();

   	void GeneratePrimaries(G4Event* anEvent);

private:
  	G4ParticleGun* particleGun;
};

#endif
