#ifndef GENERALPARTICLESOURCE_HH
#define GENERALPARTICLESOURCE_HH

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class GeneralParticleSource : public G4VUserPrimaryGeneratorAction {
  public:
	GeneralParticleSource();
	~GeneralParticleSource();

	void GeneratePrimaries(G4Event *anEvent);

  private:
	G4GeneralParticleSource *particleGun;
};

#endif
