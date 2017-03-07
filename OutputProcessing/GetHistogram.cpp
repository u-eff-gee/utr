#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	
	// Command line arguments
	TString treename = argv[1]; // Name of ROOT tree which should exist in each of the files
	TString pattern1 = argv[2]; // File name pattern 1
	TString pattern2 = argv[3]; // File name pattern 2
	TString outputfilename = argv[4]; // Output file name

	// Find all files in the current directory that contain pattern1 and pattern1 and connect them to a TChain
	TSystemDirectory dir(".",".");

	TList *files = dir.GetListOfFiles();
	TChain utr("utr");

	cout << "Joining files that contain the patterns '" << pattern1 << "' and '" << pattern2 << "':" << endl;
	
	if(files){
		TSystemFile *file;
		TString fname;

		TIter next(files);
		while (file=(TSystemFile*) next()){

			fname = file->GetName();
			if(!file->IsDirectory() && fname.Contains(pattern1) && fname.Contains(pattern2)){
				cout << fname << endl;
				utr.Add(fname);
			}
		}
	}

	// Create histogram
	TH1* h1 = new TH1F("h1", "Energy Deposition in HPGe1 (only photons)", 10000, 0.0005, 10.0005);
	TH1* h2 = new TH1F("h2", "Energy Deposition in HPGe2 (only photons)", 10000, 0.0005, 10.0005);
	TH1* h3 = new TH1F("h3", "Energy Deposition in HPGe3 (only photons)", 10000, 0.0005, 10.0005);
	TH1* h4 = new TH1F("h4", "Energy Deposition in HPGe4 (only photons)", 10000, 0.0005, 10.0005);

	// Fill histogram from TBranch in TChain with user-defined conditions
	Double_t Edep, Volume, Particle;

	utr.SetBranchAddress("edep", &Edep);
	utr.SetBranchAddress("particle", &Particle);
	utr.SetBranchAddress("volume", &Volume);

	for(int i = 0; i < utr.GetEntries(); i++){
		utr.GetEntry(i);

		if(Edep > 0. && Particle == 22){
			if(Volume == 1)
				h1->Fill(Edep);
			if(Volume == 2)
				h2->Fill(Edep);
			if(Volume == 3)
				h3->Fill(Edep);
			if(Volume == 4)
				h4->Fill(Edep);
		}
		h2->Fill(Edep);
	}

	// Write histogram to a new TFile
	TFile *of = new TFile(outputfilename, "RECREATE");

	h1->Write();
	h2->Write();
	h3->Write();
	h4->Write();

	of->Close();

	cout << "Created output file " << outputfilename << endl;
}	
