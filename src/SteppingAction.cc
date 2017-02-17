#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4LogicalVolume.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
	G4cout << step->GetPreStepPoint()
	              ->GetPhysicalVolume()
	              ->GetLogicalVolume()
	              ->GetName()
	       << G4endl;
}
