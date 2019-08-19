#include <iostream>
#include <stdlib.h>
#include <argp.h>
#include <vector>
#include <map>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TFile.h>

using std::vector;

static char doc[] = "GetSolidAngleCoverag";
static char args_doc[] = "Determine Solid Angle Coverage";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	int num_events;
	bool verbose;
	
	arguments() : tree("utr"), p1("utr"), p2(".root"), num_events(0), verbose(false) {};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2" },
{ 0, 'n', "NUM_EVENTS", 0, "Number of simulated events" },
{ 0, 'v', 0, 0, "Verbose mode" },
{ 0, 0, 0, 0, 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

	struct arguments *args = (struct arguments *) state->input;

switch (key) {
		case ARGP_KEY_ARG: break;
		case 't': args->tree = arg; break;
		case 'p': args->p1 = arg; break;
		case 'q': args->p2 = arg; break;
		case 'n': args->num_events = atoi(arg); break;
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
		cout << "#############################################" << "\n";
		cout << "> GetHistogram" << "\n";
		cout << "> TREENAME     : " << args.tree << "\n";
		cout << "> FILES        : " << "*" << args.p1 << "*" << args.p2 << "*" << "\n";
		cout << "> NUM_EVENTS   : " << args.num_events << "\n";
		cout << "#############################################" << "\n";
	}

	// Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
	TSystemDirectory dir(".",".");

	TList *files = dir.GetListOfFiles();
	TChain utr("utr");

	if(args.verbose){
		cout << "> WARNING: All objects that could shadow the detector have to be defined as detectors.\n";
		cout << "> Joining all files that contain '" << args.p1 << "' and '" << args.p2 << "':\n";
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
					cout << fname << "\n";
				}
				utr.Add(fname);
			}
			file = (TSystemFile*) next();
		}
	}

	Double_t prev_event = -1;
	Double_t event = 0;
	Double_t volume;
	
	utr.SetBranchAddress("volume", &volume);
	utr.SetBranchAddress("event", &event);

	map<Double_t,int> counters, counters_first;
	for(int i = 0; i < utr.GetEntries(); ++i) {
		utr.GetEntry(i);
		if (!counters.count(volume)) counters[volume] = 0;
		if (!counters_first.count(volume)) counters_first[volume] = 0;

		if (event != prev_event)
			counters_first[volume]++;
		counters[volume]++;
		prev_event = event;
	}

	if(args.verbose){
		cout << "> Processed " << utr.GetEntries() << " entries" << "\n";
	}

	cout << "> Results:\n";
	for(auto [vol, count] : counters) {
		if (args.num_events > 0)
			printf("    Volume%3.0f: %12d (%0.8f).\n", vol, count,
				((double)count/args.num_events));
		else
			printf("    Volume%3.0f: %12d.\n", vol, count);
	}

	cout << "> Results (only first hit per event):\n";
	for(auto [vol, count] : counters_first) {
		if (args.num_events > 0)
			printf("    Volume%3.0f: %12d (%0.8f).\n", vol, count,
				((double)count/args.num_events));
		else
			printf("    Volume%3.0f: %12d.\n", vol, count);
	}

}	
