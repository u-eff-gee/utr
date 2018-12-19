#ifndef LEADCASTLE_HH
#define LEADCASTLE_HH 1

#include "G4LogicalVolume.hh"
#include <math.h>

#include "Germanium1_TUD.hh"
#include "Germanium2_TUD.hh"
#include "Polarimeter_TUD.hh"
#include "BGO.hh"

class LeadCastle
{
public:

	LeadCastle(G4LogicalVolume *World_Log);
	~LeadCastle(){};
	void Construct(G4ThreeVector global_coordinates);
	G4ThreeVector GetVertex(){return G4ThreeVector(LeadCastle_Mother_x,LeadCastle_Mother_y,LeadCastle_Mother_z);}
	// G4ThreeVector GetBegin(){return G4ThreeVector(X_begin,Y_begin,Z_begin);}
	// G4ThreeVector GetEnd(){return G4ThreeVector(X_end,Y_end,Z_end);}
	// G4ThreeVector GetLength(){return G4ThreeVector(fabs(X_begin-X_end),fabs(Y_begin-Y_end),fabs(Z_begin-Z_end));}
private:
	G4LogicalVolume* World_Logical;
	G4Box *LeadCastle_Mother_Solid;
	G4LogicalVolume *LeadCastle_Mother_Logical;

	G4double LeadCastle_Mother_x;
	G4double LeadCastle_Mother_y;
	G4double LeadCastle_Mother_z;

	G4NistManager *nist = G4NistManager::Instance();
	G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* Fe = nist->FindOrBuildMaterial("G4_Fe");
	G4Material *Cu= nist->FindOrBuildMaterial("G4_Cu");
	G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");

	G4Colour yellow      =G4Colour(1.0, 1.0, 0.0);
	G4Colour blue        =G4Colour(0.0, 0.0, 1.0);
	G4Colour grey        =G4Colour(0.5, 0.5, 0.5);
	G4Colour light_orange=G4Colour(1.0, 0.82, 0.36);

	G4double block_x       = 300. * mm;  //x increases to the left from the perpective of the beam
	G4double block_small_x = 100. * mm;  //x increases to the left from the perpective of the beam
	G4double block_y       = 300. * mm;  //y increases towards the top
	G4double block_z       = 95. * mm;   //z lies in the direction of the beam

	G4double detectordistance1 = 225.*mm;
	G4double detectordistance2 = 225.*mm;
	G4double detectordistancepol = 225.*mm;
	G4double distcollimatortotarget = 162*mm;

	G4double lengthCollimator=block_z*10;



	Germanium1_TUD* HPGe1;
	Germanium2_TUD* HPGe2;
	Polarimeter_TUD* HPGePol;
	BGO* BGO1;
	BGO* BGO2;
	BGO* BGOPol; 



	void ConsturctCollimator(G4ThreeVector local_coordinates);
	void ConsturctIronShield(G4ThreeVector local_coordinates);
	void ConsturctLeadShield(G4ThreeVector local_coordinates);



};

#endif
