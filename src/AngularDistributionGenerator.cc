#include "AngularDistributionGenerator.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

AngularDistributionGenerator::AngularDistributionGenerator()
    : G4VUserPrimaryGeneratorAction(), particleGun(0) {
	particleGun = new G4ParticleGun(1);
}

AngularDistributionGenerator::~AngularDistributionGenerator() {
	delete particleGun;
}

void AngularDistributionGenerator::GeneratePrimaries(G4Event *anEvent) {

	G4Navigator *navi = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

// Random point will be sampled inside a box located at (source_x, source_y, source_z). The box has the dimensions [ -range_xyz*0.5 , range_xyz*0.5 ]
	G4double source_x = 0.*mm;
	G4double source_y = 0.*mm;
	G4double source_z = 0.*mm;

	G4double range_x = 20.*mm;
	G4double range_y = 20.*mm;
	G4double range_z = 4.*mm;

	G4String Source_PV_Name = "Se82_Target";

	G4double random_x, random_y, random_z;
	G4String pv;

	for(int i = 0; i <= 100; i++){
		random_x = (G4UniformRand() - 0.5)*range_x + source_x;
		random_y = (G4UniformRand() - 0.5)*range_y + source_y;
		random_z = (G4UniformRand() - 0.5)*range_z + source_z;

		pv = navi->LocateGlobalPointAndSetup(G4ThreeVector(random_x, random_y, random_z))->GetName();

		G4cout << "Random point " << i << " (" << random_x << ", " << random_y << ", " << random_z << ")" << ", Volume: " << pv << G4endl;

		if(pv == Source_PV_Name)
			G4cout << "--> Point is inside source volume!" << G4endl;
	}

	particleGun->GeneratePrimaryVertex(anEvent);
}
