/// \file RunAction.cc
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include <time.h>

#include "G4FileUtilities.hh"

//G4int RunAction::EventID = 0;
//G4int RunAction::oldEventID = 0;
//G4int RunAction::nSecondaries = 0;

RunAction::RunAction()
 : G4UserRunAction()
{  
  // Create analysis manager
  // The choice of analysis technique is done via selection of a namespace
  // in Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	analysisManager->CreateNtuple("higs", "Particle information");
	analysisManager->CreateNtupleDColumn("ekin");  
	analysisManager->CreateNtupleDColumn("edep");
	analysisManager->CreateNtupleDColumn("type");
	analysisManager->CreateNtupleDColumn("volume");
  	analysisManager->CreateNtupleDColumn("x");
  	analysisManager->CreateNtupleDColumn("y");
  	analysisManager->CreateNtupleDColumn("z");
  	analysisManager->CreateNtupleDColumn("vx");
  	analysisManager->CreateNtupleDColumn("vy");
  	analysisManager->CreateNtupleDColumn("vz");

  	analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
  	delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  	// Get analysis manager
  	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  	// Open an output file
  	// Geant4 in Multithreading mode creates files with naming convention 
  	//
  	// <filename>_t<threadId>.root
  	//
  	// where the filename is given by the user in analysisManager->OpenFile()
  	// The following code ensures that existing files are not overwritten when
  	// /control/loop/.
  
  	G4FileUtilities* fu = new G4FileUtilities();
  	std::stringstream ss;
  	G4String fileNamePrefix = "higs";

  	G4int threadId = G4Threading::G4GetThreadId();

  	for(int i = 0; i <= 100; i++){
    		ss.str("");
    		ss << fileNamePrefix << i << "_t" << threadId << ".root";

    		if(!fu->FileExists(ss.str())){
      			ss.str("");
      			ss << fileNamePrefix << i;
      			break;      
    		}
  	}
  
  	analysisManager->OpenFile(ss.str());
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();

}
