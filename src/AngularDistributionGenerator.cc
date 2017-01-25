#include "AngularDistributionGenerator.hh"
#include "G4Event.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"


AngularDistributionGenerator::AngularDistributionGenerator()
: G4VUserPrimaryGeneratorAction(),
  	particleGun(0)
{
  	particleGun = new G4ParticleGun(1);
}

AngularDistributionGenerator::~AngularDistributionGenerator(){
  	delete particleGun;
}

void AngularDistributionGenerator::GeneratePrimaries(G4Event* anEvent){
  	particleGun->GeneratePrimaryVertex(anEvent);
}
