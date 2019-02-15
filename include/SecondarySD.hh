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

#ifndef SecondarySD_h
#define SecondarySD_h 1

#include "G4VSensitiveDetector.hh"

#include "TargetHit.hh"

class SecondarySD : public G4VSensitiveDetector {
  public:
	SecondarySD(const G4String &name, const G4String &hitsCollectionName);
	virtual ~SecondarySD();

	virtual void Initialize(G4HCofThisEvent *hitCollection);
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
	virtual void EndOfEvent(G4HCofThisEvent *hitCollection);

	G4int getCurrentTrackID() { return currentTrackID; };
	void setCurrentTrackID(int trackID) { currentTrackID = trackID; };

	G4int getCurrentEventID() { return currentEventID; };
	void setCurrentEventID(unsigned int eventID) { currentEventID = eventID; };

	unsigned int getDetectorID() { return detectorID; };
	void SetDetectorID(unsigned int detID) { detectorID = detID; };

  private:
	G4int currentTrackID;
	G4int currentEventID;
	G4int detectorID;
};

#endif
