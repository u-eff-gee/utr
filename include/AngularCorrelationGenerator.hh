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

#ifndef ANGULARCORRELATIONGENERATOR_HH
#define ANGULARCORRELATIONGENERATOR_HH 1

#include "G4Navigator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include <vector>

#include "AngularDistribution.hh"

#define CHECK_POSITION_GENERATOR 1
#define CHECK_MOMENTUM_GENERATOR 1
// Maximum value for the sampled w
#define MAX_W 3.

using std::vector;

class AngularCorrelationMessenger;

class AngularCorrelationGenerator : public G4VUserPrimaryGeneratorAction {
  public:
	AngularCorrelationGenerator();
	~AngularCorrelationGenerator();

	void GeneratePrimaries(G4Event *anEvent);

	// Set-methods to use with the AngularCorrelationMessenger
	
	void AddParticle(G4ParticleDefinition* particleDefinition){ 
		particles.push_back(particleDefinition); 
		particleEnergies.push_back(0.);
		relative_angle.push_back(0.);
		relative_angle_given.push_back(false);};
	void SetEnergy(G4double energy){ particleEnergies[particleEnergies.end() - particleEnergies.begin() - 1] = energy; };
	void SetDirection(G4ThreeVector vec){ direction = vec; direction_given = true; };
	void SetRelativeAngle(G4double relangle){ 
		relative_angle[relative_angle.end() - relative_angle.begin() - 1] = relangle;
		relative_angle_given[relative_angle_given.end() - relative_angle_given.begin() - 1] = true;
	};

	void AddNStates(G4int nst){ 
		nstates.push_back(nst); 
		states.push_back(vector<G4double>(4));
		alt_states.push_back(vector<G4double>(4));
		mixing_ratios.push_back(vector<G4double>(3));
	};
	void SetState(G4int n_state, G4double jpi){ 
		states[states.end() - states.begin() - 1][n_state] = jpi; 
		if(n_state == 1){
			alt_states[states.end() - states.begin() - 1][n_state] = -jpi;
		} else{
			alt_states[states.end() - states.begin() - 1][n_state] = jpi; 
		}
	};
	void SetDelta(G4int n_transition, G4double delta){ 
		mixing_ratios[mixing_ratios.end() - mixing_ratios.begin() - 1][n_transition] = delta; };
	void SetPolarized(G4bool pol){ is_polarized.push_back(pol); };

	void SetSourceX(G4double x) { source_x = x; };
	void SetSourceY(G4double y) { source_y = y; };
	void SetSourceZ(G4double z) { source_z = z; };

	void SetSourceDX(G4double dx) { range_x = dx; };
	void SetSourceDY(G4double dy) { range_y = dy; };
	void SetSourceDZ(G4double dz) { range_z = dz; };

	void AddSourcePV(G4String physvol) { source_PV_names.push_back(physvol); };

	// Get-methods to use with the AngularCorrelationMessenger
	
	G4ParticleDefinition* GetParticleDefinition() { 
		return particles[particles.end() - particles.begin() - 1]; };
	G4double GetParticleEnergy() { return 
		particleEnergies[particleEnergies.end()-
		particleEnergies.begin() - 1]; };
	G4ThreeVector GetDirection(){ return direction; };
	G4int GetNStates(){ return nstates[nstates.end() - nstates.begin() - 1]; };
	G4double GetState(int state_number) { 
		return states[states.end() - states.begin() - 1][state_number]; };
	G4double GetDelta(int transition_number) { 
		return mixing_ratios[mixing_ratios.end() - mixing_ratios.begin() - 1][transition_number]; };
	G4bool IsPolarized(){ return is_polarized[is_polarized.end() - is_polarized.begin() - 1]; };

	G4double GetSourceX() { return source_x; };
	G4double GetSourceY() { return source_y; };
	G4double GetSourceZ() { return source_z; };
	
	G4double GetSourceDX() { return range_x; };
	G4double GetSourceDY() { return range_y; };
	G4double GetSourceDZ() { return range_z; };

	G4String GetSourcePV(int i) { return source_PV_names[i]; };

  private:
	G4ParticleTable *particleTable;
	G4ParticleGun *particleGun;
	AngularCorrelationMessenger *angCorrMessenger;
	AngularDistribution *angdist;

	/*********************************************
	 *   Input from AngularCorrelationMessenger
	 *********************************************/

	// Position and dimensions of source volumes
	vector<G4String> source_PV_names;

	G4double source_x;
	G4double source_y;
	G4double source_z;

	G4double range_x;
	G4double range_y;
	G4double range_z;

	G4String pv;

	// Particle properties
	vector<G4ParticleDefinition*> particles;
	vector<G4double> particleEnergies;

	// Angular distribution properties
	G4ThreeVector direction;
	vector<G4double> relative_angle;

	vector<G4int> nstates;
	vector< vector<G4double> > states;
	vector< vector<G4double> > alt_states;
	vector< vector<G4double> > mixing_ratios;

	vector<G4bool> is_polarized;

	/*********************************************
	 *  Local variables 
	 *********************************************/

	G4double random_x;
	G4double random_y;
	G4double random_z;

	G4double random_theta;
	G4double random_phi;
	G4double random_w;

	G4Navigator *navi;

	G4double MAX_TRIES_POSITION;
	G4double MAX_TRIES_MOMENTUM;
	G4bool position_found;
	G4bool momentum_found;
	G4bool direction_given;
	vector<G4bool> relative_angle_given;

	G4bool checked_momentum_generator;
	G4bool checked_position_generator;
};

#endif
