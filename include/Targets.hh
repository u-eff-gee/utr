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
#pragma once

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class TargetContainer {
  private:
  G4LogicalVolume *TargetContainer_Logical;

  G4double TargetContainer_Length;
  G4double TargetContainer_Wall_Thickness;
  G4double TargetContainer_OuterRadius;
  G4double TargetContainer_InnerRadius;

  public:
  TargetContainer() {
    TargetContainer_Length = 20. * mm; // Measured
    TargetContainer_Wall_Thickness = 1. * mm; // Measured
    TargetContainer_OuterRadius = 11. * mm; // Measured
    TargetContainer_InnerRadius = 10. * mm; // Measured

    G4Colour grey(.5, .5, .5);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

    G4Tubs *TargetContainer_Solid =
        new G4Tubs("TargetContainer_Solid", TargetContainer_InnerRadius,
                   TargetContainer_OuterRadius,
                   TargetContainer_Length * 0.5, 0. * deg, 360. * deg);

    TargetContainer_Logical = new G4LogicalVolume(
        TargetContainer_Solid, PVC, "TargetContainer_Logical");
    TargetContainer_Logical->SetVisAttributes(new G4VisAttributes(grey));
  };

  ~TargetContainer(){};

  G4double Get_Length() { return TargetContainer_Length; };
  G4double Get_Inner_Radius() { return TargetContainer_InnerRadius; };
  G4double Get_Outer_Radius() { return TargetContainer_OuterRadius; };
  G4LogicalVolume *Get_Logical() { return TargetContainer_Logical; };
};

class TargetContainerCap {
  private:
  G4LogicalVolume *TargetContainerCap_Logical;

  G4double TargetContainerCap_Thickness;
  G4double TargetContainerCap_Radius;

  public:
  TargetContainerCap() {
    TargetContainerCap_Thickness = 2. * mm;
    TargetContainerCap_Radius = 10. * mm;

    G4Colour grey(.5, .5, .5);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *PVC = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

    G4Tubs *TargetContainerCap_Solid = new G4Tubs(
        "TargetContainerCap_Solid", 0. * mm, TargetContainerCap_Radius,
        TargetContainerCap_Thickness * 0.5, 0. * deg, 360. * deg);

    TargetContainerCap_Logical = new G4LogicalVolume(
        TargetContainerCap_Solid, PVC, "TargetContainerCap_Logical");
    TargetContainerCap_Logical->SetVisAttributes(new G4VisAttributes(grey));
  };

  ~TargetContainerCap(){};

  G4double Get_Thickness() { return TargetContainerCap_Thickness; };
  G4double Get_Radius() { return TargetContainerCap_Radius; };
  G4LogicalVolume *Get_Logical() { return TargetContainerCap_Logical; };
};

class Cr54_Target {
  private:
  G4LogicalVolume *Cr54_Target_Logical;

  G4double Length;
  G4double Radius;
  G4double Target_Center;

  public:
  Cr54_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    Materials *mat = Materials::Instance();

    // Mother volume
    TargetContainer *targetContainer = new TargetContainer();
    TargetContainerCap *targetContainerCap = new TargetContainerCap();
    TargetContainerCap *targetContainerBottom = new TargetContainerCap();

    Length = targetContainer->Get_Length();
    Radius = targetContainer->Get_Outer_Radius();
    G4double Cr54_Target_Thickness = 4.04824 * mm;
    G4double Cr54_Target_Radius = targetContainer->Get_Inner_Radius();

    G4Tubs *Cr54_Target_Solid =
        new G4Tubs("Cr54_Target_Solid", 0. * mm, Radius, Length * 0.5,
                   0. * deg, 360. * deg);

    Cr54_Target_Logical =
        new G4LogicalVolume(Cr54_Target_Solid, air, "Cr54_Target_Logical");
    Cr54_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Target Container

    G4LogicalVolume *TargetContainer_Logical =
        targetContainer->Get_Logical();

