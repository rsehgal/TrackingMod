/*
 * MyPhysics.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: rsehgal
 */

#include "MyPhysics.h"
#include "G4ParticleTypes.hh"
#include "G4LeptonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4MuonPlus.hh"
#include "G4Gamma.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ProcessManager.hh"
MyPhysics::MyPhysics() {
  // TODO Auto-generated constructor stub
}

MyPhysics::~MyPhysics() {
  // TODO Auto-generated destructor stub
}

void MyPhysics::ConstructParticle(){
  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  //G4MuonPlus::MuonPlusDefinition();

  //Need to create Gamma because it is a mandatory particle for basic EM process like multiple scattering
  //without this the code will not run and will complain about "Gamma"
  G4Gamma::GammaDefinition();
}

void MyPhysics::ConstructProcess(){
  AddTransportation();
  ConstructEM();

}

void MyPhysics::ConstructEM(){
  G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

  theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4String particleName = particle->GetParticleName();
      if (particleName == "mu+" || particleName == "mu-") {
        // muon
        ph->RegisterProcess(new G4MuMultipleScattering, particle);
        ph->RegisterProcess(new G4MuIonisation, particle);
        ph->RegisterProcess(new G4MuBremsstrahlung, particle);
        ph->RegisterProcess(new G4MuPairProduction, particle);
      }
    }

}
