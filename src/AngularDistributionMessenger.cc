/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	sourceXCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceX", this);
	sourceXCmd->SetGuidance("Set X position of source container volume");
	sourceXCmd->SetGuidance("Default: 0.");
	sourceXCmd->SetParameterName("sourceX", true);
	sourceXCmd->SetDefaultValue(0.);

	sourceYCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceY", this);
	sourceYCmd->SetGuidance("Set Y position of source container volume");
	sourceYCmd->SetGuidance("Default: 0.");
	sourceYCmd->SetParameterName("sourceY", true);
	sourceYCmd->SetDefaultValue(0.);

	sourceZCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceZ", this);
	sourceZCmd->SetGuidance("Set Z position of source container volume");
	sourceZCmd->SetGuidance("Default: 0.");
	sourceZCmd->SetParameterName("sourceZ", true);
	sourceZCmd->SetDefaultValue(0.);

	sourceDXCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceDX", this);
	sourceDXCmd->SetGuidance("Set X dimension of source container volume");
	sourceDXCmd->SetGuidance("Default: 10. * mm");
	sourceDXCmd->SetParameterName("sourceDX", true);
	sourceDXCmd->SetDefaultValue(10. * mm);

	sourceDYCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceDY", this);
	sourceDYCmd->SetGuidance("Set Y dimension of source container volume");
	sourceDYCmd->SetGuidance("Default: 10. * mm");
	sourceDYCmd->SetParameterName("sourceDY", true);
	sourceDYCmd->SetDefaultValue(10. * mm);

	sourceDZCmd = new G4UIcmdWithADoubleAndUnit("/ang/sourceDZ", this);
	sourceDZCmd->SetGuidance("Set Z dimension of source container volume");
	sourceDZCmd->SetGuidance("Default: 10. * mm");
	sourceDZCmd->SetParameterName("sourceDZ", true);
	sourceDZCmd->SetDefaultValue(10. * mm);

	sourcePVCmd = new G4UIcmdWithAString("/ang/sourcePV", this);
	sourcePVCmd->SetGuidance("Add physical volume as a particle source.");
	sourcePVCmd->SetParameterName("sourcePV", true);
	sourcePVCmd->SetDefaultValue("");

	polarizationCmd = new G4UIcmdWithABool("/ang/polarized", this);
	polarizationCmd->SetGuidance("Assume that the exciting transition is polarized (default: true)");
	polarizationCmd->SetParameterName("is_polarized", true);
	polarizationCmd->SetDefaultValue(true);

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

	angularDistributionGenerator->SetSourceX(0.);
	angularDistributionGenerator->SetSourceY(0.);
	angularDistributionGenerator->SetSourceZ(0.);

	angularDistributionGenerator->SetSourceDX(10. * mm);
	angularDistributionGenerator->SetSourceDY(10. * mm);
	angularDistributionGenerator->SetSourceDZ(10. * mm);

	angularDistributionGenerator->SetPolarized(true);
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

	if (command == sourceXCmd) {
		angularDistributionGenerator->SetSourceX(
		    sourceXCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceYCmd) {
		angularDistributionGenerator->SetSourceY(
		    sourceYCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceZCmd) {
		angularDistributionGenerator->SetSourceZ(
		    sourceZCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDXCmd) {
		angularDistributionGenerator->SetSourceDX(
		    sourceDXCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDYCmd) {
		angularDistributionGenerator->SetSourceDY(
		    sourceDYCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDZCmd) {
		angularDistributionGenerator->SetSourceDZ(
		    sourceDZCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourcePVCmd) {
		angularDistributionGenerator->AddSourcePV(newValues);
	}
	if (command == polarizationCmd) {
		angularDistributionGenerator->SetPolarized(
				polarizationCmd->GetNewBoolValue(newValues));
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
	if (command == sourceXCmd) {
		return sourceXCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceX());
	}
	if (command == sourceYCmd) {
		return sourceYCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceY());
	}
	if (command == sourceZCmd) {
		return sourceZCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceZ());
	}
	if (command == sourceDXCmd) {
		return sourceDXCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceDX());
	}
	if (command == sourceDYCmd) {
		return sourceDYCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceDY());
	}
	if (command == sourceDZCmd) {
		return sourceDZCmd->ConvertToString(
		    angularDistributionGenerator->GetSourceDZ());
	}
	if (command == polarizationCmd) {
		return polarizationCmd->ConvertToString(
		    angularDistributionGenerator->IsPolarized());
	}

	return cv;
}
