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

#ifdef GENERATOR_ANGDIST
#include "AngularDistributionGenerator.hh"
#elif defined GENERATOR_ANGCORR
#include "AngularCorrelationGenerator.hh"
#else
#include "GeneralParticleSource.hh"
#endif

#include "EventAction.hh"
#include "RunAction.hh"

using std::vector;

ActionInitialization::ActionInitialization() : G4VUserActionInitialization(),
                                               n_threads(1) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
#ifdef GENERATOR_ANGDIST
  SetUserAction(new AngularDistributionGenerator);
#elif defined GENERATOR_ANGCORR
  SetUserAction(new AngularCorrelationGenerator);
#else
  SetUserAction(new GeneralParticleSource);
#endif

  EventAction *eventAction = new EventAction();
#ifdef G4MULTITHREADED
  eventAction->setNThreads(n_threads);
#endif
  SetUserAction(eventAction);

  RunAction *runAction = new RunAction();

  vector<bool> record_quantity(NFLAGS);
  for (short i = 0; i < NFLAGS; ++i)
    record_quantity[i] = false;

#ifdef EVENT_ID
  record_quantity[ID] = true;
#endif
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

  if (G4Threading::G4GetThreadId() == 0) {
    G4cout << "================================================================"
              "================"
           << G4endl;
#ifdef EVENT_EVENTWISE
    G4cout << "ActionInitialization: EDEP will be saved to the output file in EVENTWISE mode" << G4endl;
#else
    G4cout << "ActionInitialization: The following quantities will be saved to "
              "the output file:"
           << G4endl;
    for (auto i = 0; i < NFLAGS; i++) {
      if (record_quantity[i]) {
        G4cout << runAction->GetOutputFlagName(i) << G4endl;
      }
    }
#endif
    G4cout << "================================================================"
              "================"
           << G4endl;
  }
  SetUserAction(runAction);
}
