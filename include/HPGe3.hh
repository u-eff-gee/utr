/// \file HPGe3.hh
/// \brief Header file of HPGe3

//**************************************************************//
//	60%  HPGe detector 3 @ HIGS Serial No. 36-TN40663A
//**************************************************************//

#ifndef HPGe3_h
#define HPGe3_h 1

#include "G4LogicalVolume.hh"

class HPGe3 {
  public:
	HPGe3(G4String Detector_Name);
	~HPGe3();

	G4LogicalVolume *Get_Logical() { return HPGe3_Logical; }
	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

	G4double Length;
	G4double Radius;

  private:
	G4LogicalVolume *HPGe3_Logical;
};

#endif
