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

#include <ROOT/RDataFrame.hxx>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
#include <TSystemDirectory.h>
// #include <ROOT/RFile.hxx>

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
    {"tree", 't', "TREENAME", 0, "Name of tree composing the list of events to process (default: edep)"},
    {"pattern1", 'p', "PATTERN1", 0, "First string files must contain to be processed (default: utr)"},
    {"pattern2", 'q', "PATTERN2", 0, "Second string files must contain to be processed (default: .root)"},
    {"inputdir", 'd', "INPUTDIR", 0, "Directory to search for input files matching the patterns (default: current working directory '.' )"},
    {"filename", 'o', "OUTPUTFILENAME", 0, "Output file name, file will be overwritten! (default: {PATTERN1}_hist.root with a trailing '_t' in PATTERN1 dropped)"},
    {"outputdir", 'O', "OUTPUTDIR", 0, "Directory in which the output files will be written (default: same as INPUTDIR)"},
    //{ "begin",    'b', "START",  0,  "ID of first detector to extract" },
    //{ "end",      'e', "END",    0,  "ID of last detector to extract" },
    {"binning", 'b', "BINNING", 0, "Size of bins in the histogram in keV (default: 1 keV)"},
    {"maxenergy", 'e', "EMAX", 0, "Maximum energy displayed in histogram in MeV (rounded up to match BINNING) (default: 10 MeV)"},
    {"showbin", 'B', "BIN", 0, "Number of energy bin whose value should be displayed, -1 to disable (default: -1)"},
    {"maxid", 'n', "MAXID", 0, "Highest detection volume ID (default: 12). 'getHistogram-Eventwise' only processes energy depositions in detectors with integer volume ID numbers from 0 to MAXID (MAXID is included)."},
    {"addback", 'a', "ADDBACKSTARTID", 0, "Add back energy depositions that occurred in 4 leaves of clover detectors. Assumes clover leaves' volume IDs start at ADDBACKSTARTID and volume IDs of all leaves of one clover are consecutive. -1 to disable (default: -1)"},
    {"silent", 's', 0, 0, "Silent mode (does not silence -B option) (default: Off"},
    {"threads", 'T', "THREADS", 0, "Number of threads to be used, 0 for number of cpu cores (default: Number of cpu cores)"},
    {0, 0, 0, 0, 0}};

