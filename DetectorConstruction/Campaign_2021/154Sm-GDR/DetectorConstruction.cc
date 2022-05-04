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

#include "DetectorConstruction.hh"
#include "DetectorConstructionConfig.hh"
#include "utrConfig.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

// Materials
#include "G4NistManager.hh"
#include "Materials.hh"
#include "Units.hh"

// Geometry
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

// Detectors
#include "CeBr3_2x2.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr_3x3.hh"

// Sensitive Detectors
#include "EnergyDepositionSD.hh"
#include "G4SDManager.hh"
#include "ParticleSD.hh"
#include "SecondarySD.hh"

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <array>
using std::array;

#include <algorithm>

struct DetectorPosition {
  const string id;
  const double theta;
  const double phi;
  const double distance;
  const double intrinsicRotationAngle;
  const double filterThicknessCu;
  const double filterThicknessPb;
  const G4ThreeVector targetPosOffset;
};

// The setup ELOG entry https://elog.ikp.physik.tu-darmstadt.de/clovershare/800 only lists quickly measured, approximate filter thicknesses of 1mm or 2mm for Cu and 1mm or 2.5mm for Pb filters.
// Here these values are instead replaced by averages of the more precisely measured values from ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/747
const auto filterCuThinThickness = 1.015 * mm; // Aka 1mm Cu filter
const auto filterCuThickThickness = 2.072 * mm; // Aka 2mm Cu filter
const auto filterPbThinThickness = 1.158 * mm; // Aka 1mm Pb filter
const auto filterPbThickThickness = 2.562 * mm; // Aka 2.5mm Pb filter

array<DetectorPosition, 4> labr_positions{
    DetectorPosition{"labr_L1_Sci_01", 90. * degree, 0. * degree, 1. * inch + 52.0 * mm, 0, filterCuThickThickness, 2 * filterPbThickThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"labr_L3_Sci_02", 90. * degree, 90. * degree, 1. * inch + 45.0 * mm, 0, filterCuThickThickness, 2 * filterPbThickThickness, G4ThreeVector(-4. * mm, 0, 0)},
    DetectorPosition{"labr_L5_Sci_03", 90. * degree, 180. * degree, 1. * inch + 38.0 * mm, 0, filterCuThickThickness, 2 * filterPbThickThickness, G4ThreeVector(0, -4. * mm, 0)},
    DetectorPosition{"labr_L7_Sci_04", 90. * degree, 270. * degree, 1. * inch + 40.0 * mm, 0, 2 * filterCuThickThickness, 2 * filterPbThickThickness, G4ThreeVector(0, 0, 0)},
};

array<DetectorPosition, 6> clover_positions{
    DetectorPosition{"clover_B1_CUp_1", 135. * degree, 0. * degree, 8. * inch, 90. * degree, filterCuThickThickness, 2 * filterCuThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"clover_B2_CUp_2", 125.26 * degree, 45. * degree, 8. * inch, 0. * degree, filterCuThickThickness, filterCuThickThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"clover_B4_CUp_3", 125.26 * degree, 135. * degree, 8. * inch, 0. * degree, filterCuThickThickness, filterCuThickThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"clover_B5_CUp_4", 135. * degree, 180. * degree, 8. * inch, 270. * degree, filterCuThickThickness, filterCuThickThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"clover_B6_CDown_1", 125.26 * degree, 225. * degree, 8. * inch, 90. * degree, filterCuThickThickness, 2 * filterCuThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"clover_B8_CDown_2", 125.26 * degree, 315. * degree, 8. * inch, 180. * degree, filterCuThickThickness, filterCuThickThickness, G4ThreeVector(0, 0, 0)},
};

