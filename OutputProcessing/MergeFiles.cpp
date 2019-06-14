#include <iostream>
#include <stdlib.h>
#include <argp.h>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

static char doc[] = "MergeFiles";
static char args_doc[] = "Merge ROOT output files";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	const char* outputfilename;

	int bin;
	unsigned int multiplicity;
	bool verbose;
	
	arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("merged.root") {};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree (default: 'utr')" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1 (default: 'utr')" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2 (default: '.root')" },
{ 0, 'o', "OUTPUTFILENAME", 0, "Output file name (default: 'merged.root')" },
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

	// Write histogram to a new TFile
	TFile *of = new TFile(args.outputfilename, "RECREATE");

	utr.Write();

	of->Close();

	if(args.verbose){
		cout << "> Created output file " << args.outputfilename << endl;
	}
}	
