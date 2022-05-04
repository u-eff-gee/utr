#ifndef BEAMPIPEUPSTREAM_HH
#define BEAMPIPEUPSTREAM_HH 1

#include "G4LogicalVolume.hh"

class BeamPipe_Upstream {
  public:
  BeamPipe_Upstream(G4LogicalVolume *World_Log);
  ~BeamPipe_Upstream(){};
  void Construct(G4ThreeVector global_coordinates, G4double relative_density);

  private:
  G4LogicalVolume *World_Logical;

  G4double exit_Window_Thickness = 0.1 * mm; // Estimated
  G4double beamPipe_Inner_Radius = 19. * mm; // Estimated
  G4double beamPipe_Outer_Radius_Small = 21.25 * mm;
  G4double beamPipe_Outer_Radius_Large = 35. * mm;
  G4double beamPipe_Small_Radius_Length = 100. * mm; // Estimated
  G4double beamPipe_Large_Radius_Length = 25. * mm;
};

#endif
