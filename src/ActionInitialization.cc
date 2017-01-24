#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
}
