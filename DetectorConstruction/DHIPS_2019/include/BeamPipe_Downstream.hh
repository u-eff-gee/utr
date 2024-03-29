#ifndef BEAMPIPEDOWNSTREAM_HH
#define BEAMPIPEDOWNSTREAM_HH 1

#include "G4LogicalVolume.hh"

class BeamPipe_Downstream {
  public:
  BeamPipe_Downstream(G4LogicalVolume *World_Log);
  ~BeamPipe_Downstream(){};
  void Construct(G4ThreeVector global_coordinates, G4double relative_density);
  G4LogicalVolume *Get_Beampipe_Vacuum() { return beamPipe_Vacuum_NRF_Logical; };
  G4double Get_Z_Axis_Offset_Z() { return Z_Axis_Offset_Z; };

  private:
  G4LogicalVolume *World_Logical;
  G4LogicalVolume *beamPipe_Vacuum_NRF_Logical;

  G4double beamPipe_NRF_Window_Thickness = 3 * mm;
  G4double beamPipe_NRF_Inner_Radius = 0.5 * 54. * mm;
  G4double beamPipe_NRF_Outer_Radius = 0.5 * 60. * mm;
  G4double beamPipe_NRF_Lenght = (810. - 180.) * mm;
  G4double SiDistanceToTarget = 40.5 * mm;
  G4double Z_Axis_Offset_Z = 0. * mm;

  G4double Chamber_Outer_Length = 180. * mm;
  G4double Chamber_Inner_Length = 150. * mm;
};

#endif
