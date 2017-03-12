#ifndef AngularDistributionMessenger_h
#define AngularDistributionMessenger_h 1

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class AngularDistributionGenerator;
class G4ParticleTable;

class AngularDistributionMessenger : public G4UImessenger {
  public:
	AngularDistributionMessenger(AngularDistributionGenerator *angDistGen);
	~AngularDistributionMessenger();

	void SetNewValue(G4UIcommand *command, G4String newValues);
	G4String GetCurrentValue(G4UIcommand *command);

  private:
	AngularDistributionGenerator *angularDistributionGenerator;
	G4ParticleTable *particleTable;
	G4UIdirectory *angDistDirectory;

	G4UIcmdWithAString *particleCmd;
	G4UIcmdWithADoubleAndUnit *energyCmd;

	G4UIcmdWithAnInteger *nStatesCmd;

	G4UIcmdWithADouble *state1Cmd;
	G4UIcmdWithADouble *state2Cmd;
	G4UIcmdWithADouble *state3Cmd;
	G4UIcmdWithADouble *state4Cmd;

	G4UIcmdWithADouble *delta12Cmd;
	G4UIcmdWithADouble *delta23Cmd;
	G4UIcmdWithADouble *delta34Cmd;
};

#endif
