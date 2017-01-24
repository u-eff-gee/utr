#ifndef LABR1_HH
#define LABR1_HH

#include "G4LogicalVolume.hh"

class LaBr_TUD
{
public:
  	LaBr_TUD(G4String Detector_Name);
  	~LaBr_TUD();

  	G4LogicalVolume* Get_Logical() { return detectorLogical; }
	G4double Get_Radius() { return Radius; }
	G4double Get_Length() { return Length; }

private:
	G4double Length;
	G4double Radius;

  	G4LogicalVolume* detectorLogical;

  	G4double lengthCrystal;
  	G4double radiusCrystal;
  	G4double thicknessHousing;
  	G4double radiusHousing;
  	G4double thicknessVacuumFront;
  	G4double thicknessVacuumBack;
};

#endif
