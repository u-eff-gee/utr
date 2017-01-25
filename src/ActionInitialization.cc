#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"
#include "AngularDistributionGenerator.hh"
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
  	SetUserAction(new GeneralParticleSource);
//  	SetUserAction(new AngularDistributionGenerator);
  	SetUserAction(new RunAction);
}
