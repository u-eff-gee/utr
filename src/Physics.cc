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

//
// Largely influenced by GEANT4 example TestEm7
//

#include "Physics.hh"

#include "G4LossTableManager.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ProcessManager.hh"

// Particles
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

// Electromagnetic Processes
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4RayleighScattering.hh"

#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"

#include "G4EmProcessOptions.hh"
#include "G4eplusAnnihilation.hh"

#include "G4UAtomicDeexcitation.hh"

#include "G4CascadeInterface.hh"

// Other charged particle processes
#include "G4hIonisation.hh"
#include "G4hMultipleScattering.hh"

// Penelope model
#include "G4PenelopeComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4PenelopePhotoElectricModel.hh"
#include "G4PenelopeRayleighModel.hh"

#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4PenelopeIonisationModel.hh"

// Livermore model
#include "G4LivermoreComptonModel.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreRayleighModel.hh"

#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4LivermoreIonisationModel.hh"

// Livermore polarized model
#include "G4LivermorePolarizedComptonModel.hh"
#include "G4LivermorePolarizedGammaConversionModel.hh"
#include "G4LivermorePolarizedPhotoElectricModel.hh"
#include "G4LivermorePolarizedRayleighModel.hh"

// High-precision neutron physics
#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"

#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronInelasticProcess.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPInelastic.hh"

Physics::Physics() : G4VUserPhysicsList() { SetVerboseLevel(1); }

Physics::~Physics() {}

void Physics::ConstructParticle() {
	G4BosonConstructor bosonConstructor;
	bosonConstructor.ConstructParticle();

	G4LeptonConstructor leptonConstructor;
	leptonConstructor.ConstructParticle();

	G4MesonConstructor mesonConstructor;
	mesonConstructor.ConstructParticle();

	G4IonConstructor ionConstructor;
	ionConstructor.ConstructParticle();

	G4BaryonConstructor baryonConstructor;
	baryonConstructor.ConstructParticle();

	G4ShortLivedConstructor shortLivedConstructor;
	shortLivedConstructor.ConstructParticle();
}

void Physics::ConstructProcess() {
	AddTransportation();
	// ConstructEMPenelope();
	// ConstructEMLivermore();
	ConstructEMLivermorePolarized();
	// ConstructHPNeutron();
	// ConstructChargedParticle();
}

// Penelope model

void Physics::ConstructEMPenelope() {
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
	G4ProcessManager *procMan = G4Gamma::Gamma()->GetProcessManager();

	G4ParticleTableIterator<G4String, G4ParticleDefinition *>
	    *particleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
	particleIterator->reset();
	while ((*particleIterator)()) {
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma") {

			G4ComptonScattering *cs = new G4ComptonScattering;
			cs->SetEmModel(new G4PenelopeComptonModel(), 1);
			ph->RegisterProcess(cs, particle);

			G4GammaConversion *gc = new G4GammaConversion;
			gc->SetEmModel(new G4PenelopeGammaConversionModel);
			ph->RegisterProcess(gc, particle);

			G4PhotoElectricEffect *pe = new G4PhotoElectricEffect;
			pe->SetEmModel(new G4PenelopePhotoElectricModel);
			ph->RegisterProcess(pe, particle);

			G4RayleighScattering *rs = new G4RayleighScattering;
			rs->SetEmModel(new G4PenelopeRayleighModel);
			ph->RegisterProcess(rs, particle);

			G4PhotoNuclearProcess *thePhotoNuclearProcess =
			    new G4PhotoNuclearProcess();
			G4CascadeInterface *theLEModel = new G4CascadeInterface;
			thePhotoNuclearProcess->RegisterMe(theLEModel);
			procMan->AddDiscreteProcess(thePhotoNuclearProcess);
		}

		else if (particleName == "e-") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			G4eIonisation *ei = new G4eIonisation;
			ei->SetEmModel(new G4PenelopeIonisationModel);
			ph->RegisterProcess(ei, particle);

			G4eBremsstrahlung *eb = new G4eBremsstrahlung;
			eb->SetEmModel(new G4PenelopeBremsstrahlungModel);
			ph->RegisterProcess(eb, particle);
		} else if (particleName == "e+") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			ph->RegisterProcess(new G4eplusAnnihilation, particle);

			G4eIonisation *poi = new G4eIonisation;
			poi->SetEmModel(new G4PenelopeIonisationModel);
			ph->RegisterProcess(poi, particle);

			G4eBremsstrahlung *pb = new G4eBremsstrahlung;
			pb->SetEmModel(new G4PenelopeBremsstrahlungModel);
			ph->RegisterProcess(pb, particle);
		}
	}

	G4VAtomDeexcitation *de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(false);
	de->SetPIXE(false); // Particle Induced X-ray Emission
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

