#pragma once

#include "G4LogicalVolume.hh"

#include "HPGe_Collection.hh"
#include "HPGe_Coaxial.hh"
#include "BGO.hh"

class Detectors_80
{
public:
	Detectors_80(G4LogicalVolume *World_Log);
	~Detectors_80(){};
	void Construct(G4ThreeVector global_coordinates);
	void ConstructDetectorFilter(G4ThreeVector global_coordinates, std::string det, G4double CuLength, G4double PbLength);

	BGO BGO1;
	BGO BGO2;
	BGO BGOPol;
private:
	G4LogicalVolume *World_Logical;

	G4double g80_theta = 90. * deg;
	G4double g80_phi = 0. * deg;

	G4double g2_theta = 90 * deg;
	G4double g2_phi = 140 * deg;

	G4double gPol_theta = 90. * deg;
	G4double gPol_phi = 180. * deg;

	G4double detectordistance80 = 225. * mm;
	G4double detectordistance2 = 225. * mm;
	G4double detectordistancePol = 225. * mm;
};
