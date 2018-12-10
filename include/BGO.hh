#ifndef BGO_HH
#define BGO_HH

#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"

class BGO
{
public:
  BGO(G4String name);
  ~BGO();

  G4LogicalVolume* Get_Logical() { return bgo_Mother_Logical; }
  G4UnionSolid* Get_Al_Case_Solid() { return al_Case_Solid; };

  // Length and radius of the BGO mother volume which just encloses all of the parts of the BGO
  G4double Get_Length(){ return bgo_Mother_Length; };
  G4double Get_Radius(){ return bgo_Mother_Radius; };
  // max_penetration depth denotes how far (measured from the backward end of the BGO) a detector can be pushed inside the BGO
  G4double Get_Max_Penetration_Depth(){ return max_penetration_depth; };


private:
  G4LogicalVolume* bgo_Mother_Logical;
  G4UnionSolid* al_Case_Solid;

  G4String bgo_Name;

  G4double bgo_Mother_Length;
  G4double bgo_Mother_Radius;
  G4double max_penetration_depth;
};

#endif
