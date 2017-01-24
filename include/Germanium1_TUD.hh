#ifndef GERMANIUM1_HH
#define GERMANIUM1_HH

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class Germanium1_TUD
{
public:
  	Germanium1_TUD(G4String Detector_Name);
  	~Germanium1_TUD();

  	G4LogicalVolume* Get_Logical() { return germanium1_Logical; }
  	G4double Get_Length(){ return Length; }
  	G4double Get_Radius(){ return Radius; }

private:
	G4double Length;
	G4double Radius;

  	G4LogicalVolume* germanium1_Logical;
};

#endif
