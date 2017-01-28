# Geant4 simulation of the UTR at HIγS

![Geometry of the UTR Geant4 simulation](.media/geometry.png)

This is a [Geant4](https://geant4.web.cern.ch/) simulation of the Upstream Target Room (UTR) at the High Intensity γ-ray Source (HIγS) facility [[1]](#ref-higs), located at the Duke Free Electron Laser Laboratory (DFELL) of the Triangle University Nuclear Laboratory (TUNL) at Duke University, Durham, NC, USA.


## Features

The following objects located in the UTR are included in the simulation:

* Collimator
* Lead shieldings and walls
* Concrete wall and blocks
* Evacuated beam pipe and pipe holders
* γ³ setup [[2]](#ref-g3), including the wheel
* 2ⁿᵈ setup (also called the parasitic setup)
* 0° detector
* HPGe detectors of the TUNL and TU Darmstadt
* LaBr₃(Ce) detectors of the TU Darmstadt
* Polarimeter of the TU Darmstadt
* Metal tables
* Air
* A variety of filters, shieldings and smaller objects

Several processes are taken into consideration during the simulation: 

* Compton and Rayleigh scattering
* pair production
* The photoelectric effect
* photo nuclear inelastic scattering
* electron bremsstrahlung, ionisation and (multiple) scattering 
* ionisation and (multiple) scattering of charged particles
* positron annihilation
* atomic deexcitation

The simulation is able to generate the following: *TODO*.


## Getting started

These instructions will get you a copy of the simulation running.

### Dependencies

To build and run the simulation, the following dependencies are required:

* [Geant4](https://geant4.web.cern.ch/)
* CMake (*build*)
* Make (*build*)

### Compilation

To compile the simulation, simply execute

```bash
$ cmake .
$ make -j
```

This will create the `utr` binary in the top directory.


## Usage

To open the simulation, run the compiled `utr` binary. The visualization can be initialized with

    /control/execute init_vis.mac

This command executes the `init_vis.mac` macro that enables you to view the geometry of the simulation.
The geometry is currently defined in the `src/DetectorConstruction.cc` file.
It should be adjusted to reflect the actual configuration that was in place during the experiment you wish to simulate.

*TODO*


## License

*TODO*


## References

<a name="ref-higs">[1]</a> H. R. Weller *et al.*, “Research opportunities at the upgraded HIγS facility”. Prog. Part. Nucl. Phys. **62.1**, 257 (2009). [`doi:10.1016/j.ppnp.2008.07.001`](https://dx.doi.org/10.1016/j.ppnp.2008.07.001).  
<a name="ref-g3">[2]</a> B. Löher *et al.*, “The high-efficiency γ-ray spectroscopy setup γ³ at HIγS”. Nucl. Instr. Meth. Phys. Res. A **723**, 136 (2013). [`doi:10.1016/j.nima.2013.04.087`](https://dx.doi.org/10.1016/j.nima.2013.04.087).  

