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

#include "AngularCorrelationGenerator.hh"
#include "AngularCorrelationMessenger.hh"

AngularCorrelationMessenger::AngularCorrelationMessenger(
    AngularCorrelationGenerator *angCorrGen):n_steps(0), current_steps(0){
	angularCorrelationGenerator = angCorrGen;

	particleTable = G4ParticleTable::GetParticleTable();

	angCorrDirectory = new G4UIdirectory("/angcorr/");
	angCorrDirectory->SetGuidance(
	    "Controls for angular correlation generator.");

	stepCmd = new G4UIcmdWithAnInteger("/angcorr/steps", this);
	stepCmd->SetGuidance("Sets the number of steps in the cascade (= the number of emitted particles).");
	stepCmd->SetGuidance("Default: 0");
	stepCmd->SetParameterName("nsteps", true);
	stepCmd->SetDefaultValue(0);

	particleCmd = new G4UIcmdWithAString("/angcorr/particle", this);
	particleCmd->SetGuidance("Add primary particle.");
	particleCmd->SetGuidance("Default: gamma");
	particleCmd->SetParameterName("particlename", true);
	particleCmd->SetDefaultValue("gamma");

	energyCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/energy", this);
	energyCmd->SetGuidance("Set kinetic energy of current particle");
	energyCmd->SetGuidance("Default: 0.");
	energyCmd->SetParameterName("energy", false);
	energyCmd->SetDefaultValue(0.);

	directionCmd = new G4UIcmdWith3Vector("/angcorr/direction", this);
	directionCmd->SetGuidance("Set direction of emission of the first particle.");
	directionCmd->SetGuidance("Default: (0, 0, 0), meaning 'ignore this option'.");
	directionCmd->SetParameterName("dir_x", "dir_y", "dir_z", true);
	directionCmd->SetDefaultValue(G4ThreeVector(0., 0., 0.));

	relativeAngleCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/relativeangle", this);
	relativeAngleCmd->SetGuidance("Set emission angle w.r.t. the previous particle.");
	relativeAngleCmd->SetGuidance("Default: pi or 180*deg, meaning 'in the opposite direction'.");
	relativeAngleCmd->SetParameterName("angle", true);
	relativeAngleCmd->SetDefaultValue(180.*deg);

	nStatesCmd = new G4UIcmdWithAnInteger("/angcorr/nstates", this);
	nStatesCmd->SetGuidance("Add new cascades with given number of states.");
	nStatesCmd->SetGuidance("Default: 3");
	nStatesCmd->SetParameterName("nstates", true);
	nStatesCmd->SetDefaultValue(3);

	state1Cmd = new G4UIcmdWithADouble("/angcorr/state1", this);
	state1Cmd->SetGuidance("Set spin and parity of state 1 in current cascade.");
	state1Cmd->SetGuidance("Default: 0.");
	state1Cmd->SetParameterName("state1", true);
	state1Cmd->SetDefaultValue(0.);

	state2Cmd = new G4UIcmdWithADouble("/angcorr/state2", this);
	state2Cmd->SetGuidance("Set spin and parity of state 2 in current cascade.");
	state2Cmd->SetGuidance("Default: 0.");
	state2Cmd->SetParameterName("state2", true);
	state2Cmd->SetDefaultValue(0.);

	state3Cmd = new G4UIcmdWithADouble("/angcorr/state3", this);
	state3Cmd->SetGuidance("Set spin and parity of state 3 in current cascade.");
	state3Cmd->SetGuidance("Default: 0.");
	state3Cmd->SetParameterName("state3", true);
	state3Cmd->SetDefaultValue(0.);

	state4Cmd = new G4UIcmdWithADouble("/angcorr/state4", this);
	state4Cmd->SetGuidance("Set spin and parity of state 4 in current cascade.");
	state4Cmd->SetGuidance("Default: 0.");
	state4Cmd->SetParameterName("state4", true);
	state4Cmd->SetDefaultValue(0.);

	delta12Cmd = new G4UIcmdWithADouble("/angcorr/delta12", this);
	delta12Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 1 and 2 in current cascade.");
	delta12Cmd->SetGuidance("Default: 0.");
	delta12Cmd->SetParameterName("delta12", true);
	delta12Cmd->SetDefaultValue(0.);

	delta23Cmd = new G4UIcmdWithADouble("/angcorr/delta23", this);
	delta23Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 2 and 3 in current cascade.");
	delta23Cmd->SetGuidance("Default: 0.");
	delta23Cmd->SetParameterName("delta23", true);
	delta23Cmd->SetDefaultValue(0.);

	delta34Cmd = new G4UIcmdWithADouble("/angcorr/delta34", this);
	delta34Cmd->SetGuidance(
	    "Set multipole mixing ratio of transtion between states 3 and 4 in current cascade.");
	delta34Cmd->SetGuidance("Default: 0.");
	delta34Cmd->SetParameterName("delta34", true);
	delta34Cmd->SetDefaultValue(0.);

	polarizationCmd = new G4UIcmdWithABool("/angcorr/polarized", this);
	polarizationCmd->SetGuidance("Assume that the exciting transition is polarized (default: true)");
	polarizationCmd->SetParameterName("is_polarized", true);
	polarizationCmd->SetDefaultValue(true);


	sourceXCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceX", this);
	sourceXCmd->SetGuidance("Set X position of source container volume");
	sourceXCmd->SetGuidance("Default: 0.");
	sourceXCmd->SetParameterName("sourceX", true);
	sourceXCmd->SetDefaultValue(0.);

	sourceYCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceY", this);
	sourceYCmd->SetGuidance("Set Y position of source container volume");
	sourceYCmd->SetGuidance("Default: 0.");
	sourceYCmd->SetParameterName("sourceY", true);
	sourceYCmd->SetDefaultValue(0.);

	sourceZCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceZ", this);
	sourceZCmd->SetGuidance("Set Z position of source container volume");
	sourceZCmd->SetGuidance("Default: 0.");
	sourceZCmd->SetParameterName("sourceZ", true);
	sourceZCmd->SetDefaultValue(0.);

	sourceDXCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceDX", this);
	sourceDXCmd->SetGuidance("Set X dimension of source container volume");
	sourceDXCmd->SetGuidance("Default: 10. * mm");
	sourceDXCmd->SetParameterName("sourceDX", true);
	sourceDXCmd->SetDefaultValue(10. * mm);

	sourceDYCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceDY", this);
	sourceDYCmd->SetGuidance("Set Y dimension of source container volume");
	sourceDYCmd->SetGuidance("Default: 10. * mm");
	sourceDYCmd->SetParameterName("sourceDY", true);
	sourceDYCmd->SetDefaultValue(10. * mm);

	sourceDZCmd = new G4UIcmdWithADoubleAndUnit("/angcorr/sourceDZ", this);
	sourceDZCmd->SetGuidance("Set Z dimension of source container volume");
	sourceDZCmd->SetGuidance("Default: 10. * mm");
	sourceDZCmd->SetParameterName("sourceDZ", true);
	sourceDZCmd->SetDefaultValue(10. * mm);

	sourcePVCmd = new G4UIcmdWithAString("/angcorr/sourcePV", this);
	sourcePVCmd->SetGuidance("Add physical volume as a particle source.");
	sourcePVCmd->SetParameterName("sourcePV", true);
	sourcePVCmd->SetDefaultValue("");
}