    new G4PVPlacement(0, G4ThreeVector(), TargetContainer_Logical,
                      "TargetContainer", Cr54_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerBottom_Logical =
        targetContainerBottom->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerBottom_Logical, "TargetContainerBottom",
        Cr54_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerCap_Logical =
        targetContainerCap->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerCap->Get_Thickness() - Cr54_Target_Thickness - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerCap_Logical, "TargetContainerCap",
        Cr54_Target_Logical, false, 0);

    // Cr54 Target Material

    G4Tubs *Cr54_Solid =
        new G4Tubs("Cr54_Solid", 0. * mm, Cr54_Target_Radius,
                   Cr54_Target_Thickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Cr54_Logical = new G4LogicalVolume(
        Cr54_Solid, mat->Get_target_Cr54_2O3(), "Cr54_Logical");
    Cr54_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., Length * 0.5 - targetContainerBottom->Get_Thickness() - Cr54_Target_Thickness * 0.5),
        Cr54_Logical, "Cr54_Target", Cr54_Target_Logical, false, 0);

    Target_Center = Length * 0.5 - targetContainerBottom->Get_Thickness() -
                    Cr54_Target_Thickness * 0.5;
  };

  ~Cr54_Target(){};

  G4double Get_Thickness() { return Length; };
  G4double Get_Radius() { return Radius; };
  G4double Get_Target_Center() { return Target_Center; };

  G4LogicalVolume *Get_Logical() { return Cr54_Target_Logical; };
};

class Se82_Target {
  private:
  G4LogicalVolume *Se82_Target_Logical;

  G4double Length;
  G4double Radius;
  G4double Target_Center;

  public:
  Se82_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    Materials *mat = Materials::Instance();

    // Mother volume
    TargetContainer *targetContainer = new TargetContainer();
    TargetContainerCap *targetContainerCap = new TargetContainerCap();
    TargetContainerCap *targetContainerBottom = new TargetContainerCap();

    Length = targetContainer->Get_Length();
    Radius = targetContainer->Get_Outer_Radius();
    G4double Se82_Target_Thickness = 4. * mm;
    G4double Se82_Target_Radius = targetContainer->Get_Inner_Radius();

    G4Tubs *Se82_Target_Solid =
        new G4Tubs("Se82_Target_Solid", 0. * mm, Radius, Length * 0.5,
                   0. * deg, 360. * deg);

    Se82_Target_Logical =
        new G4LogicalVolume(Se82_Target_Solid, air, "Se82_Target_Logical");
    Se82_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Target Container

    G4LogicalVolume *TargetContainer_Logical =
        targetContainer->Get_Logical();

    new G4PVPlacement(0, G4ThreeVector(), TargetContainer_Logical,
                      "TargetContainer", Se82_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerBottom_Logical =
        targetContainerBottom->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerBottom_Logical, "TargetContainerBottom",
        Se82_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerCap_Logical =
        targetContainerCap->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerCap->Get_Thickness() - Se82_Target_Thickness - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerCap_Logical, "TargetContainerCap",
        Se82_Target_Logical, false, 0);

    // Se82 Target Material

    G4Tubs *Se82_Solid =
        new G4Tubs("Se82_Solid", 0. * mm, Se82_Target_Radius,
                   Se82_Target_Thickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Se82_Logical = new G4LogicalVolume(
        Se82_Solid, mat->Get_target_Se(), "Se82_Logical");
    Se82_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., Length * 0.5 - targetContainerBottom->Get_Thickness() - Se82_Target_Thickness * 0.5),
        Se82_Logical, "Se82_Target", Se82_Target_Logical, false, 0);

    Target_Center = Length * 0.5 - targetContainerBottom->Get_Thickness() -
                    Se82_Target_Thickness * 0.5;
  };

  ~Se82_Target(){};

  G4double Get_Thickness() { return Length; };
  G4double Get_Radius() { return Radius; };
  G4double Get_Target_Center() { return Target_Center; };

  G4LogicalVolume *Get_Logical() { return Se82_Target_Logical; };
};

class Nd150_Target {
  private:
  G4LogicalVolume *Nd150_Target_Logical;

