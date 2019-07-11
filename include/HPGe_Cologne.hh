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

//**************************************************************//
//	Nominal 100% coaxial HPGe, 
//	Ortec serial number: 73954
//	Dimensions from g4horus Geant4 implementation of the
//	HORUS setup in Cologne (https://gitlab.ikp.uni-koeln.de/jmayer/g4horus/)
//	Published under the MIT license (see below)
//**************************************************************//
#pragma once

#include "G4LogicalVolume.hh"

class HPGe_Cologne {
  public:
	HPGe_Cologne(G4String Detector_Name);
	~HPGe_Cologne(){};

	G4LogicalVolume *Get_Logical() { return HPGe_Cologne_Logical; }

	G4double Get_Length() { return Length; };
	G4double Get_Radius() { return Radius; };
	G4double Get_Front_Radius() {return Front_Radius; };

  private:
	G4double Length;
	G4double Radius;
	G4double Front_Radius;

	G4LogicalVolume *HPGe_Cologne_Logical;
};

/*
The MIT License (MIT)
=====================

Copyright © `2015` `Jan Mayer`

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
