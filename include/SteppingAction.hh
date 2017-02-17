#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"

class SteppingAction : public G4UserSteppingAction {
  public:
	SteppingAction();
	virtual ~SteppingAction();

	virtual void UserSteppingAction(const G4Step *);

  private:
};

#endif