AngularCorrelationMessenger::~AngularCorrelationMessenger() {
	delete angCorrDirectory;
}

void AngularCorrelationMessenger::SetNewValue(G4UIcommand *command,
                                               G4String newValues) {

	if (command == stepCmd) {
		n_steps = stepCmd->GetNewIntValue(newValues);
	}

	if (command == particleCmd) {
		if(current_steps >= n_steps){
			G4cout << "Warning:AngularCorrelationMessenger.cc:SetNewValue(): Prevented the addition of another step to the cascade. This warning may occured because '/angcorr/steps' was called with the wrong number of intended steps. It may also occur if the same macro was called several times. In this case, it is intentional." << G4endl;
		} else{
			G4ParticleDefinition *pd = particleTable->FindParticle(newValues);
			if (pd != NULL) {
				angularCorrelationGenerator->AddParticle(pd);
			} else {
				G4cout << "Error! AngularCorrelationMessenger: Particle "
					  "definition not found."
				       << G4endl;
				return;
			}
			++current_steps;
		}
	}

	if (command == energyCmd) {
		angularCorrelationGenerator->SetEnergy(
		    energyCmd->GetNewDoubleValue(newValues));
	}

	if (command == directionCmd) {
		angularCorrelationGenerator->SetDirection(
				directionCmd->GetNew3VectorValue(newValues));	
	}

	if (command == relativeAngleCmd) {
		angularCorrelationGenerator->SetRelativeAngle(
		    relativeAngleCmd->GetNewDoubleValue(newValues));
	}

	if (command == nStatesCmd) {
		angularCorrelationGenerator->AddNStates(
		    nStatesCmd->GetNewIntValue(newValues));
	}

	if (command == state1Cmd) {
		angularCorrelationGenerator->SetState(
		    0, state1Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state2Cmd) {
		angularCorrelationGenerator->SetState(
		    1, state2Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state3Cmd) {
		angularCorrelationGenerator->SetState(
		    2, state3Cmd->GetNewDoubleValue(newValues));
	}
	if (command == state4Cmd) {
		angularCorrelationGenerator->SetState(
		    3, state4Cmd->GetNewDoubleValue(newValues));
	}

	if (command == delta12Cmd) {
		angularCorrelationGenerator->SetDelta(
		    0, delta12Cmd->GetNewDoubleValue(newValues));
	}
	if (command == delta23Cmd) {
		angularCorrelationGenerator->SetDelta(
		    1, delta23Cmd->GetNewDoubleValue(newValues));
	}
	if (command == delta34Cmd) {
		angularCorrelationGenerator->SetDelta(
		    2, delta34Cmd->GetNewDoubleValue(newValues));
	}
	if (command == polarizationCmd) {
		angularCorrelationGenerator->SetPolarized(
				polarizationCmd->GetNewBoolValue(newValues));
	}

	if (command == sourceXCmd) {
		angularCorrelationGenerator->SetSourceX(
		    sourceXCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceYCmd) {
		angularCorrelationGenerator->SetSourceY(
		    sourceYCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceZCmd) {
		angularCorrelationGenerator->SetSourceZ(
		    sourceZCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDXCmd) {
		angularCorrelationGenerator->SetSourceDX(
		    sourceDXCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDYCmd) {
		angularCorrelationGenerator->SetSourceDY(
		    sourceDYCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourceDZCmd) {
		angularCorrelationGenerator->SetSourceDZ(
		    sourceDZCmd->GetNewDoubleValue(newValues));
	}
	if (command == sourcePVCmd) {
		angularCorrelationGenerator->AddSourcePV(newValues);
	}
}

G4String AngularCorrelationMessenger::GetCurrentValue(G4UIcommand *command) {
	G4String cv = "";

	if (command == particleCmd) {
		G4ParticleDefinition *pd =
		    angularCorrelationGenerator->GetParticleDefinition();
		return pd->GetParticleName();
	}

	if (command == energyCmd) {
		return energyCmd->ConvertToString(
		    angularCorrelationGenerator->GetParticleEnergy());
	}

	if (command == nStatesCmd) {
		return nStatesCmd->ConvertToString(
		    angularCorrelationGenerator->GetNStates());
	}

	if (command == state1Cmd) {
		return state1Cmd->ConvertToString(
		    angularCorrelationGenerator->GetState(0));
	}

	if (command == state2Cmd) {
		return state2Cmd->ConvertToString(
		    angularCorrelationGenerator->GetState(1));
	}

	if (command == state3Cmd) {
		return state3Cmd->ConvertToString(
		    angularCorrelationGenerator->GetState(2));
	}

	if (command == state4Cmd) {
		return state4Cmd->ConvertToString(
		    angularCorrelationGenerator->GetState(3));
	}

	if (command == delta12Cmd) {
		return delta12Cmd->ConvertToString(
		    angularCorrelationGenerator->GetDelta(0));
	}

	if (command == delta23Cmd) {
		return delta23Cmd->ConvertToString(
		    angularCorrelationGenerator->GetDelta(1));
	}

	if (command == delta34Cmd) {
		return delta34Cmd->ConvertToString(
		    angularCorrelationGenerator->GetDelta(2));
	}
	if (command == sourceXCmd) {
		return sourceXCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceX());
	}
	if (command == sourceYCmd) {
		return sourceYCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceY());
	}
	if (command == sourceZCmd) {
		return sourceZCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceZ());
	}
	if (command == sourceDXCmd) {
		return sourceDXCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceDX());
	}
	if (command == sourceDYCmd) {
		return sourceDYCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceDY());
	}
	if (command == sourceDZCmd) {
		return sourceDZCmd->ConvertToString(
		    angularCorrelationGenerator->GetSourceDZ());
	}
	if (command == polarizationCmd) {
		return polarizationCmd->ConvertToString(
		    angularCorrelationGenerator->IsPolarized());
	}

	return cv;
}
