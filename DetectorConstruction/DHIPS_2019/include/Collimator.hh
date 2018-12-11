#ifndef COLLIMATOR_HH
#define COLLIMATOR_HH 1

#include "G4LogicalVolume.hh"
#include <math.h>
// Collimator


class Collimator
{
public:

	Collimator(G4LogicalVolume *World_Log);
	~Collimator(){};
	void Construct(G4ThreeVector global_coordinates);
	G4ThreeVector GetVertex(){return G4ThreeVector(Collimator_Mother_x,Collimator_Mother_y,Collimator_Mother_z);}
	G4ThreeVector GetBegin(){return G4ThreeVector(X_begin,Y_begin,Z_begin);}
	G4ThreeVector GetEnd(){return G4ThreeVector(X_end,Y_end,Z_end);}
	G4ThreeVector GetLength(){return G4ThreeVector(fabs(X_begin-X_end),fabs(Y_begin-Y_end),fabs(Z_begin-Z_end));}
private:
	G4LogicalVolume* World_Logical;

	G4double Collimator_Mother_x;
	G4double Collimator_Mother_y;
	G4double Collimator_Mother_z;

	G4double block_x       = 300. * mm;  //x increases to the left from the perpective of the beam
	G4double block_small_x = 100. * mm;  //x increases to the left from the perpective of the beam
	G4double block_y       = 300. * mm;  //y increases towards the top
	G4double block_z       = 95. * mm;   //z lies in the direction of the beam

	G4double colholeradius_min=6. * mm;
	G4double colholeradius_max=10. * mm;
	G4double hole_radius;
	const G4int NBlocks = 10;

	G4double X_begin=-block_x*0.5;
	G4double X_end=X_begin+block_y;

	G4double Y_begin=-block_y*0.5;
	G4double Y_end=Y_begin+block_y;
	
	G4double Z_begin=-0.5*NBlocks*block_z;
	G4double Z_end=Z_begin+NBlocks*block_z;



};

#endif