  G4double Nd150_Container_OuterHeight;
  G4double Nd150_Container_InnerHeight;
  G4double Nd150_Container_OuterRadius;
  G4double Nd150_Container_InnerRadius;
  G4double Nd150_Container_CapThickness;

  public:
  Nd150_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);
    G4Colour grey(.5, .5, .5);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    // HDPE (High-density polyethylene) used for container according to Oak Rich, G4_POLYETHYLENE matches the definition of HDPE in density
    G4Material *HDPE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    Materials *mat = Materials::Instance();

    // Mother volume
    Nd150_Container_OuterHeight = 7.5 * mm; // According to Oak Rich
    Nd150_Container_InnerHeight = Nd150_Container_OuterHeight - 1. * mm; // Estimated
    Nd150_Container_OuterRadius = 0.5 * inch; // According to Oak Rich
    Nd150_Container_InnerRadius = Nd150_Container_OuterRadius - 1. * mm; // Estimated
    Nd150_Container_CapThickness = (Nd150_Container_OuterHeight -
                                    Nd150_Container_InnerHeight) /
                                   2;

    G4Tubs *Nd150_Target_Solid =
        new G4Tubs("Nd150_Target_Solid", 0. * mm, Nd150_Container_OuterRadius, Nd150_Container_OuterHeight * 0.5,
                   0. * deg, 360. * deg);

    Nd150_Target_Logical =
        new G4LogicalVolume(Nd150_Target_Solid, air, "Nd150_Target_Logical");
    Nd150_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Target Container Barrel

    G4Tubs *Nd150_Container_Solid =
        new G4Tubs("Nd150_Container_Solid", Nd150_Container_InnerRadius,
                   Nd150_Container_OuterRadius,
                   Nd150_Container_OuterHeight * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Nd150_Container_Logical = new G4LogicalVolume(
        Nd150_Container_Solid, HDPE, "Nd150_Container_Logical");
    Nd150_Container_Logical->SetVisAttributes(new G4VisAttributes(grey));

    new G4PVPlacement(0, G4ThreeVector(), Nd150_Container_Logical,
                      "Nd150_Container", Nd150_Target_Logical, false, 0);

    // Target Container Caps
    G4Tubs *Nd150_ContainerCap_Solid = new G4Tubs(
        "Nd150_ContainerCap_Solid", 0. * mm, Nd150_Container_InnerRadius,
        Nd150_Container_CapThickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Nd150_ContainerCap_Logical = new G4LogicalVolume(
        Nd150_ContainerCap_Solid, HDPE, "Nd150_ContainerCap_Logical");
    Nd150_ContainerCap_Logical->SetVisAttributes(new G4VisAttributes(grey));

    // Target Container Bottom
    new G4PVPlacement(
        0, G4ThreeVector(0., 0., (Nd150_Container_OuterHeight - Nd150_Container_CapThickness) * 0.5),
        Nd150_ContainerCap_Logical, "Nd150_ContainerBottom",
        Nd150_Target_Logical, false, 0);

    // Target Container Top
    new G4PVPlacement(
        0, G4ThreeVector(0., 0., -(Nd150_Container_OuterHeight - Nd150_Container_CapThickness) * 0.5),
        Nd150_ContainerCap_Logical, "Nd150_ContainerTop",
        Nd150_Target_Logical, false, 0);

    // Nd150 Target Material

    G4Tubs *Nd150_Solid =
        new G4Tubs("Nd150_Solid", 0. * mm, Nd150_Container_InnerRadius,
                   Nd150_Container_InnerHeight * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Nd150_Logical = new G4LogicalVolume(
        Nd150_Solid, mat->Get_target_Nd150(), "Nd150_Logical");
    Nd150_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0, G4ThreeVector(), Nd150_Logical, "Nd150_Target", Nd150_Target_Logical, false, 0);
  };

  ~Nd150_Target(){};

  G4double Get_Thickness() { return Nd150_Container_OuterHeight; };
  G4double Get_Radius() { return Nd150_Container_OuterRadius; };
  G4double Get_Target_Center() { return 0.; };

  G4LogicalVolume *Get_Logical() { return Nd150_Target_Logical; };
};

class Sm152_Target {
  private:
  G4LogicalVolume *Sm152_Target_Logical;

