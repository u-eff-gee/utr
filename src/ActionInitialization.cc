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

#include <vector>

#include "ActionInitialization.hh"

#ifdef USE_GPS
#include "GeneralParticleSource.hh"
#else
#include "AngularDistributionGenerator.hh"
#endif

#include "RunAction.hh"
#include "SteppingAction.hh"

using std::vector;

ActionInitialization::ActionInitialization() : 
G4VUserActionInitialization(), 
n_threads(1) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
#ifdef USE_GPS
	SetUserAction(new GeneralParticleSource);
#else
	SetUserAction(new AngularDistributionGenerator);
#endif

	SteppingAction *steppingAction = new SteppingAction();
	SetUserAction(steppingAction);
#ifdef G4MULTITHREADED
	steppingAction->setNThreads(n_threads);
#endif

	RunAction *runAction = new RunAction();
	runAction->setOutputDir(outputdir);

	vector<bool> record_quantity(NFLAGS);
	for(short i = 0; i < NFLAGS; ++i)
		record_quantity[i] = false;

#ifdef EVENT_EDEP
	record_quantity[EDEP] = true;
#endif
#ifdef EVENT_EKIN
	record_quantity[EKIN] = true;
#endif
#ifdef EVENT_PARTICLE
	record_quantity[PARTICLE] = true;
#endif
#ifdef EVENT_VOLUME
	record_quantity[VOLUME] = true;
#endif
#ifdef EVENT_POSX
	record_quantity[POSX] = true;
#endif
#ifdef EVENT_POSY
	record_quantity[POSY] = true;
#endif
#ifdef EVENT_POSZ
	record_quantity[POSZ] = true;
#endif
#ifdef EVENT_MOMX
	record_quantity[MOMX] = true;
#endif
#ifdef EVENT_MOMY
	record_quantity[MOMY] = true;
#endif
#ifdef EVENT_MOMZ
	record_quantity[MOMZ] = true;
#endif

	G4cout << "================================================================"
	          "================"
	       << G4endl;
	G4cout << "ActionInitialization: The following quantities will be saved to "
	          "the output file:"
	       << G4endl;
	for (auto i = 0; i < NFLAGS; i++) {
		if (record_quantity[i]) {
			G4cout << runAction->GetOutputFlagName(i) << G4endl;
		}
	}
	G4cout << "================================================================"
	          "================"
	       << G4endl;

	SetUserAction(runAction);
}
