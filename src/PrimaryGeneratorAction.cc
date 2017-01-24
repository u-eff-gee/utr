#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  particleGun(0)
{
  particleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  particleGun->GeneratePrimaryVertex(anEvent);
}
