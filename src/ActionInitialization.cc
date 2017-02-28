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
	output_flags[EDEP] = 0;
	output_flags[PARTICLE] = 1;
	output_flags[VOLUME] = 1;
	output_flags[POSX] = 0;
	output_flags[POSY] = 0;
	output_flags[POSZ] = 0;
	output_flags[MOMX] = 0;
	output_flags[MOMY] = 0;
	output_flags[MOMZ] = 0;
	runAction->SetOutputFlags(output_flags);

	SetUserAction(runAction);
}
