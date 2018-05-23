/* utr - Geant4 simulation of the UTR at HIGS
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

#ifndef BEAMPIPE_UPSTREAM_HH 
#define BEAMPIPE_UPSTREAM_HH 1

#include "G4LogicalVolume.hh"

class Beampipe_Upstream{
public:
	Beampipe_Upstream(G4double relative_density);
	~Beampipe_Upstream(){};

	G4LogicalVolume *Get_Logical(){ return Beampipe_Upstream_Logical; }
	
	G4double Get_Length(){ return Beampipe_Upstream_Length; };
	G4double Get_Z_Axis_Offset_Z(){ return Z_Axis_Offset_Z; };

private:
	G4double Beampipe_Upstream_Length;
	G4double Z_Axis_Offset_Z;

	G4LogicalVolume *Beampipe_Upstream_Logical;
};

#endif
