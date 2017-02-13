#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <time.h>

#include "G4FileUtilities.hh"

RunAction::RunAction() : G4UserRunAction() {
	// Create analysis manager
	// The choice of analysis technique is done via selection of a namespace
	// in Analysis.hh
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->CreateNtuple("utr", "Particle information");
	analysisManager->CreateNtupleDColumn("ekin");
	analysisManager->CreateNtupleDColumn("edep");
	analysisManager->CreateNtupleDColumn("particle");
	analysisManager->CreateNtupleDColumn("volume");
	analysisManager->CreateNtupleDColumn("x");
	analysisManager->CreateNtupleDColumn("y");
	analysisManager->CreateNtupleDColumn("z");
	analysisManager->CreateNtupleDColumn("vx");
	analysisManager->CreateNtupleDColumn("vy");
	analysisManager->CreateNtupleDColumn("vz");

	analysisManager->FinishNtuple();
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run *) {
	// Get analysis manager
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	// Open an output file
	// Geant4 in Multithreading mode creates files with naming convention
	//
	// <filename>_t<threadId>.root
	//
	// where the filename is given by the user in analysisManager->OpenFile()

	G4FileUtilities *fu = new G4FileUtilities();
	std::stringstream sstr;
	std::stringstream sstr2;
	G4String fileNamePrefix = "utr";

	G4int threadId = G4Threading::G4GetThreadId();

	if (threadId !=
	    -1) { // threadId == -1 is the master thread which contains no data
		for (int i = 0; i <= 100; i++) {
			sstr.str("");
			sstr << fileNamePrefix << i << ".root";
			sstr2 << fileNamePrefix << i << "_t" << threadId << ".root";

			G4cout << "Checking file " << sstr2.str() << G4endl;

			if (fu->FileExists(sstr2.str())) {
				sstr.str("");
				sstr2.str("");
				continue;
			}

			break;
		}

		analysisManager->OpenFile(sstr.str());
		sstr.str("");
		sstr2.str("");

	} else {
		analysisManager->OpenFile("master.root");
	}
}

void RunAction::EndOfRunAction(const G4Run *) {
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();
}
