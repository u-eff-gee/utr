#ifndef Materials_h
#define Materials_h 1

#include "G4Material.hh"

class Materials {

  private:
	Materials();
	~Materials();

	static Materials *instance;

  public:
	static Materials *Instance() {
		if (instance == NULL) {
			instance = new Materials();
		}
		return instance;
	}

	void ConstructMaterials();

	// Get Materials
	G4Material *Get_target_TiO2() { return target_TiO2; }
	G4Material *Get_target_Se() { return target_Se; }
	G4Material *Get_target_Sn() { return target_Sn; }
	G4Material *Get_target_Kr() { return target_Kr; }

	G4Material *Get_Stainless_Steel() { return stainlessSteel; }

	G4Material *Get_Labr3() { return LaBr3; }
	G4Material *Get_LaBr3Ce() { return LaBr3Ce; }
	G4Material *Get_LaCl3Ce() { return LaCl3Ce; }

	G4Material *Get_Pump_Vacuum() { return pump_vacuum; }

  private:
	G4Material *target_TiO2;
	G4Material *target_Se;
	G4Material *target_Sn;
	G4Material *target_Kr;

	G4Material *stainlessSteel;

	G4Material *LaBr3;
	G4Material *LaBr3Ce;
	G4Material *LaCl3Ce;

	G4Material *pump_vacuum;
};

#endif
