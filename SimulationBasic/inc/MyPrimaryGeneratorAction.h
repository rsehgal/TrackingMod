/*
#ifndef MyPrimaryGeneratorAction
#define MyPrimaryGeneratorAction

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"


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

#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "globals.hh"


#ifdef USE_CRY
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"
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
    MyPrimaryGeneratorAction(const char *inputfile);
     ~MyPrimaryGeneratorAction();

    // method from the base class
    void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

#ifdef USE_CRY
    void ForCry(const char *inputfile);
    void GeneratePrimariesForCry(G4Event *anEvent);
    void InputCRY();
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);

#endif
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    //G4Box* fEnvelopeBox;
#ifdef USE_CRY
    std::vector<CRYParticle*> *vect; // vector of generated particles
    G4ParticleTable* particleTable;
    G4ParticleGun* particleGun;
    CRYGenerator* gen;
    G4int InputState;
    PrimaryGeneratorMessenger* gunMessenger;
#endif
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
