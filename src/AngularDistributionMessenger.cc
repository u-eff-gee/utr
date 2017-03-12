#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADouble.hh"

#include "AngularDistributionGenerator.hh"
#include "AngularDistributionMessenger.hh"

AngularDistributionMessenger::AngularDistributionMessenger(
    AngularDistributionGenerator *angDistGen) {
	angularDistributionGenerator = angDistGen;

	particleTable = G4ParticleTable::GetParticleTable();

	angDistDirectory = new G4UIdirectory("/ang/");
	angDistDirectory->SetGuidance(
	    "Controls for angular distribution generator.");

	particleCmd = new G4UIcmdWithAString("/ang/particle", this);
	particleCmd->SetGuidance("Set primary particle.");
	particleCmd->SetGuidance("Default: gamma");
	particleCmd->SetParameterName("particlename", true);
	particleCmd->SetDefaultValue("gamma");

	nStatesCmd = new G4UIcmdWithAnInteger("/ang/nstates", this);
	nStatesCmd->SetGuidance("Set number of states in level cascade.");
	nStatesCmd->SetGuidance("Default: 3");
	nStatesCmd->SetParameterName("nstates", true);
	nStatesCmd->SetDefaultValue(3);

	state1Cmd = new G4UIcmdWithADouble("/ang/state1", this);
	state1Cmd->SetGuidance("Set spin and parity of state 1.");
	state1Cmd->SetGuidance("Default: 0.");
	state1Cmd->SetParameterName("state1", true);
	state1Cmd->SetDefaultValue(0.);

	state2Cmd = new G4UIcmdWithADouble("/ang/state2", this);
	state2Cmd->SetGuidance("Set spin and parity of state 2.");
	state2Cmd->SetGuidance("Default: 0.");
	state2Cmd->SetParameterName("state2", true);
	state2Cmd->SetDefaultValue(0.);

	state3Cmd = new G4UIcmdWithADouble("/ang/state3", this);
	state3Cmd->SetGuidance("Set spin and parity of state 3.");
	state3Cmd->SetGuidance("Default: 0.");
	state3Cmd->SetParameterName("state3", true);
	state3Cmd->SetDefaultValue(0.);

	state4Cmd = new G4UIcmdWithADouble("/ang/state4", this);
	state4Cmd->SetGuidance("Set spin and parity of state 4.");
	state4Cmd->SetGuidance("Default: 0.");
	state4Cmd->SetParameterName("state4", true);
	state4Cmd->SetDefaultValue(0.);

	delta12Cmd = new G4UIcmdWithADouble("/ang/delta12", this);
	delta12Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 1 and 2.");
	delta12Cmd->SetGuidance("Default: 0.");
	delta12Cmd->SetParameterName("delta12", true);
	delta12Cmd->SetDefaultValue(0.);

	delta23Cmd = new G4UIcmdWithADouble("/ang/delta23", this);
	delta23Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 2 and 3.");
	delta23Cmd->SetGuidance("Default: 0.");
	delta23Cmd->SetParameterName("delta23", true);
	delta23Cmd->SetDefaultValue(0.);

	delta34Cmd = new G4UIcmdWithADouble("/ang/delta34", this);
	delta34Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 3 and 4.");
	delta34Cmd->SetGuidance("Default: 0.");
	delta34Cmd->SetParameterName("delta34", true);
	delta34Cmd->SetDefaultValue(0.);

	energyCmd = new G4UIcmdWithADoubleAndUnit("/ang/energy", this);

	angularDistributionGenerator->SetParticleDefinition(
	    particleTable->FindParticle("gamma"));
	angularDistributionGenerator->SetParticleEnergy(3. * MeV);

	angularDistributionGenerator->SetNStates(3);
	angularDistributionGenerator->SetState(0, 0.);
	angularDistributionGenerator->SetState(1, 0.);
	angularDistributionGenerator->SetState(2, 0.);
	angularDistributionGenerator->SetState(3, 0.);
	angularDistributionGenerator->SetDelta(0, 0.);
	angularDistributionGenerator->SetDelta(1, 0.);
	angularDistributionGenerator->SetDelta(2, 0.);
}

AngularDistributionMessenger::~AngularDistributionMessenger() {
	delete particleCmd;
	delete angDistDirectory;
}

void AngularDistributionMessenger::SetNewValue(G4UIcommand *command,
                                               G4String newValues) {
	if (command == particleCmd) {
		G4ParticleDefinition *pd = particleTable->FindParticle(newValues);
		if (pd != NULL) {
			angularDistributionGenerator->SetParticleDefinition(pd);
		} else {
			G4cout << "Error! AngularDistributionMessenger: Particle "
			          "definition not found."
			       << G4endl;
			return;
		}
	}

	if (command == energyCmd) {
		angularDistributionGenerator->SetParticleEnergy(
		    energyCmd->GetNewDoubleValue(newValues));
	}

	if (command == nStatesCmd) {
		angularDistributionGenerator->SetNStates(
		    nStatesCmd->GetNewIntValue(newValues));
	}

	if (command == state1Cmd) {
		angularDistributionGenerator->SetState(
		    0, state1Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state2Cmd) {
		angularDistributionGenerator->SetState(
		    1, state2Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state3Cmd) {
		angularDistributionGenerator->SetState(
		    2, state3Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state4Cmd) {
		angularDistributionGenerator->SetState(
		    3, state4Cmd->GetNewDoubleValue(newValues));
	}

	if (command == delta12Cmd) {
		angularDistributionGenerator->SetDelta(
		    0, delta12Cmd->GetNewDoubleValue(newValues));
	}
	if (command == delta23Cmd) {
		angularDistributionGenerator->SetDelta(
		    1, delta23Cmd->GetNewDoubleValue(newValues));
	}
	if (command == delta34Cmd) {
		angularDistributionGenerator->SetDelta(
		    2, delta34Cmd->GetNewDoubleValue(newValues));
	}
}

G4String AngularDistributionMessenger::GetCurrentValue(G4UIcommand *command) {
	G4String cv = "";

	if (command == particleCmd) {
		G4ParticleDefinition *pd =
		    angularDistributionGenerator->GetParticleDefinition();
		return pd->GetParticleName();
	}

	if (command == energyCmd) {
		return energyCmd->ConvertToString(
		    angularDistributionGenerator->GetParticleEnergy());
	}

	if (command == nStatesCmd) {
		return nStatesCmd->ConvertToString(
		    angularDistributionGenerator->GetNStates());
	}

	if (command == state1Cmd) {
		return state1Cmd->ConvertToString(
		    angularDistributionGenerator->GetState(0));
	}

	if (command == state2Cmd) {
		return state2Cmd->ConvertToString(
		    angularDistributionGenerator->GetState(1));
	}

	if (command == state3Cmd) {
		return state3Cmd->ConvertToString(
		    angularDistributionGenerator->GetState(2));
	}

	if (command == state4Cmd) {
		return state4Cmd->ConvertToString(
		    angularDistributionGenerator->GetState(3));
	}

	if (command == delta12Cmd) {
		return delta12Cmd->ConvertToString(
		    angularDistributionGenerator->GetDelta(0));
	}

	if (command == delta23Cmd) {
		return delta23Cmd->ConvertToString(
		    angularDistributionGenerator->GetDelta(1));
	}

	if (command == delta34Cmd) {
		return delta34Cmd->ConvertToString(
		    angularDistributionGenerator->GetDelta(2));
	}

	return cv;
}
