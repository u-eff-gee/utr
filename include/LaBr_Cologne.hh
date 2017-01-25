#ifndef LABR2_HH
#define LABR2_HH

#include "G4Material.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class LaBr_Cologne {
  public:
	LaBr_Cologne(G4String Detector_Name);
	~LaBr_Cologne();

	G4LogicalVolume *Get_Logical() { return detectorLogical; }
	G4double Get_Radius() { return Radius; };
	G4double Get_Length() { return Length; };

  private:
	G4LogicalVolume *detectorLogical;

	G4double Radius;
	G4double Length;

	G4double lengthCrystal;
	G4double radiusCrystal;
	G4double thicknessHousing;
	G4double radiusHousing;
	G4double lengthHousing;
	G4double lengthEndcap;
	G4double radiusEndcap;
	G4double innerRadiusEndcap;
	G4double thicknessEndcap;
	G4double thicknessFoam;
	G4double thicknessVacuumFront;
};

#endif
