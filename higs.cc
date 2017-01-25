#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "DetectorConstruction.hh"
#include "Physics.hh"
//#include "AngularDistributionGenerator.hh"
//#include "GeneralParticleSource.hh"
#include "RunAction.hh"
#include "ActionInitialization.hh"

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include <argp.h>
#include <time.h>

using namespace std;

const char *argp_program_version = "0.1.0"; 
const char *argp_program_bug_address = "<ugayer@ikp.tu-darmstadt.de>"; 
static char doc[] = "GEANT4 simulation of Bremsstrahlung generation at the DHIPS site at the S-DALINAC.\vExecution without specified macro file will start UI mode."; 
static char args_doc[] = "MACROFILE"; 
static struct argp_option options[] = {     
    { "macrofile", 'm', "MACRO", 0, "Macro file"},        
    { "nthreads", 't', "THREAD", 0, "Number of threads"},               
    { 0, 0, 0, 0, 0, 0 } 
};

struct arguments {     
    int nthreads = 1;     
    char *macrofile = 0;      
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {     
    struct arguments *arguments = (struct arguments*)state->input;     
    switch (key) {     
         case 't':         
            arguments->nthreads = atoi(arg);
            break;
         case 'm':         
            arguments->macrofile = arg;     
            break;       
        default: return ARGP_ERR_UNKNOWN;  
    }      
    return 0; 
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char* argv[]){

  	struct arguments arguments;      
  	argp_parse(&argp, argc, argv, 0, 0, &arguments);
  
  	G4Random::setTheEngine(new CLHEP::RanecuEngine);
// 	time_t timer;
// 'Real' random results
//  	G4Random::setTheSeed(time(&timer));
// Deterministic results
  G4Random::setTheSeed(1);
  
#ifdef G4MULTITHREADED
  	G4MTRunManager* runManager = new G4MTRunManager;
  	runManager->SetNumberOfThreads(arguments.nthreads);
#else
  	G4RunManager* runManager = new G4RunManager;
#endif

  	G4cout << "Initializing DetectorConstruction..." << G4endl;
  	runManager->SetUserInitialization(new DetectorConstruction);

  	G4cout << "Initializing PhysicsList..." << G4endl;
  	Physics* physicsList = new Physics();
  	runManager->SetUserInitialization(physicsList);

  	G4cout << "ActionInitialization..." << G4endl;  
  	ActionInitialization* actionInitialization = new ActionInitialization();
  	runManager->SetUserInitialization(actionInitialization);

	if(!arguments.macrofile){
		G4cout << "Initializing VisManager" << G4endl;
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();
	}

  	G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  	if( arguments.macrofile ){
        	G4cout << "Executing macro file " << arguments.macrofile << G4endl;
        	G4String command = "/control/execute ";
        	UImanager->ApplyCommand(command+arguments.macrofile);
  	} else{
        	G4cout << "Starting UI mode..." << G4endl;
        	G4UIExecutive* ui = 0;
        	ui = new G4UIExecutive(argc, argv);
        	ui->SessionStart();
        	delete ui;
  }

	if(G4VisManager::GetConcreteInstance())
		delete G4VisManager::GetConcreteInstance();
  	

	delete runManager;
  	return 0;
}
