//**************************************************************//
//	55%  HPGe Detector 1 @ HIGS Serial No. 41-TN31524A
//**************************************************************//

#ifndef HPGe1_55_h
#define HPGe1_55_h 1

#include "G4LogicalVolume.hh"

class HPGe1_55
{
public:
  	HPGe1_55(G4String Detector_Name);
  	~HPGe1_55();
  
  	G4LogicalVolume* Get_Logical() { return HPGe1_55_Logical; }

	G4double Get_Length(){ return Length; };
	G4double Get_Radius(){ return Radius; };
  
private:
  	G4double Length;
  	G4double Radius;
  
  	G4LogicalVolume* HPGe1_55_Logical;
};

#endif
