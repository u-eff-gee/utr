# Geant4 simulation of the UTR at HIγS

![Geometry of the UTR Geant4 simulation](.media/wallpaper_resize.png)

This is a [Geant4](https://geant4.web.cern.ch/) [[1]](#ref-g4_1) [[2]](#ref-g4_2) [[3]](#ref-g4_3) simulation of the Upstream Target Room (UTR) at the High Intensity γ-ray Source (HIγS) facility [[4]](#ref-higs), located at the Duke Free Electron Laser Laboratory (DFELL) of the Triangle University Nuclear Laboratory (TUNL) at Duke University, Durham, NC, USA.

# Table of Contents

 1. [Quick Start](#quickstart)
 2. [Features](#features)
 
    2.1 [Geometry](#geometry)
    
    2.2 [Sensitive Detectors](#sensitivedetectors)
    
    2.3 [Event Generation](#eventgeneration)
    
    2.4 [Physics](#physics)
    
    2.5 [Random Number Engine](#random)
    
    2.6 [Output File Format](#outputfileformat)
    
 3. [Installation](#installation)
 
    3.1 [Dependencies](#dependencies)
    
    3.2 [Compilation](#compilation)
    
 4. [Usage and Visualization](#usage)
 5. [Output Processing](#outputprocessing)
 6. [Unit Tests](#unittests)
 
    6.1 [AngularDistributionGenerator](#angulardistributiongenerator)
    
 7. [License](#license)
 8. [Acknowledgements](#acknowledgements)
 9. [References](#references)

## 1 Quick Start <a name="quickstart"></a>

This section is an overview or checklist for a user who wants to implement his own geometry and do simulations.

### 1.1 Follow the installation instructions

See section [3 Installation](#installation)

### 1.2 Read about the usage

See section [4 Usage and Visualization](#usage)

### 1.3 Create a DetectorConstruction

 1. Create a new campaign in `DetectorConstruction/` and/or a new directory in `DetectorConstruction/Campaign_YEAR`.
 2. Implement a geometry in a `DetectorConstruction.hh` and `DetectorConstruction.cc` file in this directory and maybe some auxiliary files following the conventions (see section [2.1 Geometry](#geometry)).
 3. Configure `utr` to use the new geometry using the cmake build option (see sections [2.1 Geometry](#geometry) or [3 Installation](#installation))

### 1.4 Define sensitive volumes

 1. In `DetectorConstruction.cc`, set volumes as sensitive detectors. There is the choice between 3 different detector types that record different information about particles (see section [2.2 Sensitive Detectors](#sensitivedetectors)).
 2. In `src/ActionInitialization.cc`, set the quantities that should be written to the output file (see section [2.2 Sensitive Detectors](#sensitivedetectors)).

### 1.5 Choose a primary source

Configure `utr` to use either the `G4GeneralParticleSource` or the `AngularDistributionGenerator` using the CMake build option (see sections [2.3 Event Generation](#eventgeneration) or [3 Installation](#installation))

### 1.6 Choose the physics processes

In `src/Physics.cc` activate or deactivate processes, choose between different models, or include certain particles (see section [2.4 Physics](#physics))
 
### 1.7 Choose random number seed

In `utr.cc`, set the random number seed explicitly to get deterministic results. It is not clear whether this works in multithreaded mode. See section [2.5 Random Number Engine](#random)

### 1.8 Set up a macro file

Create a macro file that contains instructions for the primary generator (see section [2.3 Event Generation](#eventgeneration)). 

Do not simulate more than 2^32 ~ 2 billion particles using `/run/beamOn`, since this causes an overflow in the random number seed, giving you in principle the same results over and over again. Execute the same simulation multiple times instead.

### 1.9 Run the simulation

Consider creating a macro like `loop.mac` to loop over variables in your macro file.
(See section [4 Usage and Visualization](#usage))

### 1.10 Analyze the output

See section [5 Output Processing](#output)

## 2 Features <a name="features"></a>

### 2.1 Geometry <a name="geometry"></a>
In Geant4, all geometry is implemented in a class which is derived from `G4VUserDetectorConstruction`. In `utr`, the user-implemented geometry can be found in files called `DetectorConstruction.hh` or `DetectorConstruction.cc`. Since the setup at HIGS is subject to many changes in between, and also during experiments, many different geometry implementations exist.

Since the 2018 campaign, we encourage to stick to the following directory structure and naming convention for the geometry files:

```
utr/
    utr.cc      # main()
    include/    # user actions and auxiliary permanent geometry
    src/        # user actions and auxiliary permanent geometry
    DetectorConstruction/
        Campaign_YEAR/
            include/    # auxiliary campaign- and run-specific geometry
                Setup_RUNS.hh
            src/        # auxiliary campaign- and run-specific geometry
                Setup_RUNS.cc
            TARGETS_RUNS/
                DetectorConstruction.hh
                DetectorConstruction.cc
```

As in reality, a geometry file only holds for a certain number of runs, which are part of a campaign. Therefore, unique `DetectorConstruction.cc` files can be found at the run-level. In principle, one could put all the geometry into this file, and before the 2018 campaign, it was done like that because the file was growing and growing. In the meantime, UG has tried to learn from his mistakes: The `DetectorConstructiion.cc` files should be as short as possible, delegating all the low-level construction jobs to auxiliary classes (please see [2.1.1 Note on auxiliary files](#auxiliary_files)). The auxiliary classes can be placed into different directories, depending on their nature:
Geometry which is expected to be permanent (for example detectors, or furniture), even between different campaigns, should be placed in the `utr/include` and `utr/src` directories.
More volatile geometry should be placed in the corresponding campaign, i.e. `utr/DetectorConstruction/Campaign_YEAR/include` and `utr/DetectorConstruction/Campaign_YEAR/src`.
*Sometimes, the difference may not be so clear, and at the moment the sorting is probably not consistent, but our team is still tidying up the 2018 campaign. Everything before 2018 does not follow these conventions, but has been sorted according to the directory structure above anyway to ensure backward compatibility.*

At the moment, detailed implementations of the geometry of the 2016/2017 campaign and parts of the 2018 campaign exist. They include geometry close to the beamline in the collimator room and the UTR including, of course, the experimental setups (γ³ setup [[5]](#ref-g3) and second setup). Less detailed geometries exist for selected experiments in older campaigns. The geometries for the 2013 and 2014/2015 campaign are based on the geometry from 2016/2017, the geometry for the 2012 campaign is based on the geometry from 2018.

By default, `utr` will use the geometry in `DetectorConstruction/Campaign_2018/64Ni_271_279`. Existing geometries can be switched very quickly using cmake build variables. For example, to use the geometry in `utr/DetectorConstruction/Campaign_YEAR/TARGETS_RUNS`, set the corresponding CMake variables and re-build the code (see also [3 Installation](#installation)):

```bash
$ cmake -DCAMPAIGN="Campaign_YEAR" -DDETECTOR_CONSTRUCTION="TARGETS_RUNS" .
```

Several pre-defined classes exist to simplify the geometry construction which are explained in the following.

#### 2.1.1 Note on auxiliary files <a name="auxiliary_files"></a>

In order to prevent the geometry files from growing too large, it is very helpful to divide the setup into logical parts and implement them im separate auxiliary classes. This also makes it easy to switch parts of the geometry on and off without having to comment hundreds of lines.

A common practice is to create `LogicalVolume` objects (pointers) in the auxiliary files that contain several parts, import their mother volume into `DetectorConstruction.cc`, and place it:

```
// Auxiliary.hh
    ...
    class Auxiliary{
        ...
        Auxiliary();
        G4LogicalVolume* Auxiliary_Logical;
        G4LogicalVolume* Get_Logical(){ return Auxiliary_Logical };
        ...
    };
    ...
// Auxiliary.cc
    ...
    Auxiliary::Auxiliary(){
        ...
        Auxiliary_Logical = new G4LogicalVolume(...);
        ...
    }
    ...
// DetectorConstruction.cc
    ...
    Auxiliary_Logical aux;
    G4LogicalVolume* aux_log = aux.Get_Logical();
    new G4PVPlacement(rot, pos, aux_log, "aux", ... );
    ...
```

However, this caused some problems in the case of `utr`. 
For example, the floor and the walls are clearly a logical unit that can be implemented in a separate file. Since the floor and the walls are as long and wide as the whole setup, the mother volume of this auxiliary class would unnecessarily overlap with everything.

Here, instead (except for the detectors, see also [2.1.2 Detectors](#detectors)), we chose to pass the global mother volume to the auxiliary classes and let them make the placements in a `Construct(G4ThreeVector global_coordinates)` method.

```
// Auxiliary.hh
    ...
    class Auxiliary{
        ...
        Auxiliary(G4LogicalVolume* World_Log){World_Logical = World_Log};
        Construct(G4ThreeVector global_coordinates);
        G4LogicalVolume* World_Logical;
        ...
    }
    ...
// Auxiliary.cc
    ...
    Auxiliary::Construct(G4ThreeVector global_coordinates){
        ...
        new G4PVPlacement(rot, global_coordinates + pos, Aux_Logical, "aux", World_Logical, ...);
        ...
    }
    ...
// DetectorConstruction.cc
    ...
    Auxiliary_Logical aux(World_Logical);
    aux.Construct(global_Coordinates);
    ...
```

By passing the global coordinates to the auxiliary file, the placements there can still be done in a local coordinate system.  These examples are only to illustrate the differences, have a look at the geometry from the 2018 campaign for more details.

This positioning scheme prevents overlaps of mother volumes. **However, the order of placements is still important!** The last element placed will be at the top of the hierarchy. For example, if there is a target mounted inside the beam pipe (i.e. the volume of the pipe completely covers the target), the following would be the correct order of placements:

```
// DetectorConstruction.cc
    beamPipe.Construct(global_coordinates_beamPipe);
    target.Construct(global_coordinates_target);
```

The alternative order

```
// DetectorConstruction.cc
    target.Construct(global_coordinates_target);
    beamPipe.Construct(global_coordinates_beamPipe);
```

would place the beam pipe on top of the target, and therefore ignore the target when tracking particles. Note that this problem would also occur if we used the other placement scheme.

A disadvantage of this placement scheme is that the volume hierarchy is very flat, because everything is placed inside the global mother volume. This makes it harder, to find a specific volume in the UI mode and easier to have name clashes. Nevertheless, we think that the advantages are more important.

#### 2.1.2 Detectors <a name="detectors"></a>
Classes for several different detectors exist. In all of those, a G4LogicalVolume that contains all the parts of a detector is implemented which is returned by the class' Get_Logical() method. Furthermore, each detector class can return its mother volume's length and radius.

To place a detector DetectorXY in the geometry, create an instance of the detector class in DetectorConstruction.cc, get the logical volume and place it in the geometry (using Get_Length() and Get_Radius() if necessary):

```
DetectorXY* detectorXY_Instance = new DetectorXY("DetectorXY_Name");
G4LogicalVolume* DetectorXY_Logical = detectorXY_Instance->Get_Logical();
new G4PVPlacement( ... )
```

The name "DetectorXY_Name" is the name of the logical volume of the detector crystal which can be used to register it as a sensitive detector.

At the moment, the following detectors are implemented:

* Cologne LaBr, Saint Gobain BrilLanCe 380 1.5x1.5"
* Darmstadt LaBr, Saint Gobain BrilLanCe 380, 3x3"
* Duke 60% HPGe "1" (Ortec serial number 36-TN31061A)
* Duke 60% HPGe "2" (Ortec serial number 36-TN30986A)
* Duke 60% HPGe "3" (Ortec serial number 36-TN40663A)
* Duke 60% HPGe "4" (Ortec serial number 36-TN21033A)
* Duke 55% HPGe (Ortec serial number 4-TN21638A)
* Duke 55% HPGe (Ortec serial number 4-TN31524A)
* Duke 120% HPGe "Zero degree detector" (Ortec serial number 33-P40383A)
* Darmstadt HPGe "1" (Canberra serial number 10PC473156-A)
* Darmstadt HPGe "2" (Eurisys Mesures serial number 10PC447589-A)
* Darmstadt Clover "Polarimeter" (Eurisys Mesures serial number 10PC447590-a)
* Cologne 100% HPGe (Ortec serial number 73954)
* Stuttgart 86.2% HPGe (Canberra serial number 37-N311204)

#### 2.1.3 Why so much code?

When looking at the code, even at the latest campaign, you may notice that parts of the code are redundant (for example several `Table2_XY.cc` files which contain almost the same information except for little changes). Furthermore, if you are used to codes like the [Agata Simulation Code](http://agata.pd.infn.it/documents/simulations/agataCode.html) or [G4Horus](https://gitlab.ikp.uni-koeln.de/jmayer/g4horus.git), you may notice that they heavily employ templates or abstract classes to reduce the code volume, for example for HPGe detectors, which all have about the same topology (a crystal, a cold finger, some housing ...).

However, if there is one thing we have learned about the UTR over the years, it is that anything can change. We are using different detectors every time, which have to be moved/shielded/exchanged between runs, shielding on the tables is reinforced or removed, new things are very often custom-made and don't have simple shapes, ...
The code somehow reflects this volatility of the setup, therefore it includes some more copy & paste and less abstract programming than other simulations.

#### 2.1.4 Bricks
*Deprecated! Only valid for geometries before 2018 campaign*

For maximum flexibility, the shielding of the setup can be constructed brick by brick. To avoid the `G4Solid->G4LogicalVolume->G4PhysicalVolume` procedure each time one would like to place a standardized brick, a lot of them are predefined as classes in `Bricks.hh`.

Once instantiated in DetectorConstruction.cc, bricks can be placed inside the G4LogicalVolume which was defined to be their mother volume via their constructor. To place a brick, use the `Put(x, y, z, angle_x, angle_y, angle_z)` method in which the coordinates and rotation angles around the coordinate axes can be specified.

Bricks are assumed to be cuboid objects, i.e. they can have 3 different side lengths. In `Bricks.hh`, the convention is that the long side points in z-direction, the medium side in x-direction and the short side in y-direction, if they can be distinguished. The respective lengths can be accessed via the member variables L, M and S.

#### 2.1.5 Filters
*Deprecated! Only valid for geometries before 2018 campaign*

Similar to bricks, filters and filter cases in front of detectors are implemented in `Filters.hh` and can be placed using their `Put()` methods.
The intent of this was to give the user an overview which filters are really there at the UTR. Sometimes, the documentation of experiments only mentions "thin", "medium" and "thick" filters, but no actual dimensions. Providing fixed filter types hopefully helps with such issues.

#### 2.1.6 Targets
*Deprecated! Only valid for geometries before 2018 campaign*

Complicated targets can be implemented in `Targets.hh`. The placement in DetectorConstruction.cc works analog to the placement of detectors. Relevant properties of the targets can be made accessible by implementing Get() methods.

### 2.2 Sensitive Detectors <a name="sensitivedetectors"></a>

Information about the simulated particles is recorded by instances of the G4VSensitiveDetector class. Any unique logical volume can be declared a sensitive detector.

*Unique* means volumes with a unique logical volume name. This precaution is here because all bricks and filters of the same type from the previous sections have the same logical volume names. Making one of those a sensitive detector might yield unexpected results.

Any time a particle executes a step inside a G4VSensitiveDetector object, its ProcessHits routine will access information of the step. This way, live information about a particle can be accessed. Note that a "hit" in the GEANT4 sense does not necessarily imply an interaction with the sensitive detector. Any volume crossing is also a hit. Therefore, also non-interacting geantinos can generate hits, making them a nice tool to explore to explore the geometry, measure solid-angle coverage, etc. ...
After a complete event, a collection of all hits inside a given volume will be accessible via its HitsCollection. This way, cumulative information like the energy deposition inside the volume can be accessed.

Three types of sensitive detectors are implemented at the moment:

* **EnergyDepositionSD**
    Records the total energy deposition by any particle per single event inside the sensitive detector.
* **ParticleSD**
    Records the first step of any particle inside the sensitive detector.
* **SecondarySD**
    Records the first step of any secondary particle inside the sensitive detector.
    
No matter which type of sensitive detector is chosen, the simulation output will be a [ROOT](https://root.cern.ch/) tree with a user-defined subset (see section [2.6 Output File Format](#outputfileformat)) of the following 10 branches:

* **ekin**
    Kinetic energy (in MeV) of the particle at the time of its first hit of the sensitive detector.
* **edep**
    Total energy deposition (in MeV) of the event (EnergyDepositionSD) OR energy deposition of the first hit of the sensitive detector (ParticleSD, SecondarySD)
* **particle**
    Type of the particle whose first hit of the sensitive detector it was (ParticleSD, SecondarySD) OR type of the very first particle in this event that hit the sensitive detector (EnergyDepositionSD). The type of the particle is encoded in the [Monte Carlo Particle Numbering Scheme](http://pdg.lbl.gov/mc_particle_id_contents.html).
* **volume**
    User-defined identifier of the sensitive detector that recorded this set of data.
* **x/y/z**
    Coordinates (in mm) of the first hit of the sensitive detector by a particle (ParticleSD, SecondarySD) OR coordinates of the first hit by the first particle in this event that hit the sensitive detector (EnergyDepositionSD)
* **vx/vy/vz**
    Momentum (in MeV/c) of the particle at the position of the first hit of the sensitive detector (ParticleSD, SecondarySD) OR momentum of the first particle hitting the sensitive detector in this event at the position of its first hit (EnergyDepositionSD).
    
The meaning of the columns sometimes changes with the choice of the sensitive detector.

To make a volume whose logical volume name is "Logic_Name" a sensitive detector of type XYSD, add the following lines in DetectorConstruction::ConstructSDandField(){}

```
XYSD *xySD = new XYSD("SD_Name", "SD_HitsCollection_Name");
G4SDManager::GetSDMpointer()->AddNewDetector(xySD);
xySD->SetDetectorID(volume); // volume is the detector ID which will be displayed in the ROOT output file
SetSensitiveDetector("Logic_Name", xySD, true);
```

The following examples illustrates how the different sensitive detectors work.

![Interaction with sensitive detector](.media/grid.png)

In the figure, a photon (orange, sinusoidal line) and several electrons (blue arrow) are shown which are part of a single event. Each hit in the sensitive detector (black circle) has a label that contains the particle number and the number of the hit. The history of the event is as follows:

The primary particle 1 (a photon with an energy of 2.5 MeV) enters the sensitive detector at (0,3). It is Compton-scattered at (2,5) and transfers 1.0 MeV to an electron (particle 2). The electron slowly loses its kinetic energy again in scattering processes at (4,8), (6,9) and (7,8) inside the sensitive detector which do not create new secondary particles. In the meantime, particle 1 travels to (6,1) and creates an e-/e+ pair with its remaining 1.5 MeV. Each lepton (3 and 4) gets an initial energy of (1.500 - 1.022)/2 = 0.239 MeV. Lepton 3 loses its kinetic energy inside the sensitive detector, Lepton 4 leaves the sensitive detector.

The following information would be recorded if the volume was a ...

* ... ParticleSD


|ekin |edep |particle|volume|x    |y    |vx   |vy   |
|:---:|:---:|:------:|:----:|:---:|:---:|:---:|:---:|
|2.5  |0.   |22      |0		 |0	     |3    |2    |2    |
|1.0  |0.   |11      |0		 |2	     |5    |2    |3    |
|0.239|0.   |11      |0		 |6      |1    |3    |3    |
|0.239|0.   |11      |0		 |6	     |1    |-3   |-3   |

* ... SecondarySD


|ekin |edep |particle|volume|x    |y    |vx   |vy   |
|:---:|:---:|:------:|:----:|:---:|:---:|:---:|:---:|
|1.0  |0.   |11      |0     |2    |5    |2    |3    |
|0.239|0.   |11      |0     |6    |1    |3    |3    |
|0.239|0.   |11      |0     |6    |1    |-3   |-3   |

* ... EnergyDepositionSD


|ekin |edep |particle|volume|x    |y    |vx   |vy   |
|:---:|:---:|:------:|:----:|:---:|:---:|:---:|:---:|
|2.5  |1.989|22      |0     |0    |3    |2    |2    |


For simplicity, the momentum vector is given as the vector of the depicted arrow/wave.
The real values would be

```
vx_real = 1/sqrt(vx^2 + vy^2)*cos(arctan(y/x))*ekin/c
vy_real = 1/sqrt(vx^2 + vy^2)*sin(arctan(y/x))*ekin/c
```

### 2.3 Event Generation <a name="eventgeneration"></a>

Event generation is done by classes derived from the `G4VUserPrimaryGeneratorAction`. In the following, the two existing event generators are described. 

By default, `utr` uses the Geant4 standard G4GeneralParticleSource. To use the AngularDistributionGenerator of `utr`, which implements angular distributions for Nuclear Resonance Fluorescence (NRF) applications, unset the `USE_GPS` option when building the source code (see also [3 Installation](#installation)):

```bash
$ cmake -DUSE_GPS=OFF .
```
 
#### 2.3.1 GeneralParticleSource

This event generator uses the G4GeneralParticleSource (GPS) whose parameters can be controlled by a macro file. For further information, see the [GPS documentation](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch02s07.html).

#### 2.3.2 AngularDistributionGenerator

The AngularDistributionGenerator generates monoenergetic particles that originate in a set of G4PhysicalVolumes of the DetectorConstruction and have a certain angular distribution.

The physical volumes (the "sources") and angular distribution can have arbitrary shapes. Starting positions and momentum directions are created using a Monte-Carlo method:
Given a(n)

* Cuboid in 3 dimensions ("container volume") that completely contains the source volume
* Angular distribution W(θ, φ)

AngularDistributionGenerator generates uniform random
 
* Positions `(random_x, random_y, random_z)` 
* Tuples `(random_θ, random_φ, random_W)`

until 

* The position `(random_x, random_y, random_z)` is inside one of the source physical volumes
* `random_W <= W(random_θ, random_φ)`

If both conditions are fulfilled, a particle is emitted from `(random_x, random_y, random_z)` in the direction `(θ, φ)`.

The process of finding a starting point is depicted in 2 dimensions in the figure below. The yellow source volume consists of two disconnected parts. It is placed inside a dashed box which is the container volume. Random points inside the container volume are either black or red, depending on whether they are inside or outside the source volumes. Only from the black points, particles are emitted (in random directions).

![MC position generator](.media/MC_Position_Generator.png)

The process of finding a starting vector is shown in one dimension (`W` is only dependent on `θ`) in in the figure below. First, a random value `random_θ` for `θ` with a uniform random distribution **on a sphere**. Note that this is not the same as a uniform distribution of values between 0 and π for θ. Then, a unform random number between 0 and an upper limit `W_max` is drawn. If the value `W_max` is lower than `W(random_θ)` (black points), then a particle will be emitted at that angle.

![MC momentum generator](.media/MC_Momentum_Generator.png)


Clearly, the algorithm works well if

* the cuboid approximates the shape of the sources well and wraps it tightly
* the angular distribution varies smoothly in the `(θ, φ)` plane and W_max is very close to the actual maximum value of `W`

The maximum number of randomly sampled points is hard-coded `AngularDistributionGenerator.cc` where it says:

```
MAX_TRIES_POSITION = 10000
MAX_TRIES_MOMENTUM = 10000
```

AngularDistributionGenerator can do a self-check before the actual simulation in which it creates `MAX_TRIES_XY` points and evaluates how many of the were valid. From this, the probability *p* of not hitting one of the source volumes / angular distribution can be estimated. An individual check is done for each source volume. If `p * N >~ 1`, where N is the number of particles to be simulated, the algorithm will very probably fail once in a while so try increasing `MAX_TRIES_XY`. A typical output of the self-check for the position generator looks like:

```
G4WT0 > ========================================================================
G4WT0 > Checking Monte-Carlo position generator with 100 3D points ...
G4WT0 > Check finished. Of 100 random 3D points, 82 were inside volume 'Se82_Target' ( 82 % )
G4WT0 > Probability of failure:	pow( 0.18, 100 ) = 3.36706e-73 %
G4WT0 > ========================================================================
```
The momentum generator contains one more piece of information. This is because the value of `W_max` that was introduced above has been arbitrarily set to 3 in `AngularDistributionGenerator.hh`, which should be a sensible choice for most angular distributions. However, it may be that `W(θ, φ) > 3` for some distribution. This can also be detected by the self-check with a Monte-Carlo method. For each of the MAX_TRIES_MOMENTUM tries, `utr` will also check whether the inequality `W_max <= W(random_θ, random_φ)` holds. If yes, this could be a hint that the value of `W_max` is too low and should be increased manually.
The corresponding message would look like
```
G4WT0 > In 5624 out of 10000 cases (56.24 % ) 
W(random_theta, random_phi) == MAX_W == 1 was valid. 
This may mean that MAX_W is set too low and 
the angular distribution is truncated.
```
If everything is okay, it will display
```
G4WT0 > MAX_W == 3 seems to be high enough.
```
However, using `W_max = 3` might still be inefficient, so it is good to know the minimum and maximum value of the angular distribution in advance. For debugging of the angular distribution, a unit test exists in the repository, which is especially recommended if one defines new angular distributions.

To change parameters of the AngularDistributionGenerator, an AngularDistributionMessenger has been implemented that makes the following macro commands available:

* `/ang/particle NAME`
    Set the particle type by name. See the GEANT4 documentation for a list of possible particles.
* `/ang/energy ENERGY UNIT`
    Set the kinetic energy of the particle
* `/ang/nstates NSTATES`
    Define the number of states for the angular distribution (see below) 
* `/ang/stateN VALUE`
    Define state number N (where N is in {1, 2, 3, 4})
* `/ang/deltaN1N2 VALUE`
    Define the multipole mixing ratio for the transition between states N1 and N2
* `/ang/sourceX VALUE UNIT`
    Along with `sourceY` and `sourceZ`, defines the position of the container box
* `/ang/sourceDX VALUE UNIT`
    Along with `sourceDY` and `sourceDZ`, defines the dimensions of the container box.
* `/ang/sourcePV VALUE`
    Enter the name of a physical volume that should act as a source. To add more physical volumes, call `/ang/sourcePV` multiple times with different arguments (about using multiple sources, see also the [caveat](#multiplesources) at the end of this section).
    
The container volume's inside will be the interval [X - DX/2, X + DX/2], [Y - DY/2, Y + DY/2] and [Z - DZ/2, Z + DZ/2].

The identifiers of the angular distribution are given to the simulation as an array of numbers `states  = {state1, state2, ...}` whose length `NSTATES` can to be specified by the user.
For "real" NRF angular distributions, this array of numbers will be the spins of the excited states in a cascade, with the parity indicated by the sign of the numbers.

Due to parity symmetry, an inversion of all the parities in a cascade will result in the same angular distribution. For example, the transition 0+ -> 1- -> 0+ has the same angular distribution as 0- -> 1+ -> 0-. This is indicated by the notation "+-" and "-+" in the list below (the example is listed as "0+- -> 1-+ -> 0+-").

It was chosen to represent the parity of a state as the sign of the spin quantum number. Unfortunately, this makes it impossible to represent 0- states, because the number "-0" is the same as "+0". Therefore, the value "-0.1" represents a 0- state.

At the moment, the following distributions are implemented:

* `states == {0.1, 0.1, 0.1}`
    Wildcard for test distributions
* `states == {0., 0., 0.}`
    Isotropic distribution
* `states == {0., 1., 0.} or {-0.1, -1., -0.1}`
    0+- -> 1+- -> 0+-
* `states == {0., -1., 0.} or {-0.1, 1., -0.1}`
    0+- -> 1-+ -> 0+-
* `states == {0., 2., 0.} or {-0.1, -2., -0.1}`
    0+- -> 2+- -> 0+-
* `states == {0., 2., 2.} or {-0.1, -2., -2.}`
    0+- -> 2+- -> 2+-
* `states == {0., 1., 2.} or {-0.1, -1., -2.}`
    0+- -> 1+- -> 2+-
* `states == {0., -1., 2.} or {-0.1, 1., -2.}`
    0+- -> 1-+ -> 2+-
* `states == {1.5, -2.5, 1.5} or {-1.5, 2.5, -1.5}`
    3/2+- -> 5/2-+ -> 3/2+-
* `states == {1.5, 2.5, 1.5} or {-1.5, -2.5, -1.5}`
    3/2+- -> 5/2+- -> 3/2+-
* `states == {1.5, 1.5, 1.5} or {-1.5, -1.5, -1.5}`
    3/2+- -> 3/2+- -> 3/2+-
* `states == {1.5, -1.5, 1.5} or {-1.5, 1.5, -1.5}`
    3/2+- -> 3/2-+ -> 3/2+-
* `states == {0.5, 1.5, 0.5} or {-0.5, -1.5, -0.5}`
    1/2+- -> 3/2+- -> 1/2+-
* `states == {0.5, -1.5, 0.5} or {-0.5, 1.5, -0.5}`
    1/2+- -> 3/2-+ -> 1/2+-
* `states == {2.5, 1.5, 2.5} or {-2.5, -1.5, -2.5}`
    5/2+- -> 3/2+- -> 5/2+-
* `states == {2.5, -1.5, 2.5} or {-2.5, 1.5, -2.5}`
    5/2+- -> 3/2-+ -> 5/2+-
* `states == {2.5, 2.5, 2.5} or {-2.5, -2.5, -2.5}`
    5/2+- -> 5/2+- -> 5/2+-
* `states == {2.5, -2.5, 2.5} or {-2.5, 2.5, -2.5}`
    5/2+- -> 5/2-+ -> 5/2+-
* `states == {2.5, 3.5, 2.5} or {-2.5, -3.5, -2.5}`
    5/2+- -> 7/2+- -> 5/2+-
* `states == {2.5, -3.5, 2.5} or {-2.5, 3.5, -2.5}`
    5/2+- -> 7/2-+ -> 5/2+-

Finding the correct dimensions of the container box might need visualization. Try placing a `G4Box` with the desired dimensions at the desired position in the geometry and see whether it encloses the source volume completely and as close as possible. In fact, most of the predefined geometries have such a `G4Box` called `AuxBox` which is commented out. After using it to determine the size and position of the container box, remember to comment out the code again.

#### 2.3.1 Caveat: Multiple sources <a name="multiplesources"></a>

When using multiple sources, be aware that `AngularDistributionGenerator` samples the points with a uniform random distribution inside the container volume. How many particles are emitted from a certain part of the source will only depend on its volume. 

This is not always desirable. Imagine the following example: The goal is to simulate a beam on two disconnected targets. The first target has twice the volume of the second target, but the second target has a four times larger density. That means, the reaction would occur approximately twice as often in the second target than in the first. Implementing the targets like this in `AngularDistributionGenerator` would not give realistic results because of the weighting by volume. In a case like this, one would rather simulate the individual parts of the target and compute a weighted sum of the results.

### 2.4 Physics <a name="physics"></a>
The physics processes are defined in `/src/Physics.cc`. In the Physics::ConstructProcess() method, physics lists for groups of particles can be activated by uncommenting the desired physics list. If you are working on a slow machine and you just want to visualize the geometry, it can be advantageous to switch off all physics.

At the moment, physics processes for gammas, electrons/positrons, neutrons and other charged particles are available. Check Physics.cc to see which processes are defined.
A description of the processes can be found in the [GEANT4 Physics Reference Manual](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/PhysicsReferenceManual/fo/PhysicsReferenceManual.pdf)

The physics list is optimized for NRF applications at HIGS and largely inspired by the GEANT4 examples `TestEm7` and `Hadr04`.

#### 2.4.1 Transportation
Mandatory for all particles. Without this, the simulation will do almost nothing.

#### 2.4.2 EMPenelope, EMLivermore
Uses the Penelope OR Livermore low-energy electromagnetic physics framework for gammas, electrons and positrons. Only one of them can be active at the same time. Both frameworks aim at a more precise description of these processes than the standard G4 physics which are optimized for particle physics applications at CERN.

At the moment, only EMLivermore provides models for the interaction of polarized photons with matter, therefore, G4LivermorePolarized* is used by default.

#### 2.4.3 HPNeutron
A specialized neutron physics framework for low energies.

#### 2.4.4 ChargedParticle
Standard processes for any other charged particle that might appear in the simulation. The probability for this in NRF applications is low, so this list is not very detailed.

### 2.5 Random Number Engine <a name="random"></a>
In `utr.cc`, the random number engine's seed is set by using the current CPU time, making it a "real" random generator.

```
G4Random::setTheEngine(new CLHEP::RanecuEngine);
// 'Real' random results
time_t timer;
G4Random::setTheSeed(time(&timer));
```

If you want deterministic results for some reason, comment these lines out and uncomment 

```
G4Random::setTheSeed(long n);
```

Re-compile afterwards. 
After this change, every restart of the simulation with unchanged code will yield the same results.

### 2.6 Output File Format <a name="outputfileformat"></a>
In section [2.2 Sensitive Detectors](#sensitivedetectors) the format of the ROOT output file was already introduced. The possible branches are 

* **ekin**
* **edep**
* **particle**
* **volume**
* **x/y/z**
* **vx/vy/vz**

The user can specify in `ActionInitialization.cc` which of these quantities should be written to the ROOT file, to avoid creating unnecessarily large files. When the `RunAction` is initialized in `ActionInitialization.cc`, set the value of the corresponding output flag to 0 if this quantity should not be written to the ROOT file.

For example, the code

```
output_flags[EKIN] = 0;
output_flags[EDEP] = 1;
output_flags[PARTICLE] = 1;
output_flags[VOLUME] = 1;
output_flags[POSX] = 0;
output_flags[POSY] = 0;
output_flags[POSZ] = 0;
output_flags[MOMX] = 0;
output_flags[MOMY] = 0;
output_flags[MOMZ] = 0;
```

would only write the energy deposition, the particle type and the sensitive detector volume.

## 3 Installation <a name="installation"></a>

These instructions will get you a copy of the simulation running.

### 3.1 Dependencies <a name="dependencies"></a>

To build and run the simulation, the following dependencies are required:

* [Geant4](https://geant4.web.cern.ch/) (tested with version 10.04)
* CMake (*build*)
* Make (*build*)

Furthermore, to use the analysis scripts:

* [ROOT 6](https://root.cern.ch/)

Optional components:

* [Qt](https://www.qt.io/) as a visualization driver. Compile Geant4 with the `GEANT4_USE_QT` option (tested with Qt4)
* [ccmake](https://cmake.org/cmake/help/v3.0/manual/ccmake.1.html) UI for CMake which gives an overview of available build options

### 3.2 Compilation <a name="compilation"></a>

The most simple way to build and compile the simulation is

```bash
$ cmake .
$ make
```

This will compile the simulation using a default geometry (see [2.1 Geometry](#geometry)) and the GeneralParticleSource as a primary generator (see [2.3 Event Generation](#eventgeneration)).

To use another geometry in the directory `DetectorConstruction/`, set the `CAMPAIGN` and the `DETECTOR_CONSTRUCTION` build variable when executing the `cmake` command:

```bash
$ cmake -DCAMPAIGN="Campaign_YEAR" -DDETECTOR_CONSTRUCTION="TARGETS_RUNS" .
```

Since the 2018 campaign, there is also a build variable to quickly remove the targets in the geometry, for example if one would like to simulate detector efficiencies:

```bash
$ cmake -DUSE_TARGETS=OFF .
```
Have a look at the `DetectorConstruction.cc` files of the 2018 campaign to see how this option influences the compilation.

To use the AngularDistributionGenerator instead of the GeneralParticleSource, set the `USE_GPS` variable

```bash
$ cmake -DUSE_GPS=OFF .
```

Any of the commands above will create the `utr` binary in the top directory.
In case CMake complains that it cannot find `Geant4Config.cmake` or `geant4-config.cmake`, set the *CMAKE_INSTALL_PREFIX* variable to the directory where GEANT4 has been installed:

```bash
$ cmake -DCMAKE_INSTALL_PREFIX=/PATH/TO/G4/INSTALL .
$ make
```

## 4 Usage and Visualization <a name="usage"></a>

The compiled `utr` binary can be run with different arguments. To get an overview, type

```bash
$ ./utr --help
```
Any execuation of `utr` will print (amongst the output of Geant4 itself) information about the output quantities and (since the 2018 campaign) about the position of important parts in the geometry:

```bash
==============================================================
  DetectorConstruction: Info (all dimensions in mm)
> Collimator entrance position : (  0.00,  0.00, -3894.18 )
> Ideal position of G3 target  : (  0.00,  0.00,  0.00 )
> Ideal position of 2nd target : (  0.00,  0.00, 1574.80 )
> World dimensions             : ( 3000.00, 3150.00, 8000.00 )
==============================================================
G4WT0 > ================================================================================
G4WT0 > ActionInitialization: The following quantities will be saved to the output file:
G4WT0 > EDEP
G4WT0 > PARTICLE
G4WT0 > VOLUME
G4WT0 > MOMX
G4WT0 > MOMY
G4WT0 > MOMZ
G4WT0 > ================================================================================
```

Important optional arguments besides `--help` are:
```bash
$ ./utr -m MACROFILE
```
Executes macro file MACROFILE
```bash
$ ./utr -t NTHREADS
```
Sets the number of threads in multithreaded mode (default: 1)
```bash
$ ./utr -o OUTPUTDIR
```

Sets the output directory of `utr` where the ROOT files will be placed.

While running a simulation, `utr` will automatically print information about the progress in the following format, using the `G4VUserSteppingAction` class:

```bash
Progresss: [          160000/100000000]  0.16 %  Running time:   0d  0h   0mn   4s   Estimated remaining time:   0d  0h  2mn    4s
```

That means there is no need to use the `/run/printProgress` macro of Geant4 any more. The number of events `NEVENTS` after which a new progress update is printed can be set using the `PRINT_PROGRESS` preprocessor variable at compile-time (see also [3 Installation](#installation)):

```bash
$ cmake -DPRINT_PROGRESS=NEVENTS.
```

Running `utr` without any argument will launch a UI session where macro commands can be entered. It should also automatically execute the macro file `init_vis.mac`, which visualizes the geometry.

If this does not work, or to execute any other macro file MACROFILE, type 
```bash
/control/execute MACROFILE
```
in the UI session. In this mode, it is important to know how the angles are defined, to be able to set the view using the `/vis/viewer/set/viewpointThetaPhi` command:
```bash
/vis/viewer/set/viewpointThetaPhi 180 0 deg
```
views the setup in beam direction,
```bash
/vis/viewer/set/viewpointThetaPhi 0 0 deg
```
views it against the beam direction. To view the geometry from above and below, use
```bash
/vis/viewer/set/viewpointThetaPhi 270 270 deg
```
and
```bash
/vis/viewer/set/viewpointThetaPhi 270 90 deg
```
respectively.

It is also possible to create 3D visualization files that can be viewed by an external viewer like [Blender](https://www.blender.org/) (the title picture was made in Blender, for example). The macro `vrml.mac` shows how to create a `.wrl` file. Run it in UI mode with
```bash
/control/execute vrml.mac
```

## 5 Output Processing <a name="outputprocessing"></a>

The directory `OutputProcessing` contains some **sample** ROOT scripts that can be adapted by the user to process their simulation output. Executing

```bash
$ cd OutputProcessing/
$ make
```
in this directory should compile all the scripts and move the executables to the `utr/` directory.
The compilation may fail if the `ROOTSYS` environment variable is not set on your system.

Executables can be run like

```bash
$ ./EXECUTABLENAME {ARGUMENTS}
```
The executables can be removed using the command

```bash
$ make clean
```
in `OutputProcessing`

### 5.1 RootToTxt.cpp
`RootToTxt` converts a ROOT output file (*TFile*) containing an n-tuple of data (a *TTree* with *TBranch* objects) to a simple text file with the same content. If you want to convert a ROOT file ROOTFILE, type
```bash
$ ./rootToTxt ROOTFILE
```
The ROOT file can have a TTree with an arbitrary name and an arbitrary number of TBranch objects. The output text file has the same name as the ROOT file but with a ".txt" suffix.
Be aware that conversion into text files increases the file size.

### 5.2 getHistogram
`getHistogram` sorts the data from multiple output files (for example, several threads of the same simulation) into a ROOT histogram and saves the histogram to a new file.
Executing

```bash
$ ./getHistogram --help
Usage: getHistogram [OPTION...] Create histograms from a list of events
GetHistogram

  -b BIN                     Number of energy bin whose value should be
                             displayed
  -m MULTIPLICITY            Particle multiplicity
  -o OUTPUTFILENAME          Output file name
  -p PATTERN1                File name pattern 1
  -q PATTERN2                File name pattern 2
  -t TREENAME                Name of tree
  -v                         Verbose mode (does not silence -b option)
  -?, --help                 Give this help list
      --usage                Give a short usage message
```

shows how to use the script. The meaning of the arguments to the options is:

* TREE: Name of the ROOT tree (Default: TREE=="utr") in all of the output files.
* PATTERN1 and 2: Two strings that identify the set of files to be merged. See also the example below. (Default: PATTERN1=="utr", PATTERN2==".root")
* OUTPUTFILE: Name of the output file that contains the histograms. (Default: OUTPUTFILENAME=="hist.root")
* MULTIPLICITY: Determines how many events should be accumulated before adding information to the histogram. This can be used, for example, to simulate higher multiplicity events in a detector: Imagine two photons with energies of 511 keV hit a detector and deposit all their energy. However, the two events cannot be distinguished by the detector due to pileup, so a single event with an energy of 1022 keV will be added to the spectrum. Similarly, Geant4 simulates events by event. In order to simulate pileup of n events, set MULTIPLICITY==n. (Default: MULTIPLICITY==1)
* BIN: Number of the histogram bin that should be printed to the screen while executing `getHistogram`. This option was introduced because often, one is only interested in the content of a special bin in the histograms (for example the full-energy peak). If the histograms are defined such that bin `3000` contains the events with an energy deposition between `2.9995 MeV` and `3.0005 MeV` and so on, so there is an easy correspondence between bin number and energy. (Default: BIN==-1)

**A short example:**
The typical output of two different simulations on 2 threads each are the files
```
$ ls
master.root
utr0_t0.root
utr0_t1.root
utr1_t0.root
utr1_t1.root
```
(The `master.root` file may be there but it contains no data. It is just an artifact of the multithreaded mode.) 
Assuming you would like to merge both threads of `utr0` and write the accumulated histogram to `hist.root`, you execute
```bash
$ ./getHistogram -t utr -p utr0 -q .root -o hist.root
```
This will create an output file called `hist.root` with the desired histogram.
If, instead, you wanted to merge *all* ROOT files, do
```bash
$ ./getHistogram -t utr -p utr -q .root -o hist.root
```
In this simple example, already the first pattern uniquely identifies the files you want to merge. However, imagine there was a file `utr0.txt` in the same directory, then the second pattern could be used to exclude this.

Yet another possibility is that you would like to merge both threads of `utr0` into one histogram, and both threads of `utr1` into another one. The shell script `loopGetHistogram.sh` can be used for this task. Executing
```bash
$ ./loopGetHistogram 0 1 utr utr
```
would do just what is described above, creating the output files `utr0.root` and `utr1.root`.

The user has to hard-code the desired histograms in `OutputProcessing/GetHistogram.cpp`, i.e. edit the lines from

```bash
//
//      START OF USER-DEFINED OUTPUT
//
```
and
```bash
//
//      END OF USER-DEFINED OUTPUT
//
```

The standard `getHistogram` script that is included in this repository creates histograms (0 to 10 MeV, 10^4 bins) for the energy deposition by photons in the volumes with numbers 1 to 9 (about setting the number of a sensitive volume, see [2.2 Sensitive Detectors](#sensitivedetectors)). Setting a higher multiplicity means that the energy deposition of several simulated events is summed up.

### 5.3 histogramToTxt (executable)
A direct follow-up to `getHistogram`, `HistogramToTxt.cpp` takes a ROOT file that contains **only** 1D histograms (*TH1* objects) and converts each histogram to a single text file. The script is used as follows:

```bash
$ ./histogramToTxt FILENAME.root
```

Where FILENAME is the name of the ROOT file that contains TH1 objects. `histogramToTxt` outputs text files with the naming pattern

```bash
HISTNAME_FILENAME.txt
```

where HISTNAME is the name of the TH1 object and FILENAME the same as above.
The shell script `loopHistogramToTxt.sh` shows how to loop the script over a large number of files.

## 6 Unit Tests <a name="unittests"></a>

## 6.1 AngularDistributionGenerator <a name="angulardistributiongenerator"></a>

For testing the `AngularDistributionGenerator`, a dedicated geometry can be found in `/DetectorConstruction/unit_tests/AngularDistributionGenerator_Test/` and a macro file and output processing script are located in `/unit_tests/AngularDistributionGenerator_Test/`. The test geometry consists of a very small spherical particle source surrounded by a large hollow sphere which acts as a **ParticleSD**. Geantino particles emitted by this source and detected by the hollow sphere should have the desired angular distribution. This test was originally implemented to test the built-in angular distributions which are manually coded from the output of a computer algebra program, and to get a feeling of how large the value of `W_max` has to be.

In order to use the unit test, the following things have to be prepared:

  1. Build the correct geometry by setting the build variables `CAMPAIGN` to `unit_tests` and `DETECTOR_CONSTRUCTION` to `AngularDistributionGenerator_Test` (see [3.2 Compilation](#compilation)).
  2. Ensure that the momentum vectors of the particles are written to a ROOT file by enabling the correct flags in `ActionInitialization.hh` (see [2.6 Output File Format](#outputfileformat))
  3. Run a simulation with geantinos and the desired angular distribution. A sample macro is `/unit_tests/AngularDistributionGenerator/angdist_test.mac`
  4. Compile the analysis script `AngularDistributionGenerator_Test.cpp` in the same directory by typing `make`. It can be executed with a number of simulation output files similar to `GetHistogram` (see [5. Output Processing](#outputprocessing)).

When executed, the analysis script creates a file `hist.root` that contains a histogram in the `(θ, φ)` plane which shows the frequency of particles with a certain direction. This can already be visually compared to the shape of the continuous function that was used as an input. An example for a `0^+ -> 1^+ -> 0^+` cascade is shown below:

![Output of AngDist unit test](.media/010_histogram.png)

With the bad choice `W_max == 1`, the same analysis would result in the following histogram which is clearly different:

![Output of AngDist unit test](.media/010_histogram_bad.png)

In order to have a more quantitative analysis, the histogram can also be fit with the input distribution as it is implemented in `src/AngularDistribution.cc`. For this purpose, `AngularDistribution.hh` is already included in `AngularDistributionGenerator_Test.cpp`. 
To select a certain distribution, hard code the identifiers and parameters in the source code of the script where it says
```
	//
	//	START OF USER-DEFINED OUTPUT
	//

		nstates = 3;

		states[0] = 0.;
		states[1] = 1.;
		states[2] = 0.;

		mix[0] = 0.;
		mix[1] = 0.;

	//
	//	END OF USER-DEFINED OUTPUT
	//
```
and recompile afterwards. Running the script with the `-f` option (the only option that is different from `GetHistogram`) will fit the input distribution to the simulation output.
Now, by looking at the fit

![Output of AngDist unit test, fitted](.media/010_histogram_fit.png)

and especially the fit residuals

![Output of AngDist unit test, fit residuals](.media/010_histogram_fit_residuals.png)

one can see clear systematic deviations from the input distribution which are a clear indication that `W_max == 1` is not a good choice for this distribution.


## 7 License <a name="license"></a>

Copyright (C) 2017

U. Gayer (ugayer@ikp.tu-darmstadt.de)

O. Papst (opapst@ikp.tu-darmstadt.de)

This code is distributed under the terms of the GNU General Public License. See the COPYING file for more information.

## 8 Acknowledgements <a name="acknowledgements"></a>

UG would like to acknowledge the untiring effort of user Jörn Kleemann in debugging the code and making `utr` better in general.

## 9 References <a name="references"></a>

<a name="ref-g4_1">[1]</a> S. Agostinelli *et al.*, “GEANT4 - a simulation toolkit”, Nucl. Inst. Meth. A **506.3**, 250 (2003). [`doi:10.1016/S0168-9002(03)01368-8`](https://doi.org/10.1016/S0168-9002(03)01368-8).  
<a name="ref-g4_2">[2]</a> J. Allison *et al.*, “GEANT4 developments and applications”, IEEE Transactions on Nuclear Science, **53.1**, 270 (2006). [`doi:10.1109/TNS.2006.869826`](https://doi.org/10.1109/TNS.2006.869826).  
<a name="ref-g4_3">[3]</a> J. Allison *et al.*, “Recent developments in GEANT4”, Nucl. Inst. Meth. A **835**, 186 (2016). [`doi:10.1016/j.nima.2016.06.125`](https://doi.org/10.1016/j.nima.2016.06.125).  
<a name="ref-higs">[4]</a> H. R. Weller *et al.*, “Research opportunities at the upgraded HIγS facility”, Prog. Part. Nucl. Phys. **62.1**, 257 (2009). [`doi:10.1016/j.ppnp.2008.07.001`](https://doi.org/10.1016/j.ppnp.2008.07.001).  
<a name="ref-g3">[5]</a> B. Löher *et al.*, “The high-efficiency γ-ray spectroscopy setup γ³ at HIγS”, Nucl. Instr. Meth. Phys. Res. A **723**, 136 (2013). [`doi:10.1016/j.nima.2013.04.087`](https://doi.org/10.1016/j.nima.2013.04.087).  
