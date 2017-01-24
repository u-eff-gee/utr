#include "EnergyDepositionSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "Analysis.hh"
#include "TargetHit.hh"

EnergyDepositionSD::EnergyDepositionSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   hitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

EnergyDepositionSD::~EnergyDepositionSD() 
{}

void EnergyDepositionSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

	hitsCollection = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection( hcID, hitsCollection ); 
}

G4bool EnergyDepositionSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	TargetHit* hit = new TargetHit();

	G4Track* track = aStep->GetTrack();

	hit->SetKineticEnergy( track->GetKineticEnergy() );
	hit->SetEnergyDeposition( aStep->GetTotalEnergyDeposit() );
	hit->SetParticleType(track->GetDefinition()->GetPDGEncoding());
	hit->SetDetectorID( GetDetectorID() );
	hit->SetPosition( track->GetPosition() );
	hit->SetMomentum( track->GetMomentum() );

	hitsCollection->insert( hit );

	return true;
}

void EnergyDepositionSD::EndOfEvent(G4HCofThisEvent*)
{
	G4int nHits = hitsCollection->entries();
	G4double totalEnergyDeposition = 0.;

	if(nHits > 0){
//			G4cout << "Listing hits in volume " << GetDetectorID() << G4endl; 
		for(G4int i = 0; i < nHits; i++){
//			G4cout << (i+1) << ": \t" << (*hitsCollection)[i]->GetKineticEnergy() << G4endl;
//			G4cout << (*hitsCollection)[i]->GetEnergyDeposition() << G4endl;
			totalEnergyDeposition += (*hitsCollection)[i]->GetEnergyDeposition(); 
//			G4cout << (*hitsCollection)[i]->GetPosition() << G4endl;
//			G4cout << (*hitsCollection)[i]->GetMomentum() << G4endl;
		}
	}

	if(totalEnergyDeposition > 0.){
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		analysisManager->FillNtupleDColumn(0, (*hitsCollection)[0]->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(1, totalEnergyDeposition);
		analysisManager->FillNtupleDColumn(2, (*hitsCollection)[0]->GetParticleType());
		analysisManager->FillNtupleDColumn(3, GetDetectorID());
		analysisManager->FillNtupleDColumn(4, (*hitsCollection)[0]->GetPosition().x());
		analysisManager->FillNtupleDColumn(5, (*hitsCollection)[0]->GetPosition().y());
		analysisManager->FillNtupleDColumn(6, (*hitsCollection)[0]->GetPosition().z());
		analysisManager->FillNtupleDColumn(7, (*hitsCollection)[0]->GetMomentum().x());
		analysisManager->FillNtupleDColumn(8, (*hitsCollection)[0]->GetMomentum().y());
		analysisManager->FillNtupleDColumn(9, (*hitsCollection)[0]->GetMomentum().z());

		analysisManager->AddNtupleRow();

		G4cout << "Total energy deposition: " << totalEnergyDeposition << " MeV" << G4endl;
	}
}
