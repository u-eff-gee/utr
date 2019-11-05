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

#include <iostream>
#include <stdlib.h>
#include <argp.h>
#include <vector>
#include <string>
#include <sstream>
#include <dirent.h>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using std::cout;
using std::cerr;
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
	{ "tree", 't', "TREENAME", 0, "Name of tree composing the list of events to process (default: utr)" },
	{ "pattern1", 'p', "PATTERN1", 0, "First string files must contain to be processed (default: utr)" },
	{ "pattern2", 'q', "PATTERN2", 0, "Second string files must contain to be processed (default: .root)" },
    { "inputdir", 'd', "INPUTDIR", 0, "Directory to search for input files matching the patterns (default: current working directory '.' )" },
    { "filename", 'o', "OUTPUTFILENAME", 0, "Output file name, file will be overwritten! (default: {PATTERN1}_hist.root with a trailing '_t' in PATTERN1 dropped)" },
    { "outputdir", 'O', "OUTPUTDIR", 0, "Directory in which the output files will be written (default: same as INPUTDIR)" },
  //{ "begin",    'b', "START",  0,  "ID of first detector to extract" },
  //{ "end",      'e', "END",    0,  "ID of last detector to extract" },
	{ "binning", 'b', "BINNING", 0, "Size of bins in the histogram in keV (default: 1 keV)" },
	{ "maxenergy", 'e', "EMAX", 0, "Maximum energy displayed in histogram in MeV (rounded up to match BINNING) (default: 10 MeV)" },
	{ "showbin", 'B', "BIN", 0, "Number of energy bin whose value should be displayed, -1 to disable (default: -1)" },
	{ "maxid", 'n', "MAXID", 0, "Highest detection volume ID (default: 12). 'getHistogram' only processes energy depositions in detectors with integer volume ID numbers from 0 to MAXID." },
	{ "multiplicity", 'm', "MULTIPLICITY", 0, "Particle multiplicity, sum energy depositions for each detector among MULTIPLICITY events (default: 1)" },
	{ "addback", 'a', 0, 0, "Add back energy depositions that occurred in a single event to the detector first listed in the event (usually this is the first one hit) (default: Off)" },
	{ "silent", 's', 0, 0, "Silent mode (does not silence -B option) (default: Off" },
	{ 0, 0, 0, 0, 0 }
};

// Used by main to communicate with parse_opt
struct arguments {
	string tree="utr";
	string p1="utr";
	string p2=".root";
	string inputDir=".";
	string outputFilename="";
	string outputDir="";
	//int firstDetectorID=0;
	//int lastDetectorID=12;
	double binning=1./1000.;
	double eMax=10.;
	int binToPrint=-1;
	unsigned int nhistograms=12 + 1; // Default value for MAXID of 12 and +1 (histograms 0 to 12)
	unsigned int multiplicity=1;
	bool addback=false;
	bool verbose=true;
};

