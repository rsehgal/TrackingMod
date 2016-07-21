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

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
//#include "PhysListEmStandard.hh"
#include "G4EmStandardPhysics.hh"
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
  G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
  //(new G4EmStandardPhysics())->ConstructProcess();
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
        std::cout<<"---- Here ------" << std::endl;
        ph->RegisterProcess(new G4MuMultipleScattering, particle);
        //std::cout<<"Registered MultipleScattering for : " << particleName << std::endl;

        ph->RegisterProcess(new G4MuIonisation, particle);
        //std::cout<<"Registered Ionization for : " << particleName << std::endl;
        ph->RegisterProcess(new G4MuBremsstrahlung, particle);
        //std::cout<<"Registered Brehsstrahlung for : " << particleName << std::endl;
        ph->RegisterProcess(new G4MuPairProduction, particle);
        //std::cout<<"Registered PairProduction for : " << particleName << std::endl;
      }

      if (particleName == "e+") {
            //positron
            ph->RegisterProcess(new G4eMultipleScattering, particle);
            ph->RegisterProcess(new G4eIonisation,         particle);
            ph->RegisterProcess(new G4eBremsstrahlung,     particle);
           // ph->RegisterProcess(new G4eplusAnnihilation,   particle);

          }
    }


}
