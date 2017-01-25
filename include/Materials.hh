#ifndef Materials_h
#define Materials_h 1

#include "G4Material.hh"

class Materials {

  public:
	Materials();
	~Materials();

	void ConstructMaterials();

	// Get Materials
	G4Material *Get_target_TiO2() { return target_TiO2; }
	G4Material *Get_Labr3() { return LaBr3; }
	G4Material *Get_LaBr3Ce() { return LaBr3Ce; }
	G4Material *Get_LaCl3Ce() { return LaCl3Ce; }

  private:
	G4Material *target_TiO2;
	G4Material *LaBr3;
	G4Material *LaBr3Ce;
	G4Material *LaCl3Ce;
};

#endif
