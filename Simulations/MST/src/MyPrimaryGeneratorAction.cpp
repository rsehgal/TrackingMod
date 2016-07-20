#include "MyPrimaryGeneratorAction.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4Electron.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(){}
MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction(){
	delete fParticleGun;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event){

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

/*
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);
  */

  //fParticleGun->SetParticleDefinition(G4Proton::ProtonDefinition());
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());

  //Set the kinetic energy of the protons to 50 keV
  //and tell the gun to emit them along the x-axis
  fParticleGun->SetParticleEnergy(50.*keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,90*cm*0.1));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,-1.));
  fParticleGun->GeneratePrimaryVertex(event);

}
