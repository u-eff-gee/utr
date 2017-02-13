#ifndef SecondarySD_h
#define SecondarySD_h 1

#include "G4VSensitiveDetector.hh"

#include "TargetHit.hh"

class SecondarySD : public G4VSensitiveDetector {
  public:
	SecondarySD(const G4String &name, const G4String &hitsCollectionName);
	virtual ~SecondarySD();

	virtual void Initialize(G4HCofThisEvent *hitCollection);
	virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
	virtual void EndOfEvent(G4HCofThisEvent *hitCollection);

	unsigned int getCurrentTrackID() { return currentTrackID; };
	void setCurrentTrackID(int trackID) { currentTrackID = trackID; };

	unsigned int getCurrentEventID() { return currentEventID; };
	void setCurrentEventID(unsigned int eventID) { currentEventID = eventID; };

	unsigned int getDetectorID() { return detectorID; };
	void SetDetectorID(unsigned int detID) { detectorID = detID; };

  private:
	TargetHitsCollection *fHitsCollection;
	unsigned int currentTrackID;
	unsigned int currentEventID;
	unsigned int detectorID;
};

#endif
