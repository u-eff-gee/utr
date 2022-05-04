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
#pragma once

#include "G4UIcmdWithABool.hh"
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

  G4UIcmdWithADoubleAndUnit *sourceXCmd;
  G4UIcmdWithADoubleAndUnit *sourceYCmd;
  G4UIcmdWithADoubleAndUnit *sourceZCmd;

  G4UIcmdWithADoubleAndUnit *sourceDXCmd;
  G4UIcmdWithADoubleAndUnit *sourceDYCmd;
  G4UIcmdWithADoubleAndUnit *sourceDZCmd;

  G4UIcmdWithAString *sourcePVCmd;

  G4UIcmdWithABool *polarizationCmd;
};
