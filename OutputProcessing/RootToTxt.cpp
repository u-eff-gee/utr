#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <sstream>

#include<getopt.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::setprecision;
using std::ofstream;

#define MAXNBRANCHES 20

int main(int argc, char* argv[]){
//void RootToTxt(string filename){
  int c           = 0;
  string filename;
  while (c > -1)
  {
      static struct option long_options[] =
      {
          {"-f",                  required_argument,          0, 'f'},
          {"help",                no_argument,                0, 'h'},
          {"help",                no_argument,                0, '?'}
      };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "f:h?",long_options, &option_index);

      switch (c)
      {
        case 0:
        
        break;

        case 'f':
        filename=optarg;
        break;
        
       

        case 'h':
        case '?':
        cout << "Usage: ./rootToTxt -f ROOTFILE" << endl;
	cout << endl;
        cout << "RootToTxt converts a ROOT output file (TFile) containing an n-tuple of data (a TTree with TBranch objects) to a simple text file with the same content. If you want to convert a ROOT file ROOTFILE, type"<<endl;
	cout << endl;
	cout << "\t  ./rootToTxt -f ROOTFILE" << endl;
	cout << endl;
	cout << "The ROOT file can have a TTree with an arbitrary name and an arbitrary number of TBranch objects." << endl;
	cout << "The output text file has the same name as the ROOT file but with a '.txt' suffix."<<endl;
	cout << "Be aware that conversion into text files increases the file size." <<endl;
	cout << "For further information: https://github.com/uga-uga/utr" <<endl;
        return 0;


        default:
        cerr << "Usage: ./rootToTxt -f ROOTFILE" << endl;
	cerr << endl;
        cerr << "RootToTxt converts a ROOT output file (TFile) containing an n-tuple of data (a TTree with TBranch objects) to a simple text file with the same content. If you want to convert a ROOT file ROOTFILE, type"<<endl;
	cerr << endl;
	cerr << "\t  ./rootToTxt -f ROOTFILE" << endl;
	cerr << endl;
	cerr << "The ROOT file can have a TTree with an arbitrary name and an arbitrary number of TBranch objects." << endl;
	cerr << "The output text file has the same name as the ROOT file but with a '.txt' suffix."<<endl;
	cerr << "Be aware that conversion into text files increases the file size." <<endl;
	cerr << "For further information: https://github.com/uga-uga/utr" <<endl;
	return -1;
    }
}

	// Open ROOT file
	TFile *f = new TFile(argv[1]);
	cout << "Opened ROOT file " << filename << endl;

	// Get number of TTree objects in the TFile (hopefully only one)
	Int_t nkeys = f->GetNkeys();
	if(nkeys < 1){
		cout << "Error: File does not contain any object to read from." << endl;
		abort();
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

