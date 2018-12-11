#ifndef BEAMPIPEUPSTREAM_HH
#define BEAMPIPEUPSTREAM_HH 1

#include "G4LogicalVolume.hh"


class BeamPipe_Upstream
{
public:

	BeamPipe_Upstream(G4LogicalVolume *World_Log);
	~BeamPipe_Upstream(){};
	void Construct(G4ThreeVector global_coordinates,G4double relative_density);
	G4ThreeVector GetBegin(){return G4ThreeVector(X_begin,Y_begin,Z_begin);}
	G4ThreeVector GetEnd(){return G4ThreeVector(X_end,Y_end,Z_end);}
	G4ThreeVector GetLength(){return G4ThreeVector(fabs(X_begin-X_end),fabs(Y_begin-Y_end),fabs(Z_begin-Z_end));}
private:
	G4LogicalVolume* World_Logical;

	G4double exit_Window_Thickness = 0.1*mm; // Estimated
	G4double beamPipe_Inner_Radius = 19.*mm; // Estimated
	G4double beamPipe_Outer_Radius_Small = 21.25*mm;
	G4double beamPipe_Outer_Radius_Large = 35.*mm;
	G4double beamPipe_Small_Radius_Length = 100.*mm; // Estimated
	G4double beamPipe_Large_Radius_Length = 25.*mm;

	G4double X_begin=-beamPipe_Outer_Radius_Large*0.5;
	G4double X_end=X_begin+beamPipe_Outer_Radius_Large;

	G4double Y_begin=-beamPipe_Outer_Radius_Large*0.5;
	G4double Y_end=Y_begin+beamPipe_Outer_Radius_Large;
	
	G4double Z_begin=-0.5*beamPipe_Small_Radius_Length;
	G4double Z_end=Z_begin+beamPipe_Small_Radius_Length;
};

#endif
