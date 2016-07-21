/*
 * MyPhysics.h
 *
 *  Created on: Jul 20, 2016
 *      Author: rsehgal
 */

#ifndef SIMULATIONS_MST_INC_MYPHYSICS_H_
#define SIMULATIONS_MST_INC_MYPHYSICS_H_
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class MyPhysics : public G4VUserPhysicsList {
public:
  MyPhysics();
  ~MyPhysics();
  void ConstructParticle();
  void ConstructProcess();
  void ConstructEM();
};

#endif /* SIMULATIONS_MST_INC_MYPHYSICS_H_ */
