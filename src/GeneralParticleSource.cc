#include "GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

GeneralParticleSource::GeneralParticleSource()
    : G4VUserPrimaryGeneratorAction(), particleGun(0) {
	particleGun = new G4GeneralParticleSource();
}

GeneralParticleSource::~GeneralParticleSource() { delete particleGun; }

void GeneralParticleSource::GeneratePrimaries(G4Event *anEvent) {
	particleGun->GeneratePrimaryVertex(anEvent);
}
