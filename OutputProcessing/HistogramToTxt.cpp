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
#include <fstream>
#include <stdlib.h>
#include <argp.h>
#include <string>
#include <sstream>

#include <TROOT.h>
#include <TH1.h>
#include <TFile.h>
//#include <TApplication.h>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::ofstream;

// Program documentation.
static char doc[] = "Extract TH1 histograms from a ROOT file as separate txt histograms";
// Description of the accepted/required arguments 
static char args_doc[] = "ROOT_FILE";

// The options argp understands
static struct argp_option options[] = {
	{ "skipempty",  's', 0,        0, "Skip empty histograms (default: Off)" },
	{ "countsonly", 'c', 0,        0, "Only write bin contents (without energies) (default: Off)" },
	{ "begin",      'b', "START",  0, "Index of first histogram to extract (default: 0)" },
	{ "end",        'e', "END",    0, "Index of last histogram to extract, -1 to extract to the last one (default: -1)" },
	{ "filename",   'f', "PREFIX", 0, "Output files' name prefix (default: ROOT_FILE's filename without its extension)" },
	{  0 }
};

// Used by main to communicate with parse_opt
struct arguments {
  	char *args[1]; // Non-option Argument
  	bool skipEmpty=false;
  	bool countsOnly=false;
  	int indexBegin=0;
  	int indexEnd=-1;
  	string filenamePrefix="";
};

// Function to parse a single option
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  	// Get the input argument from argp_parse, which is a pointer to the arguments structure
  	struct arguments *arguments = (struct arguments *) state->input;

  	switch (key) {
  	  case 's':
  	    arguments->skipEmpty = true;
  	    break;
  	  case 'c':
  	    arguments->countsOnly = true;
  	    break;
  	  case 'b':
  	    arguments->indexBegin = atoi(arg);
  	    break;
  	  case 'e':
  	    arguments->indexEnd = atoi(arg);
  	    break;
  	  case 'f':
  	    arguments->filenamePrefix = arg;
  	    break;

  	  case ARGP_KEY_ARG:
  	    if (state->arg_num >= 1) {
			// Too many arguments
			cerr << "Error: histogramToTxt takes only one argument!" << endl;
			argp_usage (state);
		}
  	    arguments->args[state->arg_num] = arg;
  	    break;

  	  case ARGP_KEY_END:
  	    if (state->arg_num < 1) {
  	    	// Not enough arguments
			cerr << "Error: histogramToTxt needs exactly one argument!" << endl;
			argp_usage (state);
		}
  	    break;

  	  default:
  	    return ARGP_ERR_UNKNOWN;
  	}
  	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {

  	struct arguments arguments;
  	// Parse arguments
	argp_parse (&argp, argc, argv, 0, 0, &arguments);
	string inputFilename = arguments.args[0];
	if (arguments.filenamePrefix == "") {
		arguments.filenamePrefix=inputFilename.substr(0, inputFilename.find_last_of("."));
	}
	//TApplication *app = new TApplication("app", 0, 0);

	// Open TFile
	TFile *inFile = new TFile(inputFilename.c_str());

	Int_t nkeys = inFile->GetNkeys();

	if(nkeys < 1){
		cerr << "Error: File does not contain any object to read from." << endl;
		return 1;
	}
	if(arguments.indexBegin < 0) {
		cerr << "Error: Index of first histogram to extract cannot be smaller than 0." << endl;
		return 1;
	}
	int maxIndex=nkeys-1;
	if(0 <= arguments.indexEnd && arguments.indexEnd < maxIndex) {
		maxIndex=arguments.indexEnd;
	}

	// Loop over all keys (hopefully all of them are TH1 histograms) and write their content to separate output files
	stringstream outputFilename;
	ofstream outFile;

	for(Int_t i = arguments.indexBegin; i <= maxIndex; i++){
		TString histogramName = inFile->GetListOfKeys()->At(i)->GetName();
		cout << "Converting histogram " << histogramName << " ...\t";		
		
		TH1* hist = (TH1*) inFile->Get(histogramName);
		
		if (arguments.skipEmpty && hist->GetEntries() == 0) {
			cout << "Histogram " << histogramName << " is empty. Skipping..." << endl;
		} else {
			outputFilename << arguments.filenamePrefix << "_" << histogramName << ".txt";
			outFile.open(outputFilename.str().c_str());

			Int_t nbins = hist->GetNbinsX();
			for(Int_t j = 1; j <= nbins; j++) {
				if (!arguments.countsOnly) {
					outFile << hist->GetBinCenter(j) << "\t";
				}
				outFile << hist->GetBinContent(j) << "\n";
			}

			outFile.close();
			cout << "Output file " << outputFilename.str() << " created." << endl;
			outputFilename.str("");
		}
	}
	return 0;
}