  G4double Length;
  G4double Radius;
  G4double Target_Center;

  public:
  Sm152_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    Materials *mat = Materials::Instance();

    // Mother volume
    TargetContainer *targetContainer = new TargetContainer();
    TargetContainerCap *targetContainerCap = new TargetContainerCap();
    TargetContainerCap *targetContainerBottom = new TargetContainerCap();

    Length = targetContainer->Get_Length();
    Radius = targetContainer->Get_Outer_Radius();
    G4double Sm152_Target_Thickness = 4. * mm; // Estimated
    G4double Sm152_Target_Radius = targetContainer->Get_Inner_Radius();

    G4Tubs *Sm152_Target_Solid =
        new G4Tubs("Sm152_Target_Solid", 0. * mm, Radius, Length * 0.5,
                   0. * deg, 360. * deg);

    Sm152_Target_Logical =
        new G4LogicalVolume(Sm152_Target_Solid, air, "Sm152_Target_Logical");
    Sm152_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Target Container

    G4LogicalVolume *TargetContainer_Logical =
        targetContainer->Get_Logical();

    new G4PVPlacement(0, G4ThreeVector(), TargetContainer_Logical,
                      "TargetContainer", Sm152_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerBottom_Logical =
        targetContainerBottom->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerBottom_Logical, "TargetContainerBottom",
        Sm152_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerCap_Logical =
        targetContainerCap->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerCap->Get_Thickness() - Sm152_Target_Thickness - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerCap_Logical, "TargetContainerCap",
        Sm152_Target_Logical, false, 0);

    // Sm152 Target Material

    G4Tubs *Sm152_Solid =
        new G4Tubs("Sm152_Solid", 0. * mm, Sm152_Target_Radius,
                   Sm152_Target_Thickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Sm152_Logical = new G4LogicalVolume(
        Sm152_Solid, mat->Get_target_Sm152(), "Sm152_Logical");
    Sm152_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., Length * 0.5 - targetContainerBottom->Get_Thickness() - Sm152_Target_Thickness * 0.5),
        Sm152_Logical, "Sm152_Target", Sm152_Target_Logical, false, 0);

    Target_Center = Length * 0.5 - targetContainerBottom->Get_Thickness() -
                    Sm152_Target_Thickness * 0.5;
  };

  ~Sm152_Target(){};

  G4double Get_Thickness() { return Length; };
  G4double Get_Radius() { return Radius; };
  G4double Get_Target_Center() { return Target_Center; };

  G4LogicalVolume *Get_Logical() { return Sm152_Target_Logical; };
};

class Dy164_Target {
  private:
  G4LogicalVolume *Dy164_Target_Logical;

  G4double Length;
  G4double Radius;
  G4double Target_Center;

  public:
  Dy164_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    Materials *mat = Materials::Instance();

    // Mother volume
    TargetContainer *targetContainer = new TargetContainer();

    Length = targetContainer->Get_Length();
    Radius = targetContainer->Get_Outer_Radius();
    G4double Dy164_Target_Thickness = 0.40909 * mm;
    G4double Dy164_Target_Radius = targetContainer->Get_Inner_Radius();

    G4Tubs *Dy164_Target_Solid =
        new G4Tubs("Dy164_Target_Solid", 0. * mm, Radius, Length * 0.5,
                   0. * deg, 360. * deg);

    Dy164_Target_Logical = new G4LogicalVolume(Dy164_Target_Solid, air,
                                               "Dy164_Target_Logical");
    Dy164_Target_Logical->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Dy164 metallic Target
    G4Tubs *Dy164_Solid =
        new G4Tubs("Dy164_Solid", 0. * mm, Dy164_Target_Radius,
                   Dy164_Target_Thickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Dy164_Logical = new G4LogicalVolume(
        Dy164_Solid, mat->Get_target_Dy164(), "Dy164_Logical");
    Dy164_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., Length * 0.5 - Dy164_Target_Thickness * 0.5),
        Dy164_Logical, "Dy164_Target", Dy164_Target_Logical, false, 0);

    Target_Center = Length * 0.5 - Dy164_Target_Thickness * 0.5;
  };

  ~Dy164_Target(){};

  G4double Get_Thickness() { return Length; };
  G4double Get_Radius() { return Radius; };
  G4double Get_Target_Center() { return Target_Center; };

  G4LogicalVolume *Get_Logical() { return Dy164_Target_Logical; };
};

