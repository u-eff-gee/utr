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
#include <TVector3.h>

#include "AngularDistribution.hh"

static char doc[] = "AngularCorrelationGenerator_Test";
static char args_doc[] = "Create histogram of momentum distribution in theta and phi relative to other particles in a cascade";

struct arguments{
	const char* tree;
	const char* p1;
	const char* p2;
	const char* outputfilename;
	unsigned int nsteps;
	bool is_unpolarized;
	bool fit;

	arguments() : tree("utr"), p1("utr"), p2(".root"), outputfilename("hist.root"), nsteps(2), is_unpolarized(false), fit(false){};
};

static struct argp_option options[] = {
{ 0, 't', "TREENAME", 0, "Name of tree" },
{ 0, 'p', "PATTERN1", 0, "File name pattern 1" },
{ 0, 'q', "PATTERN2", 0, "File name pattern 2" },
{ 0, 'o', "OUTPUTFILENAME", 0, "Output file name" },
{ 0, 'u', 0, 0, "Assume unpolarized excitation"},
{ 0, 'n', "NSTEPS", 0, "Number of steps of the cascade."},
{ 0, 'f', 0, 0, "Fit a function to the simulation result"},
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
	case 'u': args->is_unpolarized = true; break;
	case 'n': args->nsteps = (unsigned int) atoi(arg); break;
	case 'f': args->fit = true; break;
	case ARGP_KEY_END: break;
	default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

using namespace std;

// Function object used in the ROOT fit
// In order to define the same angular distribution that was used in utr, it makes use of the AngularDistribution class
class W_Function{
public:
	W_Function(bool unp):is_unpolarized(false){
	//
	//	START OF USER-DEFINED OUTPUT
	//

		nstates = 3;

		states[0] = 0.;
		states[1] = 1.;
		states[2] = 0.;

		for(int i = 0; i < 4; ++i)
			alt_states[i] = states[i];
		alt_states[1] = -states[1];

		mix[0] = 0.;
		mix[1] = 0.;

		is_unpolarized = unp;

	//
	//	END OF USER-DEFINED OUTPUT
	//

	};

	Double_t operator() (Double_t *x, Double_t *par) {
		if(is_unpolarized){
			return par[0] * sin(x[0]) * (angdist.AngDist(x[0], x[1], states, nstates, mix) + angdist.AngDist(x[0], x[1], alt_states, nstates, mix));
		}
		return par[0] * sin(x[0]) * angdist.AngDist(x[0], x[1], states, nstates, mix);
	}

	double states[4];
	double alt_states[4];
	double mix[3];
	int nstates;
	bool is_unpolarized;
	
	AngularDistribution angdist;
};

int main(int argc, char* argv[]){

	const Double_t pi = TMath::Pi();
	
	struct arguments args;
	argp_parse(&argp, argc, argv, 0, 0, &args);

	cout << "#############################################" << endl;
	cout << "> AngularDistributionGenerator_Test" << endl;
	cout << "> TREENAME     : " << args.tree << endl;
	cout << "> FILES        : " << "*" << args.p1 << "*" << args.p2 << "*" << endl;
	cout << "> OUTPUTFILE   : " << args.outputfilename << endl;
	cout << "> NSTEPS       : " << args.nsteps<< endl;
	cout << "> FIT          : " << args.fit << endl;
	cout << "> UNPOLARIZED  : " << args.is_unpolarized << endl;
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

	// Create histogram
	
	Int_t nbins_phi = 50;
	Int_t nbins_theta = 50;
	Double_t phi_low = 0.;
	Double_t phi_up = pi;
	Double_t theta_low = 0.;
	Double_t theta_up = pi;

	vector<TH2F*> hist;
	stringstream sst;
	for(unsigned int i = 0; i < args.nsteps; ++i){
		sst.str("");	
		sst << "hist" << (i+1);
		hist.push_back(new TH2F(sst.str().c_str(), "Momentum distribution in (theta, phi)", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up));
		hist[i]->GetXaxis()->SetTitle("Theta");
		hist[i]->GetYaxis()->SetTitle("Phi");
		hist[i]->GetZaxis()->SetTitle("Angular distribution");
	}

	// Fill histogram from TBranch in TChain with user-defined conditions
	// Define variables and read their values from the tree using the GetEntry() method
	Double_t momentum_x, momentum_y, momentum_z;

	utr.SetBranchAddress("vx", &momentum_x);
	utr.SetBranchAddress("vy", &momentum_y);
	utr.SetBranchAddress("vz", &momentum_z);

	TVector3 vec(0., 0., 1.);

	for(int i = 0; i < utr.GetEntries(); i++){
		utr.GetEntry(i);

		vec.SetXYZ(momentum_x, momentum_y, momentum_z);

		// Need to distinguish between different sectors of the unit circle when filling the histogram
		hist[(long unsigned int) i%args.nsteps]->Fill(vec.Theta(), vec.Phi());
		//if(momentum_x >= 0. && momentum_y < 0.){
		//	hist[(long unsigned int) i%args.nsteps]->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), two_pi + atan(momentum_y/momentum_x));
		//}
		//else if(momentum_x >= 0. && momentum_y >= 0.){
		//	hist[(long unsigned int) i% args.nsteps]->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), atan(momentum_y/momentum_x));
		//	cout << acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))) << ", " << atan(momentum_y/momentum_x) << endl;
		//}
		//else if(momentum_x < 0. && momentum_y >= 0.){
		//	hist[(long unsigned int) i%args.nsteps]->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), pi + atan(momentum_y/momentum_x));
		//}
		//else if(momentum_x < 0. && momentum_y < 0.){
		//	hist[(long unsigned int) i%args.nsteps]->Fill(acos((momentum_z/sqrt(momentum_x*momentum_x + momentum_y*momentum_y + momentum_z*momentum_z))), pi + atan(momentum_y/momentum_x));
		//}
	}

	// Create TFile to write output
	TFile *of = new TFile(args.outputfilename, "RECREATE");

	// Fit the histogram
	
//	if(args.fit){
//		W_Function w_function(args.is_unpolarized);
//	
//		TF2 *ang_dist = new TF2("ang_dist", w_function, 0., pi, 0., two_pi, 1);
//		Double_t ang_dist_params[1] = { 1. };
//		ang_dist->SetParameters(ang_dist_params);
//
//		hist->Fit("ang_dist");
//		ang_dist->Write();
//
//		// Calculate absolute value of the residuals of the fit
//		
//		TH2F *residuals= new TH2F("residuals", "(absolute value of) Residuals of momentum distribution in (theta, phi)", nbins_theta, theta_low, theta_up, nbins_phi, phi_low, phi_up);
//
//		for(Int_t th = 0; th < nbins_theta; ++th){
//			for(Int_t ph = 0; ph < nbins_phi; ++ph){
//				residuals->SetBinContent(th, ph, abs(hist->GetBinContent(th, ph) - ang_dist->Eval(hist->GetXaxis()->GetBinCenter(th), hist->GetYaxis()->GetBinCenter(ph))));
//			}
//		}
//
//		residuals->Write();
//	}
	
	// Write histogram to a new TFile
	for(auto h: hist)
		h->Write();

	of->Close();

	cout << "> Created output file " << args.outputfilename << endl;
}
