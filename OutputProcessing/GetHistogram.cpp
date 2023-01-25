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

#include <argp.h>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
#include <TSystemDirectory.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

// Program documentation.
static char doc[] = "Create histograms of energy depositions in detectors from a list of events stored among multiple ROOT files";
// Description of the accepted/required arguments
static char args_doc[] = ""; // No arguments, only options!

// The options argp understands
static struct argp_option options[] = {
    {"tree", 't', "TREENAME", 0, "Name of tree composing the list of events to process (default: utr)"},
    {"pattern1", 'p', "PATTERN1", 0, "First string files must contain to be processed (default: utr)"},
    {"pattern2", 'q', "PATTERN2", 0, "Second string files must contain to be processed (default: .root)"},
    {"inputdir", 'd', "INPUTDIR", 0, "Directory to search for input files matching the patterns (default: current working directory '.' )"},
    {"filename", 'o', "OUTPUTFILENAME", 0, "Output file name, file will be overwritten! (default: {PATTERN1}_hist.root with a trailing '_t' in PATTERN1 dropped)"},
    {"outputdir", 'O', "OUTPUTDIR", 0, "Directory in which the output files will be written (default: same as INPUTDIR)"},
    //{ "begin",    'b', "START",  0,  "ID of first detector to extract" },
    //{ "end",      'e', "END",    0,  "ID of last detector to extract" },
    {"quantity", 'y', "QUANTITY", 0, "Either 'edep' or 'ekin' values are processed to create histograms (default: edep)"},
    {"binning", 'b', "BINNING", 0, "Size of bins in the histogram in keV (default: 1 keV)"},
    {"maxenergy", 'e', "EMAX", 0, "Maximum energy displayed in histogram in MeV (rounded up to match BINNING) (default: 10 MeV)"},
    {"showbin", 'B', "BIN", 0, "Number of energy bin whose value should be displayed, -1 to disable (default: -1)"},
    {"maxid", 'n', "MAXID", 0, "Highest detection volume ID (default: 12). 'getHistogram' only processes energy depositions in detectors with integer volume ID numbers from 0 to MAXID (MAXID is included)."},
    {"multiplicity", 'm', "MULTIPLICITY", 0, "Particle multiplicity, sum energy depositions for each detector among MULTIPLICITY events (default: 1)"},
    {"addback", 'a', 0, 0, "Add back energy depositions that occurred in a single event to the detector first listed in the event (usually this is the first one hit) (default: Off)"},
    {"silent", 's', 0, 0, "Silent mode (does not silence -B option) (default: Off"},
    {0, 0, 0, 0, 0}};

// Used by main to communicate with parse_opt
struct arguments {
  string tree = "utr";
  string p1 = "utr";
  string p2 = ".root";
  string inputDir = ".";
  string outputFilename = "";
  string outputDir = "";
  // int firstDetectorID=0;
  // int lastDetectorID=12;
  string quantity = "edep";
  double binning = 1. / 1000.;
  double eMax = 10.;
  int binToPrint = -1;
  unsigned int nhistograms = 12 + 1; // Default value for MAXID of 12 and +1 (histograms 0 to 12)
  unsigned int multiplicity = 1;
  bool addback = false;
  bool verbose = true;
};

