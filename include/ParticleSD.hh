#ifndef ParticleSD_h
#define ParticleSD_h 1

#include "G4VSensitiveDetector.hh"

#include "TargetHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class ParticleSD : public G4VSensitiveDetector {
  public:
	ParticleSD(const G4String &name, const G4String &hitsCollectionName);
	virtual ~ParticleSD();

	// methods from base class
	virtual void Initialize(G4HCofThisEvent *hitCollection);
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
	virtual void EndOfEvent(G4HCofThisEvent *hitCollection);
	unsigned int getCurrentTrackID() { return currentTrackID; };
	void setCurrentTrackID(unsigned int trackID) { currentTrackID = trackID; };
	unsigned int getDetectorID() { return detectorID; };
	void SetDetectorID(unsigned int detID) { detectorID = detID; };

  private:
	// TargetHitsCollection* fHitsCollection;
	unsigned int currentTrackID = 0;
	unsigned int detectorID = 0;
};

#endif
