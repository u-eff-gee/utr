#ifndef RADIATORTARGET_HH
#define RADIATORTARGET_HH 1

#include "G4LogicalVolume.hh"

class RadiatorTarget
{
public:
	RadiatorTarget(G4double target_z, G4String radiator_material, G4String target_name, G4double attenuator_z, G4String attenuator_material);	
	~RadiatorTarget(){};
	G4LogicalVolume *Get_Logical(){ return radiatorTarget; };
	G4double Get_Z(){ return radiator_Mother_z; };
	G4double Get_Window_Position(){ return radiator_Window_Position; };

private:
	G4LogicalVolume *radiatorTarget;

	G4double target_thickness;
	G4double radiator_Mother_x;
	G4double radiator_Mother_y;
	G4double radiator_Mother_z;
	G4double radiator_Window_Position;
};

#endif
