#include "ParticleSD.hh"
#include "Analysis.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

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
	unsigned int trackID = track->GetTrackID();

	const G4Event *event = G4RunManager::GetRunManager()->GetCurrentEvent();
	unsigned int eventID = event->GetEventID();

	if (trackID != getCurrentTrackID() || eventID != getCurrentEventID()) {

		setCurrentTrackID(trackID);
		setCurrentEventID(eventID);

		if (track->GetKineticEnergy() == 0.)
			return false;

		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		RunAction *runAction =
		    (RunAction *)G4RunManager::GetRunManager()->GetUserRunAction();
		unsigned int *output_flags = runAction->GetOutputFlags();

		unsigned int nentry = 0;

		if (output_flags[EKIN]){
		analysisManager->FillNtupleDColumn(nentry, track->GetKineticEnergy());
			nentry++;
		}
		if (output_flags[EDEP]){
		analysisManager->FillNtupleDColumn(nentry, aStep->GetTotalEnergyDeposit());
			nentry++;
		}
		if (output_flags[PARTICLE]){
		analysisManager->FillNtupleDColumn(
		    nentry, track->GetDefinition()->GetPDGEncoding());
			nentry++;
		}
		if (output_flags[VOLUME]){
		analysisManager->FillNtupleDColumn(nentry, getDetectorID());
			nentry++;
		}
		if (output_flags[POSX]){
		analysisManager->FillNtupleDColumn(nentry, track->GetPosition().x());
			nentry++;
		}
		if (output_flags[POSY]){
		analysisManager->FillNtupleDColumn(nentry, track->GetPosition().y());
			nentry++;
		}
		if (output_flags[POSZ]){
		analysisManager->FillNtupleDColumn(nentry, track->GetPosition().z());
			nentry++;
		}
		if (output_flags[MOMX]){
		analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().x());
			nentry++;
		}
		if (output_flags[MOMY]){
		analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().y());
			nentry++;
		}
		if (output_flags[MOMZ]){
		analysisManager->FillNtupleDColumn(nentry, track->GetMomentum().z());
			nentry++;
		}

		analysisManager->AddNtupleRow();
	}

	return true;
}

void ParticleSD::EndOfEvent(G4HCofThisEvent *) {}
