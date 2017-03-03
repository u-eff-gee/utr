void HistogramToTxt(TString filename){

	// Open TFile
	TFile *f = new TFile(filename);

	Int_t nkeys = f->GetNkeys();
		
	if(nkeys < 1){
		cout << "Error: File does not contain any object to read from." << endl;
	        exit();
	}

	// Loop over all keys (hopefully all of them are TH1 histograms) and write their content to separate output files
	stringstream outputfilename;
	ofstream of;

	TH1* hist;
	Int_t nbins;

	for(Int_t i = 0; i < nkeys; i++){
		TString histogramname = f->GetListOfKeys()->At(i)->GetName();

		cout << "Converting histogram " << histogramname << " ..." << endl;

		outputfilename << histogramname << ".txt"; 
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
}
