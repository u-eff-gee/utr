#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
	DetectorConstruction();
	~DetectorConstruction();

	virtual G4VPhysicalVolume *Construct();
	virtual void ConstructSDandField();
};

#endif
