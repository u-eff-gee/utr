//**************************************************************//
//	120%  Zero Degree detector @ HIGS Serial No. 33-P40383A
//**************************************************************//

#ifndef ZeroDegree_h
#define ZeroDegree_h 1

#include "G4LogicalVolume.hh"

class ZeroDegree {
  public:
	ZeroDegree(G4String Detector_Name);
	~ZeroDegree();

	G4LogicalVolume *Get_Logical() { return ZeroDegree_Logical; }

	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };

  private:
	G4double Length;
	G4double Radius;

	G4LogicalVolume *ZeroDegree_Logical;
};

#endif
