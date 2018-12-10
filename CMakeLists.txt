#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
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

#----------------------------------------------------------------------------

# Build options
# Choose campaign (an experimental campaign usually has a bulk geometry which is not changed, and several files like the detector arrangment which differ from run to run)
set(CAMPAIGN "DHIPS_2019" CACHE STRING "Select experimental campaign from directory DetectorConstruction/")
# Choose geometry
set(DETECTOR_CONSTRUCTION "B11_RSA" CACHE STRING "Select detector construction from directory DetectorConstruction/$CAMPAIGN/")
set(PRINT_PROGRESS 100000 CACHE STRING "Set the frequency of printed updates about the progress of utr (unit: number of events processed)")
# Choose primary generator
option(USE_GPS "Use G4GeneralParticleSource as primary generator" ON)
option(USE_TARGETS "Use Targets in the geometry" ON)

#----------------------------------------------------------------------------
# Enable configuration of the source code by cmake
configure_file(
	"${PROJECT_SOURCE_DIR}/utrConfig.h.in"
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

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(utr utr.cc ${sources} ${headers})
target_link_libraries(utr ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build utr. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(UTR_SCRIPTS
  beam.mac
  source.mac
  angdist.mac
  init_vis.mac
  vis.mac
  )

foreach(_script ${UTR_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS utr DESTINATION bin)
