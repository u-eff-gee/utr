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

#include "ParticleSD.hh"
#include "Analysis.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "RunAction.hh"

#include "utrConfig.h"

ParticleSD::ParticleSD(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name) {
	collectionName.insert(hitsCollectionName);

	currentEventID = 0;
	currentTrackID = 0;
	detectorID = 0;
}

ParticleSD::~ParticleSD() {}

void ParticleSD::Initialize(G4HCofThisEvent *) {}

G4bool ParticleSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	G4Track *track = aStep->GetTrack();

	G4int trackID = track->GetTrackID();
	G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

	if (trackID != getCurrentTrackID() || eventID != getCurrentEventID()) {

		setCurrentTrackID(trackID);
		setCurrentEventID(eventID);

		if (track->GetKineticEnergy() == 0.)
			return false;

		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		unsigned int nentry = 0;

#ifdef EVENT_ID
	analysisManager->FillNtupleDColumn(nentry, eventID);
	++nentry;
#endif
#ifdef EVENT_EDEP
	analysisManager->FillNtupleDColumn(nentry, aStep->GetTotalEnergyDeposit());
	++nentry;
#endif
#ifdef EVENT_EKIN
	analysisManager->FillNtupleDColumn(nentry, aStep->GetPreStepPoint()->GetKineticEnergy());	
	++nentry;
#endif
#ifdef EVENT_PARTICLE
	analysisManager->FillNtupleDColumn(nentry, track->GetDefinition()->GetPDGEncoding());
	++nentry;
#endif
#ifdef EVENT_VOLUME
	analysisManager->FillNtupleDColumn(nentry, getDetectorID());
	++nentry;
#endif
#ifdef EVENT_POSX
	analysisManager->FillNtupleDColumn(nentry, track->GetPosition().x());
	++nentry;
#endif
#ifdef EVENT_POSY
	analysisManager->FillNtupleDColumn(nentry, track->GetPosition().y());
	++nentry;
#endif
#ifdef EVENT_POSZ
	analysisManager->FillNtupleDColumn(nentry, track->GetPosition().z());
	++nentry;
#endif
#ifdef EVENT_MOMX
	analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().x());
	++nentry;
#endif
#ifdef EVENT_MOMY
	analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().y());
	++nentry;
#endif
#ifdef EVENT_MOMZ
	analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().z());
#endif

	analysisManager->AddNtupleRow();
	}

	return true;
}

void ParticleSD::EndOfEvent(G4HCofThisEvent *) {}
