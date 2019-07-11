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

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <string>

using std::string;

enum output_flags: short {
	ID = 0,
	EDEP = 1,	
	EKIN = 2,	
	PARTICLE = 3,	
	VOLUME = 4,	
	POSX = 5,	
	POSY = 6,	
	POSZ = 7,	
	MOMX = 8,	
	MOMY = 9,	
	MOMZ = 10,
	NFLAGS = 11
};

class RunAction : public G4UserRunAction {
  public:
	RunAction();
	virtual ~RunAction();

	virtual void BeginOfRunAction(const G4Run *);
	virtual void EndOfRunAction(const G4Run *);

	G4String GetOutputFlagName(unsigned int n);

	void setOutputDir(string output) { outputdir = output; };
	void setFilenameID(unsigned int fid) {filenameid = fid; };

  private:
	unsigned int filenameid;
	string outputdir;
};
