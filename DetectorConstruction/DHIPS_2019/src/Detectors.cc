#include "Detectors.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

using std::cout;
using std::endl;
using std::stringstream;

DetectorPos DetectorPos::get(PositionDHIPS pos) {
  switch (pos) {
    case PositionDHIPS::Right90:
      return DetectorPos({90. * deg, 0. * deg, 225. * mm});
    case PositionDHIPS::Left90:
      return DetectorPos({90. * deg, 180. * deg, 225. * mm});
    case PositionDHIPS::Left130:
      return DetectorPos({90. * deg, 140. * deg, 225. * mm});
    default:
      abort();
  }
}

DetectorInfo DetectorInfo::get(DetectorDHIPS det, PositionDHIPS pos, bool _dewar) {
  return DetectorInfo::get(det, DetectorPos::get(pos), _dewar);
}

DetectorInfo DetectorInfo::get(DetectorDHIPS det, DetectorPos pos, bool _dewar) {
  HPGe_Collection hpge_Collection;
  switch (det) {
    case DetectorDHIPS::HPGe1:
      return DetectorInfo(hpge_Collection.HPGe_100_TUD_73760, "HPGe1", pos, _dewar);
    case DetectorDHIPS::HPGe2:
      return DetectorInfo(hpge_Collection.HPGe_100_TUD_72902, "HPGe2", pos, _dewar);
    case DetectorDHIPS::HPGePol:
      return DetectorInfo(hpge_Collection.HPGe_100_TUD_72930, "HPGePol", pos, _dewar);
    case DetectorDHIPS::HPGe80:
      return DetectorInfo(hpge_Collection.HPGe_80_TUD_90006, "HPGe80", pos, _dewar);
    default:
      abort();
  }
}

Detectors::Detectors(G4LogicalVolume *World_Log)
    : World_Logical(World_Log) {}

void Detectors::ConstructDetector(G4ThreeVector global_coordinates, DetectorInfo &det_info) {
  HPGe_Coaxial HPGe(World_Logical, det_info.name);
  HPGe.setProperties(det_info.properties);
  if (det_info.dewar) {
    HPGe.useDewar();
  }

  HPGe.Construct(global_coordinates + G4ThreeVector(0, 0, 0),
                 -90. * deg - det_info.position.phi, 0.,
                 det_info.position.distance + (BGO::Get_Length() - BGO::Get_Max_Penetration_Depth()));
}

void Detectors::ConstructDetectorBGO(G4ThreeVector global_coordinates, DetectorInfo &det_info, BGO &BGO) {
  G4double BGO_Dist = -(det_info.position.distance + BGO::Get_Length() * 0.5);
  BGO.Construct(global_coordinates, det_info.position.theta, det_info.position.phi, BGO_Dist);
  ConstructDetector(global_coordinates, det_info);
}

void Detectors::ConstructDetectorFilter(G4ThreeVector global_coordinates, DetectorInfo &det_info, G4double CuLength, G4double PbLength) {
  G4double Cu_dist = -det_info.position.distance + 0.5 * CuLength;
  G4double Pb_dist = -det_info.position.distance + 0.5 * PbLength + CuLength;

  ConstructDetectorFilter(global_coordinates, det_info, CuLength, "Cu", Cu_dist, G4Color::Brown());
  ConstructDetectorFilter(global_coordinates, det_info, PbLength, "Pb", Pb_dist, G4Color::Grey());
}

void Detectors::ConstructDetectorFilter(G4ThreeVector global_coordinates, DetectorInfo &det_info, G4double length, G4String material, G4double dist, G4Color color) {
  if (length > 0.) {
    G4RotationMatrix *RotDet = new G4RotationMatrix();
    RotDet->rotateY(det_info.position.phi - 90 * deg);
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *mat = nist->FindOrBuildMaterial("G4_" + material);

    G4ThreeVector Filter_Dist(dist * sin(det_info.position.theta) * cos(det_info.position.phi), dist * cos(det_info.position.theta), dist * sin(det_info.position.theta) * sin(det_info.position.phi));
    G4Tubs *Filter_Solid = new G4Tubs(material + "_Filter_Solid", 0, 25 * mm, length * 0.5, 0. * deg, 360. * deg);
    G4LogicalVolume *Filter_Logical = new G4LogicalVolume(Filter_Solid, mat, "Filter_Logical", 0, 0, 0);
    Filter_Logical->SetVisAttributes(color);
    new G4PVPlacement(RotDet, global_coordinates + Filter_Dist, Filter_Logical, (det_info.name + "_" + material + "_Filter").c_str(), World_Logical, 0, 0);
  }
}
