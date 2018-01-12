#include <iostream>
#include <stdlib.h>
#include <argp.h>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH2.h>
#include <TF2.h>
#include <TFile.h>
#include <TMath.h>

static char doc[] = "AngularDistributionGenerator_Test";
static char args_doc[] = "Create histogram of momentum distribution in theta and phi";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	const char* outputfilename;

	arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("hist.root"){};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2" },
{ 0, 'o', "OUTPUTFILENAME", 0, "Output file name" },
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

// When implementing an angular distribution, remember to add the sin(theta) - factor
Double_t w(Double_t *x, Double_t *par){
	return par[0] * sin(x[0]) * 0.75 * (1 + pow(cos(x[0]), 2) + pow(sin(x[0]), 2) * cos(2 * x[1]));
	//return par[0] * sin(x[0]);
}

int main(int argc, char* argv[]){

	//const Double_t half_pi = TMath::Pi()*0.5;
	const Double_t pi = TMath::Pi();
	const Double_t two_pi = TMath::Pi()*2.;
	
	struct arguments args;
	argp_parse(&argp, argc, argv, 0, 0, &args);

	cout << "#############################################" << endl;
	cout << "> AngularDistributionGenerator_Test" << endl;
	cout << "> TREENAME     : " << args.tree << endl;
	cout << "> FILES        : " << "*" << args.p1 << "*" << args.p2 << "*" << endl;
	cout << "> OUTPUTFILE   : " << args.outputfilename << endl;
	cout << "#############################################" << endl;

	// Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
	TSystemDirectory dir(".",".");

	TList *files = dir.GetListOfFiles();
	TChain utr("utr");

	cout << "> Joining all files that contain '" << args.p1 << "' and '" << args.p2 << "':" << endl;
	
	if(files){
		TSystemFile *file;
		TString fname;

		TIter next(files);
		file = (TSystemFile*) next();
		while (file){
			fname = file->GetName();
			if(!file->IsDirectory() && fname.Contains(args.p1) && fname.Contains(args.p2)){
				cout << fname << endl;
				utr.Add(fname);
			}
			file = (TSystemFile*) next();
		}
	}

	//
	//	START OF USER-DEFINED OUTPUT
	//

	// Create histogram
	
	Int_t nbins_phi = 50;
	Int_t nbins_theta = 50;
//	Double_t phi_low = -pi;
//	Double_t phi_up = pi;
	Double_t phi_low = 0.;
	Double_t phi_up = two_pi;
//	Double_t theta_low = -half_pi;
//	Double_t theta_up = half_pi;
	Double_t theta_low = 0.;
	Double_t theta_up = pi;

	TH2F *hist = new TH2F("hist", "Momentum distribution in (theta, phi)", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);

	// Fill histogram from TBranch in TChain with user-defined conditions
	// Define variables and read their values from the tree using the GetEntry() method
	Double_t momentum_x, momentum_y, momentum_z;

	utr.SetBranchAddress("vx", &momentum_x);
	utr.SetBranchAddress("vy", &momentum_y);
	utr.SetBranchAddress("vz", &momentum_z);

	for(int i = 0; i < utr.GetEntries(); i++){
		utr.GetEntry(i);

		if(momentum_x >= 0. && momentum_y < 0.){
			hist->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), two_pi + atan(momentum_y/momentum_x));
		}
		else if(momentum_x >= 0. && momentum_y >= 0.){
			hist->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), atan(momentum_y/momentum_x));
		}
		else if(momentum_x < 0. && momentum_y >= 0.){
			hist->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), pi + atan(momentum_y/momentum_x));
		}
		else if(momentum_x < 0. && momentum_y < 0.){
			hist->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), pi + atan(momentum_y/momentum_x));
		}
	}

	// Fit the histogram
	
	TF2 *ang_dist = new TF2("ang_dist", w, 0., pi, 0., two_pi, 1);
	Double_t ang_dist_params[1] = { 1. };
	ang_dist->SetParameters(ang_dist_params);

	hist->Fit("ang_dist");
	
	//
	//	END OF USER-DEFINED OUTPUT
	//

	// Write histogram to a new TFile
	TFile *of = new TFile(args.outputfilename, "RECREATE");

	hist->Write();
	//ang_dist->Write();

	of->Close();

	cout << "> Created output file " << args.outputfilename << endl;
}	
