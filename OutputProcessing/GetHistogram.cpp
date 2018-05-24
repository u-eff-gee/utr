#include <iostream>
#include <stdlib.h>
#include <argp.h>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

static char doc[] = "GetHistogram";
static char args_doc[] = "Create histograms from a list of events";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	const char* outputfilename;

	int bin;
	unsigned int multiplicity;
	bool verbose;
	
	arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("hist.root"), bin(-1), multiplicity(1), verbose(false) {};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2" },
{ 0, 'o', "OUTPUTFILENAME", 0, "Output file name" },
{ 0, 'b', "BIN", 0, "Number of energy bin whose value should be displayed" },
{ 0, 'm', "MULTIPLICITY", 0, "Particle multiplicity" },
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
		case 'm': args->multiplicity = (unsigned int) atoi(arg); break;
		case 'v': args->verbose = true;
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
		cout << "> MULTIPLICITY : " << args.multiplicity << endl;
		if(args.bin != -1){
			cout << "> BIN          : " << args.bin << endl;
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

	//
	//	START OF USER-DEFINED OUTPUT
	//

	// Create histograms
	
	const Int_t nhistograms = 13;

	TH1* hist[nhistograms];

	hist[0] = new TH1F("hpge0", "Energy Deposition in ZeroDegree", 12000, 0.0005, 12.0005);
	hist[1] = new TH1F("hpge1", "Energy Deposition in HPGe1", 12000, 0.0005, 12.0005);
	hist[2] = new TH1F("hpge2", "Energy Deposition in HPGe2", 12000, 0.0005, 12.0005);
	hist[3] = new TH1F("hpge3", "Energy Deposition in HPGe3", 12000, 0.0005, 12.0005);
	hist[4] = new TH1F("hpge4", "Energy Deposition in HPGe4", 12000, 0.0005, 12.0005);
	hist[5] = new TH1F("labr1", "Energy Deposition in LaBr1", 12000, 0.0005, 12.0005);
	hist[6] = new TH1F("labr2", "Energy Deposition in LaBr2", 12000, 0.0005, 12.0005);
	hist[7] = new TH1F("labr3", "Energy Deposition in LaBr3", 12000, 0.0005, 12.0005);
	hist[8] = new TH1F("labr4", "Energy Deposition in LaBr4", 12000, 0.0005, 12.0005);
	hist[9] = new TH1F("hpge9", "Energy Deposition in HPGe9", 12000, 0.0005, 12.0005);
	hist[10] = new TH1F("hpge10", "Energy Deposition in HPGe10", 12000, 0.0005, 12.0005);
	hist[11] = new TH1F("hpge11", "Energy Deposition in HPGe11", 12000, 0.0005, 12.0005);
	hist[12] = new TH1F("hpge12", "Energy Deposition in HPGe12", 12000, 0.0005, 12.0005);

	UInt_t multiplicity_counter[nhistograms] = {0};

	// Fill histogram from TBranch in TChain with user-defined conditions
	// Define variables and read their values from the tree using the GetEntry() method
	Double_t Edep, Volume;
	Double_t Edep_hist[nhistograms] = {0.};


	utr.SetBranchAddress("edep", &Edep);
	utr.SetBranchAddress("volume", &Volume);

	for(int i = 0; i < utr.GetEntries(); i++){
		utr.GetEntry(i);

		if(Edep > 0.){
			
			if(Volume == 0){
				Edep_hist[0] += Edep;
				++multiplicity_counter[0];
				if(multiplicity_counter[0] == args.multiplicity){
					hist[0]->Fill(Edep_hist[0]);
					Edep_hist[0] = 0.;
					multiplicity_counter[0] = 0;
				}
			}
			if(Volume == 1){
				Edep_hist[1] += Edep;
				++multiplicity_counter[1];
				if(multiplicity_counter[1] == args.multiplicity){
					hist[1]->Fill(Edep_hist[1]);
					Edep_hist[1] = 0.;
					multiplicity_counter[1] = 0;
				}
			}
			if(Volume == 2){
				Edep_hist[2] += Edep;
				++multiplicity_counter[2];
				if(multiplicity_counter[2] == args.multiplicity){
					hist[2]->Fill(Edep_hist[2]);
					Edep_hist[2] = 0.;
					multiplicity_counter[2] = 0;
				}
			}
			if(Volume == 3){
				Edep_hist[3] += Edep;
				++multiplicity_counter[3];
				if(multiplicity_counter[3] == args.multiplicity){
					hist[3]->Fill(Edep_hist[3]);
					Edep_hist[3] = 0.;
					multiplicity_counter[3] = 0;
				}
			}
			if(Volume == 4){
				Edep_hist[4] += Edep;
				++multiplicity_counter[4];
				if(multiplicity_counter[4] == args.multiplicity){
					hist[4]->Fill(Edep_hist[4]);
					Edep_hist[4] = 0.;
					multiplicity_counter[4] = 0;
				}
			}
			if(Volume == 5){
				Edep_hist[5] += Edep;
				++multiplicity_counter[5];
				if(multiplicity_counter[5] == args.multiplicity){
					hist[5]->Fill(Edep_hist[5]);
					Edep_hist[5] = 0.;
					multiplicity_counter[5] = 0;
				}
			}
			if(Volume == 6){
				Edep_hist[6] += Edep;
				++multiplicity_counter[6];
				if(multiplicity_counter[6] == args.multiplicity){
					hist[6]->Fill(Edep_hist[6]);
					Edep_hist[6] = 0.;
					multiplicity_counter[6] = 0;
				}
			}
			if(Volume == 7){
				Edep_hist[7] += Edep;
				++multiplicity_counter[7];
				if(multiplicity_counter[7] == args.multiplicity){
					hist[7]->Fill(Edep_hist[7]);
					Edep_hist[7] = 0.;
					multiplicity_counter[7] = 0;
				}
			}
			if(Volume == 8){
				Edep_hist[8] += Edep;
				++multiplicity_counter[8];
				if(multiplicity_counter[8] == args.multiplicity){
					hist[8]->Fill(Edep_hist[8]);
					Edep_hist[8] = 0.;
					multiplicity_counter[8] = 0;
				}
			}
			if(Volume == 9){
				Edep_hist[9] += Edep;
				++multiplicity_counter[9];
				if(multiplicity_counter[9] == args.multiplicity){
					hist[9]->Fill(Edep_hist[9]);
					Edep_hist[9] = 0.;
					multiplicity_counter[9] = 0;
				}
			}
			if(Volume == 10){
				Edep_hist[10] += Edep;
				++multiplicity_counter[10];
				if(multiplicity_counter[10] == args.multiplicity){
					hist[10]->Fill(Edep_hist[10]);
					Edep_hist[10] = 0.;
					multiplicity_counter[10] = 0;
				}
			}
			if(Volume == 11){
				Edep_hist[11] += Edep;
				++multiplicity_counter[11];
				if(multiplicity_counter[11] == args.multiplicity){
					hist[11]->Fill(Edep_hist[11]);
					Edep_hist[11] = 0.;
					multiplicity_counter[11] = 0;
				}
			}
			if(Volume == 12){
				Edep_hist[12] += Edep;
				++multiplicity_counter[12];
				if(multiplicity_counter[12] == args.multiplicity){
					hist[12]->Fill(Edep_hist[12]);
					Edep_hist[12] = 0.;
					multiplicity_counter[12] = 0;
				}
			}
		}
	}
	
	//
	//	END OF USER-DEFINED OUTPUT
	//

	if(args.bin != -1){
//		cout << "> Content of bin #" << args.bin << ":" << endl;
//		for(Int_t i = 0; i < nhistograms; ++i){
//			cout << hist[i]->GetName() << " : " << hist[i]->GetBinContent(args.bin) << endl;
//		}

		cout << "[ ";
		for(Int_t i = 0; i < nhistograms; ++i){
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
		cout << "> Created output file " << args.outputfilename << endl;
	}
}	
