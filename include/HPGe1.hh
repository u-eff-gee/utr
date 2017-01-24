//**************************************************************//
//	60%  HPGe detector 1 @ HIGS Serial No. 36-TN31061A
//**************************************************************//

#ifndef HPGe1_h
#define HPGe1_h 1

#include "G4LogicalVolume.hh"

class HPGe1
{
public:
  	HPGe1(G4String Detector_Name);
  	~HPGe1();
  
  	G4LogicalVolume* Get_Logical() { return HPGe1_Logical; }

	G4double Get_Length(){ return Length; };
	G4double Get_Radius(){ return Radius; };
  
private:
  	G4double Length;
  	G4double Radius;
  
  	G4LogicalVolume* HPGe1_Logical;
};

#endif
