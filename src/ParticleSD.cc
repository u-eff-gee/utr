#include "ParticleSD.hh"
#include "Analysis.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
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

		analysisManager->FillNtupleDColumn(0, track->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(1, aStep->GetTotalEnergyDeposit());
		analysisManager->FillNtupleDColumn(
		    2, track->GetDefinition()->GetPDGEncoding());
		analysisManager->FillNtupleDColumn(3, getDetectorID());
		analysisManager->FillNtupleDColumn(4, track->GetPosition().x());
		analysisManager->FillNtupleDColumn(5, track->GetPosition().y());
		analysisManager->FillNtupleDColumn(6, track->GetPosition().z());
		analysisManager->FillNtupleDColumn(7, track->GetMomentum().x());
		analysisManager->FillNtupleDColumn(8, track->GetMomentum().y());
		analysisManager->FillNtupleDColumn(9, track->GetMomentum().z());

		analysisManager->AddNtupleRow();
	}

	return true;
}

void ParticleSD::EndOfEvent(G4HCofThisEvent *) {}
