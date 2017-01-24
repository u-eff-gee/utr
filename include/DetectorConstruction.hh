#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

	const G4double inch = 0.0254*m;

class DetectorConstruction: public G4VUserDetectorConstruction{
public:
  	DetectorConstruction();
  	~DetectorConstruction();

  	virtual G4VPhysicalVolume* Construct();
  	virtual void ConstructSDandField();
  
};


#endif