// Used by main to communicate with parse_opt
struct arguments {
  string tree = "edep";
  string p1 = "utr";
  string p2 = ".root";
  string inputDir = ".";
  string outputFilename = "";
  string outputDir = "";
  // int firstDetectorID=0;
  // int lastDetectorID=12;
  double binning = 1. / 1000.;
  double eMax = 10.;
  int binToPrint = -1;
  unsigned int nhistograms = 12 + 1; // Default value for MAXID of 12 and +1 (histograms 0 to 12)
  int addback = -1;
  bool verbose = true;
  unsigned int threads = 0;
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
    case 'a':
      arguments->addback = atoi(arg);
      break;
    case 's':
      arguments->verbose = false;
      break;
    case 'T':
      arguments->threads = (unsigned int)atoi(arg);
      break;
    case ARGP_KEY_ARG:
      cerr << "> Error: getHistogram-Eventwise takes only options and no arguments!" << endl;
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
    cout << "#############################################\n";
    cout << "> getHistogram-Eventwise\n";
    cout << "> TREENAME     : " << arguments.tree << "\n";
    cout << "> FILES        : "
         << "*" << arguments.p1 << "*" << arguments.p2 << "*\n";
    cout << "> INPUTDIR     : " << arguments.inputDir << "\n";
    cout << "> OUTPUTFILE   : " << arguments.outputFilename << "\n";
    cout << "> OUTPUTDIR    : " << arguments.outputDir << "\n";
    cout << "> BINNING      : " << arguments.binning * 1000 << " keV\n";
    cout << "> EMAX         : " << arguments.eMax << " MeV\n";
    cout << "> MAXID        : " << arguments.nhistograms - 1 << "\n";
    if (arguments.binToPrint != -1) {
      cout << "> BIN          : " << arguments.binToPrint << "\n";
    }
    if (arguments.addback != -1) {
      cout << "> ADDBACK      : " << arguments.addback << "\n";
    }
    if (arguments.threads != 0) {
      cout << "> THREADS      : " << arguments.threads << "\n";
    }
    cout << "#############################################\n";
  }

  // Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
  if (!opendir(arguments.inputDir.c_str())) {
    cerr << "> ERROR: Supplied INPUTDIR is not a valid directory! Aborting...\n";
    exit(1);
  }
  if (!opendir(arguments.outputDir.c_str())) {
    cerr << "> ERROR: Supplied OUTPUTDIR is not a valid directory! Aborting...\n";
    exit(1);
  }
  if (arguments.verbose) {
    cout << "> Joining all files in '" << arguments.inputDir << "' that contain '" << arguments.p1 << "' and '" << arguments.p2 << "':\n";
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
        cout << fname << "\n";
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
    cout << "> Rounded up EMAX from " << arguments.eMax << " MeV to " << eMax << " MeV in order to match the requested BINNING of " << arguments.binning << " MeV\n";
  }

  if (arguments.threads == 0) {
    ROOT::EnableImplicitMT();
  } else if (arguments.threads != 1) {
    ROOT::EnableImplicitMT(arguments.threads);
  }

  auto df = ROOT::RDataFrame(fileChain);

  vector<ROOT::RDF::RResultPtr<TH1D>> histPtr(arguments.nhistograms);
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

    histPtr[i] = df
                     .Filter([](double e) { return e > 0.; }, {"det" + std::to_string(i)})
                     .Histo1D(TH1D(histname.str().c_str(), histtitle.str().c_str(), nbins, emin, eMax), "det" + std::to_string(i));
    histname.str("");
    histtitle.str("");
  }

  if (arguments.addback >= 0) {
    int clover = 1;
    for (unsigned int i = static_cast<unsigned int>(arguments.addback); i + 3 < arguments.nhistograms; i += 4) {
      histname << "addback" << clover;
      histtitle << "Addback energy deposition in clover detector " << clover;
      histPtr.push_back(
          df.Define("ADDBACK" + std::to_string(clover),
                    [](double e1, double e2, double e3, double e4) { return e1 + e2 + e3 + e4; },
                    {"det" + std::to_string(i), "det" + std::to_string(i + 1), "det" + std::to_string(i + 2), "det" + std::to_string(i + 3)})
              .Filter([](double e) { return e > 0.; }, {"ADDBACK" + std::to_string(clover)})
              .Histo1D(TH1D(histname.str().c_str(), histtitle.str().c_str(), nbins, emin, eMax), "ADDBACK" + std::to_string(clover)));
      histname.str("");
      histtitle.str("");
      clover++;
    }
  }

  vector<TH1D> hist(histPtr.size() + 1); // +1 For sum histogram
  hist[arguments.nhistograms] = TH1D("sum", "Sum spectrum of all detectors", nbins, emin, eMax);
  for (unsigned int i = 0; i < arguments.nhistograms; ++i) {
    hist[i] = histPtr[i].GetValue();
    hist[arguments.nhistograms].Add(&(hist[i]));
  }
  for (unsigned int i = arguments.nhistograms + 1; i < hist.size(); ++i) {
    hist[i] = histPtr[i - 1].GetValue();
  }

  if (arguments.verbose) {
    cout << "> Processed " << fileChain.GetEntries() << " entries\n";
  }

  // Display counts of a specific bin in each histogram, if requested
  if (arguments.binToPrint != -1) {
    cout << "Counts in bin " << arguments.binToPrint << " (centered around " << hist[0].GetBinCenter(arguments.binToPrint) << " MeV ) for each histogram : [ ";
    for (unsigned int i = 0; i <= arguments.nhistograms; ++i) {
      if (i != 0) {
        cout << ", ";
      }
      cout << hist[i].GetBinContent(arguments.binToPrint);
    }
    cout << "]\n";
  }

  // Write histogram to a new TFile
  TFile *outFile = new TFile((arguments.outputDir + "/" + arguments.outputFilename).c_str(), "RECREATE");
  for (auto h : hist) {
    h.Write();
  }
  outFile->Close();

  if (arguments.verbose) {
    cout << "> Created output file " << arguments.outputFilename << "\n";
  }
}
