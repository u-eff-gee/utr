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

#include <string>

#include "G4VUserActionInitialization.hh"
#include "utrConfig.h"
#include "globals.hh"

using std::string;

// Custom class definition
class ActionInitialization : public G4VUserActionInitialization {
  public:
	ActionInitialization();
	virtual ~ActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

	void setFilenameID(unsigned int fid) {filenameid = fid; };
	void setNThreads(const int nt) { n_threads = nt; };
	void setOutputDir(string output) { outputdir = output; };

  private:
	string outputdir;
	unsigned int filenameid;
	G4int n_threads;
};
