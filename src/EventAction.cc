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

#include "Timing.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4LogicalVolume.hh"
#include "utrConfig.h"

using std::setw;
using std::flush;

EventAction::EventAction(): n_threads(1) {}

EventAction::~EventAction() {}

void EventAction::EndOfEventAction(const G4Event *event) {

#ifdef G4MULTITHREADED
	G4RunManager* runManager = G4MTRunManager::GetRunManager();
#else
	G4RunManager* runManager = G4RunManager::GetRunManager();
#endif
	G4double thread_norm = 1./n_threads; 	// To normalize the given times to the
						// number of threads.

	int eID = event->GetEventID();
	int NbEvents = runManager->GetNumberOfEventsToBeProcessed();
	int threadID = G4Threading::G4GetThreadId();
        if((0 == (eID % print_progress))  ||  (eID == print_progress) ) {
		CurrentRunTime = clock();
		int ElapsedTime = (CurrentRunTime-StartRunTime)/CLOCKS_PER_SEC*thread_norm;
		int days 	= ElapsedTime/(24*3600);
		int hours 	= (ElapsedTime-days*(24*3600))/3600;
		int minutes 	= (ElapsedTime-days*(24*3600)-hours*3600)/60;
		int sec 	= (ElapsedTime-days*(24*3600)-hours*3600-minutes*60);
		float percent = (float)((float)eID/(float)NbEvents*100.);
		if(threadID<10)
		{
			G4cout  << " Progress: ["<<setw(16)<<eID<<"/"<<NbEvents<<"]  "
			<<setw(4)<<std::setprecision(2)<<std::fixed<<percent<<" %"
			<<"\tRunning time: "<< setw(3)<<days<<"d "<<setw(2)<<hours<<"h "<<setw(3)<<minutes<<"mn "<<setw(3)<<sec<<"s   "
			<< G4endl;	
		}
		else
		{
			G4cout  << "Progress: ["<<setw(16)<<eID<<"/"<<NbEvents<<"]  "
			<<setw(4)<<std::setprecision(2)<<std::fixed<<percent<<" %"
			<<"\tRunning time: "<< setw(3)<<days<<"d "<<setw(2)<<hours<<"h "<<setw(3)<<minutes<<"mn "<<setw(3)<<sec<<"s   "
			<< G4endl;
		}
		
				
				
				
        }
}
