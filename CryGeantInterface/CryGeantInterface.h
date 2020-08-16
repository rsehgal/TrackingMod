/*
 * CryGeantInterface.h
 *
 *  Created on: Jan 1, 2018
 *      Author: rsehgal
 */

#ifndef CRYGEANTINTERFACE_CRYGEANTINTERFACE_H_
#define CRYGEANTINTERFACE_CRYGEANTINTERFACE_H_

#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4Event.hh"

#include "G4SystemOfUnits.hh"


class CryGeantInterface {
public:
	CryGeantInterface();
	virtual ~CryGeantInterface();
	CryGeantInterface(const char *inputfile);
	void ForCry(const char *inputfile);
	void GeneratePrimariesForCry(G4Event *anEvent,bool tm=true);
	void InputCRY();
	void UpdateCRY(std::string* MessInput);
	void CRYFromFile(G4String newValue);
	const G4ParticleGun* GetParticleGun() const { return particleGun; }

private:
	    //G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
	    std::vector<CRYParticle*> *vect; // vector of generated particles
	    G4ParticleTable* particleTable;
	    G4ParticleGun* particleGun;
	    CRYGenerator* gen;
	    G4int InputState;
	    PrimaryGeneratorMessenger* gunMessenger;

};

#endif /* CRYGEANTINTERFACE_CRYGEANTINTERFACE_H_ */
