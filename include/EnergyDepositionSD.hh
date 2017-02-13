#ifndef EnergyDepositionSD_h
#define EnergyDepositionSD_h 1

#include "G4VSensitiveDetector.hh"

#include "TargetHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class EnergyDepositionSD : public G4VSensitiveDetector {
  public:
	EnergyDepositionSD(const G4String &name,
	                   const G4String &hitsCollectionName);
	virtual ~EnergyDepositionSD();

	// methods from base class
	virtual void Initialize(G4HCofThisEvent *hitCollection);
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
	virtual void EndOfEvent(G4HCofThisEvent *hitCollection);
	unsigned int getCurrentTrackID() { return currentTrackID; };
	void setCurrentTrackID(unsigned int trackID) { currentTrackID = trackID; };
	unsigned int GetDetectorID() { return detectorID; };
	void SetDetectorID(unsigned int detID) { detectorID = detID; };

  private:
	TargetHitsCollection *hitsCollection;
	unsigned int currentTrackID;
	unsigned int detectorID;
};

#endif
