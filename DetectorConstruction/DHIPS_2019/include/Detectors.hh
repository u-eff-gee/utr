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
	void ConstructDetectorFilter(G4ThreeVector global_coordinates, std::string det, G4double CuLength, G4double PbLength);
private:
	G4LogicalVolume* World_Logical;

	G4double g1_theta=90.*deg;
  	G4double g1_phi=5.*deg;

	G4double g2_phi= 140*deg; 
	G4double g2_theta= 90*deg;

  	G4double gPol_theta=90.*deg;
  	G4double gPol_phi=180.*deg;

  	G4double detectordistance1 = 225.*mm;
	G4double detectordistance2 = 225.*mm;
	G4double detectordistancePol = 225.*mm;
};

#endif
