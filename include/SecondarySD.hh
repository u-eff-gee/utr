#ifndef SecondarySD_h
#define SecondarySD_h 1

#include "G4VSensitiveDetector.hh"

#include "TargetHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class SecondarySD : public G4VSensitiveDetector
{
  public:
    SecondarySD(const G4String& name, 
                const G4String& hitsCollectionName);
    virtual ~SecondarySD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
	unsigned int getNSecondaries(){ return nSecondaries; };
	void setNSecondaries(unsigned int nSec){ nSecondaries = nSec; };
	unsigned int getDetectorID(){ return detectorID; };
	void setDetectorID( unsigned int detID ){ detectorID = detID; };
	int getCurrentTrackID(){ return trackID; };
	void setCurrentTrackID( int tID ){ trackID = tID; };

  private:
    	TargetHitsCollection* fHitsCollection;
	unsigned int detectorID = 0;
	unsigned int nSecondaries = 0;
	int trackID = 0;
};

#endif
