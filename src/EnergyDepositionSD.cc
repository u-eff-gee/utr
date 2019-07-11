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

#include "EnergyDepositionSD.hh"
#include "g4root.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "RunAction.hh"
#include "TargetHit.hh"

#include "utrConfig.h"

EnergyDepositionSD::EnergyDepositionSD(const G4String &name,
                                       const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name), hitsCollection(NULL), detectorID(0), eventID(0) {
	
	collectionName.insert(hitsCollectionName);
}

EnergyDepositionSD::~EnergyDepositionSD() {}

void EnergyDepositionSD::Initialize(G4HCofThisEvent *hce) {

	hitsCollection =
	    new TargetHitsCollection(SensitiveDetectorName, collectionName[0]);

	G4int hcID =
	    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection(hcID, hitsCollection);

	eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();	
}

G4bool EnergyDepositionSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {

	TargetHit *hit = new TargetHit();

	G4Track *track = aStep->GetTrack();

	hit->SetKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());
	hit->SetEnergyDeposition(aStep->GetTotalEnergyDeposit());
	hit->SetParticleType(track->GetDefinition()->GetPDGEncoding());
	hit->SetDetectorID(GetDetectorID());
	hit->SetEventID(eventID);
	hit->SetPosition(track->GetPosition());
	hit->SetMomentum(track->GetMomentum());

	hitsCollection->insert(hit);

	return true;
}

void EnergyDepositionSD::EndOfEvent(G4HCofThisEvent *) {

	G4int nHits = hitsCollection->entries();
	G4double totalEnergyDeposition = 0.;

	if (nHits > 0) {
		for (G4int i = 0; i < nHits; i++) {
			totalEnergyDeposition +=
			    (*hitsCollection)[i]->GetEnergyDeposition();
		}
	}

	if (totalEnergyDeposition > 0.) {
		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		unsigned int nentry = 0;

#ifdef EVENT_ID
	analysisManager->FillNtupleDColumn(nentry, eventID);
	++nentry;
#endif
#ifdef EVENT_EDEP
	analysisManager->FillNtupleDColumn(nentry, totalEnergyDeposition);
	++nentry;
#endif
#ifdef EVENT_EKIN
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetKineticEnergy());	
	++nentry;
#endif
#ifdef EVENT_PARTICLE
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetParticleType());
	++nentry;
#endif
#ifdef EVENT_VOLUME
	analysisManager->FillNtupleDColumn(nentry, GetDetectorID());
	++nentry;
#endif
#ifdef EVENT_POSX
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetPosition().x());
	++nentry;
#endif
#ifdef EVENT_POSY
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetPosition().y());
	++nentry;
#endif
#ifdef EVENT_POSZ
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetPosition().z());
	++nentry;
#endif
#ifdef EVENT_MOMX
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetMomentum().x());
	++nentry;
#endif
#ifdef EVENT_MOMY
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetMomentum().y());
	++nentry;
#endif
#ifdef EVENT_MOMZ
	analysisManager->FillNtupleDColumn(nentry, (*hitsCollection)[0]->GetMomentum().z());
#endif
	
	analysisManager->AddNtupleRow();
	}
}
