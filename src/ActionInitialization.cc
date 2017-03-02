#include "ActionInitialization.hh"
#include "AngularDistributionGenerator.hh"
#include "GeneralParticleSource.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
	SetUserAction(new GeneralParticleSource);
	// SetUserAction(new AngularDistributionGenerator);
	// SetUserAction(new SteppingAction); // Useful for debugging

	RunAction *runAction = new RunAction();

	unsigned int output_flags[runAction->n_output_flags];
	output_flags[EKIN] = 1;
	output_flags[EDEP] = 1;
	output_flags[PARTICLE] = 1;
	output_flags[VOLUME] = 1;
	output_flags[POSX] = 1;
	output_flags[POSY] = 1;
	output_flags[POSZ] = 1;
	output_flags[MOMX] = 1;
	output_flags[MOMY] = 1;
	output_flags[MOMZ] = 1;
	runAction->SetOutputFlags(output_flags);

	SetUserAction(runAction);
}
