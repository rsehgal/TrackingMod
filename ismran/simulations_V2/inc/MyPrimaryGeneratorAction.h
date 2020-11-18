/*
#ifndef MyPrimaryGeneratorAction
#define MyPrimaryGeneratorAction

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "../../base/Global.h"

class G4VUserPrimaryGeneratorAction;
class G4Event;
class G4ParticleGun;

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGeneratorAction();
	~MyPrimaryGeneratorAction();
	void GeneratePrimaries(G4Event* event);
	G4ParticleGun* GetParticleGun(){return fParticleGun;}

private:
	G4ParticleGun *fParticleGun;

	
};
#endif
*/

#ifndef MyPrimaryGeneratorAction_h
#define MyPrimaryGeneratorAction_h 

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#ifdef USE_CRY
#include "CryGeantInterface.h"
#endif

class G4ParticleGun;
class G4Event;
class G4Box;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MyPrimaryGeneratorAction();    
     ~MyPrimaryGeneratorAction();
    MyPrimaryGeneratorAction(const char *inputfile);
    // method from the base class
    void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    //G4Box* fEnvelopeBox;
    #ifdef USE_CRY
    CryGeantInterface *cryG4Interface;
#endif

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
