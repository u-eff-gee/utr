/// \file HPGe2.hh
/// \brief Header file of HPGe2

//**************************************************************//
//	60%  HPGe detector 2 @ HIGS Serial No. 36-TN30986A
//**************************************************************//

#ifndef HPGe2_h
#define HPGe2_h 1

#include "G4LogicalVolume.hh"

class HPGe2 {
  public:
	HPGe2(G4String Detector_Name);
	~HPGe2();

	G4LogicalVolume *Get_Logical() { return HPGe2_Logical; }
	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

  private:
	G4double Length;
	G4double Radius;

	G4LogicalVolume *HPGe2_Logical;
};

#endif
