#include <chrono>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void getHistogramRDF(
	string directory = ".",
	string outputfilename="hist.root",
	vector<string> filename_identifiers = {"utr", ".root"},
	vector<string> conditions = {
		"volume==0",
		"volume==1",
		"volume==2",
		"volume==3",
		"volume==4",
		"volume==5",
		"volume==6",
		"volume==7",
		"volume==8",
		"volume==9",
		"volume==10",
		"volume==11",
		"volume==12",
	},
	vector<string> histogramnames = {
		"det0",
		"det1",
		"det2",
		"det3",
		"det4",
		"det5",
		"det6",
		"det7",
		"det8",
		"det9",
		"det10",
		"det11",
		"det12",
	},
	string general_condition="true",
	double xlow = 0.0005,
	double xup = 10.0005,
	unsigned int nbins = 10000,
	string tree="utr"){

	high_resolution_clock::time_point t_start = high_resolution_clock::now();

	if(conditions.size() != histogramnames.size()){
		cout << "> Error: Number of conditions and histogram names do not match. Aborting ..." << endl;
		abort();
	}
	
	ROOT::EnableImplicitMT();

	cout << "> Looking for files whose names contain the strings ";
	for(auto fid : filename_identifiers){
		cout << "'" << fid << "' ";
	}
	cout << "in the directory '" << directory << "/' ..." << endl;
	TSystemDirectory dir("directory", directory.c_str());

	TList *files = dir.GetListOfFiles();
	TChain chain(tree.c_str());

	bool skip = false;

	if(files){
		TSystemFile *file;
		TString fname;

		TIter next(files);
		file = (TSystemFile*) next();
		while (file){
			skip = false;
			fname = file->GetName();
			if(!file->IsDirectory()){
				for(auto fid : filename_identifiers){
					if(!fname.Contains(fid)){
						skip = true;
						break;
					}

				}
				if(!skip){
					cout << "\tAdding " << directory << "/" << fname << endl;
					chain.Add(directory+"/"+fname);
				}
			}
			file = (TSystemFile*) next();
		}
	}

	if(chain.GetEntries() == 0){
		cout << "> Error: No file found which contains all of the given strings. Aborting ..." << endl;
		abort();
	}

	vector<ROOT::RDF::RResultPtr<::TH1D>> hist;
	ROOT::RDataFrame df(chain);
	df.Filter("edep==4.");

	stringstream histname, histtitle, volume_condition;

	cout << endl << "> Filling the following histograms:" << endl;
	cout << "\tGENERAL CONDITION : " << general_condition << endl;
	cout << "\tNAME : CONDITION" << endl;
	for(unsigned int i = 0; i < conditions.size(); ++i){
		cout << "\t" << histogramnames[i] << " : '" << conditions[i] << "'" << endl;
		hist.push_back(df.Filter(general_condition.c_str()).Filter(conditions[i].c_str()).Histo1D({histogramnames[i].c_str(), conditions[i].c_str(), (int) nbins, xlow, xup}, "edep"));

		histname.str("");
		histtitle.str("");
	}

	cout << endl << "> Writing output to '" << outputfilename << "' ..." << endl;
	cout << "\tNAME : ENTRIES" << endl;
	TFile outputfile(outputfilename.c_str(), "RECREATE"); 
	for(unsigned int i = 0; i < conditions.size(); ++i){
		cout << "\t" << histogramnames[i] << " : " << hist[i]->GetEntries() << endl;
		hist[i]->Write();
	}
	outputfile.Close();

	duration<double> delta_t = duration_cast<duration<double>>(high_resolution_clock::now()-t_start);
	high_resolution_clock::time_point t_stop = high_resolution_clock::now();
	cout << "> Execution took " << delta_t.count() << " seconds." << endl;
}
