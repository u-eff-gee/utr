#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

#define MAXNBRANCHES 20

void RootToTxt(string filename){

	// Open ROOT file
	TFile *f = new TFile(filename.c_str());
	cout << "Opened ROOT file " << filename << endl;

	// Get number of TTree objects in the TFile (hopefully only one)
	Int_t nkeys = f->GetNkeys();
	if(nkeys < 1){
		cout << "Error: File does not contain any object to read from." << endl;
		exit();
	}
	if(nkeys > 1)
		cout << "Warning: File contains more than one object. This is not expected for a normal simulation output." << endl;

	// Open TTree #0
	
	string treename = f->GetListOfKeys()->At(0)->GetName(); 
	TTree *t = (TTree*) f->Get(treename.c_str());

	cout << "Reading from TTree " << t->GetName() << endl;

	// Get number of TBranch objects in the TTree, output their names and put them in a list
	Int_t nbranches = t->GetNbranches();
	if(nbranches > MAXNBRANCHES)
		cout << "Warning: TTree contains more than " << MAXNBRANCHES << "TBranch objects, change the variable MAXNBRANCHES in the code appropriately." << endl;

	TBranch* branches[MAXNBRANCHES];
	cout << "TTree contains " << nbranches << " TBranches:" << endl;
	for(int i = 0; i < nbranches; i++){
		cout << (i + 1) << ".\t"  << t->GetListOfBranches()->At(i)->GetName() << endl;
		branches[i] = (TBranch*) t->GetListOfBranches()->At(i);
	}

	// Open output file
	stringstream outputfilename;
	outputfilename << filename.substr(0, filename.length() - 5);
	outputfilename << ".txt";

	ofstream of;
	of.open(outputfilename.str().c_str());

	cout << "Opened output file " << outputfilename.str() << endl;

	// Read out the content of TBranch objects and write it to a text file
	Double_t b[MAXNBRANCHES];

	for(int i = 0; i < nbranches; i++){
		t->SetBranchAddress(branches[i]->GetName(), &b[i]);
	}

	double percent;
	double nextpercent = 10;
	long tstart = time(0);

	for(int i = 0; i < t->GetEntries(); i++){
		t->GetEntry(i);
		for(int j = 0; j < nbranches; j++){
			of << std::scientific << std::setprecision(6) << b[j] << "\t";
		}
		of << endl;

		percent = (i+1)/((double) t->GetEntries())*100.;
		if(percent >= nextpercent){
                      	nextpercent = nextpercent + 10;
                      
		      	cout << "[";
			for(int k = 1; k <= ((int) percent) / 10; k++)
			      	cout << "=" ;
			for(int k = ((int) percent / 10) +1; k <= 10; k++)
				cout << " ";

			cout << "], " <<  time(0) - tstart << "s" << endl;
		}
	}

	of.close();

	cout << "Close output file " << outputfilename.str() << endl;
}

