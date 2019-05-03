#include <iostream>
#include <stdlib.h>
#include <argp.h>
#include <vector>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using std::vector;

static char doc[] = "GetHistogram";
static char args_doc[] = "Create histograms from a list of events";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	const char* outputfilename;

	int bin;
	double emax;
	unsigned int nhistograms;
	unsigned int multiplicity;
	bool addback;
	bool verbose;
	
	arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("hist.root"), bin(-1), emax(15.), nhistograms(12), multiplicity(1), addback(false), verbose(false) {};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2" },
{ 0, 'o', "OUTPUTFILENAME", 0, "Output file name" },
{ 0, 'b', "BIN", 0, "Number of energy bin whose value should be displayed" },
{ 0, 'e', "EMAX", 0, "Maximum energy displayed in histogram in MeV (default: 15 MeV)" },
{ 0, 'n', "NHISTOGRAMS", 0, "Number of detection volumes (default: 12). 'getHistogram' assumes \
that the NHISTOGRAMS histograms are labeled with integer number from 0 to NHISTOGRAMS-1." },
{ 0, 'm', "MULTIPLICITY", 0, "Particle multiplicity (default: 1). \
Will be ignored if used simultaneously with the '-a' option." },
{ 0, 'a', 0, 0, "Add back energy depositions that occurred in a single event to the first detector which was hit. \
Overrides the '-m MULTIPLICITY' option." },
{ 0, 'v', 0, 0, "Verbose mode (does not silence -b option)" },
{ 0, 0, 0, 0, 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

	struct arguments *args = (struct arguments *) state->input;

switch (key) {
		case ARGP_KEY_ARG: break;
		case 't': args->tree = arg; break;
		case 'p': args->p1 = arg; break;
		case 'q': args->p2 = arg; break;
		case 'o': args->outputfilename = arg; break;
		case 'b': args->bin = atoi(arg); break;
		case 'e': args->emax = atof(arg); break;
		case 'n': args->nhistograms = (unsigned int) atoi(arg); break;
		case 'm': args->multiplicity = (unsigned int) atoi(arg); break;
		case 'a': args->addback = true; break;
		case 'v': args->verbose = true; break;
		case ARGP_KEY_END: break;
		default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


using namespace std;

int main(int argc, char* argv[]){
	
	struct arguments args;
	argp_parse(&argp, argc, argv, 0, 0, &args);

if(args.verbose){
		cout << "#############################################" << endl;
		cout << "> GetHistogram" << endl;
		cout << "> TREENAME     : " << args.tree << endl;
		cout << "> FILES        : " << "*" << args.p1 << "*" << args.p2 << "*" << endl;
		cout << "> OUTPUTFILE   : " << args.outputfilename << endl;
		cout << "> NHISTOGRAMS  : " << args.nhistograms << endl;
		cout << "> EMAX         : " << args.emax << " MeV" << endl;
		cout << "> MULTIPLICITY : ";
		if(args.addback){
			cout << "IGNORED" << endl;
		} else{
			cout << args.multiplicity << endl;
		}
		if(args.bin != -1){
			cout << "> BIN          : " << args.bin << endl;
		}
		cout << "> ADDBACK      : ";
		if(args.addback){
			cout << "TRUE" << endl;
		} else{
			cout << "FALSE" << endl;
		}
		cout << "#############################################" << endl;
	}

	// Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
	TSystemDirectory dir(".",".");

	TList *files = dir.GetListOfFiles();
	TChain utr("utr");

	if(args.verbose){
		cout << "> Joining all files that contain '" << args.p1 << "' and '" << args.p2 << "':" << endl;
	}
	
	if(files){
		TSystemFile *file;
		TString fname;

		TIter next(files);
		file = (TSystemFile*) next();
		while (file){
			fname = file->GetName();
			if(!file->IsDirectory() && fname.Contains(args.p1) && fname.Contains(args.p2)){
				if(args.verbose){
					cout << fname << endl;
				}
				utr.Add(fname);
			}
			file = (TSystemFile*) next();
		}
	}

	// Create histograms
	
	const int nbins = 15000; // Number of bins in the histograms
	const double emin = 0.0005; // Minimum energy of histograms in MeV

	vector<TH1*> hist(args.nhistograms + 1);
	stringstream histname, histtitle;

	for(long unsigned int i = 0; i < (long unsigned int) args.nhistograms; ++i){
		histname << "det" << i;
		histtitle << "Energy deposition in Detector " << i;

		hist[i] = new TH1F(histname.str().c_str(), histtitle.str().c_str(), nbins, emin, args.emax+0.0005);
		histname.str("");
		histtitle.str("");
	}
	hist[args.nhistograms] = new TH1F("all", "Sum spectrum of all detectors", nbins, emin, args.emax+0.0005);

	UInt_t multiplicity_counter[args.nhistograms] = {0};

	// Fill histogram from TBranch in TChain with user-defined conditions
	// Define variables and read their values from the tree using the GetEntry() method
	Double_t Event = 0;
	Double_t Edep, Volume;
	Double_t Edep_hist[args.nhistograms] = {0.};

	utr.SetBranchAddress("edep", &Edep);
	utr.SetBranchAddress("volume", &Volume);
	if(args.addback)
		utr.SetBranchAddress("event", &Event);

	unsigned int addback_counter = 0;

	if(args.addback){
		long unsigned int addback_volume = 0;
		unsigned int addback_event = 0;
		for(int i = 0; i < utr.GetEntries(); ++i){
			utr.GetEntry(i);
			if(addback_event != Event){
				hist[addback_volume]->Fill(Edep_hist[addback_volume]);
				hist[args.nhistograms]->Fill(Edep_hist[addback_volume]);
				Edep_hist[addback_volume] = 0.;
				addback_volume = (long unsigned int) Volume;
				addback_event = (unsigned int) Event;
				Edep_hist[addback_volume] = Edep;
				++addback_counter;
			} else{
				Edep_hist[addback_volume] += Edep;
			}
		}
	} else{
		for(int i = 0; i < utr.GetEntries(); ++i){
			utr.GetEntry(i);
			++multiplicity_counter[(long unsigned int) Volume];
			Edep_hist[(long unsigned int) Volume] += Edep;
			if(multiplicity_counter[(long unsigned int) Volume]==args.multiplicity){
				hist[(long unsigned int) Volume]->Fill(Edep_hist[(long unsigned int) Volume]);
				hist[args.nhistograms]->Fill(Edep_hist[(long unsigned int) Volume]);
				Edep_hist[(long unsigned int) Volume] = 0.;
				multiplicity_counter[(long unsigned int) Volume] = 0;
			}
		}
	}
		
	cout << "> Processed " << utr.GetEntries() << " entries" << endl;

	if(args.bin != -1){
		cout << "[ ";
		for(UInt_t i = 0; i <= args.nhistograms; ++i){
			if(i != 0){
				cout << ", ";
			}
			cout << hist[i]->GetBinContent(args.bin);
		}
		cout << "]" << endl;
	}

	// Write histogram to a new TFile
	TFile *of = new TFile(args.outputfilename, "RECREATE");

		for(auto h: hist){
			h->Write();
		}

	of->Close();

	if(args.verbose){
		if(addback_counter == 0)
			cout << "> No events added back" << endl;
		else
			cout << "> Percentage of events added back: " << ((1.-((double) addback_counter) / (double) utr.GetEntries())*100.) << " %" << endl;
		cout << "> Created output file " << args.outputfilename << endl;
	}
}	
