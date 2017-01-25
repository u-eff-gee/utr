#include "SecondarySD.hh"
#include "Analysis.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"

SecondarySD::SecondarySD(const G4String &name,
                         const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name), fHitsCollection(NULL) {
	collectionName.insert(hitsCollectionName);
}

SecondarySD::~SecondarySD() {}

void SecondarySD::Initialize(G4HCofThisEvent *) {}

G4bool SecondarySD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
	G4Track *track = aStep->GetTrack();

	if (track->GetParentID() > 0 &&
	    track->GetTrackID() != getCurrentTrackID()) {

		if (track->GetKineticEnergy() == 0.)
			return false;

		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		analysisManager->FillNtupleDColumn(0, track->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(1, aStep->GetTotalEnergyDeposit());
		analysisManager->FillNtupleDColumn(
		    2, track->GetDefinition()->GetPDGEncoding());
		//		G4cout << "PDGEncoding: " <<
		// track->GetDefinition()->GetPDGEncoding() << G4endl;
		//		G4cout << "\tParticle Name: " <<
		// track->GetDefinition()->GetParticleName() << G4endl;
		analysisManager->FillNtupleDColumn(3, getDetectorID());
		analysisManager->FillNtupleDColumn(4, track->GetPosition().x());
		analysisManager->FillNtupleDColumn(5, track->GetPosition().y());
		analysisManager->FillNtupleDColumn(6, track->GetPosition().z());
		analysisManager->FillNtupleDColumn(7, track->GetMomentum().x());
		analysisManager->FillNtupleDColumn(8, track->GetMomentum().y());
		analysisManager->FillNtupleDColumn(9, track->GetMomentum().z());

		analysisManager->AddNtupleRow();

		setCurrentTrackID(track->GetTrackID());
	}

	const G4TrackVector *trackVector = aStep->GetSecondary();
	setNSecondaries(trackVector->size());

	return true;
}

void SecondarySD::EndOfEvent(G4HCofThisEvent *) {}
