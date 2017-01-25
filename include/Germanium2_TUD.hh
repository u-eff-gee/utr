#ifndef GERMANIUM2_HH
#define GERMANIUM2_HH

#include "G4LogicalVolume.hh"

class Germanium2_TUD {
  public:
	Germanium2_TUD(G4String Detector_Name);
	~Germanium2_TUD();

	G4LogicalVolume *Get_Logical() { return germanium2_Logical; }
	G4double Get_Length() { return Length; }
	G4double Get_Radius() {
		return Radius;
		return Radius;
	}

  private:
	G4double Length;
	G4double Radius;
	G4LogicalVolume *germanium2_Logical;
};

#endif
