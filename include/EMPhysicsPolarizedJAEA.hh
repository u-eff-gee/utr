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

// Physics list largely based on G4EmLivermorePolarizedPhysics in Geant4 version 10.5
// Modified to include elastic scattering processes implemented by
// Omer and Hajima (see references below)
//
// ------------------------------------------------------------------------------------------
// Authors:
// M. Omer and R. Hajima  on   17 October 2016
// contact:
// omer.mohamed@jaea.go.jp and hajima.ryoichi@qst.go.jp
// Publication Information:
// 1- M. Omer, R. Hajima, Including Delbrück scattering in Geant4,
// Nucl. Instrum. Methods Phys. Res. Sect. B, vol. 405, 2017, pp. 43-49.,
// https://doi.org/10.1016/j.nimb.2017.05.028
// 2- M. Omer, R. Hajima, Geant4 physics process for elastic scattering of gamma-rays,
// JAEA Technical Report 2018-007, 2018.
// https://doi.org/10.11484/jaea-data-code-2018-007
// ------------------------------------------------------------------------------------------

#ifndef EMPhysicsPolarizedJAEA_h
#define EMPhysicsPolarizedJAEA_h 1

#include "G4VPhysicsConstructor.hh"
#include "G4EmParticleList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EMPhysicsPolarizedJAEA : public G4VPhysicsConstructor
{
public:

  explicit EMPhysicsPolarizedJAEA(G4int ver=1, const G4String& name="");

  virtual ~EMPhysicsPolarizedJAEA();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
  G4EmParticleList partList;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

