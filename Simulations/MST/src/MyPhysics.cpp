/*
 * MyPhysics.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: rsehgal
 */

#include "MyPhysics.h"
#include "G4ParticleTypes.hh"
#include "G4LeptonConstructor.hh"
MyPhysics::MyPhysics() {
  // TODO Auto-generated constructor stub
}

MyPhysics::~MyPhysics() {
  // TODO Auto-generated destructor stub
}

void MyPhysics::ConstructParticle(){
  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();
}

void MyPhysics::ConstructProcess(){
  AddTransportation();
}
