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

/*
 * Sample script to process output of a PhotonFlux simulation.
 * The script uses a '*' wild card to process all files with similar names in the directory.
 *
 * At the moment, it creates 4 histograms for each target layer.
 * The four histograms apply different filters to the recorded events.
 *
 * 1) 'hist_ekin_particle_gate': Histogram of the kinetic energies of all photons in the current layer.
 * 	This condition is filters out unwanted particles.
 * 2) 'hist_ekin_particle_z_gate': Histogram of the kinetic energies of all photons in the current layer which were recorded at the surface (area of the target which is supposed to be hit first by a beam propagation along the z axis).
 * 	This condition filters out particles that enter the target from outside or backwards-propagating particles.
 * 3) 'hist_ekin_particle_vz_gate': Histogram of the kinetic energies of all photons in the current layer which propagate in forward-direction, i.e. whose momentum vector has a positive z component.
 * 	This condition filters out backward-propagating particles.
 * 4) 'hist_ekin_particle_vz_z_gate': Combination of (2) and (3).
 * 	In fact, condition (2) should include all particles which fulfil (3).
 * 	A highly improbable case where condition (3) applies, but 2 does not, would be when a particle is created exactly at the surface, but propagates backwards.
 *
 */

#include <chrono>

// The following lines should match the configuration of PhotonFlux/DetectorConstruction.cc
const size_t n_target_layers = 100;
const double target_length = 100.; // Standard unit of Geant4 for lengths is mm
const double target_radius = 10.;

using std::chrono::duration;
using std::chrono::high_resolution_clock;

void ekin_hist() {
  high_resolution_clock::time_point t_start = high_resolution_clock::now();

  ROOT::EnableImplicitMT();
  ROOT::RDataFrame df("utr", "*.root");

  stringstream hist_ekin_particle_gate_name, hist_ekin_particle_gate_filter,
      hist_ekin_particle_z_gate_name, hist_ekin_particle_z_gate_filter,
      hist_ekin_particle_vz_gate_name, hist_ekin_particle_vz_gate_filter,
      hist_ekin_particle_vz_z_gate_name, hist_ekin_particle_vz_z_gate_filter;
  vector<ROOT::RDF::RResultPtr<TH1D>> hist_ekin_particle_gate(n_target_layers);
  vector<ROOT::RDF::RResultPtr<TH1D>> hist_ekin_particle_z_gate(n_target_layers);
  vector<ROOT::RDF::RResultPtr<TH1D>> hist_ekin_particle_vz_gate(n_target_layers);
  vector<ROOT::RDF::RResultPtr<TH1D>> hist_ekin_particle_vz_z_gate(n_target_layers);

  const double target_layer_length = target_length / n_target_layers;
  double current_z = 0.;

  for (size_t i = 0; i < n_target_layers; ++i) {
    current_z = -0.5 * target_length + i * target_layer_length;

    hist_ekin_particle_gate_name << "ekin_particle_gate" << i;
    hist_ekin_particle_gate_filter << "volume==" << i << " && particle == 22";
    hist_ekin_particle_gate[i] = df.Filter(hist_ekin_particle_gate_filter.str().c_str()).Histo1D({hist_ekin_particle_gate_name.str().c_str(), hist_ekin_particle_gate_name.str().c_str(), 8000, 0.0005, 8.0005}, "ekin");

    hist_ekin_particle_gate_name.str("");
    hist_ekin_particle_gate_filter.str("");

    hist_ekin_particle_z_gate_name << "ekin_particle_z_gate" << i;
    hist_ekin_particle_z_gate_filter << "volume==" << i << " && particle == 22 && z == " << current_z;
    hist_ekin_particle_z_gate[i] = df.Filter(hist_ekin_particle_z_gate_filter.str().c_str()).Histo1D({hist_ekin_particle_z_gate_name.str().c_str(), hist_ekin_particle_z_gate_name.str().c_str(), 8000, 0.0005, 8.0005}, "ekin");

    hist_ekin_particle_z_gate_name.str("");
    hist_ekin_particle_z_gate_filter.str("");

    hist_ekin_particle_vz_gate_name << "ekin_particle_vz_gate" << i;
    hist_ekin_particle_vz_gate_filter << "volume==" << i << " && particle == 22 && vz > 0.";
    hist_ekin_particle_vz_gate[i] = df.Filter(hist_ekin_particle_vz_gate_filter.str().c_str()).Histo1D({hist_ekin_particle_vz_gate_name.str().c_str(), hist_ekin_particle_vz_gate_name.str().c_str(), 8000, 0.0005, 8.0005}, "ekin");

    hist_ekin_particle_vz_gate_name.str("");
    hist_ekin_particle_vz_gate_filter.str("");

    hist_ekin_particle_vz_z_gate_name << "ekin_particle_vz_z_gate" << i;
    hist_ekin_particle_vz_z_gate_filter << "volume==" << i << " && particle == 22 && vz > 0. && z == " << current_z;
    hist_ekin_particle_vz_z_gate[i] = df.Filter(hist_ekin_particle_vz_z_gate_filter.str().c_str()).Histo1D({hist_ekin_particle_vz_z_gate_name.str().c_str(), hist_ekin_particle_vz_z_gate_name.str().c_str(), 8000, 0.0005, 8.0005}, "ekin");

    hist_ekin_particle_vz_z_gate_name.str("");
    hist_ekin_particle_vz_z_gate_filter.str("");
  }
  TFile *f = new TFile("processed.root", "RECREATE");
  for (auto h : hist_ekin_particle_gate)
    h->Write();
  for (auto h : hist_ekin_particle_z_gate)
    h->Write();
  for (auto h : hist_ekin_particle_vz_gate)
    h->Write();
  for (auto h : hist_ekin_particle_vz_z_gate)
    h->Write();

  high_resolution_clock::time_point t_stop = high_resolution_clock::now();
  duration<double> delta_t = t_stop - t_start;
  cout << "Execution took " << delta_t.count() << " s" << endl;
}
