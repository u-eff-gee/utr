#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#define EKIN 0
#define EDEP 1
#define PARTICLE 2
#define VOLUME 3
#define POSX 4
#define POSY 5
#define POSZ 6
#define MOMX 7
#define MOMY 8
#define MOMZ 9

class RunAction : public G4UserRunAction {
  public:
	RunAction();
	virtual ~RunAction();

	virtual void BeginOfRunAction(const G4Run *);
	virtual void EndOfRunAction(const G4Run *);

	void SetOutputFlags(unsigned int *o_flags);
	unsigned int *GetOutputFlags();
	unsigned int GetNOutputFlags() { return n_output_flags; };

	static const int n_output_flags = 10;

  private:
	unsigned int output_flags[n_output_flags];
};

#endif
