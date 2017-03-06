#include "SecondarySD.hh"
#include "Analysis.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "RunAction.hh"

SecondarySD::SecondarySD(const G4String &name,
                         const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name), fHitsCollection(NULL) {
	collectionName.insert(hitsCollectionName);

	currentTrackID = 0;
	currentEventID = 0;
	detectorID = 0;
}

SecondarySD::~SecondarySD() {}

void SecondarySD::Initialize(G4HCofThisEvent *) {}

G4bool SecondarySD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	G4Track *track = aStep->GetTrack();
	unsigned int trackID = track->GetTrackID();

	const G4Event *event = G4RunManager::GetRunManager()->GetCurrentEvent();
	unsigned int eventID = event->GetEventID();

	if ((trackID != getCurrentTrackID() && trackID > 1) ||
	    (eventID != getCurrentEventID() && trackID > 1)) {

		setCurrentTrackID(trackID);
		setCurrentEventID(eventID);

		if (track->GetKineticEnergy() == 0.)
			return false;

		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		RunAction *runAction =
		    (RunAction *)G4RunManager::GetRunManager()->GetUserRunAction();
		unsigned int *output_flags = runAction->GetOutputFlags();

		unsigned int nentry = 0;

		if (output_flags[EKIN]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetKineticEnergy());
			nentry++;
		}
		if (output_flags[EDEP]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   aStep->GetTotalEnergyDeposit());
			nentry++;
		}
		if (output_flags[PARTICLE]) {
			analysisManager->FillNtupleDColumn(
			    nentry, track->GetDefinition()->GetPDGEncoding());
			nentry++;
		}
		if (output_flags[VOLUME]) {
			analysisManager->FillNtupleDColumn(nentry, getDetectorID());
			nentry++;
		}
		if (output_flags[POSX]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetPosition().x());
			nentry++;
		}
		if (output_flags[POSY]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetPosition().y());
			nentry++;
		}
		if (output_flags[POSZ]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetPosition().z());
			nentry++;
		}
		if (output_flags[MOMX]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetMomentum().x());
			nentry++;
		}
		if (output_flags[MOMY]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetMomentum().y());
			nentry++;
		}
		if (output_flags[MOMZ]) {
			analysisManager->FillNtupleDColumn(nentry,
			                                   track->GetMomentum().z());
			nentry++;
		}

		analysisManager->AddNtupleRow();
	}

	return true;
}

void SecondarySD::EndOfEvent(G4HCofThisEvent *) {}
