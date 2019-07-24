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

#include "G4Types.hh"
#include <string>

using std::string;

// Custom class definition
class utrFilenameTools {
  public:
	utrFilenameTools();
	virtual ~utrFilenameTools();

	static bool setOutputDir(string odir); // Also checks whether directory exists (return false) and creates it if necessary (return true)
	static string getOutputDir() { return outputDir; };
	static void setFilenamePrefix(string fpre) { filenamePrefix = fpre; };
	static string getFilenamePrefix() { return filenamePrefix; };
	static void setFilenameID(unsigned int fid) { filenameID = fid; };
    static unsigned int getFilenameID() { return filenameID; };
	static unsigned int incrementFilenameID() { return ++filenameID; };
	static void setUseFilenameID(unsigned int ufid) { useFilenameID = ufid; };
    static bool getUseFilenameID() { return useFilenameID; };
	static unsigned int findNextFreeFilenameID();

  private:
    // statics are set as statics here so they are shared and available to all threads
	// they got introduced because the master thread did/does not run through utrFilenameTools::Build() and hence had no acess to these variables needed in RunAction::BeginOfRunAction
	static string outputDir;
	static string filenamePrefix;
	static unsigned int filenameID;
	static bool useFilenameID;
};