// Livermore model

void Physics::ConstructEMLivermore() {
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
	G4ProcessManager *procMan = G4Gamma::Gamma()->GetProcessManager();

	G4ParticleTableIterator<G4String, G4ParticleDefinition *>
	    *particleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
	particleIterator->reset();
	while ((*particleIterator)()) {
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma") {

			G4ComptonScattering *cs = new G4ComptonScattering;
			cs->SetEmModel(new G4LivermoreComptonModel(), 1);
			ph->RegisterProcess(cs, particle);

			G4GammaConversion *gc = new G4GammaConversion;
			gc->SetEmModel(new G4LivermoreGammaConversionModel);
			ph->RegisterProcess(gc, particle);

			G4PhotoElectricEffect *pe = new G4PhotoElectricEffect;
			pe->SetEmModel(new G4LivermorePhotoElectricModel);
			ph->RegisterProcess(pe, particle);

			G4RayleighScattering *rs = new G4RayleighScattering;
			rs->SetEmModel(new G4LivermoreRayleighModel);
			ph->RegisterProcess(rs, particle);

			G4PhotoNuclearProcess *thePhotoNuclearProcess =
			    new G4PhotoNuclearProcess();
			G4CascadeInterface *theLEModel = new G4CascadeInterface;
			thePhotoNuclearProcess->RegisterMe(theLEModel);
			procMan->AddDiscreteProcess(thePhotoNuclearProcess);
		}

		else if (particleName == "e-") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			G4eIonisation *ei = new G4eIonisation;
			ei->SetEmModel(new G4LivermoreIonisationModel);
			ph->RegisterProcess(ei, particle);

			G4eBremsstrahlung *eb = new G4eBremsstrahlung;
			eb->SetEmModel(new G4LivermoreBremsstrahlungModel);
			ph->RegisterProcess(eb, particle);
		} else if (particleName == "e+") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			ph->RegisterProcess(new G4eplusAnnihilation, particle);

			ph->RegisterProcess(new G4eIonisation(), particle);
			// Livermore ionisation model not applicable to positrons
			// G4eIonisation* poi = new G4eIonisation;
			// poi->SetEmModel(new G4LivermoreIonisationModel);
			// ph->RegisterProcess(poi, particle);

			G4eBremsstrahlung *pb = new G4eBremsstrahlung;
			pb->SetEmModel(new G4LivermoreBremsstrahlungModel);
			ph->RegisterProcess(pb, particle);
		}
	}

	G4VAtomDeexcitation *de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(false);
	de->SetPIXE(false); // Particle Induced X-ray Emission
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

// Polarized Livermore model

