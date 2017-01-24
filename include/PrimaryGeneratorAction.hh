#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction{
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();

   	void GeneratePrimaries(G4Event* anEvent);

private:
  	G4GeneralParticleSource* particleGun;
};

#endif
