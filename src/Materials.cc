#include "Materials.hh"

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

Materials::Materials() { ConstructMaterials(); }

Materials::~Materials() {}

void Materials::ConstructMaterials() {

	// Implement ma which are not in the Geant4 materials database
	// http://geant4.cern.ch/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/apas08.html
	// for example isotopically enriched materials or materials with a
	// non-standard density.

	G4NistManager *nist = G4NistManager::Instance();

	G4Element *nat_La = nist->FindOrBuildElement("La");
	G4Element *nat_Br = nist->FindOrBuildElement("Br");
	G4Element *nat_Ce = nist->FindOrBuildElement("Ce");
	G4Element *nat_Cl = nist->FindOrBuildElement("Cl");

	G4double z, a, n;
	G4double density, abundance, fractionmass;
	G4String name, symbol;
	G4int ncomponents, natoms;

	/******************************************************/
	// Enriched Ti target from 2014/2015 NRF experiments
	/******************************************************/

	G4Isotope *Ti50 =
	    new G4Isotope(name = "50Ti", z = 22, n = 28, a = 49.945791 * g / mole);
	G4Isotope *Ti49 =
	    new G4Isotope(name = "49Ti", z = 22, n = 27, a = 48.947870 * g / mole);
	G4Isotope *Ti48 =
	    new G4Isotope(name = "48Ti", z = 22, n = 26, a = 47.947946 * g / mole);
	G4Isotope *Ti47 =
	    new G4Isotope(name = "47Ti", z = 22, n = 25, a = 46.951763 * g / mole);
	G4Isotope *Ti46 =
	    new G4Isotope(name = "46Ti", z = 22, n = 24, a = 45.952631 * g / mole);

	G4Element *enriched_Ti =
	    new G4Element(name = "enriched Ti", symbol = "Ti", ncomponents = 5);

	enriched_Ti->AddIsotope(Ti50, abundance = 67.62 * perCent);
	enriched_Ti->AddIsotope(Ti49, abundance = 3.01 * perCent);
	enriched_Ti->AddIsotope(Ti48, abundance = 24.06 * perCent);
	enriched_Ti->AddIsotope(Ti47, abundance = 2.53 * perCent);
	enriched_Ti->AddIsotope(Ti46, abundance = 2.78 * perCent);

	G4Element *nat_O = nist->FindOrBuildElement("O");

	target_TiO2 = new G4Material(name = "enriched Ti", density = 4. * g / cm3,
	                             ncomponents = 2);
	// Density is a mean value of different TiO2 crystal lattice configurations.
	target_TiO2->AddElement(enriched_Ti, natoms = 1);
	target_TiO2->AddElement(nat_O, natoms = 2);

	/***********************************/
	//       Lanthanum bromide
	/***********************************/

	// Standard LaBr
	density = 5.06 * g / cm3;
	LaBr3 = new G4Material(name = "LaBr3", density, ncomponents = 2);

	LaBr3->AddElement(nat_La, 1);
	LaBr3->AddElement(nat_Br, 3);

	// Brillance 380 from Enrique Nacher (Santiago)
	LaBr3Ce = new G4Material("Brillance380", density = 5.08 * g / cm3,
	                         ncomponents = 3);
	LaBr3Ce->AddElement(nat_La, fractionmass = 34.855 * perCent);
	LaBr3Ce->AddElement(nat_Br, fractionmass = 60.145 * perCent);
	LaBr3Ce->AddElement(nat_Ce, fractionmass = 5.0 * perCent);

	// Brillance350 from Enrique Nacher (Santiago)
	LaCl3Ce = new G4Material("Brillance350", density = 3.85 * g / cm3,
	                         ncomponents = 3);
	LaCl3Ce->AddElement(nat_La, fractionmass = 53.804 * perCent);
	LaCl3Ce->AddElement(nat_Cl, fractionmass = 41.196 * perCent);
	LaCl3Ce->AddElement(nat_Ce, fractionmass = 5.0 * perCent);
	//
}