class Dy164_2O3_Target {
  private:
  G4LogicalVolume *Dy164_2O3_Target_Logical;

  G4double Length;
  G4double Radius;
  G4double Target_Center;

  public:
  Dy164_2O3_Target() {

    G4Colour yellow(1.0, 1.0, 0.0);

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    Materials *mat = Materials::Instance();

    // Mother volume
    TargetContainer *targetContainer = new TargetContainer();
    TargetContainerCap *targetContainerCap = new TargetContainerCap();
    TargetContainerCap *targetContainerBottom = new TargetContainerCap();

    Length = targetContainer->Get_Length();
    Radius = targetContainer->Get_Outer_Radius();
    G4double Dy164_2O3_Target_Thickness = 0.31423 * mm;
    G4double Dy164_2O3_Target_Radius = targetContainer->Get_Inner_Radius();

    G4Tubs *Dy164_2O3_Target_Solid =
        new G4Tubs("Dy164_2O3_Target_Solid", 0. * mm, Radius, Length * 0.5,
                   0. * deg, 360. * deg);

    Dy164_2O3_Target_Logical = new G4LogicalVolume(
        Dy164_2O3_Target_Solid, air, "Dy164_2O3_Target_Logical");
    Dy164_2O3_Target_Logical->SetVisAttributes(
        G4VisAttributes::GetInvisible());

    // Target Container

    G4LogicalVolume *TargetContainer_Logical =
        targetContainer->Get_Logical();

    new G4PVPlacement(0, G4ThreeVector(), TargetContainer_Logical,
                      "TargetContainer", Dy164_2O3_Target_Logical, false,
                      0);

    G4LogicalVolume *TargetContainerBottom_Logical =
        targetContainerBottom->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerBottom_Logical, "TargetContainerBottom",
        Dy164_2O3_Target_Logical, false, 0);

    G4LogicalVolume *TargetContainerCap_Logical =
        targetContainerCap->Get_Logical();

    new G4PVPlacement(
        0, G4ThreeVector(0., 0., targetContainer->Get_Length() * 0.5 - targetContainerCap->Get_Thickness() - Dy164_2O3_Target_Thickness - targetContainerBottom->Get_Thickness() * 0.5),
        TargetContainerCap_Logical, "TargetContainerCap",
        Dy164_2O3_Target_Logical, false, 0);

    // 164Dy2O3 Target Material
    G4Tubs *Dy164_2O3_Solid =
        new G4Tubs("Dy164_2O3_Solid", 0. * mm, Dy164_2O3_Target_Radius,
                   Dy164_2O3_Target_Thickness * 0.5, 0. * deg, 360. * deg);

    G4LogicalVolume *Dy164_2O3_Logical = new G4LogicalVolume(
        Dy164_2O3_Solid, mat->Get_target_Dy164_2O3(), "Dy164_2O3_Logical");
    Dy164_2O3_Logical->SetVisAttributes(new G4VisAttributes(yellow));

    new G4PVPlacement(
        0,
        G4ThreeVector(0., 0., Length * 0.5 - targetContainerBottom->Get_Thickness() - Dy164_2O3_Target_Thickness * 0.5),
        Dy164_2O3_Logical, "Dy164_2O3_Target", Dy164_2O3_Target_Logical,
        false, 0);

    Target_Center = Length * 0.5 - targetContainerBottom->Get_Thickness() -
                    Dy164_2O3_Target_Thickness * 0.5;
  };

  ~Dy164_2O3_Target(){};

  G4double Get_Thickness() { return Length; };
  G4double Get_Radius() { return Radius; };
  G4double Get_Target_Center() { return Target_Center; };

  G4LogicalVolume *Get_Logical() { return Dy164_2O3_Target_Logical; };
};
