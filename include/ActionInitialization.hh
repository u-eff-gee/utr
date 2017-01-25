#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

// Custom class definition
class ActionInitialization : public G4VUserActionInitialization {
  public:
	ActionInitialization();
	virtual ~ActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;
};

#endif
