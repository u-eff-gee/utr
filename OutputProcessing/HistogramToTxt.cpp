#include <TROOT.h>
#include <TH1.h>
#include <TFile.h>
//#include <TApplication.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include<getopt.h>

using namespace std;

int main(int argc, char** argv){
  int c           = 0;
  TString filename;
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
        cout << "Usage: ./histogramToTxt -f FILENAME.root" << endl;
	cout << endl;
        cout << "A direct follow-up to getHistogram, HistogramToTxt.cpp takes a ROOT file that contains only 1D histograms (TH1 objects) and converts each histogram to a single text file. The script is used as follows:"<<endl;
	cout << endl;
	cout << "\t ./histogramToTxt -f FILENAME.root" << endl;
	cout << endl;
	cout << "Where FILENAME is the name of the ROOT file that contains TH1 objects." << endl;
	cout << "histogramToTxt outputs text files with the naming pattern HISTNAME_FILENAME.txt where HISTNAME is the name of the TH1 object and FILENAME the same as above."<<endl;
	cout << "The shell script loopHistogramToTxt.sh shows how to loop the script over a large number of files." <<endl;
	cout << "For further information: https://github.com/uga-uga/utr" <<endl;
        return 0;


        default:
        cerr << "Usage: ./histogramToTxt -f FILENAME.root" << endl;
	cerr << endl;
        cerr << "A direct follow-up to getHistogram, HistogramToTxt.cpp takes a ROOT file that contains only 1D histograms (TH1 objects) and converts each histogram to a single text file. The script is used as follows:"<<endl;
	cerr << endl;
	cerr << "\t ./histogramToTxt -f FILENAME.root" << endl;
	cerr << endl;
	cerr << "Where FILENAME is the name of the ROOT file that contains TH1 objects." << endl;
	cerr << "histogramToTxt outputs text files with the naming pattern HISTNAME_FILENAME.txt where HISTNAME is the name of the TH1 object and FILENAME the same as above."<<endl;
	cerr << "The shell script loopHistogramToTxt.sh shows how to loop the script over a large number of files." <<endl;
	cerr << "For further information: https://github.com/uga-uga/utr" <<endl;
	return -1;
    }
}
	//TApplication *app = new TApplication("app", 0, 0);


	// Open TFile
	TFile *f = new TFile(filename);

	Int_t nkeys = f->GetNkeys();
		
	if(nkeys < 1){
		cout << "Error: File does not contain any object to read from." << endl;
		return 0;
	}

	// Loop over all keys (hopefully all of them are TH1 histograms) and write their content to separate output files
	stringstream outputfilename;
	ofstream of;

	TH1* hist;
	Int_t nbins;

	for(Int_t i = 0; i < nkeys; i++){
		TString histogramname = f->GetListOfKeys()->At(i)->GetName();

		cout << "Converting histogram " << histogramname << " ..." << endl;

		outputfilename << histogramname << "_" << filename(0, filename.Length() - 5) << ".txt"; 
		of.open(outputfilename.str().c_str());
	
		hist = (TH1*) f->Get(histogramname);


		nbins = hist->GetNbinsX();

		for(Int_t j = 1; j <= nbins; j++){
			of << hist->GetBinCenter(j) << "\t" << hist->GetBinContent(j) << endl;
		}

		of.close();
		cout << "Output file " << outputfilename.str() << " created." << endl;
		outputfilename.str("");
	}

	return 0;
}
