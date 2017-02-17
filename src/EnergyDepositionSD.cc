#include "EnergyDepositionSD.hh"
#include "Analysis.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "RunAction.hh"
#include "TargetHit.hh"

EnergyDepositionSD::EnergyDepositionSD(const G4String &name,
                                       const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name), hitsCollection(NULL) {
	collectionName.insert(hitsCollectionName);

	currentTrackID = 0;
	detectorID = 0;
}

EnergyDepositionSD::~EnergyDepositionSD() {}

void EnergyDepositionSD::Initialize(G4HCofThisEvent *hce) {

	hitsCollection =
	    new TargetHitsCollection(SensitiveDetectorName, collectionName[0]);

	G4int hcID =
	    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection(hcID, hitsCollection);
}

G4bool EnergyDepositionSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {

	TargetHit *hit = new TargetHit();

	G4Track *track = aStep->GetTrack();

	hit->SetKineticEnergy(track->GetKineticEnergy());
	hit->SetEnergyDeposition(aStep->GetTotalEnergyDeposit());
	hit->SetParticleType(track->GetDefinition()->GetPDGEncoding());
	hit->SetDetectorID(GetDetectorID());
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

		RunAction *runAction =
		    (RunAction *)G4RunManager::GetRunManager()->GetUserRunAction();
		unsigned int *output_flags = runAction->GetOutputFlags();

		unsigned int nentry = 0;

		if (output_flags[EKIN]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetKineticEnergy());
			nentry++;
		}
		if (output_flags[EDEP]) {
			analysisManager->FillNtupleDColumn(nentry, totalEnergyDeposition);
			nentry++;
		}
		if (output_flags[PARTICLE]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetParticleType());
			nentry++;
		}
		if (output_flags[VOLUME]) {
			analysisManager->FillNtupleDColumn(nentry, GetDetectorID());
			nentry++;
		}
		if (output_flags[POSX]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetPosition().x());
			nentry++;
		}
		if (output_flags[POSY]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetPosition().y());
			nentry++;
		}
		if (output_flags[POSZ]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetPosition().z());
			nentry++;
		}
		if (output_flags[MOMX]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetMomentum().x());
			nentry++;
		}
		if (output_flags[MOMY]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetMomentum().y());
			nentry++;
		}
		if (output_flags[MOMZ]) {
			analysisManager->FillNtupleDColumn(
			    nentry, (*hitsCollection)[0]->GetMomentum().z());
			nentry++;
		}
		analysisManager->AddNtupleRow();
	}
}
