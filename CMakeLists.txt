#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)
project(utr)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# CADMesh
option(WITH_CADMESH "Build with CADMesh" OFF)
if(WITH_CADMESH)
  find_package(cadmesh)
  include_directories(${CADMESH_INCLUDE_DIRS})
endif()

#----------------------------------------------------------------------------

# Build options
#----------------------------------------------------------------------------

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  list(REMOVE_ITEM dirlist src)
  list(REMOVE_ITEM dirlist include)
  SET(${result} ${dirlist})
ENDMACRO()

#----------------------------------------------------------------------------
# Choose campaign (an experimental campaign usually has a bulk geometry which is not changed, and several files like the detector arrangment which differ from run to run)
set(CAMPAIGN "Campaign_2018_2019" CACHE STRING "Select experimental campaign from directory DetectorConstruction/")
SUBDIRLIST(SETUP_CHOICE ${CMAKE_CURRENT_SOURCE_DIR}/DetectorConstruction)
set_property(CACHE CAMPAIGN PROPERTY STRINGS ${SETUP_CHOICE})

# Choose geometry
set(DETECTOR_CONSTRUCTION "64Ni_271_279" CACHE STRING "Select detector construction from directory DetectorConstruction/$CAMPAIGN/")
SUBDIRLIST(DETECTOR_CONSTRUCTION_CHOICE ${CMAKE_CURRENT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN})
set_property(CACHE DETECTOR_CONSTRUCTION PROPERTY STRINGS ${DETECTOR_CONSTRUCTION_CHOICE})

mark_as_advanced(CLEAR CAMPAIGN DETECTOR_CONSTRUCTION)

set(PRINT_PROGRESS 100000 CACHE STRING "Set the frequency of printed updates about the progress of utr (unit: number of events processed)")
set(ZERODEGREE_OFFSET 30 CACHE STRING "Set the offset of the zero-degree detector from the optical axis in mm. (Default: 30 mm, which reproduced experimental results well in the past.)")
# Choose primary generator
option(GENERATOR_ANGDIST "Use AngularDistributionGenerator as primary generator instead of G4GeneralParticleSource (has a higher priority than USE_ANGCORR if both are checked)" OFF)
option(GENERATOR_ANGCORR "Use AngularCorrelationGenerator as primary generator instead of G4GeneralParticleSource" OFF)
option(USE_TARGETS "Use Targets in the geometry" ON)
option(USE_ZERODEGREE "Use zerodegree detector in the geometry" ON)

option(EM_FAST "Use G4EmStandardPhysics_option1" OFF)
option(EM_STANDARD "Use G4EmStandardPhysics_option4" OFF)
option(EM_LIVERMORE "Use G4EmLivermorePhysics" OFF)
option(EM_LIVERMORE_POLARIZED "Use G4EmLivermorePolarizedPhysics" ON)
option(EM_LIVERMORE_POLARIZED_JAEA "Use G4EmLivermorePolarizedPhysics with elastic scattering processes from JAEA" OFF)
option(EM_PENELOPE "Use G4EmPenelopePhysics" OFF)
option(EM_EXTRA "Use G4EmExtraPhysics" OFF)

option(HADRON_ELASTIC_STANDARD "Use G4HadronElasticPhysics" ON)
option(HADRON_ELASTIC_HP "Use G4HadronElasticPhysicsHP" OFF)
option(HADRON_ELASTIC_LEND "Use G4HadronElasticPhysicsLEND" OFF)

option(HADRON_INELASTIC_STANDARD "Use G4HadronPhysicsFTFP_BERT" ON)
option(HADRON_INELASTIC_HP "Use G4HadronPhysicsFTFP_BERT_HP" OFF)
option(HADRON_INELASTIC_LEND "Use G4HadronPhysicsShieldingLEND" OFF)

option(BACKWARD_COMPATIBLE_BUILD "Build for Geant4 versions < 10.5" OFF)

option(EVENT_EVENTWISE "For each event, record the total energy deposition in each detector in a single root entry (row). Causes all other EVENT_* cmake build options to be ignored." OFF)
option(EVENT_ID "For each event, record the event number." OFF)
option(EVENT_EDEP "For each event, record total energy deposition in the detectors" ON)
option(EVENT_EKIN "For each event, record kinetic energy at the time a particle first hits a detector" OFF)
option(EVENT_PARTICLE "For each event, record type of the first particle that hit a detector" ON)
option(EVENT_VOLUME "For each event, record the detector volumes that were hit by a particle" ON)
option(EVENT_POSX "For each event, record the X position of the first particle that hit a detector" OFF)
option(EVENT_POSY "For each event, record the Y position of the first particle that hit a detector" OFF)
option(EVENT_POSZ "For each event, record the Z position of the first particle that hit a detector" OFF)
option(EVENT_MOMX "For each event, record the momentum in X direction of the first particle that hit a detector" OFF)
option(EVENT_MOMY "For each event, record the momentum in Y direction of the first particle that hit a detector" OFF)
option(EVENT_MOMZ "For each event, record the momentum in Z direction of the first particle that hit a detector" OFF)

#----------------------------------------------------------------------------
# Enable configuration of the source code by cmake
configure_file(
	"${PROJECT_SOURCE_DIR}/include/utrConfig.h.in"
	"${PROJECT_SOURCE_DIR}/include/utrConfig.h"
)
#----------------------------------------------------------------------------

# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/include)
include_directories(${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/${DETECTOR_CONSTRUCTION})

#----------------------------------------------------------------------------
# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/src/*.cc ${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/${DETECTOR_CONSTRUCTION}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/include/*.hh ${PROJECT_SOURCE_DIR}/DetectorConstruction/${CAMPAIGN}/${DETECTOR_CONSTRUCTION}/*.hh)
if(BACKWARD_COMPATIBLE_BUILD)
	list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/EMPhysicsPolarizedJAEA.cc)
	list(REMOVE_ITEM headers ${PROJECT_SOURCE_DIR}/include/EMPhysicsPolarizedJAEA.hh)
endif(BACKWARD_COMPATIBLE_BUILD)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(utr ${PROJECT_SOURCE_DIR}/src/utr.cc ${sources} ${headers})
target_link_libraries(utr ${Geant4_LIBRARIES})
if(WITH_CADMESH)
  target_link_libraries(utr ${cadmesh_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build utr. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(UTR_SCRIPTS
  vis.mac
  )

foreach(_script ${UTR_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/scripts/${_script}
    ${PROJECT_BINARY_DIR}/scripts/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS utr DESTINATION bin)
