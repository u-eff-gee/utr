/*
utr - Geant4 simulation of the UTR at HIGS
Copyright (C) 2017 the developing team (see README.md)

This file is part of utr.

utr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

utr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with utr.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "G4Material.hh"
#include "Units.hh"

class Materials {

  private:
	~Materials();

  public:
	Materials();

	static Materials *Instance() {
		if (instance == nullptr) {
			instance = new Materials();
		}
		return instance;
	}

	static Materials *instance;

	void ConstructMaterials();

	// Get Materials
	G4Material *Get_target_TiO2() { return target_TiO2; }
	G4Material *Get_target_Se() { return target_Se; }
	G4Material *Get_target_Sn() { return target_Sn; }
	G4Material *Get_target_Kr() { return target_Kr; }
	G4Material *Get_target_Nd150() { return target_Nd150; }
	G4Material *Get_target_Sm152() { return target_Sm152; }
	G4Material *Get_target_Dy164() { return target_Dy164; }
	G4Material *Get_target_Dy164_2O3() { return target_Dy164_2O3; }
	G4Material *Get_target_Cr54_2O3() { return target_Cr54_2O3; }
	G4Material *Get_target_Pu240() { return target_Pu240; }
	G4Material *Get_target_Pu239() { return target_Pu239; }
	G4Material *Get_target_Th232() { return target_Th232; }
	G4Material *Get_target_U238() { return target_U238; }

	G4Material *Get_Stainless_Steel() { return stainlessSteel; }
	G4Material *Get_Brass() { return brass; }
	G4Material *Get_One_Third_Density_Brass() { return one_third_density_brass; }
	G4Material *Get_Half_Density_Al() { return half_density_Al; }
	G4Material *Get_One_Third_Density_Al() { return one_third_density_Al; }
	G4Material *Get_Peek() { return peek; }
	G4Material *Get_P10() { return P10; }
	G4Material *Get_CF4() { return CF4; }
	G4Material *Get_PCB_coating() { return PCB_coating; }

	G4Material *Get_Labr3() { return LaBr3; }
	G4Material *Get_LaBr3Ce() { return LaBr3Ce; }
	G4Material *Get_LaCl3Ce() { return LaCl3Ce; }

	G4Material *Get_Pump_Vacuum() { return pump_vacuum; }

  private:
	G4Material *target_TiO2;
	G4Material *target_Se;
	G4Material *target_Sn;
	G4Material *target_Kr;
	G4Material *target_Nd150;
	G4Material *target_Sm152;
	G4Material *target_Dy164;
	G4Material *target_Dy164_2O3;
	G4Material *target_Cr54_2O3;
	G4Material *target_Pu240;
	G4Material *target_Pu239;
	G4Material *target_Th232;
	G4Material *target_U238;

	G4Material *half_density_Al;
	G4Material *one_third_density_Al;
	G4Material *stainlessSteel;
	G4Material *brass;
	G4Material *one_third_density_brass;
	G4Material *peek;
	G4Material *P10;
	G4Material *CF4;
	G4Material *PCB_coating;

	G4Material *LaBr3;
	G4Material *LaBr3Ce;
	G4Material *LaCl3Ce;

	G4Material *pump_vacuum;
};
