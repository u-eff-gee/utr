//**************************************************************//
//	55%  HPGe Detector 2 @ HIGS Serial No. 41-TN21638A
//**************************************************************//

#ifndef HPGe2_55_h
#define HPGe2_55_h 1

#include "G4LogicalVolume.hh"

class HPGe2_55 {
  public:
	HPGe2_55(G4String Detector_Name);
	~HPGe2_55();

	G4LogicalVolume *Get_Logical() { return HPGe2_55_Logical; }

	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

  private:
	G4double Length;
	G4double Radius;

	G4LogicalVolume *HPGe2_55_Logical;
};

#endif
