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

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class LaBr_Cologne {
  public:
  LaBr_Cologne(G4String Detector_Name);
  ~LaBr_Cologne();

  G4LogicalVolume *Get_Logical() { return detectorLogical; }
  G4double Get_Radius() { return Radius; };
  G4double Get_Length() { return Length; };

  private:
  G4LogicalVolume *detectorLogical;

  G4double Radius;
  G4double Length;

  G4double lengthCrystal;
  G4double radiusCrystal;
  G4double thicknessHousing;
  G4double radiusHousing;
  G4double lengthHousing;
  G4double lengthEndcap;
  G4double radiusEndcap;
  G4double innerRadiusEndcap;
  G4double thicknessEndcap;
  G4double thicknessFoam;
  G4double thicknessVacuumFront;
};
