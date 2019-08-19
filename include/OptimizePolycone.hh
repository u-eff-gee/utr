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

#include "G4SystemOfUnits.hh"

class OptimizePolycone {

  public:
	OptimizePolycone(){};

	G4int Optimize(double *zPlane, double *rInner, double *rOuter,
	               double *zPlaneOpt, double *rInnerOpt, double *rOuterOpt,
	               int nsteps, G4String PolyconeName) {

		G4double last_ri = rInner[0];
		G4double last_ro = rOuter[0];

		zPlaneOpt[0] = zPlane[0];
		rInnerOpt[0] = last_ri;
		rOuterOpt[0] = last_ro;

		G4int nsteps_optimized = 1;

		for (int i = 1; i < nsteps - 1; i++) {
			if (rInner[i] != last_ri || rOuter[i] != last_ro) {
				zPlaneOpt[nsteps_optimized] = zPlane[i - 1];
				rInnerOpt[nsteps_optimized] = rInner[i - 1];
				rOuterOpt[nsteps_optimized] = rOuter[i - 1];

				zPlaneOpt[nsteps_optimized + 1] = zPlane[i];
				rInnerOpt[nsteps_optimized + 1] = rInner[i];
				rOuterOpt[nsteps_optimized + 1] = rOuter[i];

				last_ri = rInner[i];
				last_ro = rOuter[i];

				nsteps_optimized += 2;
			}
		}

		zPlaneOpt[nsteps_optimized] = zPlane[nsteps - 1];
		rInnerOpt[nsteps_optimized] = rInner[nsteps - 1];
		rOuterOpt[nsteps_optimized] = rOuter[nsteps - 1];

		nsteps_optimized++;

		G4double reductionfactor =
		    (1. - (double)nsteps_optimized / nsteps) * 100.;
		G4double tolerance = (zPlane[nsteps - 1] - zPlane[0]) / nsteps;

		G4cout << "Polycone " << PolyconeName << " optimized." << G4endl;
		G4cout << "  Using " << reductionfactor
		       << " percent less points than uniform Polycone." << G4endl;
		G4cout << "  Geometrical resolution: " << tolerance / mm << "mm"
		       << G4endl;

		return nsteps_optimized;
	}
};
