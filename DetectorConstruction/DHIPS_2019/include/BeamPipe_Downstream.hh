#ifndef BEAMPIPEDOWNSTREAM_HH
#define BEAMPIPEDOWNSTREAM_HH 1

#include "G4LogicalVolume.hh"


class BeamPipe_Downstream
{
public:

	BeamPipe_Downstream(G4LogicalVolume *World_Log);
	~BeamPipe_Downstream(){};
	void Construct(G4ThreeVector global_coordinates,G4double relative_density);
	G4ThreeVector GetBegin(){return G4ThreeVector(X_begin,Y_begin,Z_begin);}
	G4ThreeVector GetEnd(){return G4ThreeVector(X_end,Y_end,Z_end);}
	G4ThreeVector GetLength(){return G4ThreeVector(fabs(X_begin-X_end),fabs(Y_begin-Y_end),fabs(Z_begin-Z_end));}
private:
	G4LogicalVolume* World_Logical;

	G4double beamPipe_NRF_Window_Thickness = 3*mm;
	G4double beamPipe_NRF_Inner_Radius     = 0.5*54.*mm;
	G4double beamPipe_NRF_Outer_Radius     = 0.5*60.*mm;
	G4double beamPipe_NRF_Lenght           = (810.-180.)*mm;
	G4double SiDistanceToTarget            = 40.5*mm;

	G4double Chamber_Outer_Length = 180.*mm;
	G4double Chamber_Inner_Length = 150.*mm;


	G4double X_begin=-beamPipe_NRF_Outer_Radius*0.5;
	G4double X_end=X_begin+beamPipe_NRF_Outer_Radius;

	G4double Y_begin=-beamPipe_NRF_Outer_Radius*0.5;
	G4double Y_end=Y_begin+beamPipe_NRF_Outer_Radius;
	
	G4double Z_begin=-0.5*beamPipe_NRF_Lenght;
	G4double Z_end=Z_begin+beamPipe_NRF_Lenght;
};

#endif