array<DetectorPosition, 10> cebr_positions{
    DetectorPosition{"cebr_B_Sci_05", 90. * degree, 27.5 * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_C_Sci_06", 90. * degree, 45. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_D_Sci_07", 90. * degree, 62.5 * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_F_Sci_08", 90. * degree, 117.5 * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_G_Sci_09", 90. * degree, 135. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_H_Sci_10", 90. * degree, 152.5 * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_K_Sci_11", 90. * degree, 225. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_O_Sci_12", 90. * degree, 315. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_BD_Sci_13", 135. * degree, 90. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
    DetectorPosition{"cebr_BK_Sci_14", 135. * degree, 270. * degree, 8. * inch, 0., filterCuThinThickness, filterPbThinThickness, G4ThreeVector(0, 0, 0)},
};

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {

  const G4Colour invisible(1.0, 1.0, 1.0, 0.);
  const G4Colour white(1.0, 1.0, 1.0);
  const G4Colour lightGrey(0.75, 0.75, 0.75);
  const G4Colour grey(0.5, 0.5, 0.5);
  const G4Colour transparentGrey(0.5, 0.5, 0.5, 0.5);
  const G4Colour darkGrey(0.25, 0.25, 0.25);
  const G4Colour black(0.0, 0.0, 0.0);
  const G4Colour red(1.0, 0.0, 0.0);
  const G4Colour green(0.0, 1.0, 0.0);
  const G4Colour blue(0.0, 0.0, 1.0);
  const G4Colour cyan(0.0, 1.0, 1.0);
  const G4Colour magenta(1.0, 0.0, 1.0);
  const G4Colour yellow(1.0, 1.0, 0.0);
  const G4Colour orange(1.0, 0.5, 0.0);

  G4NistManager *nist = G4NistManager::Instance();

  // =============== Measurements ===============

  const auto feet = 12 * inch;

  // --------------- Room ---------------

  const auto utrRoomLength = 6529. * mm; // Measured off utr drawings
  const auto utrRoomHeight = 3015. * mm; // Distance floor to top-end of metal bars (?). Does the utr even have a proper concrete ceiling? Measured off utr drawings
  const auto utrBeamLineToSideWall = 1713. * mm; // Measured off utr drawings
  const auto utrBeamLineHeight = 1245. * mm; // Distance beam line to floor. Read off utr drawings
  const auto utrFloorThickness = 203. * mm; // Measured off utr drawings
  const auto utrSideWallThickness = 162. * mm; // Measured off utr drawings

  // --------------- General ---------------

  const auto subtractionSolidBuffer = 10. * mm; // G4SubtractionSolid doesn't always fully cut solids when the cutting solid is equal in dimension to the to be cut solid, hence, add some buffer

  // --------------- Beam Pipe ---------------
  const auto beamPipeOuterDiameter = 2. * inch; // From nutr
  const auto beamPipeInnerDiameter = 1.75 * inch; // From nutr
  const auto beamPipeRelativeAirDensity = (1013.25 - 965.) / 1013.25; // From nutr: A vacuum pump can be attached to the beam pipe to evacuate the beam line. If the pump is running at maximum power, a value of 28.5(3) inches Hg was read off on the vacuum gauge, which corresponds to about 965(10) hPa of pressure difference between room and beam pipe or about 4.7% of standard air density/pressure (assuming 1013.25 hPa as standard atmospheric pressure).

  const double beamPipeDownstreamLidToTargetPosition = 23.75 * inch; // From nutr and ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/800

  const double beamPipeInsertPipeLidThickness = 2. * mm; // From nutr: Estimated
  const double beamPipeInsertPipeLidOuterDiameter = 2.16 * inch; // From nutr: Estimated
  const double beamPipeInsertPipeFrontLength = 2. * inch; // From nutr: Estimated
  const double beamPipeInsertPipeFrontInnerDiameter = 1.7 * inch; // From nutr: Estimated
  const double beamPipeInsertPipeMainLength = beamPipeDownstreamLidToTargetPosition - beamPipeInsertPipeLidThickness - beamPipeInsertPipeFrontLength / 2.; // From nutr
  const double beamPipeInsertPipeInnerDiameter = 1.6 * inch; // From nutr

  const double targetRingLength = 1. * inch; // From nutr: Estimated
  const double targetRingInnerDiameter = beamPipeInsertPipeInnerDiameter; // From nutr
  const double targetRingOuterDiameter = beamPipeInsertPipeFrontInnerDiameter; // From nutr

  const double beamPipeLength = 3. * m; // Estimated

  const double beamPipeUpstreamCapLidThickness = 2. * mm; // From nutr
  const double beamPipeUpstreamCapOuterDiameter = 2.3 * inch; // From nutr
  const double beamPipeUpstreamCapLength = 1. * inch; // From nutr

  const auto beamPipeZ = -beamPipeLength / 2. + (beamPipeDownstreamLidToTargetPosition - beamPipeInsertPipeLidThickness);

  // --------------- Upstream ---------------

  const auto utrSecondLeadWallToTargetPos = 46.5 * inch; // From nutr and ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/800
  const auto utrSecondLeadWallLength = 8. * inch; // From nutr
  const auto utrSecondLeadWallWidth = 24. * inch; // From nutr
  const auto utrSecondLeadWallHeight = 20. * inch; // From nutr
  const auto utrSecondLeadWallHoleSize = beamPipeOuterDiameter + 2. * mm; // From nutr: Estimated

  const auto utrFirstLeadWallToTargetPos = 60. * inch; // From nutr: Estimated
  const auto utrFirstLeadWallLength = 8. * inch; // From nutr
  const auto utrFirstLeadWallWidth = 28. * inch; // From nutr
  const auto utrFirstLeadWallHeight = 24. * inch; // From nutr
  const auto utrFirstLeadWallHoleAperture = beamPipeOuterDiameter + 2. * mm; // From nutr: Estimated

  const auto utrUpstreamWallToTargetPos = utrFirstLeadWallToTargetPos + utrFirstLeadWallLength;
  const auto utrUpstreamWallThickness = 162. * mm; // Measured off utr drawings
  const auto utrUpstreamWallHoleAperture = beamPipeOuterDiameter + 2. * mm; // Estimated, not even sure if the hole is cylindrical

  const auto collimatorRoomSecondLeadWallToTargetPos = utrUpstreamWallToTargetPos + utrUpstreamWallThickness;
  const auto collimatorRoomSecondLeadWallLength = 16. * inch; // From nutr
  const auto collimatorRoomSecondLeadWallWidth = 24. * inch; // From nutr
  const auto collimatorRoomSecondLeadWallHeight = 16. * inch; // From nutr
  const auto collimatorRoomSecondLeadWallHoleAperture = beamPipeOuterDiameter + 2. * mm; // From nutr: Estimated

  const auto collimatorRoomFirstLeadWallToTargetPos = collimatorRoomSecondLeadWallToTargetPos + collimatorRoomSecondLeadWallLength + 320. * mm; // From nutr
  const auto collimatorRoomFirstLeadWallLength = 16. * inch; // From nutr
  const auto collimatorRoomFirstLeadWallWidth = 24. * inch; // From nutr
  const auto collimatorRoomFirstLeadWallHeight = 16. * inch; // From nutr
  const auto collimatorRoomFirstLeadWallHoleAperture = beamPipeOuterDiameter + 2. * mm; // From nutr: Estimated

  const auto collimatorRoomCollimatorToTargetPos = collimatorRoomFirstLeadWallToTargetPos + collimatorRoomFirstLeadWallLength + 200. * mm; // From nutr
  const auto collimatorRoomCollimatorLength = 6. * inch; // From Mark Emamian's collimator inventory list
  const auto collimatorRoomCollimatorWidth = 2.5 * inch; // From Mark Emamian's collimator inventory list
  const auto collimatorRoomCollimatorHeight = 2.5 * inch; // From Mark Emamian's collimator inventory list
  const auto collimatorRoomCollimatorAperture = 8. * mm;

  const auto collimatorRoomCutoffPoint = collimatorRoomCollimatorToTargetPos + collimatorRoomCollimatorLength + 100. * mm; // Just some extra space

  // --------------- Downstream ---------------

  const auto utrDownstreamWallToTargetPos = utrRoomLength - utrUpstreamWallToTargetPos;
  const auto utrDownstreamWallThickness = 614. * mm; // Measured off utr drawings
  const auto utrDownstreamWallHoleAperture = beamPipeOuterDiameter + 2. * mm; // Estimated, not even sure if the hole is cylindrical

  const auto beamDumpLength = 203. * mm; // Measured off utr drawings
  const auto beamDumpWidth = 600. * mm; // Measured off utr drawings
  const auto beamDumpHeight = 600. * mm; // Measured off utr drawings
  const auto beamDumpToTargetPos = utrDownstreamWallToTargetPos - beamDumpLength;

  const auto activationTargetHolderToTargetPos = 54.0 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderBaseRodDiameter = 0.5 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderLength = 0.7 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderGrooveLength = activationTargetHolderLength - 2 * 0.04 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderWidth = 1.38 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  // const auto activationTargetHolderGrooveWidth = activationTargetHolderWidth - 2 * 0.15 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderHeight = 0.8 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderGrooveDepth = 0.22 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2025
  const auto activationTargetHolderGrooveDiameter = 1.5 * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2026
  const auto activationTargetHolderBaseRodHeight = 100. * mm; // Estimated
  const auto activationTargetHolderToBeamLine = activationTargetHolderGrooveDiameter / 2. - activationTargetHolderGrooveDepth;

  const auto zeroDegreeToTargetPos = 9. * feet; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/800
  const auto zeroDegreeYOffset = zerodegree_offset * mm; // 23. * mm; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/2027

  // --------------- Filter holder ---------------
  // Filter holder material: PLA (Polylactic Acid, density "1.5297 g/cm3", Wikipedia: density 1.210 to 1.430 g/cm3)
  // Filter holder hole radius: 47mm
  // Filter holder front thickness: 7mm
  // Filter holder side thickness: 4.5mm
  // Filter holder length: 37mm
  // Filter holder width and height: 118mm

  //  =============== Construction ===============
  //  --------------- World ---------------

  const auto worldX = (utrBeamLineToSideWall + utrSideWallThickness) * 2;
  const auto worldY = std::max(utrBeamLineHeight + utrFloorThickness, utrRoomHeight - utrBeamLineHeight) * 2;
  const auto worldZ = std::max(collimatorRoomCutoffPoint, utrDownstreamWallToTargetPos + utrDownstreamWallThickness) * 2;

  auto *worldSolid = new G4Box("worldSolid", worldX / 2., worldY / 2., worldZ / 2.);
  auto *worldLogical = new G4LogicalVolume(worldSolid, nist->FindOrBuildMaterial("G4_AIR"), "worldLogical");
  G4VPhysicalVolume *worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical, "world", nullptr, false, 0);
  auto worldVis = G4VisAttributes(red);
  worldVis.SetForceWireframe(true);
  worldLogical->SetVisAttributes(worldVis);

  //  --------------- Template code for quick test constructions ---------------

  // auto labrTest = LaBr_3x3(worldLogical, "labrTest");
  // labrTest.useHousing();
  // labrTest.Add_Filter("G4_Cu", 2.*mm, -1.);
  // labrTest.Add_Filter("G4_Pb", 4.*mm, -1.);
  // // labrTest.Add_Wrap("G4_Pb", 4.*mm);
  // labrTest.Construct(G4ThreeVector(), 0 , 0 , 0);

  // auto cebrTest = CeBr3_2x2(worldLogical, "cebrTest");
  // cebrTest.Add_Filter("G4_Cu", 2.*mm, -1.);
  // cebrTest.Add_Filter("G4_Pb", 4.*mm, -1.);
  // cebrTest.Construct(G4ThreeVector(), 0 , 0 , 0);

  // HPGe_Collection hpge_Collection2;
  // auto cloverTest = HPGe_Clover(worldLogical, "cloverTest");
  // cloverTest.setProperties(hpge_Collection2.HPGe_Clover_Yale);
  // cloverTest.Add_Filter("G4_Cu", 2.*mm, -1.);
  // cloverTest.Add_Filter("G4_Pb", 4.*mm, -1.);
  // cloverTest.Construct(G4ThreeVector(), 0 , 0 , 0);

  // HPGe_Collection hpge_Collection2;
  // HPGe_Coaxial test(worldLogical, "test");
  // test.setProperties(hpge_Collection2.HPGe_120_TUNL_40383);
  // // test.useDewar();
  // test.Construct(G4ThreeVector(0., 0, 0), 0 * degree, 180. * degree, 0);

  // auto *testBlockSolid = new G4Box("testBlockSolid", 100. * mm / 2., 100. * mm / 2., 100. * mm / 2.);
  // auto *testBlockLogical = new G4LogicalVolume(testBlockSolid, nist->FindOrBuildMaterial("G4_Pb"), "testBlockLogical");
  // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), testBlockLogical, "testBlock", worldLogical, false, 0);
  // testBlockLogical->SetVisAttributes(green);

  // return worldPhysical;

  // --------------- Floor and Walls ---------------

  auto *utrFloorSolid = new G4Box("utrFloorSolid", worldX / 2., utrFloorThickness / 2., worldZ / 2.);
  auto *utrFloorLogical = new G4LogicalVolume(utrFloorSolid, nist->FindOrBuildMaterial("G4_CONCRETE"), "utrFloorLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, -(utrBeamLineHeight + utrFloorThickness / 2.), 0), utrFloorLogical, "utrFloor", worldLogical, false, 0);
  utrFloorLogical->SetVisAttributes(grey);

  auto *utrSideWallSolid = new G4Box("utrSideWallSolid", utrSideWallThickness / 2., utrRoomHeight / 2., utrRoomLength / 2.);
  auto *utrSideWallLogical = new G4LogicalVolume(utrSideWallSolid, nist->FindOrBuildMaterial("G4_CONCRETE"), "utrSideWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(-(utrBeamLineToSideWall + utrSideWallThickness / 2.), utrRoomHeight / 2. - utrBeamLineHeight, utrRoomLength / 2. - utrUpstreamWallToTargetPos), utrSideWallLogical, "utrSideWall", worldLogical, false, 0);
  auto utrSideWallVis = G4VisAttributes(grey);
  utrSideWallVis.SetForceWireframe(true);
  utrSideWallLogical->SetVisAttributes(utrSideWallVis);

  auto *utrUpstreamWallSolidBox = new G4Box("utrUpstreamWallSolidBox", worldX / 2., utrRoomHeight / 2., utrUpstreamWallThickness / 2.);
  auto *utrUpstreamWallSolidHole = new G4Tubs("utrUpstreamWallSolidHole", 0., utrUpstreamWallHoleAperture / 2., utrUpstreamWallThickness / 2. + subtractionSolidBuffer, 0., twopi);
  auto *utrUpstreamWallSolid = new G4SubtractionSolid("utrUpstreamWallSolid", utrUpstreamWallSolidBox, utrUpstreamWallSolidHole, nullptr, G4ThreeVector(0, -(utrRoomHeight / 2. - utrBeamLineHeight), 0));
  auto *utrUpstreamWallLogical = new G4LogicalVolume(utrUpstreamWallSolid, nist->FindOrBuildMaterial("G4_CONCRETE"), "utrUpstreamWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, utrRoomHeight / 2. - utrBeamLineHeight, -(utrUpstreamWallToTargetPos + utrUpstreamWallThickness / 2.)), utrUpstreamWallLogical, "utrUpstreamWall", worldLogical, false, 0);
  utrUpstreamWallLogical->SetVisAttributes(transparentGrey);

  auto *utrDownstreamWallSolidBox = new G4Box("utrDownstreamWallSolidBox", worldX / 2., utrRoomHeight / 2., utrDownstreamWallThickness / 2.);
  auto *utrDownstreamWallSolidHole = new G4Tubs("utrDownstreamWallSolidHole", 0., utrDownstreamWallHoleAperture / 2., utrDownstreamWallThickness / 2. + subtractionSolidBuffer, 0., twopi);
  auto *utrDownstreamWallSolid = new G4SubtractionSolid("utrDownstreamWallSolid", utrDownstreamWallSolidBox, utrDownstreamWallSolidHole, nullptr, G4ThreeVector(0, -(utrRoomHeight / 2. - utrBeamLineHeight), 0));
  auto *utrDownstreamWallLogical = new G4LogicalVolume(utrDownstreamWallSolid, nist->FindOrBuildMaterial("G4_CONCRETE"), "utrDownstreamWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, utrRoomHeight / 2. - utrBeamLineHeight, utrDownstreamWallToTargetPos + utrDownstreamWallThickness / 2.), utrDownstreamWallLogical, "utrDownstreamWall", worldLogical, false, 0);
  utrDownstreamWallLogical->SetVisAttributes(transparentGrey);

  // --------------- Collimator ---------------

  // Works, but the visualization of the collimator is nicer with the G4SubtractionSolid (actual hole instead of a cylinder inside the box)
  // auto *collimatorSolid = new G4Box("collimatorSolidSubstractionBox", collimatorRoomCollimatorWidth / 2., collimatorRoomCollimatorHeight / 2., collimatorRoomCollimatorLength / 2.);
  // auto *collimatorLogical = new G4LogicalVolume(collimatorSolid, nist->FindOrBuildMaterial("G4_Pb"), "collimatorLogical");
  // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(collimatorRoomCollimatorToTargetPos + collimatorRoomCollimatorLength / 2.)), collimatorLogical, "collimator", worldLogical, false, 0);
  // collimatorLogical->SetVisAttributes(green);
  // auto *collimatorHoleSolid = new G4Tubs("collimatorHoleSolid", 0., collimatorRoomCollimatorAperture/2., collimatorRoomCollimatorLength / 2., 0., twopi);
  // auto *collimatorHoleLogical = new G4LogicalVolume(collimatorHoleSolid, nist->FindOrBuildMaterial("G4_AIR"), "collimatorHoleLogical");
  // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), collimatorHoleLogical, "collimatorHole", collimatorLogical, false, 0);
  // collimatorHoleLogical->SetVisAttributes(invisible);

  auto *collimatorSolidSubstractionBox = new G4Box("collimatorSolidSubstractionBox", collimatorRoomCollimatorWidth / 2., collimatorRoomCollimatorHeight / 2., collimatorRoomCollimatorLength / 2.);
  auto *collimatorSolidSubstractionHole = new G4Tubs("collimatorSolidSubstractionHole", 0., collimatorRoomCollimatorAperture / 2., collimatorRoomCollimatorLength / 2. + subtractionSolidBuffer, 0., twopi);
  auto *collimatorSolid = new G4SubtractionSolid("collimatorSolid", collimatorSolidSubstractionBox, collimatorSolidSubstractionHole);
  auto *collimatorLogical = new G4LogicalVolume(collimatorSolid, nist->FindOrBuildMaterial("G4_Pb"), "collimatorLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(collimatorRoomCollimatorToTargetPos + collimatorRoomCollimatorLength / 2.)), collimatorLogical, "collimator", worldLogical, false, 0);
  collimatorLogical->SetVisAttributes(green);

  // --------------- Collimator Room Pb Walls ---------------

  auto *collimatorRoomFirstLeadWallSolidBox = new G4Box("collimatorRoomFirstLeadWallSolidBox", collimatorRoomFirstLeadWallWidth / 2., collimatorRoomFirstLeadWallHeight / 2., collimatorRoomFirstLeadWallLength / 2.);
  auto *collimatorRoomFirstLeadWallSolidHole = new G4Tubs("collimatorRoomFirstLeadWallSolidHole", 0., collimatorRoomFirstLeadWallHoleAperture / 2., collimatorRoomFirstLeadWallLength / 2. + subtractionSolidBuffer, 0., twopi);
  auto *collimatorRoomFirstLeadWallSolid = new G4SubtractionSolid("collimatorRoomFirstLeadWallSolid", collimatorRoomFirstLeadWallSolidBox, collimatorRoomFirstLeadWallSolidHole);
  auto *collimatorRoomFirstLeadWallLogical = new G4LogicalVolume(collimatorRoomFirstLeadWallSolid, nist->FindOrBuildMaterial("G4_Pb"), "collimatorRoomFirstLeadWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(collimatorRoomFirstLeadWallToTargetPos + collimatorRoomFirstLeadWallLength / 2.)), collimatorRoomFirstLeadWallLogical, "collimatorRoomFirstLeadWall", worldLogical, false, 0);
  collimatorRoomFirstLeadWallLogical->SetVisAttributes(green);

  auto *collimatorRoomSecondLeadWallSolidBox = new G4Box("collimatorRoomSecondLeadWallSolidBox", collimatorRoomSecondLeadWallWidth / 2., collimatorRoomSecondLeadWallHeight / 2., collimatorRoomSecondLeadWallLength / 2.);
  auto *collimatorRoomSecondLeadWallSolidHole = new G4Tubs("collimatorRoomSecondLeadWallSolidHole", 0., collimatorRoomSecondLeadWallHoleAperture / 2., collimatorRoomSecondLeadWallLength / 2. + subtractionSolidBuffer, 0., twopi);
  auto *collimatorRoomSecondLeadWallSolid = new G4SubtractionSolid("collimatorRoomSecondLeadWallSolid", collimatorRoomSecondLeadWallSolidBox, collimatorRoomSecondLeadWallSolidHole);
  auto *collimatorRoomSecondLeadWallLogical = new G4LogicalVolume(collimatorRoomSecondLeadWallSolid, nist->FindOrBuildMaterial("G4_Pb"), "collimatorRoomSecondLeadWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(collimatorRoomSecondLeadWallToTargetPos + collimatorRoomSecondLeadWallLength / 2.)), collimatorRoomSecondLeadWallLogical, "collimatorRoomSecondLeadWall", worldLogical, false, 0);
  collimatorRoomSecondLeadWallLogical->SetVisAttributes(green);

  // --------------- UTR Pb Walls ---------------

  auto *utrFirstLeadWallSolidBox = new G4Box("utrFirstLeadWallSolidBox", utrFirstLeadWallWidth / 2., utrFirstLeadWallHeight / 2., utrFirstLeadWallLength / 2.);
  auto *utrFirstLeadWallSolidHole = new G4Tubs("utrFirstLeadWallSolidHole", 0., utrFirstLeadWallHoleAperture / 2., utrFirstLeadWallLength / 2. + subtractionSolidBuffer, 0., twopi);
  auto *utrFirstLeadWallSolid = new G4SubtractionSolid("utrFirstLeadWallSolid", utrFirstLeadWallSolidBox, utrFirstLeadWallSolidHole);
  auto *utrFirstLeadWallLogical = new G4LogicalVolume(utrFirstLeadWallSolid, nist->FindOrBuildMaterial("G4_Pb"), "utrFirstLeadWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(utrFirstLeadWallToTargetPos + utrFirstLeadWallLength / 2.)), utrFirstLeadWallLogical, "utrFirstLeadWall", worldLogical, false, 0);
  utrFirstLeadWallLogical->SetVisAttributes(green);

  auto *utrSecondLeadWallSolidBox = new G4Box("utrSecondLeadWallSolidBox", utrSecondLeadWallWidth / 2., utrSecondLeadWallHeight / 2., utrSecondLeadWallLength / 2.);
  auto *utrSecondLeadWallSolidHole = new G4Box("utrSecondLeadWallSolidHole", utrSecondLeadWallHoleSize / 2., utrSecondLeadWallHoleSize / 2., utrSecondLeadWallLength / 2. + subtractionSolidBuffer);
  auto *utrSecondLeadWallSolid = new G4SubtractionSolid("utrSecondLeadWallSolid", utrSecondLeadWallSolidBox, utrSecondLeadWallSolidHole);
  auto *utrSecondLeadWallLogical = new G4LogicalVolume(utrSecondLeadWallSolid, nist->FindOrBuildMaterial("G4_Pb"), "utrSecondLeadWallLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(utrSecondLeadWallToTargetPos + utrSecondLeadWallLength / 2.)), utrSecondLeadWallLogical, "utrSecondLeadWall", worldLogical, false, 0);
  utrSecondLeadWallLogical->SetVisAttributes(green);

  // --------------- Activation Target Holder ---------------

  auto *activationTargetHolderSolidBox = new G4Box("activationTargetHolderSolidBox", activationTargetHolderWidth / 2., activationTargetHolderHeight / 2., activationTargetHolderLength / 2.);
  auto *activationTargetHolderSolidGroove = new G4Tubs("activationTargetHolderSolidGroove", 0., activationTargetHolderGrooveDiameter / 2., activationTargetHolderGrooveLength / 2., 0., twopi);
  auto *activationTargetHolderSolid = new G4SubtractionSolid("activationTargetHolderSolid", activationTargetHolderSolidBox, activationTargetHolderSolidGroove, nullptr, G4ThreeVector(0., activationTargetHolderHeight / 2. + activationTargetHolderGrooveDiameter / 2. - activationTargetHolderGrooveDepth, 0.));
  auto *activationTargetHolderLogical = new G4LogicalVolume(activationTargetHolderSolid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "activationTargetHolderLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, -(activationTargetHolderToBeamLine + activationTargetHolderHeight / 2.), activationTargetHolderToTargetPos + activationTargetHolderLength / 2.), activationTargetHolderLogical, "activationTargetHolder", worldLogical, false, 0);

  auto *activationTargetHolderBaseRodSolid = new G4Tubs("activationTargetHolderBaseRodSolid", 0., activationTargetHolderBaseRodDiameter / 2., activationTargetHolderBaseRodHeight / 2., 0., twopi);
  auto *activationTargetHolderBaseRodLogical = new G4LogicalVolume(activationTargetHolderBaseRodSolid, nist->FindOrBuildMaterial("G4_Al"), "activationTargetHolderBaseRodLogical");
  new G4PVPlacement(new G4RotationMatrix(G4ThreeVector(1., 0., 0.), 90. * degree), G4ThreeVector(0, -(activationTargetHolderToBeamLine + activationTargetHolderHeight + activationTargetHolderBaseRodHeight / 2.), activationTargetHolderToTargetPos + activationTargetHolderLength / 2.), activationTargetHolderBaseRodLogical, "activationTargetHolderBaseRod", worldLogical, false, 0);

  // --------------- Beam Dump ---------------

  auto *beamDumpSolid = new G4Box("beamDumpSolid", beamDumpWidth / 2., beamDumpHeight / 2., beamDumpLength / 2.);
  auto *beamDumpLogical = new G4LogicalVolume(beamDumpSolid, nist->FindOrBuildMaterial("G4_Pb"), "beamDumpLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamDumpToTargetPos + beamDumpLength / 2.), beamDumpLogical, "beamDump", worldLogical, false, 0);
  beamDumpLogical->SetVisAttributes(green);

  // --------------- Beam Pipe ---------------
  // Main pipe

  auto *beamPipeVacuumMaterial = nist->ConstructNewGasMaterial("beamPipeVacuum", "G4_AIR", 293. * kelvin, beamPipeRelativeAirDensity * atmosphere);

  auto *beamPipeSolid = new G4Tubs("beamPipeSolid", 0., beamPipeOuterDiameter / 2., beamPipeLength / 2., 0., twopi);
  auto *beamPipeLogical = new G4LogicalVolume(beamPipeSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamPipeZ), beamPipeLogical, "beamPipe", worldLogical, false, 0);

  auto *beamPipeVacuumSolid = new G4Tubs("beamPipeVacuumSolid", 0., beamPipeInnerDiameter / 2., beamPipeLength / 2., 0., twopi);
  auto *beamPipeVacuumLogical = new G4LogicalVolume(beamPipeVacuumSolid, beamPipeVacuumMaterial, "beamPipeVacuumLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), beamPipeVacuumLogical, "beamPipeVacuum", beamPipeLogical, false, 0);
  beamPipeVacuumLogical->SetVisAttributes(cyan);

  // Upstream cap

  auto *beamPipeUpstreamCapSolid = new G4Tubs("beamPipeUpstreamCapSolid", beamPipeOuterDiameter / 2., beamPipeUpstreamCapOuterDiameter / 2., beamPipeUpstreamCapLength / 2., 0., twopi);
  auto *beamPipeUpstreamCapLogical = new G4LogicalVolume(beamPipeUpstreamCapSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeUpstreamCapLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamPipeZ - (beamPipeLength / 2. - beamPipeUpstreamCapLength / 2.)), beamPipeUpstreamCapLogical, "beamPipeUpstreamCap", worldLogical, false, 0);

  auto *beamPipeUpstreamCapLidSolid = new G4Tubs("beamPipeUpstreamCapLidSolid", 0., beamPipeUpstreamCapOuterDiameter / 2., beamPipeUpstreamCapLidThickness / 2., 0., twopi);
  auto *beamPipeUpstreamCapLidLogical = new G4LogicalVolume(beamPipeUpstreamCapLidSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeUpstreamCapLidLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamPipeZ - (beamPipeLength / 2. + beamPipeUpstreamCapLidThickness / 2.)), beamPipeUpstreamCapLidLogical, "beamPipeUpstreamCapLid", worldLogical, false, 0);

  // Insert pipe

  auto *beamPipeInsertPipeLidSolid = new G4Tubs("beamPipeInsertPipeLidSolid", 0., beamPipeInsertPipeLidOuterDiameter / 2., beamPipeInsertPipeLidThickness / 2., 0., twopi);
  auto *beamPipeInsertPipeLidLogical = new G4LogicalVolume(beamPipeInsertPipeLidSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeInsertPipeLidLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamPipeZ + (beamPipeLength + beamPipeInsertPipeLidThickness) / 2.), beamPipeInsertPipeLidLogical, "beamPipeInsertPipeLid", worldLogical, false, 0);

  auto *beamPipeInsertPipeSolid = new G4Tubs("beamPipeInsertPipeSolid", beamPipeInsertPipeInnerDiameter / 2., beamPipeInnerDiameter / 2., beamPipeInsertPipeMainLength / 2., 0., twopi);
  auto *beamPipeInsertPipeLogical = new G4LogicalVolume(beamPipeInsertPipeSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeInsertPipeLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, beamPipeLength / 2. - beamPipeInsertPipeMainLength / 2.), beamPipeInsertPipeLogical, "beamPipeInsertPipe", beamPipeVacuumLogical, false, 0);

  auto *beamPipeInsertPipeFrontSolid = new G4Tubs("beamPipeInsertPipeFrontSolid", beamPipeInsertPipeFrontInnerDiameter / 2., beamPipeInnerDiameter / 2., beamPipeInsertPipeFrontLength / 2., 0., twopi);
  auto *beamPipeInsertPipeFrontLogical = new G4LogicalVolume(beamPipeInsertPipeFrontSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "beamPipeInsertPipeFrontLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), beamPipeInsertPipeFrontLogical, "beamPipeInsertPipeFront", beamPipeVacuumLogical, false, 0);

  auto *targetRingSolid = new G4Tubs("targetRingSolid", targetRingInnerDiameter / 2., targetRingOuterDiameter / 2., targetRingLength / 2., 0., twopi);
  auto *targetRingLogical = new G4LogicalVolume(targetRingSolid, nist->FindOrBuildMaterial("G4_PLEXIGLASS"), "targetRingLogical");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), targetRingLogical, "targetRing", beamPipeVacuumLogical, false, 0);
  targetRingLogical->SetVisAttributes(darkGrey);

  // --------------- Detectors ---------------

  HPGe_Collection hpge_Collection;
  vector<HPGe_Clover> clovers;
  for (auto det_pos : clover_positions) {
    clovers.push_back(HPGe_Clover(worldLogical, det_pos.id));
    clovers[clovers.size() - 1].setProperties(hpge_Collection.HPGe_Clover_Yale);
    clovers[clovers.size() - 1].useDewar();
    clovers[clovers.size() - 1].Add_Filter("G4_Cu", det_pos.filterThicknessCu, -1.);
    clovers[clovers.size() - 1].Add_Filter("G4_Pb", det_pos.filterThicknessPb, -1.);
    clovers[clovers.size() - 1].Construct(det_pos.targetPosOffset, det_pos.theta, det_pos.phi, det_pos.distance, det_pos.intrinsicRotationAngle);
  }

  vector<LaBr_3x3> labrs;
  for (auto det_pos : labr_positions) {
    labrs.push_back(LaBr_3x3(worldLogical, det_pos.id));
    labrs[labrs.size() - 1].useHousing();
    labrs[labrs.size() - 1].Add_Filter("G4_Cu", det_pos.filterThicknessCu, -1.);
    labrs[labrs.size() - 1].Add_Filter("G4_Pb", det_pos.filterThicknessPb, -1.);
    labrs[labrs.size() - 1].Construct(det_pos.targetPosOffset, det_pos.theta, det_pos.phi, det_pos.distance);
  }

  vector<CeBr3_2x2> cebrs;
  for (auto det_pos : cebr_positions) {
    cebrs.push_back(CeBr3_2x2(worldLogical, det_pos.id));
    cebrs[cebrs.size() - 1].Add_Filter("G4_Cu", det_pos.filterThicknessCu, -1.);
    cebrs[cebrs.size() - 1].Add_Filter("G4_Pb", det_pos.filterThicknessPb, -1.);
    cebrs[cebrs.size() - 1].Construct(det_pos.targetPosOffset, det_pos.theta, det_pos.phi, det_pos.distance);
  }

#ifdef USE_ZERODEGREE
  HPGe_Coaxial zerodegree(worldLogical, "ZeroDegree");
  zerodegree.setProperties(hpge_Collection.HPGe_120_TUNL_40383);
  zerodegree.Construct(G4ThreeVector(0., zeroDegreeYOffset, zeroDegreeToTargetPos), 0 * degree, 180. * degree, 0 * mm);
#endif

  // --------------- Targets ---------------
  // TARGET variable content is supplied by the cmake variable TARGET supplied via the -DTARGET=VALUE argument to cmake
  // Geometric beam widening from 8mm collimator to target position:
  // From H.R. Weller et al. Prog. Part. Nucl. Phys. 62, 257 (2009): "collimator is located about 60 m away from the collision point."
  // From U. Friman-Gayer and S. Finch private communication: "It is 53 m from the beam collision point to collimator"
  const auto collisionPointToCollimator = 53. * m;
  const auto collisionPointToTargetPos = collisionPointToCollimator + collimatorRoomCollimatorToTargetPos;
  const auto beamDiameterAtTargetPos = collimatorRoomCollimatorAperture * collisionPointToTargetPos / collisionPointToCollimator;
  // const auto beamDiameterAtTargetPos = std::floor(beamDiameterAtTargetPosExact /(0.1*mm)) * (0.1*mm);

  std::cout << "DetectorConstruction: Requested TARGET is '" << TARGET << "'\n";
  if (TARGET == "154Sm") {
    const auto target154SmContainerWallThickness = 1. * mm; // From target container technical drawings, attached to ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/829
    const auto target154SmContainerLength = 11. * mm; // From target container technical drawings
    const auto target154SmContainerOuterDiameter = 14. * mm; // From target container technical drawings
    const auto target154SmContainerInnerLength = target154SmContainerLength - 3. * target154SmContainerWallThickness; // From target container technical drawings
    const auto target154SmContainerInnerDiameter = target154SmContainerOuterDiameter - 2. * target154SmContainerWallThickness; // From target container technical drawings
    const auto target154SmContainerLidRecessInnerDiameter = target154SmContainerInnerDiameter - 2. * target154SmContainerWallThickness; // From target container technical drawings
    const auto target154SmBothContainersLength = 2. * target154SmContainerLength; // Both containers were taped together bottom-to-bottom, so construct both as a single volume
    const auto target154SmMass = ((2.01160 - 0.52290 - 0.13625) + (2.01906 - 0.49394 - 0.13619)) * g; // Total used mass, from ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/829
    const auto target154SmDensity = target154SmMass / (pi / 4. * target154SmContainerInnerDiameter * target154SmContainerInnerDiameter * 2. * target154SmContainerInnerLength);

    auto *target154SmEnrichedSm = new G4Element("target154SmEnrichedSm", "Sm154e", 6);
    target154SmEnrichedSm->AddIsotope(new G4Isotope("147Sm", 62, 147), 0.15 * perCent); // From Trace Sciences International Inc. Certificate of Analysis #96-3a ("assay") available in ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/844
    target154SmEnrichedSm->AddIsotope(new G4Isotope("148Sm", 62, 148), 0.20 * perCent); // Geant4 would name it Sm148, but when we construct it ourselfs already, better give it a different name
    target154SmEnrichedSm->AddIsotope(new G4Isotope("149Sm", 62, 149), 0.22 * perCent);
    target154SmEnrichedSm->AddIsotope(new G4Isotope("150Sm", 62, 150), 0.15 * perCent);
    target154SmEnrichedSm->AddIsotope(new G4Isotope("152Sm", 62, 152), 0.78 * perCent);
    target154SmEnrichedSm->AddIsotope(new G4Isotope("154Sm", 62, 154), 98.50 * perCent);

    auto *target154SmMaterial = new G4Material("target154SmMaterial", target154SmDensity, 2);
    target154SmMaterial->AddElement(target154SmEnrichedSm, 2); // 2 Sm atoms in Sm(2)O(3)
    target154SmMaterial->AddElement(nist->FindOrBuildElement("O"), 3); // 3 O atoms in Sm(2)O(3)

    auto *target154SmBothContainersNoLidRecessSolid = new G4Tubs("target154SmBothContainersNoLidRecessSolid", 0, target154SmContainerOuterDiameter / 2., target154SmBothContainersLength / 2., 0., twopi);
    auto *target154SmContainerLidRecessSolid = new G4Tubs("target154SmContainerLidRecessSolid", 0, target154SmContainerLidRecessInnerDiameter / 2., target154SmContainerWallThickness, 0., twopi);
    auto *target154SmBothContainersIntermediateSolid = new G4SubtractionSolid("target154SmBothContainersIntermediateSolid", target154SmBothContainersNoLidRecessSolid, target154SmContainerLidRecessSolid, nullptr, G4ThreeVector(0, 0, target154SmBothContainersLength / 2.));
    auto *target154SmBothContainersSolid = new G4SubtractionSolid("target154SmBothContainersSolid", target154SmBothContainersIntermediateSolid, target154SmContainerLidRecessSolid, nullptr, G4ThreeVector(0, 0, -target154SmBothContainersLength / 2.));
    auto *target154SmBothContainersLogical = new G4LogicalVolume(target154SmBothContainersSolid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "target154SmBothContainersLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), target154SmBothContainersLogical, "target154SmBothContainers", beamPipeVacuumLogical, false, 0);
    target154SmBothContainersLogical->SetVisAttributes(lightGrey);

    auto *target154SmFirstMaterialSolid = new G4Tubs("target154SmFirstMaterialSolid", 0, target154SmContainerInnerDiameter / 2., target154SmContainerInnerLength / 2., 0., twopi);
    auto *target154SmFirstMaterialLogical = new G4LogicalVolume(target154SmFirstMaterialSolid, target154SmMaterial, "target154SmFirstMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -(target154SmContainerInnerLength / 2. + target154SmContainerWallThickness)), target154SmFirstMaterialLogical, "target154SmFirstMaterial", target154SmBothContainersLogical, false, 0);
    target154SmFirstMaterialLogical->SetVisAttributes(yellow);

    auto *target154SmFirstMaterialIrradiatedSolid = new G4Tubs("target154SmFirstMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, target154SmContainerInnerDiameter) / 2., target154SmContainerInnerLength / 2., 0., twopi);
    auto *target154SmFirstMaterialIrradiatedLogical = new G4LogicalVolume(target154SmFirstMaterialIrradiatedSolid, target154SmMaterial, "target154SmFirstMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), target154SmFirstMaterialIrradiatedLogical, "target154SmFirstMaterialIrradiated", target154SmFirstMaterialLogical, false, 0);
    target154SmFirstMaterialIrradiatedLogical->SetVisAttributes(orange);

    auto *target154SmSecondMaterialSolid = new G4Tubs("target154SmSecondMaterialSolid", 0, target154SmContainerInnerDiameter / 2., target154SmContainerInnerLength / 2., 0., twopi);
    auto *target154SmSecondMaterialLogical = new G4LogicalVolume(target154SmSecondMaterialSolid, target154SmMaterial, "target154SmSecondMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, target154SmContainerInnerLength / 2. + target154SmContainerWallThickness), target154SmSecondMaterialLogical, "target154SmSecondMaterial", target154SmBothContainersLogical, false, 0);
    target154SmSecondMaterialLogical->SetVisAttributes(yellow);

    auto *target154SmSecondMaterialIrradiatedSolid = new G4Tubs("target154SmSecondMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, target154SmContainerInnerDiameter) / 2., target154SmContainerInnerLength / 2., 0., twopi);
    auto *target154SmSecondMaterialIrradiatedLogical = new G4LogicalVolume(target154SmSecondMaterialIrradiatedSolid, target154SmMaterial, "target154SmSecondMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), target154SmSecondMaterialIrradiatedLogical, "target154SmSecondMaterialIrradiated", target154SmSecondMaterialLogical, false, 0);
    target154SmSecondMaterialIrradiatedLogical->SetVisAttributes(orange);

  } else if (TARGET == "140Ce") {
    const auto target140CeContainerWallThickness = 1. * mm; // Approximate measurement on closed and tape-wrapped target container
    const auto target140CeContainerLength = 6. * mm; // Approximate measurement on closed and tape-wrapped target container
    const auto target140CeContainerOuterDiameter = 22. * mm; // Approximate measurement on closed and tape-wrapped target container
    const auto target140CeContainerInnerLength = target140CeContainerLength - 3 * target140CeContainerWallThickness; // Assumed
    const auto target140CeContainerInnerDiameter = target140CeContainerOuterDiameter - 2 * target140CeContainerWallThickness; // Assumed
    const auto target140CeContainerLidRecessInnerDiameter = target140CeContainerInnerDiameter - 2. * target140CeContainerWallThickness; // Assumed, has a recessed lid and it looks like this
    const auto target140CeMass = 2.3 * g; // Total used mass, label on target, from ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/98
    const auto target140CeDensity = target140CeMass / (pi / 4. * target140CeContainerInnerDiameter * target140CeContainerInnerDiameter * target140CeContainerInnerLength);

    auto *target140CeEnrichedCe = new G4Element("target140CeEnrichedCe", "Ce140e", 2);
    target140CeEnrichedCe->AddIsotope(new G4Isotope("140Ce", 58, 140), 99.5 * perCent); // From Chemotrade GmbH Certificate of Analysis 58-02-140-1314 ("assay") available in ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/98
    target140CeEnrichedCe->AddIsotope(new G4Isotope("142Ce", 58, 142), 0.5 * perCent); // Geant4 would name it Ce142, but when we construct it ourselfs already, better give it a different name

    auto *target140CeMaterial = new G4Material("target140CeMaterial", target140CeDensity, 2);
    target140CeMaterial->AddElement(target140CeEnrichedCe, 1); // 1 Ce atoms in CeO(2)
    target140CeMaterial->AddElement(nist->FindOrBuildElement("O"), 2); // 2 O atoms in CeO(2)

    auto *target140CeContainerNoLidRecessSolid = new G4Tubs("target140CeContainerNoLidRecessSolid", 0, target140CeContainerOuterDiameter / 2., target140CeContainerLength / 2., 0., twopi);
    auto *target140CeContainerLidRecessSolid = new G4Tubs("target140CeContainerLidRecessSolid", 0, target140CeContainerLidRecessInnerDiameter / 2., target140CeContainerWallThickness, 0., twopi);
    auto *target140CeContainerSolid = new G4SubtractionSolid("target140CeContainerSolid", target140CeContainerNoLidRecessSolid, target140CeContainerLidRecessSolid, nullptr, G4ThreeVector(0, 0, target140CeContainerLength / 2.));
    auto *target140CeContainerLogical = new G4LogicalVolume(target140CeContainerSolid, nist->FindOrBuildMaterial("G4_POLYETHYLENE"), "target140CeContainerLogical"); // Polyethylene is an assumption
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), target140CeContainerLogical, "target140CeContainer", beamPipeVacuumLogical, false, 0);
    target140CeContainerLogical->SetVisAttributes(lightGrey);

    auto *target140CeMaterialSolid = new G4Tubs("target140CeMaterialSolid", 0, target140CeContainerInnerDiameter / 2., target140CeContainerInnerLength / 2., 0., twopi);
    auto *target140CeMaterialLogical = new G4LogicalVolume(target140CeMaterialSolid, target140CeMaterial, "target140CeMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -target140CeContainerWallThickness / 2), target140CeMaterialLogical, "target140CeMaterial", target140CeContainerLogical, false, 0);
    target140CeMaterialLogical->SetVisAttributes(yellow);

    auto *target140CeMaterialIrradiatedSolid = new G4Tubs("target140CeMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, target140CeContainerInnerDiameter) / 2., target140CeContainerInnerLength / 2., 0., twopi);
    auto *target140CeMaterialIrradiatedLogical = new G4LogicalVolume(target140CeMaterialIrradiatedSolid, target140CeMaterial, "target140CeMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), target140CeMaterialIrradiatedLogical, "target140CeMaterialIrradiated", target140CeMaterialLogical, false, 0);
    target140CeMaterialIrradiatedLogical->SetVisAttributes(orange);

  } else if (TARGET == "natC") {
    const auto targetNatCLength = 6.6 * mm; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/843
    const auto targetNatCDiameter = 13. * mm; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/843

    const auto targetNatCMass = 1.38648 * g; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/843
    const auto targetNatCDensity = targetNatCMass / (pi / 4. * targetNatCDiameter * targetNatCDiameter * targetNatCLength);
    auto *targetNatCMaterial = new G4Material("targetNatCMaterial", targetNatCDensity, nist->FindOrBuildMaterial("G4_GRAPHITE"));

    auto *targetNatCMaterialSolid = new G4Tubs("targetNatCMaterialSolid", 0, targetNatCDiameter / 2., targetNatCLength / 2., 0., twopi);
    auto *targetNatCMaterialLogical = new G4LogicalVolume(targetNatCMaterialSolid, targetNatCMaterial, "targetNatCMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), targetNatCMaterialLogical, "targetNatCMaterial", beamPipeVacuumLogical, false, 0);
    targetNatCMaterialLogical->SetVisAttributes(yellow);

    auto *targetNatCMaterialIrradiatedSolid = new G4Tubs("targetNatCMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, targetNatCDiameter) / 2., targetNatCLength / 2., 0., twopi);
    auto *targetNatCMaterialIrradiatedLogical = new G4LogicalVolume(targetNatCMaterialIrradiatedSolid, targetNatCMaterial, "targetNatCMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), targetNatCMaterialIrradiatedLogical, "targetNatCMaterialIrradiated", targetNatCMaterialLogical, false, 0);
    targetNatCMaterialIrradiatedLogical->SetVisAttributes(orange);

  } else if (TARGET == "natC XL") {
    const auto targetNatCXLDiameter = 1. * inch; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/980
    const auto targetNatCXLMass = 24.9 * g; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/980
    const auto targetNatCXLDensity = 2. * g / cm3; // Assumption
    const auto targetNatCXLLength = targetNatCXLMass / targetNatCXLDensity / (pi / 4. * targetNatCXLDiameter * targetNatCXLDiameter);

    auto *targetNatCXLMaterial = new G4Material("targetNatCXLMaterial", targetNatCXLDensity, nist->FindOrBuildMaterial("G4_GRAPHITE"));

    auto *targetNatCXLMaterialSolid = new G4Tubs("targetNatCXLMaterialSolid", 0, targetNatCXLDiameter / 2., targetNatCXLLength / 2., 0., twopi);
    auto *targetNatCXLMaterialLogical = new G4LogicalVolume(targetNatCXLMaterialSolid, targetNatCXLMaterial, "targetNatCXLMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), targetNatCXLMaterialLogical, "targetNatCXLMaterial", beamPipeVacuumLogical, false, 0);
    targetNatCXLMaterialLogical->SetVisAttributes(yellow);

    auto *targetNatCXLMaterialIrradiatedSolid = new G4Tubs("targetNatCXLMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, targetNatCXLDiameter) / 2., targetNatCXLLength / 2., 0., twopi);
    auto *targetNatCXLMaterialIrradiatedLogical = new G4LogicalVolume(targetNatCXLMaterialIrradiatedSolid, targetNatCXLMaterial, "targetNatCXLMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), targetNatCXLMaterialIrradiatedLogical, "targetNatCXLMaterialIrradiated", targetNatCXLMaterialLogical, false, 0);
    targetNatCXLMaterialIrradiatedLogical->SetVisAttributes(orange);

  } else if (TARGET == "natSi") {
    const auto targetNatSiDiameter = 16.4 * mm; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/820 and https://elog.ikp.physik.tu-darmstadt.de/clovershare/894
    const auto targetNatSiMass = 2.545 * g; // From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/820 and https://elog.ikp.physik.tu-darmstadt.de/clovershare/894
    // const auto targetNatSiLength = 200. / 1000. * inch; // Writing on box: "Thickness: 200 mils" From ELOG https://elog.ikp.physik.tu-darmstadt.de/clovershare/820
    // const auto targetNatSiDensity = targetNatSiMass / (pi / 4. * targetNatSiDiameter * targetNatSiDiameter * targetNatSiLength);
    const auto targetNatSiDensity = 2.33 * g / cm3; // Rather use default Geant4 Si density and compute the target to get the overall mass right
    const auto targetNatSiLength = targetNatSiMass / targetNatSiDensity / (pi / 4. * targetNatSiDiameter * targetNatSiDiameter); // This gives 5.17mm which matches the 5.08mm from the box label very well

    auto *targetNatSiMaterial = nist->FindOrBuildMaterial("G4_Si");

    auto *targetNatSiMaterialSolid = new G4Tubs("targetNatSiMaterialSolid", 0, targetNatSiDiameter / 2., targetNatSiLength / 2., 0., twopi);
    auto *targetNatSiMaterialLogical = new G4LogicalVolume(targetNatSiMaterialSolid, targetNatSiMaterial, "targetNatSiMaterialLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -beamPipeZ), targetNatSiMaterialLogical, "targetNatSiMaterial", beamPipeVacuumLogical, false, 0);
    targetNatSiMaterialLogical->SetVisAttributes(yellow);

    auto *targetNatSiMaterialIrradiatedSolid = new G4Tubs("targetNatSiMaterialIrradiatedSolid", 0, std::min(beamDiameterAtTargetPos, targetNatSiDiameter) / 2., targetNatSiLength / 2., 0., twopi);
    auto *targetNatSiMaterialIrradiatedLogical = new G4LogicalVolume(targetNatSiMaterialIrradiatedSolid, targetNatSiMaterial, "targetNatSiMaterialIrradiatedLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), targetNatSiMaterialIrradiatedLogical, "targetNatSiMaterialIrradiated", targetNatSiMaterialLogical, false, 0);
    targetNatSiMaterialIrradiatedLogical->SetVisAttributes(orange);

  } else if (TARGET == "NONE") {

  } else {
    G4cerr << "ERROR: Unknown TARGET '" << TARGET << "' was requested for the DetectorConstruction! Aborting..." << G4endl;
    throw std::exception();
  }

  return worldPhysical;
}

void DetectorConstruction::ConstructSDandField() {
  int detIDNo = 0;
  for (auto det_pos : labr_positions) {
    detIDNo++;
    auto detName = det_pos.id;
    EnergyDepositionSD *sensitiveDet = new EnergyDepositionSD(detName, detName);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDet);
    sensitiveDet->SetDetectorID(detIDNo);
    SetSensitiveDetector(detName, sensitiveDet, true);
  }
  for (auto det_pos : cebr_positions) {
    detIDNo++;
    auto detName = det_pos.id;
    EnergyDepositionSD *sensitiveDet = new EnergyDepositionSD(detName, detName);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDet);
    sensitiveDet->SetDetectorID(detIDNo);
    SetSensitiveDetector(detName, sensitiveDet, true);
  }
  for (auto det_pos : clover_positions) {
    for (int subCrystalNo = 1; subCrystalNo < 5; subCrystalNo++) {
      detIDNo++;
      auto detName = det_pos.id + "_" + std::to_string(subCrystalNo);
      EnergyDepositionSD *sensitiveDet = new EnergyDepositionSD(detName, detName);
      G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDet);
      sensitiveDet->SetDetectorID(detIDNo);
      SetSensitiveDetector(detName, sensitiveDet, true);
    }
  }
  Max_Sensitive_Detector_ID = detIDNo; // Necessary for EVENT_EVENTWISE output mode

#ifdef USE_ZERODEGREE
  EnergyDepositionSD *ZeroDegreeSD = new EnergyDepositionSD("ZeroDegree", "ZeroDegree");
  G4SDManager::GetSDMpointer()->AddNewDetector(ZeroDegreeSD);
  ZeroDegreeSD->SetDetectorID(0);
  SetSensitiveDetector("ZeroDegree", ZeroDegreeSD, true);
#endif
}