// Function to parse a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  // Get the input argument from argp_parse, which is a pointer to the arguments structure
  struct arguments *arguments = (struct arguments *)state->input;

  switch (key) {
    case 't':
      arguments->tree = arg;
      break;
    case 'p':
      arguments->p1 = arg;
      break;
    case 'q':
      arguments->p2 = arg;
      break;
    case 'd':
      arguments->inputDir = arg;
      break;
    case 'o':
      arguments->outputFilename = arg;
      break;
    case 'O':
      arguments->outputDir = arg;
      break;
    case 'y':
      arguments->quantity = arg;
      break;
    case 'b':
      arguments->binning = atof(arg) / 1000.;
      break;
    case 'e':
      arguments->eMax = atof(arg);
      break;
    case 'B':
      arguments->binToPrint = atoi(arg);
      break;
    case 'n':
      arguments->nhistograms = (unsigned int)atoi(arg) + 1;
      break; // = MAXID + 1 (histograms 0 to MAXID)
    case 'm':
      arguments->multiplicity = (unsigned int)atoi(arg);
      break;
    case 'a':
      arguments->addback = true;
      break;
    case 's':
      arguments->verbose = false;
      break;
    case ARGP_KEY_ARG:
      cerr << "> Error: getHistogram takes only options and no arguments!" << endl;
      argp_usage(state);
      break;
    case ARGP_KEY_END:
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {

  struct arguments arguments;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // If no outputDir was given, use the same as inputDir
  if (arguments.outputDir == "") {
    arguments.outputDir = arguments.inputDir;
  }

  // If no outputFilename was given, create an outputFilename based on pattern1 with "_hist.root" appended
  if (arguments.outputFilename == "") {
    // If pattern1 ends on "_t", additionally remove this in the outputFilename
    if (arguments.p1.size() >= 2 && arguments.p1.compare(arguments.p1.size() - 2, 2, "_t") == 0) {
      arguments.outputFilename = arguments.p1.substr(0, arguments.p1.size() - 2) + "_hist.root";
    } else {
      arguments.outputFilename = arguments.p1 + "_hist.root";
    }
  }

  if (arguments.verbose) {
    cout << "#############################################" << endl;
    cout << "> getHistogram" << endl;
    cout << "> TREENAME     : " << arguments.tree << endl;
    cout << "> FILES        : "
         << "*" << arguments.p1 << "*" << arguments.p2 << "*" << endl;
    cout << "> INPUTDIR     : " << arguments.inputDir << endl;
    cout << "> OUTPUTFILE   : " << arguments.outputFilename << endl;
    cout << "> OUTPUTDIR    : " << arguments.outputDir << endl;
    cout << "> QUANTITY     : " << arguments.quantity << endl;
    cout << "> BINNING      : " << arguments.binning * 1000 << " keV" << endl;
    cout << "> EMAX         : " << arguments.eMax << " MeV" << endl;
    cout << "> MAXID        : " << arguments.nhistograms - 1 << endl;
    cout << "> MULTIPLICITY : " << arguments.multiplicity << endl;
    if (arguments.binToPrint != -1) {
      cout << "> BIN          : " << arguments.binToPrint << endl;
    }
    cout << "> ADDBACK      : ";
    if (arguments.addback) {
      cout << "TRUE" << endl;
    } else {
      cout << "FALSE" << endl;
    }
    cout << "#############################################" << endl;
  }

  // Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
  if (!opendir(arguments.inputDir.c_str())) {
    cerr << "> ERROR: Supplied INPUTDIR is not a valid directory! Aborting..." << endl;
    exit(1);
  }
  if (!opendir(arguments.outputDir.c_str())) {
    cerr << "> ERROR: Supplied OUTPUTDIR is not a valid directory! Aborting..." << endl;
    exit(1);
  }
  if (arguments.verbose) {
    cout << "> Joining all files in '" << arguments.inputDir << "' that contain '" << arguments.p1 << "' and '" << arguments.p2 << "':" << endl;
  }
  TSystemDirectory dir("INPUTDIRECTORY", arguments.inputDir.c_str());
  TChain fileChain(arguments.tree.c_str());
  TString fname;
  TIter next(dir.GetListOfFiles());
  TSystemFile *file = (TSystemFile *)next();

  while (file) {
    fname = arguments.inputDir + "/" + file->GetName();
    if (!file->IsDirectory() && fname.Contains(arguments.p1) && fname.Contains(arguments.p2)) {
      if (arguments.verbose) {
        cout << fname << endl;
      }
      fileChain.Add(fname);
    }
    file = (TSystemFile *)next();
  }

  // Prepare empty histograms

  // Minimum energy of histograms in MeV: bin centered around 0
  const double emin = 0 - arguments.binning / 2;
  // Number of bins in the histograms: Chosen so that the end of the last bin using the given binning is greater or equal to the given maximum energy
  const int nbins = (int)ceil((arguments.eMax - emin) / arguments.binning);
  // Maximum energy of histograms in MeV: Choosen so that it matches the given binning
  const double eMax = emin + nbins * arguments.binning;

  if (arguments.verbose && eMax != arguments.eMax) {
    cout << "> Rounded up EMAX from " << arguments.eMax << " MeV to " << eMax << " MeV in order to match the requested BINNING of " << arguments.binning << " MeV" << endl;
  }

  vector<TH1 *> hist(arguments.nhistograms + 1); // +1 For sum histogram
  stringstream histname, histtitle;

  for (unsigned int i = 0; i < arguments.nhistograms; ++i) {
    histname << "det" << i;
    histtitle << "Energy deposition in Detector " << i;
    // Choice of proper data type in TH1 is VERY important here! A TH1F for example uses Floats as the datatype for the bin contents, limiting their precision to about 7 digits.
    // With this precision at a bin content of 1.67772e+07 an incrementation by one gets lost in precision, leaving the value effectively unchanged.
    // Hence the fill() method would fail unnoticed for (Float) bins as soon as they reach this content, effectively limiting the bin's content to this value (although the Float
    // datatype could handle much higher values, just not with the needed precision on integer basis).
    // Hence a TH1D is used: The Double datatype has a precision of about 14 digits (more digits than an Integer can store), and the incrementation by one gets lost at
    // a bin content of about 9.0e+15, which should suffice for all (utr) cases (one could also implement throwing an exception if a bin passes some threshold after filling).
    hist[i] = new TH1D(histname.str().c_str(), histtitle.str().c_str(), nbins, emin, eMax);
    histname.str("");
    histtitle.str("");
  }
  hist[arguments.nhistograms] = new TH1D("sum", "Sum spectrum of all detectors", nbins, emin, eMax);

  vector<unsigned int> multiplicity_counter(arguments.nhistograms, 0);

  // Fill histogram from TBranch in TChain with user-defined conditions
  // Define variables and automatically update their values from the ROOT tree using the GetEntry method after registering them with the SetBranchAddress method
  double Event, lastEvent;
  double Volume; // Needs to be double to correctly work with GetEntry and SetBranchAddress methods
  unsigned int lastVolume; // Needs to be unsigned int to correctly work with array indices
  double Edep;
  vector<double> EdepBuffer(arguments.nhistograms, 0.);

  fileChain.SetBranchAddress(arguments.quantity.c_str(), &Edep);
  fileChain.SetBranchAddress("volume", &Volume);
  if (arguments.addback) {
    fileChain.SetBranchAddress("event", &Event);
  } else {
    Event = -1; // If addback is disabled, Event will not be relevant in the code below, and the ROOT tree is not required to contain it
  }

  unsigned int addback_counter = 0;
  unsigned int warningCounter = 0;

  // The addback-option compares the event number of the last energy deposition to the present event number.
  // If the present event number is different from the last one, the energy deposition buffer is filled into
  // the histogram, set to zero, and then the present energy deposition is added to the buffer.
  // This procedure requires that the 'last event' has been defined, therefore getHistogram
  // preprocesses the first event manually.
  //
  // A valid last event has a valid detector ID. The following while loop reads entries until it finds a
  // valid last event.
  fileChain.GetEntry(0);
  long entry = 1;
  while ((unsigned int)Volume >= arguments.nhistograms && entry < fileChain.GetEntries()) { // Make sure that always a valid volume is given as the last volume
    if (warningCounter < 10) {
      cout << "Warning: Entry with volume = " << (unsigned int)Volume << " > MAXID = " << arguments.nhistograms - 1 << " encountered. Skipping this entry." << endl;
      warningCounter++;
      if (warningCounter == 10) {
        cout << "Warning: No more warnings of this type will be displayed!" << endl;
      }
    }
    fileChain.GetEntry(entry);
    entry++;
  }
  lastEvent = Event;
  lastVolume = (unsigned int)Volume;
  EdepBuffer[lastVolume] = Edep;

  // Process next events in loops
  while (entry < fileChain.GetEntries()) {
    // Get the entry, this sets the values for the Edep, Volume and Event variables
    fileChain.GetEntry(entry);
    if ((unsigned int)Volume < arguments.nhistograms) { // nhistograms=MAXID+1 so must always be greater than Volume to consider that Volume
      // If addback is disabled or the event number has changed:
      if (!arguments.addback || lastEvent != Event) {
        // First process the *last* event still in the buffer:
        // Increase the volumes multiplicity counter
        multiplicity_counter[lastVolume]++;
        // If multiplicity counter is high enough write the buffered energy value to the histogram
        if (multiplicity_counter[lastVolume] == arguments.multiplicity) {
          hist[lastVolume]->Fill(EdepBuffer[lastVolume]); // Fill own histogram
          hist[arguments.nhistograms]->Fill(EdepBuffer[lastVolume]); // Fill sum histogram
          EdepBuffer[lastVolume] = 0.; // Reset energy buffer to zero
          multiplicity_counter[lastVolume] = 0; // Reset multiplicity counter to zero
        }
        // Now update history variables to *this* event and increase addback_counter
        addback_counter++;
        lastEvent = Event;
        lastVolume = (unsigned int)Volume;
      }
      // Add Edep value to buffer (necessary for addback and multiplicity), note that the *last* Volume can now already be *this* event's volume
      EdepBuffer[lastVolume] += Edep;
    } else if (arguments.verbose && warningCounter < 10) {
      cout << "Warning: Entry with volume = " << (unsigned int)Volume << " > MAXID = " << arguments.nhistograms - 1 << " encountered. Skipping this entry." << endl;
      warningCounter++;
      if (warningCounter == 10) {
        cout << "Warning: No more warnings of this type will be displayed!" << endl;
      }
    }
    ++entry;
  }

  // (Post)Process last event manually
  multiplicity_counter[lastVolume]++;
  if (multiplicity_counter[lastVolume] == arguments.multiplicity) {
    hist[lastVolume]->Fill(EdepBuffer[lastVolume]); // Fill own histogram
    hist[arguments.nhistograms]->Fill(EdepBuffer[lastVolume]); // Fill sum histogram
  }
  addback_counter++;

  if (arguments.verbose) {
    cout << "> Processed " << fileChain.GetEntries() << " entries" << endl;
  }

  // Display counts of a specific bin in each histogram, if requested
  if (arguments.binToPrint != -1) {
    cout << "Counts in bin " << arguments.binToPrint << " (centered around " << hist[0]->GetBinCenter(arguments.binToPrint) << " MeV ) for each histogram : [ ";
    for (unsigned int i = 0; i <= arguments.nhistograms; ++i) {
      if (i != 0) {
        cout << ", ";
      }
      cout << hist[i]->GetBinContent(arguments.binToPrint);
    }
    cout << "]" << endl;
  }

  // Write histogram to a new TFile
  TFile *outFile = new TFile((arguments.outputDir + "/" + arguments.outputFilename).c_str(), "RECREATE");
  for (auto h : hist) {
    h->Write();
  }
  outFile->Close();

  if (arguments.verbose) {
    if (arguments.addback) {
      if (addback_counter == fileChain.GetEntries())
        cout << "> No events added back" << endl;
      else
        cout << "> Percentage of events added back: " << ((1. - ((double)addback_counter) / (double)fileChain.GetEntries()) * 100.) << " %" << endl;
    }
    cout << "> Created output file " << arguments.outputFilename << endl;
  }
}
