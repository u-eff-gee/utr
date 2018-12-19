#ifndef DETECTORS_HH
#define DETECTORS_HH 1

#include "G4LogicalVolume.hh"
#include <math.h>

#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "BGO.hh"

class Detectors
{
public:

	Detectors(G4LogicalVolume *World_Log);
	~Detectors(){};
	void Construct(G4ThreeVector global_coordinates);
private:
	G4LogicalVolume* World_Logical;
};

#endif
