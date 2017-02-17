#include "TROOT.h"
#include "TFile.h"

#include <iostream>
#include <fstream>

using namespace std;

void RootToTxt(string filename){

	TFile *f = new TFile(filename.c_str());

	stringstream outputfilename;
	outputfilename << filename.substr(0, filename.length() - 5);
	outputfilename << ".txt";

	ofstream of;
	of.open(outputfilename.str().c_str());

	cout << "Opened root file " << filename << endl;

	Double_t Ekin, Edep, Particle, Volume, X, Y, Z, VX, VY, VZ;

	utr->SetBranchAddress("ekin", &Ekin);
	utr->SetBranchAddress("edep", &Edep);
	utr->SetBranchAddress("particle", &Particle);
	utr->SetBranchAddress("volume", &Volume);
	utr->SetBranchAddress("x", &X);
	utr->SetBranchAddress("y", &Y);
	utr->SetBranchAddress("z", &Z);
	utr->SetBranchAddress("vx", &VX);
	utr->SetBranchAddress("vy", &VY);
	utr->SetBranchAddress("vz", &VZ);
										        for(int i = 0; i <= utr->GetEntries(); i++){						utr->GetEntry(i);
												of << std::scientific << std::setprecision(6) << Ekin << "\t" << Edep << "\t" << Particle << "\t" << Volume << "\t" << X << "\t" << Y << "\t" << Z << "\t" << VX << "\t" << VY << "\t" << VZ << endl;
											}
		of.close();
											f->Close();
											 cout << "Created output file " << outputfilename.str() << endl;

}

