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

#include "utrMessenger.hh"
#include "utrFilenameTools.hh"


utrMessenger::utrMessenger() {
	utrDirectory = new G4UIdirectory("/utr/");
	utrDirectory->SetGuidance("Controls for general utr settings.");

	setFilenameCmd = new G4UIcmdWithAString("/utr/setFilename", this);
	setFilenameCmd->SetGuidance("Set the filename prefix to be used to for the output file excluding _tTHREAD.root\nDefault: 'utr'");
	setFilenameCmd->SetParameterName("filenamePrefix", true);
	setFilenameCmd->SetDefaultValue("utr");

	setUseFilenameIDCmd = new G4UIcmdWithABool("/utr/setUseFilenameID", this);
	setUseFilenameIDCmd->SetGuidance("Set whether to append sequential IDs to the filename prefix (default: true)");
	setUseFilenameIDCmd->SetParameterName("useFilenameID", true);
	setUseFilenameIDCmd->SetDefaultValue(true);
}

utrMessenger::~utrMessenger() {
	delete setFilenameCmd;
	delete setUseFilenameIDCmd;
	delete utrDirectory;
}

void utrMessenger::SetNewValue(G4UIcommand *command, G4String newValues) {
	if (command == setFilenameCmd) {
		G4cout << "Setting new filename prefix : '" << newValues << "'" << G4endl;
		utrFilenameTools::setFilenamePrefix(newValues);
		if (utrFilenameTools::getUseFilenameID()) {
			utrFilenameTools::findNextFreeFilenameID();
		}
	} else if (command == setUseFilenameIDCmd) {
		bool arg = setUseFilenameIDCmd->GetNewBoolValue(newValues);
		if (arg) {
			G4cout << "Turning on appendage of sequential IDs to the filename prefix" << G4endl;
		} else {
			G4cout << "Turning off appendage of sequential IDs to the filename prefix" << G4endl;
		}
		utrFilenameTools::setUseFilenameID(arg);
	} else {
		G4cerr << "Error! unknown command!" << G4endl;
	}
}

G4String utrMessenger::GetCurrentValue(G4UIcommand *command) {
	if (command == setFilenameCmd) {
		return utrFilenameTools::getFilenamePrefix();
	} else if (command == setUseFilenameIDCmd) {
		return setUseFilenameIDCmd->ConvertToString(utrFilenameTools::getUseFilenameID());
	}
	return "Error! unknown command!";
}
