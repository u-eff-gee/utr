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
#include "G4UImanager.hh"
#include "G4UIcmdWithAnInteger.hh"


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

	appendZerosToVarCmd = new G4UIcmdWithAString("/utr/appendZerosToVar", this);
	appendZerosToVarCmd->SetGuidance("Set an UI/macro alias (a variable) to the given numerical value appending a decimal dot and the requested number of zeros if necessary");
	appendZerosToVarCmd->SetParameterName("variableName> <variableValue> <numberOfDecimalDigits", false);
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
		if (arg != utrFilenameTools::getUseFilenameID()) {
			if (arg) {
				G4cout << "Turning on appendage of sequential IDs to the filename prefix" << G4endl;
			} else {
				G4cout << "Turning off appendage of sequential IDs to the filename prefix" << G4endl;
			}
			utrFilenameTools::setUseFilenameID(arg);
		}
	} else if (command == appendZerosToVarCmd) {
		// This command can be used to append a decimal dot and a number of zeros to an alias if necessary to
		// get more uniform filenames in combination with the setFilename command in macro loops.
		// A simple,, less powerful alternative to this custom command is adding the line
		// /control/doif {var}0 != {var} /control/alias var {var}.0
		// to a macro. With this construct, however, only variables without a decimal dot can be corrected.

		// Split newValues string into a vector of strings at whitespace
		// Could maybe have used a more generic G4UIcommand class than cmdWithAString and introduced more
		// parameters already in its definition by appendZerosToVarCmd->SetParameter and get those instead
		// of manually splitting and parsing as done here...
    	std::vector<G4String> parameters;
    	std::istringstream iStrStream(newValues);
    	for(std::string s; iStrStream >> s; ) {
        	parameters.push_back(s);
		}
		// Less than 2 parameters are illegal
		if (parameters.size() < 2) {
			G4cerr << "Error! Need at least 2 parameters!" << G4endl;
		} else {
			// Third parameter is optional and determines the number of zeros to append, 0 zeros means just append a decimal dot, a negative value means don't do anything
			int decimalPlaces;
			if (parameters.size() < 3) {
				decimalPlaces=0;
			} else {
				decimalPlaces=G4UIcmdWithAnInteger::GetNewIntValue(parameters[2]);
			}
			
			std::string aliasValue;
			if (decimalPlaces < 0) { // Negative decimal places: Don't change the value at all
				aliasValue=parameters[1];
			} else {
				// Split the string containing the float value to pad at possibly existing decimal points and put the parts into a string vector
				std::stringstream valueStrStream(parameters[1]);
				std::string segment;
				std::vector<std::string> seglist;
				while(std::getline(valueStrStream, segment, '.')) {
				   seglist.push_back(segment);
				}
				// If the string vector has a size smaller two, there was no decimal point, so just pad the decimal point and all required zeros
				if (seglist.size() < 2) {
					string padZeros(decimalPlaces,'0');
					aliasValue=seglist[0]+"."+padZeros;
				// Else see how many zeros are needed as padding by comparing to the length of the decimal part
				} else if (seglist[1].length() < (unsigned int) decimalPlaces) {
					string padZeros(decimalPlaces-seglist[1].length(),'0');
					aliasValue=seglist[0]+"."+seglist[1]+padZeros;
				} else {
					aliasValue=seglist[0]+"."+seglist[1];
				}
			}
			// Finally, apply alias command
			std::stringstream aliasCommand;
			aliasCommand << "/control/alias " << parameters[0] << " " << aliasValue;
			G4UImanager *UImanager = G4UImanager::GetUIpointer();
			UImanager->ApplyCommand(aliasCommand.str());
		}
	} else {
		G4cerr << "Error! Unknown command!" << G4endl;
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
