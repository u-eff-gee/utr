/// \file HPGe4.hh
/// \brief Header file of HPGe4

//**************************************************************//
//	60%  HPGe detector 4 @ HIGS Serial No. 36-TN21033A
//**************************************************************//

#ifndef HPGe4_h
#define HPGe4_h 1

#include "G4LogicalVolume.hh"

class HPGe4 {
  public:
	HPGe4(G4String Detector_Name);
	~HPGe4();

	G4LogicalVolume *Get_Logical() { return HPGe4_Logical; }
	G4double Get_Length() { return Length; }
	G4double Get_Radius() { return Radius; }

  private:
	G4double Length;
	G4double Radius;

	G4LogicalVolume *HPGe4_Logical;
};

#endif
