#pragma once

#include "G4Color.hh"
#include "G4LogicalVolume.hh"

#include "BGO.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"

enum class PositionDHIPS {
  Right90,
  Left90,
  Left130
};

enum class DetectorDHIPS {
  HPGe1,
  HPGe2,
  HPGePol,
  HPGe80
};

struct DetectorPos {
  G4double theta;
  G4double phi;
  G4double distance;

  DetectorPos() = default;
  DetectorPos(G4double _theta, G4double _phi, G4double _distance)
      : theta(_theta), phi(_phi), distance(_distance){};
  static DetectorPos get(PositionDHIPS pos);
};

struct DetectorInfo {
  HPGe_Coaxial_Properties properties;
  G4String name;
  DetectorPos position;
  bool dewar = true;

  DetectorInfo() = delete;
  DetectorInfo(HPGe_Coaxial_Properties _properties, G4String _name, DetectorPos _position, bool _dewar = true)
      : properties(_properties), name(_name), position(_position), dewar(_dewar){};
  static DetectorInfo get(DetectorDHIPS det, DetectorPos pos, bool dewar = true);
  static DetectorInfo get(DetectorDHIPS det, PositionDHIPS pos, bool dewar = true);
};

class Detectors {
  public:
  Detectors(G4LogicalVolume *World_Log);
  void ConstructDetector(G4ThreeVector global_coordinates, DetectorInfo &det_info);
  void ConstructDetectorBGO(G4ThreeVector global_coordinates, DetectorInfo &det_info, BGO &BGO);
  void ConstructDetectorFilter(G4ThreeVector global_coordinates, DetectorInfo &det_info, G4double CuLength, G4double PbLength);
  void ConstructDetectorFilter(G4ThreeVector global_coordinates, DetectorInfo &det_info, G4double length, G4String material, G4double dist, G4Color color);

  private:
  G4LogicalVolume *World_Logical;
};