void Physics::ConstructEMLivermorePolarized() {
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
	G4ProcessManager *procMan = G4Gamma::Gamma()->GetProcessManager();

	G4ParticleTableIterator<G4String, G4ParticleDefinition *>
	    *particleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
	particleIterator->reset();
	while ((*particleIterator)()) {
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma") {

			G4ComptonScattering *cs = new G4ComptonScattering;
			cs->SetEmModel(new G4LivermorePolarizedComptonModel(), 1);
			ph->RegisterProcess(cs, particle);

			G4GammaConversion *gc = new G4GammaConversion;
			gc->SetEmModel(new G4LivermorePolarizedGammaConversionModel);
			ph->RegisterProcess(gc, particle);

			G4PhotoElectricEffect *pe = new G4PhotoElectricEffect;
			pe->SetEmModel(new G4LivermorePolarizedPhotoElectricModel);
			ph->RegisterProcess(pe, particle);

			G4RayleighScattering *rs = new G4RayleighScattering;
			rs->SetEmModel(new G4LivermorePolarizedRayleighModel);
			ph->RegisterProcess(rs, particle);

			G4PhotoNuclearProcess *thePhotoNuclearProcess =
			    new G4PhotoNuclearProcess();
			G4CascadeInterface *theLEModel = new G4CascadeInterface;
			thePhotoNuclearProcess->RegisterMe(theLEModel);
			procMan->AddDiscreteProcess(thePhotoNuclearProcess);
		}

		else if (particleName == "e-") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			G4eIonisation *ei = new G4eIonisation;
			ei->SetEmModel(new G4LivermoreIonisationModel);
			ph->RegisterProcess(ei, particle);

			G4eBremsstrahlung *eb = new G4eBremsstrahlung;
			eb->SetEmModel(new G4LivermoreBremsstrahlungModel);
			ph->RegisterProcess(eb, particle);
		} else if (particleName == "e+") {
			ph->RegisterProcess(new G4eMultipleScattering, particle);

			ph->RegisterProcess(new G4eplusAnnihilation, particle);

			ph->RegisterProcess(new G4eIonisation(), particle);
			// Livermore ionisation model not applicable to positrons
			// G4eIonisation* poi = new G4eIonisation;
			// poi->SetEmModel(new G4LivermoreIonisationModel);
			// ph->RegisterProcess(poi, particle);

			G4eBremsstrahlung *pb = new G4eBremsstrahlung;
			pb->SetEmModel(new G4LivermoreBremsstrahlungModel);
			ph->RegisterProcess(pb, particle);
		}
	}

	G4VAtomDeexcitation *de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(false);
	de->SetPIXE(false); // Particle Induced X-ray Emission
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

void Physics::ConstructHPNeutron() {
	// Inspired by Example Hadr04
	// (http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/ExampleHadr04.html)

	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

	G4ParticleTableIterator<G4String, G4ParticleDefinition *>
	    *particleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
	particleIterator->reset();
	while ((*particleIterator)()) {
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (particleName == "neutron") {
			G4HadronElasticProcess *hadronElastic =
			    new G4HadronElasticProcess();
			hadronElastic->AddDataSet(new G4NeutronHPElasticData);
			hadronElastic->AddDataSet(new G4NeutronHPThermalScatteringData);
			G4NeutronHPElastic *elasticModel = new G4NeutronHPElastic();
			hadronElastic->RegisterMe(elasticModel);
			ph->RegisterProcess(hadronElastic, particle);

			G4NeutronInelasticProcess *neutronInelastic =
			    new G4NeutronInelasticProcess();
			neutronInelastic->AddDataSet(new G4NeutronHPInelasticData);
			G4NeutronHPInelastic *inelasticModel = new G4NeutronHPInelastic();
			neutronInelastic->RegisterMe(inelasticModel);
			ph->RegisterProcess(neutronInelastic, particle);

			G4HadronCaptureProcess *hadronCapture =
			    new G4HadronCaptureProcess();
			hadronCapture->AddDataSet(new G4NeutronHPCaptureData);
			G4NeutronHPCapture *captureModel = new G4NeutronHPCapture();
			hadronCapture->RegisterMe(captureModel);
			ph->RegisterProcess(hadronCapture, particle);
		}
	}
}

void Physics::ConstructChargedParticle() {
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

	G4ParticleTableIterator<G4String, G4ParticleDefinition *>
	    *particleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
	particleIterator->reset();
	while ((*particleIterator)()) {
		G4ParticleDefinition *particle = particleIterator->value();
		G4String particleName = particle->GetParticleName();

		if (!particle->IsShortLived() && particle->GetPDGCharge() != 0.0 &&
		    particle->GetParticleName() != "chargedgeantino" &&
		    particle->GetParticleName() != "e-" &&
		    particle->GetParticleName() != "e+") {

			ph->RegisterProcess(new G4hMultipleScattering(), particle);
			ph->RegisterProcess(new G4hIonisation(), particle);
		}
	}
}
