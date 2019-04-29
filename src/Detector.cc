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

#include "Detector.hh"

Detector::Detector(G4LogicalVolume *World_Logical, G4String name):
	world_Logical(World_Logical),
	detector_name(name)
{};

void Detector::Add_Filter(G4String filter_material, G4double filter_thickness, G4double filter_radius){
	filter_materials.push_back(filter_material);
	filter_thicknesses.push_back(filter_thickness);
	filter_radii.push_back(filter_radius);
};

void Detector::Add_Wrap(G4String wrap_material, G4double wrap_thickness){
	wrap_materials.push_back(wrap_material);
	wrap_thicknesses.push_back(wrap_thickness);
};
