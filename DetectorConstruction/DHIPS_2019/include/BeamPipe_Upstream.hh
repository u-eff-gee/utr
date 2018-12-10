#ifndef BEAMPIPEUPSTREAM_HH
#define BEAMPIPEUPSTREAM_HH 1

#include "G4LogicalVolume.hh"


class BeamPipe_Upstream
{
public:

	BeamPipe_Upstream(G4LogicalVolume *World_Log);
	~BeamPipe_Upstream(){};
	void Construct(G4ThreeVector global_coordinates,G4double relative_density);
private:
	G4LogicalVolume* World_Logical;

	G4double radiator_Mother_x;
	G4double radiator_Mother_y;
	G4double radiator_Mother_z;
	G4double radiator_Window_Position;
};

#endif
