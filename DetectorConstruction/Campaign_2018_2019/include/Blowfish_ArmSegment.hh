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

#ifndef Blowfish_ArmSegment_HH
#define Blowfish_ArmSegment_HH 1

#include "G4LogicalVolume.hh"

class Blowfish_ArmSegment {
  public:
  Blowfish_ArmSegment();
  ~Blowfish_ArmSegment(){};

  void Construct(G4ThreeVector global_coordinates);

  // Gets a pointer to the logical volume
  inline G4LogicalVolume *GetLVPointer(void) { return theArmSegLogicalVolume; }

  // Gets the dimensions of the arm's container
  inline G4double GetArmWidth(void) { return contArmWidth; }
  inline G4double GetOuterRadius(void) { return outerRadius; }
  inline G4double GetInnerRadius(void) { return innerRadius; }

  private:
  G4LogicalVolume *theArmSegLogicalVolume;

  // dimensions of the arm's container
  G4double contArmWidth;
  G4double outerRadius;
  G4double innerRadius;
};

#endif
