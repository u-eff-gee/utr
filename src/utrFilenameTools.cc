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

#include "utrFilenameTools.hh"

#include "G4FileUtilities.hh"
#include "globals.hh"

#include <dirent.h>
#include <sstream>
#include <sys/stat.h>

using std::string;
using std::stringstream;

utrFilenameTools::utrFilenameTools() {}
utrFilenameTools::~utrFilenameTools() {}

// Need to define (initialize) static members of utrFilenameTools somewhere not in the header, so here
// Values choosen here should be irrelevant anyway as also set in utr.cc
string utrFilenameTools::outputDir = ".";
string utrFilenameTools::filenamePrefix = "utr";
unsigned int utrFilenameTools::filenameID = 0;
bool utrFilenameTools::useFilenameID = true;
string utrFilenameTools::masterFilename = "";

unsigned int utrFilenameTools::findNextFreeFilenameID() {
  // Determine the next free filename (with ID) by searching for files with the name
  // '{utrFilenameTools::filenamePrefix}N.root' or '{utrFilenameTools::filenamePrefix}N_t0.root' in the requested directory
  G4FileUtilities fileutil;
  stringstream filename_single;
  stringstream filename_multi;
  unsigned int fid = 0;
  for (fid = 0; fid < INT_MAX; ++fid) {
    filename_single << outputDir << "/" << filenamePrefix << fid << ".root";
    filename_multi << outputDir << "/" << filenamePrefix << fid << "_t0.root";

    if (fileutil.FileExists(filename_single.str()) || fileutil.FileExists(filename_multi.str())) {
      filename_single.str("");
      filename_multi.str("");
      continue;
    }
    break;
  }
  G4cout << "Using file name prefix '" << filenamePrefix << fid << "' ..." << G4endl;
  filenameID = fid - 1;
  return fid;
}

bool utrFilenameTools::setOutputDir(string odir) {
  // If output directory does not exists try to create it
  if (!opendir(odir.c_str())) {
    const int dir_err = mkdir(odir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (dir_err == -1) {
      G4cout << "Error creating output directory '" << odir << "'" << G4endl;
      abort();
    } else {
      G4cout << "Created output directory '" << odir << "'" << G4endl;
      outputDir = odir;
      return true;
    }
  }
  outputDir = odir;
  return false;
}

string utrFilenameTools::getMasterFilename() {
  if (masterFilename == "") {
    char tmpfilename[L_tmpnam]; // Char array of L_tmpnam size as requested by tmpnam
    if (NULL == std::tmpnam(tmpfilename)) { // Let tmpnam create a not yet used filename and store it in the tmpfilename char array
      G4cerr << "ERROR: Could not obtain a free temporary filename for the master thread's output! tmpnam failed! Aborting..." << G4endl;
      throw std::exception();
    }
    stringstream rootFilename;
    rootFilename << tmpfilename << ".root"; // Geant4 forces a .root extension to the filename
    G4FileUtilities fileutil;
    if (fileutil.FileExists(rootFilename.str())) { // Hence manually check if the tmpnam filename with a .root extension is also non-existing, if not abort
      G4cerr << "ERROR: Could not obtain a free temporary filename for the master thread's output! Tried: '" << rootFilename.str() << "' but it is already exists! Aborting..." << G4endl;
      throw std::exception();
    }
    masterFilename = rootFilename.str(); // Remember this free filename
  }
  return masterFilename;
}

void utrFilenameTools::deleteMasterFilename() {
  if (masterFilename != "") {
    std::remove(masterFilename.c_str());
  }
}
