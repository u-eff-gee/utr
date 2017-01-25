#ifndef POLARIMETER_HH
#define POLARIMETER_HH

#include "G4VUserDetectorConstruction.hh"

class Polarimeter_TUD {
  public:
	Polarimeter_TUD(G4String Detector_Name);
	~Polarimeter_TUD();

	G4LogicalVolume *Get_Logical() { return Polarimeter_TUD_Logical; }
	G4double Get_Length() { return Length; }
	G4double Get_Radius() { return Radius; }

  private:
	G4double Length;
	G4double Radius;
	G4LogicalVolume *Polarimeter_TUD_Logical;
};

#endif
