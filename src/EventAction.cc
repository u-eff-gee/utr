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

#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include <chrono>

#include "G4LogicalVolume.hh"
#include "utrConfig.h"

using std::setw;
using std::string;
using std::to_string;

// static const auto StartRunTime = time();
static const auto StartRunTime = std::chrono::steady_clock::now();

EventAction::EventAction() : n_threads(1) {}

EventAction::~EventAction() {}

void EventAction::EndOfEventAction(const G4Event *event) {
  int eID = event->GetEventID();
  if (0 == (eID % print_progress)) {
#ifdef G4MULTITHREADED
    G4RunManager *runManager = G4MTRunManager::GetRunManager();
#else
    G4RunManager *runManager = G4RunManager::GetRunManager();
#endif
    int NbEvents = runManager->GetNumberOfEventsToBeProcessed();
    int threadID = G4Threading::G4GetThreadId();
    int numberofpadchars = to_string(n_threads - 1).length() - to_string(threadID).length();
    string padchars(numberofpadchars, ' ');
    auto elapsedSeconds = (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - StartRunTime)).count();
    int days = elapsedSeconds / (24 * 3600);
    int hours = (elapsedSeconds - days * (24 * 3600)) / 3600;
    int minutes = (elapsedSeconds - days * (24 * 3600) - hours * 3600) / 60;
    int sec = (elapsedSeconds - days * (24 * 3600) - hours * 3600 - minutes * 60);
    float percent = (float)((float)eID / (float)NbEvents * 100.);

    G4cout << padchars << "Progress: [" << setw(16) << eID << "/" << NbEvents << "]  "
           << setw(4) << std::setprecision(2) << std::fixed << percent << " %"
           << "\tRunning time: " << setw(3) << days << "d " << setw(2) << hours << "h " << setw(3) << minutes << "mn " << setw(3) << sec << "s"
           << G4endl;
  }
}