// Function to parse a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  	// Get the input argument from argp_parse, which is a pointer to the arguments structure
	struct arguments *arguments = (struct arguments *) state->input;

	switch (key) {
		case 't': arguments->tree = arg; break;
		case 'p': arguments->p1 = arg; break;
		case 'q': arguments->p2 = arg; break;
		case 'd': arguments->inputDir = arg; break;
		case 'o': arguments->outputFilename = arg; break;
		case 'O': arguments->outputDir = arg; break;
		case 'b': arguments->binning = atof(arg)/1000.; break;
		case 'e': arguments->eMax = atof(arg); break;
		case 'B': arguments->binToPrint = atoi(arg); break;
		case 'n': arguments->nhistograms = (unsigned int) atoi(arg) + 1; break; // = MAXID + 1 (histograms 0 to MAXID)
		case 'm': arguments->multiplicity = (unsigned int) atoi(arg); break;
		case 'a': arguments->addback = true; break;
		case 's': arguments->verbose = false; break;
		case ARGP_KEY_ARG: 
			cerr << "> Error: getHistogram takes only options and no arguments!" << endl;
			argp_usage (state); 
			break;
		case ARGP_KEY_END: break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char* argv[]){
	
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
			arguments.outputFilename=arguments.p1.substr(0, arguments.p1.size() - 2) + "_hist.root";
		} else {
			arguments.outputFilename=arguments.p1 + "_hist.root";
		}
	}

	if(arguments.verbose){
		cout << "#############################################" << endl;
		cout << "> getHistogram" << endl;
		cout << "> TREENAME     : " << arguments.tree << endl;
		cout << "> FILES        : " << "*" << arguments.p1 << "*" << arguments.p2 << "*" << endl;
		cout << "> INPUTDIR     : " << arguments.inputDir << endl;
		cout << "> OUTPUTFILE   : " << arguments.outputFilename << endl;
		cout << "> OUTPUTDIR    : " << arguments.outputDir << endl;
		cout << "> BINNING      : " << arguments.binning * 1000 << " keV" << endl;
		cout << "> EMAX         : " << arguments.eMax << " MeV" << endl;
		cout << "> MAXID        : " << arguments.nhistograms - 1 << endl;
		cout << "> MULTIPLICITY : " << arguments.multiplicity << endl;
		if(arguments.binToPrint != -1){
			cout << "> BIN          : " << arguments.binToPrint << endl;
		}
		cout << "> ADDBACK      : ";
		if(arguments.addback){
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
	if(arguments.verbose){
		cout << "> Joining all files in '" << arguments.inputDir << "' that contain '" << arguments.p1 << "' and '" << arguments.p2 << "':" << endl;
	}
	TSystemDirectory dir("INPUTDIRECTORY", arguments.inputDir.c_str());
	TChain fileChain(arguments.tree.c_str());
	TString fname;
	TIter next(dir.GetListOfFiles());
	TSystemFile* file = (TSystemFile*) next();
	
	while (file){
		fname = arguments.inputDir + "/" + file->GetName();
		if(!file->IsDirectory() && fname.Contains(arguments.p1) && fname.Contains(arguments.p2)){
			if(arguments.verbose){
				cout << fname << endl;
			}
			fileChain.Add(fname);
		}
		file = (TSystemFile*) next();
	}


	// Prepare empty histograms

	// Minimum energy of histograms in MeV: bin centered around 0
	const double emin  = 0             -arguments.binning/2; 
	// Number of bins in the histograms: Chosen so that the end of the last bin using the given binning is greater or equal to the given maximum energy
	const int    nbins = (int) ceil((arguments.eMax-emin)/arguments.binning);
	// Maximum energy of histograms in MeV: Choosen so that it matches the given binning
	const double eMax  = emin+nbins*arguments.binning;

	if(arguments.verbose && eMax != arguments.eMax) {
		cout << "> Rounded up EMAX from " << arguments.eMax << " MeV to " << eMax << " MeV in order to match the requested BINNING of " << arguments.binning << " MeV"<< endl;
	}	

	vector<TH1*> hist(arguments.nhistograms + 1); // +1 For sum histogram
	stringstream histname, histtitle;

	for(unsigned int i = 0; i < arguments.nhistograms; ++i){
		histname << "det" << i;
		histtitle << "Energy deposition in Detector " << i;
		hist[i] = new TH1F(histname.str().c_str(), histtitle.str().c_str(), nbins, emin, eMax);
		histname.str("");
		histtitle.str("");
	}
	hist[arguments.nhistograms] = new TH1F("sum", "Sum spectrum of all detectors", nbins, emin, eMax);

	vector<unsigned int> multiplicity_counter(arguments.nhistograms, 0);

	// Fill histogram from TBranch in TChain with user-defined conditions
	// Define variables and automatically update their values from the ROOT tree using the GetEntry method after registering them with the SetBranchAddress method
	double Event, lastEvent;
	double Volume; // Needs to be double to correctly work with GetEntry and SetBranchAddress methods
	unsigned int lastVolume; // Needs to be unsigned int to correctly work with array indices
	double Edep;
	vector<double> EdepBuffer(arguments.nhistograms, 0.);

	fileChain.SetBranchAddress("edep", &Edep);
	fileChain.SetBranchAddress("volume", &Volume);
	if (arguments.addback) {
		fileChain.SetBranchAddress("event", &Event);
	} else {
		Event=-1; // If addback is disabled, Event will not be relevant in the code below, and the ROOT tree is not required to contain it
	}

	unsigned int addback_counter = 0;
	unsigned int warningCounter = 0;
	
	// (Pre)Process first event manually (so it is considered the last event)
	fileChain.GetEntry(0);
	long entry = 1;
	while ((unsigned int) Volume >= arguments.nhistograms && entry < fileChain.GetEntries()) { // Make sure that always a valid volume is given as the last volume
		if (warningCounter < 10) {
			cout << "Warning: Entry with volume = " << (unsigned int) Volume << " > MAXID = " << arguments.nhistograms - 1  << " encountered. Skipping this entry." << endl;
			warningCounter++;
			if (warningCounter == 10) {
				cout << "Warning: No more warnings of this type will be displayed!" << endl;
			}
		}
		fileChain.GetEntry(entry);
		entry++;
	}
	lastEvent=Event;
	lastVolume=(unsigned int) Volume;
	EdepBuffer[lastVolume] = Edep;

	//Process next events in loops
	while(entry < fileChain.GetEntries()) {
		// Get the entry, this sets the values for the Edep, Volume and Event variables
		fileChain.GetEntry(entry);
		if((unsigned int) Volume < arguments.nhistograms){ // nhistograms=MAXID+1 so must always be greater than Volume to consider that Volume
			// If addback is disabled or the event number has changed:
			if (!arguments.addback || lastEvent != Event) {
				// First process the *last* event still in the buffer:
				// Increase the volumes multiplicity counter
				multiplicity_counter[lastVolume]++;
				// If multiplicity counter is high enough write the buffered energy value to the histogram
				if(multiplicity_counter[lastVolume]==arguments.multiplicity) {
					hist[lastVolume]->Fill(EdepBuffer[lastVolume]); // Fill own histogram 
					hist[arguments.nhistograms]->Fill(EdepBuffer[lastVolume]); // Fill sum histogram
					EdepBuffer[lastVolume] = 0.; // Reset energy buffer to zero
					multiplicity_counter[lastVolume] = 0; // Reset multiplicity counter to zero
				}
				// Now update history variables to *this* event and increase addback_counter
				addback_counter++;
				lastEvent=Event;
				lastVolume=(unsigned int) Volume;
			}
			// Add Edep value to buffer (necessary for addback and multiplicity), note that the *last* Volume can now already be *this* event's volume
			EdepBuffer[lastVolume] += Edep;
		} else if (arguments.verbose && warningCounter < 10){
			cout << "Warning: Entry with volume = " << (unsigned int) Volume << " > MAXID = " << arguments.nhistograms - 1  << " encountered. Skipping this entry." << endl;
			warningCounter++;
			if (warningCounter == 10) {
				cout << "Warning: No more warnings of this type will be displayed!" << endl;
			}
		}
		++entry;
	}

	// (Post)Process last event manually 
	multiplicity_counter[lastVolume]++;
	if(multiplicity_counter[lastVolume]==arguments.multiplicity) {
		hist[lastVolume]->Fill(EdepBuffer[lastVolume]); // Fill own histogram 
		hist[arguments.nhistograms]->Fill(EdepBuffer[lastVolume]); // Fill sum histogram
	}
	addback_counter++;

	if(arguments.verbose) {
		cout << "> Processed " << fileChain.GetEntries() << " entries" << endl;
	}

	// Display counts of a specific bin in each histogram, if requested
	if(arguments.binToPrint != -1){
		cout << "Counts in bin " << arguments.binToPrint << " (centered around " << hist[0]->GetBinCenter(arguments.binToPrint) << " MeV ) for each histogram : [ ";
		for(unsigned int i = 0; i <= arguments.nhistograms; ++i){
			if(i != 0){
				cout << ", ";
			}
			cout << hist[i]->GetBinContent(arguments.binToPrint);
		}
		cout << "]" << endl;
	}

	// Write histogram to a new TFile
	TFile *outFile = new TFile((arguments.outputDir + "/" + arguments.outputFilename).c_str(), "RECREATE");
	for(auto h: hist){
		h->Write();
	}
	outFile->Close();

	if(arguments.verbose && arguments.addback) {
		if(addback_counter == fileChain.GetEntries())
			cout << "> No events added back" << endl;
		else
			cout << "> Percentage of events added back: " << ((1.-((double) addback_counter) / (double) fileChain.GetEntries())*100.) << " %" << endl;
		cout << "> Created output file " << arguments.outputFilename << endl;
	}
}	
