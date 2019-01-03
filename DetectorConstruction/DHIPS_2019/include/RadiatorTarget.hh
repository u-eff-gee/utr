#ifndef RADIATORTARGET_HH
#define RADIATORTARGET_HH 1

#include "G4LogicalVolume.hh"

class RadiatorTarget
{
public:

	RadiatorTarget(G4LogicalVolume *World_Log);
	~RadiatorTarget(){};
	void Construct(G4ThreeVector global_coordinates,G4String target_name,G4String target_material,G4double target_thickness,G4String attenuator_material,G4double attenuator_thickness);

private:
	G4LogicalVolume* World_Logical;

	G4double radiator_Mother_x;
	G4double radiator_Mother_y;
	G4double radiator_Mother_z;
	G4double radiator_Window_Position;
};

#endif
