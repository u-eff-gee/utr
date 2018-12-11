#ifndef RADIATORTARGET_HH
#define RADIATORTARGET_HH 1

#include "G4LogicalVolume.hh"

class RadiatorTarget
{
public:

	RadiatorTarget(G4LogicalVolume *World_Log);
	~RadiatorTarget(){};
	void Construct(G4ThreeVector global_coordinates,G4String target_name,G4String target_material,G4double target_thickness,G4String attenuator_material,G4double attenuator_thickness);
	G4ThreeVector GetBegin(){return G4ThreeVector(X_begin,Y_begin,Z_begin);}
	G4ThreeVector GetEnd(){return G4ThreeVector(X_end,Y_end,Z_end);}
	G4ThreeVector GetLength(){return G4ThreeVector(fabs(X_begin-X_end),fabs(Y_begin-Y_end),fabs(Z_begin-Z_end));}

private:
	G4LogicalVolume* World_Logical;

	G4double radiator_Mother_x;
	G4double radiator_Mother_y;
	G4double radiator_Mother_z;
	G4double radiator_Window_Position;

	G4double X_begin;
	G4double X_end;

	G4double Y_begin;
	G4double Y_end;
	
	G4double Z_begin;
	G4double Z_end;
};

#endif
