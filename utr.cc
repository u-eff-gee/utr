/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Physics.hh"
#include "RunAction.hh"

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

#include <argp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

using namespace std;

const char *argp_program_version = "1.0.0";
const char *argp_program_bug_address = "<ugayer@ikp.tu-darmstadt.de>";
static char doc[] = "GEANT4 simulation of the UTR at HIGS";
static char args_doc[] = "MACROFILE";
static struct argp_option options[] = {
    {"macrofile", 'm', "MACRO", 0, "Macro file", 0},
    {"nthreads", 't', "THREAD", 0, "Number of threads", 0},
    {"outputdir", 'o', "OUTPUTDIR", 0, "Output directory", 0},
    {0, 0, 0, 0, 0, 0}};

struct arguments {
	int nthreads = 1;
	char *macrofile = 0;
	char *outputdir = 0;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = (struct arguments *)state->input;
	switch (key) {
	case 't':
		arguments->nthreads = atoi(arg);
		break;
	case 'm':
		arguments->macrofile = arg;
		break;
	case 'o':
		arguments->outputdir = arg;
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {

	struct arguments arguments;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	// 'Real' random results
	time_t timer;
	G4Random::setTheSeed(time(&timer));
// Deterministic results
// G4Random::setTheSeed(1);

#ifdef G4MULTITHREADED
	G4MTRunManager *runManager = new G4MTRunManager;
	runManager->SetNumberOfThreads(arguments.nthreads);
#else
	G4RunManager *runManager = new G4RunManager;
#endif

	G4cout << "Initializing DetectorConstruction..." << G4endl;
	runManager->SetUserInitialization(new DetectorConstruction);

	G4cout << "Initializing PhysicsList..." << G4endl;
	Physics *physicsList = new Physics();
	runManager->SetUserInitialization(physicsList);

	G4cout << "ActionInitialization..." << G4endl;
	ActionInitialization *actionInitialization = new ActionInitialization();
	// Pass output directory to RunAction via ActionInitialization
	if (arguments.outputdir) {
		actionInitialization->setOutputDir(arguments.outputdir);
		if (!opendir(arguments.outputdir)) {
			stringstream command;
			const int dir_err = mkdir(arguments.outputdir,
			                          S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (dir_err == -1) {
				G4cout << "Error creating output directory" << G4endl;
				abort();
			}
		} else {
			G4cout << __FILE__ << ": main(): Warning: Output directory '"
			       << arguments.outputdir << "' already exists" << G4endl;
		}
	} else {
		actionInitialization->setOutputDir(".");
	}
	runManager->SetUserInitialization(actionInitialization);

	if (!arguments.macrofile) {
		G4cout << "Initializing VisManager" << G4endl;
		G4VisManager *visManager = new G4VisExecutive;
		visManager->Initialize();
	}

	G4UImanager *UImanager = G4UImanager::GetUIpointer();

	if (arguments.macrofile) {
		G4cout << "Executing macro file " << arguments.macrofile << G4endl;
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + arguments.macrofile);
	} else {
		G4cout << "Starting UI mode..." << G4endl;
		G4UIExecutive *ui = 0;

#ifdef G4UI_USE_QT
		ui = new G4UIExecutive(argc, argv, "qt");
#else
		ui = new G4UIExecutive(argc, argv);
#endif

		UImanager->ApplyCommand("/control/execute init_vis.mac");

		ui->SessionStart();
		delete ui;
	}

	if (G4VisManager::GetConcreteInstance())
		delete G4VisManager::GetConcreteInstance();

	delete runManager;
	return 0;
}
