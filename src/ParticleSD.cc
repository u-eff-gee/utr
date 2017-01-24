#include "ParticleSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "Analysis.hh"

ParticleSD::ParticleSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name)//,   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

ParticleSD::~ParticleSD() 
{}

void ParticleSD::Initialize(G4HCofThisEvent* )
{
  // Create hits collection

  //fHitsCollection 
  //  = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  //G4int hcID 
  //  = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  //hce->AddHitsCollection( hcID, fHitsCollection ); 
}

G4bool ParticleSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	G4Track* track = aStep->GetTrack();
	unsigned int trackID = track->GetTrackID();

	if( trackID != getCurrentTrackID() ){
		//G4cout << "--> First step in volume, Ekin = " << aStep->GetPreStepPoint()->GetKineticEnergy() << " keV" << G4endl;
		setCurrentTrackID(trackID);
	
		if(track->GetKineticEnergy() == 0.)
			return false;
	
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		analysisManager->FillNtupleDColumn(0, track->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(1, aStep->GetTotalEnergyDeposit());
		analysisManager->FillNtupleDColumn(2, track->GetDefinition()->GetPDGEncoding());
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

void ParticleSD::EndOfEvent(G4HCofThisEvent*)
{

}
