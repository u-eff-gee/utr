/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ActionInitialization.hh"

#ifdef USE_GPS
#include "GeneralParticleSource.hh"
#else
#include "AngularDistributionGenerator.hh"
#endif

#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
#if USE_GPS
	SetUserAction(new GeneralParticleSource);
#else
	SetUserAction(new AngularDistributionGenerator);
#endif
	// SetUserAction(new SteppingAction); // Useful for debugging

	RunAction *runAction = new RunAction();
	runAction->setOutputDir(outputdir);

	unsigned int output_flags[runAction->n_output_flags];
	output_flags[EKIN] = 0;
	output_flags[EDEP] = 1;
	output_flags[PARTICLE] = 1;
	output_flags[VOLUME] = 1;
	output_flags[POSX] = 0;
	output_flags[POSY] = 0;
	output_flags[POSZ] = 0;
	output_flags[MOMX] = 0;
	output_flags[MOMY] = 0;
	output_flags[MOMZ] = 0;
	runAction->SetOutputFlags(output_flags);

	G4cout << "================================================================"
	          "================"
	       << G4endl;
	G4cout << "ActionInitialization: The following quantities will be saved to "
	          "the output file:"
	       << G4endl;
	for (int i = 0; i < runAction->n_output_flags; i++) {
		if (output_flags[i] == 1) {
			G4cout << runAction->GetOutputFlagName(i) << G4endl;
		}
	}
	G4cout << "================================================================"
	          "================"
	       << G4endl;

	SetUserAction(runAction);
}
