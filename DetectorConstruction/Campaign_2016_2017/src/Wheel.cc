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
#include <sstream>

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Units.hh"
#include "Wheel.hh"

Wheel::Wheel(G4LogicalVolume *World_Log) : World_Logical(World_Log),
                                           Wheel_Thickness(0.) {
  Ring_Thickness = {0.75 * inch, 0.5 * inch, 0.5 * inch, 1. * inch, 0.5 * inch, 0.75 * inch, 1. * inch};
  for (auto t : Ring_Thickness) {
    Wheel_Thickness += t;
  }
}

void Wheel::Construct(G4ThreeVector global_coordinates) {

  G4Colour grey(0.5, 0.5, 0.5);

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

  G4double Hole_Radius = 0.5 * 16. * inch;

  // Ring numbers go from upstream to downstream

  std::vector<G4double> Ring_Radius = {0.5 * 19. * inch, 0.5 * 33.5 * inch, 0.5 * 25.5 * inch, 0.5 * 17. * inch, 0.5 * 25.5 * inch, 0.5 * 20.5 * inch, 0.5 * 22. * inch};

  // Construct the rings
  G4Tubs *Wheel_Ring_Solid;
  G4LogicalVolume *Wheel_Ring_Logical;

  std::stringstream solidName;
  std::stringstream logicalName;
  std::stringstream physicalName;

  G4double pos_z = -Wheel_Thickness * 0.5;

  for (unsigned int i = 0; i < Ring_Radius.size(); ++i) {
    solidName << "Wheel_Ring_" << i << "_Solid";
    Wheel_Ring_Solid = new G4Tubs(solidName.str(), Hole_Radius, Ring_Radius[i], Ring_Thickness[i] * 0.5, 0., twopi);

    logicalName << "Wheel_Ring_" << i << "_Logical";
    Wheel_Ring_Logical = new G4LogicalVolume(Wheel_Ring_Solid, Al, logicalName.str());
    Wheel_Ring_Logical->SetVisAttributes(grey);

    physicalName << "Wheel_Ring_" << i;
    pos_z += Ring_Thickness[i] * 0.5;
    new G4PVPlacement(0, global_coordinates + G4ThreeVector(0., 0., pos_z), Wheel_Ring_Logical, physicalName.str(), World_Logical, false, 0, false);
    pos_z += Ring_Thickness[i] * 0.5;

    solidName.str("");
    logicalName.str("");
    physicalName.str("");
  }
}
