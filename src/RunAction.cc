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
	// Default setting for output file
	for (unsigned int i = 0; i < n_output_flags; i++)
		output_flags[i] = 1;
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run *) {
	// Get analysis manager
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->CreateNtuple("utr", "Particle information");
	if (output_flags[EKIN])
		analysisManager->CreateNtupleDColumn("ekin");
	if (output_flags[EDEP])
		analysisManager->CreateNtupleDColumn("edep");
	if (output_flags[PARTICLE])
		analysisManager->CreateNtupleDColumn("particle");
	if (output_flags[VOLUME])
		analysisManager->CreateNtupleDColumn("volume");
	if (output_flags[POSX])
		analysisManager->CreateNtupleDColumn("x");
	if (output_flags[POSY])
		analysisManager->CreateNtupleDColumn("y");
	if (output_flags[POSZ])
		analysisManager->CreateNtupleDColumn("z");
	if (output_flags[MOMX])
		analysisManager->CreateNtupleDColumn("vx");
	if (output_flags[MOMY])
		analysisManager->CreateNtupleDColumn("vy");
	if (output_flags[MOMZ])
		analysisManager->CreateNtupleDColumn("vz");
	analysisManager->FinishNtuple();

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

	delete G4AnalysisManager::Instance();
}

void RunAction::SetOutputFlags(unsigned int *o_flags) {
	for (int i = 0; i < n_output_flags; i++)
		output_flags[i] = o_flags[i];
}

unsigned int *RunAction::GetOutputFlags() { return output_flags; }

G4String RunAction::GetOutputFlagName(unsigned int n) {
	switch (n) {
	case EKIN:
		return "EKIN";
	case EDEP:
		return "EDEP";
	case PARTICLE:
		return "PARTICLE";
	case VOLUME:
		return "VOLUME";
	case POSX:
		return "POSX";
	case POSY:
		return "POSY";
	case POSZ:
		return "POSZ";
	case MOMX:
		return "MOMX";
	case MOMY:
		return "MOMY";
	case MOMZ:
		return "MOMZ";
	default:
		G4cout << "RunAction: Error! Output flag index not found." << G4endl;
		return "";
	}
}
