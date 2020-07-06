#include "MyPrimaryGeneratorAction.h"

#include "G4Box.hh"
#include "G4Electron.hh"
#include "G4MuonPlus.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Proton.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  //fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
  fParticleGun->SetParticleDefinition(G4MuonPlus::Definition());

  // Set the kinetic energy of the electrons to 50 keV
  // and tell the gun to emit them along the x-axis
  //fParticleGun->SetParticleEnergy(50. * keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(-5*cm, 0., 80 * cm ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));

}
MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete fParticleGun; }

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {

//#define RANDOM_GUN
#ifdef RANDOM_GUN
double gunZ=150*cm;
G4ThreeVector pt1(GenRandomDet(-50.*cm,50.*cm),  GenRandomDet(-50.*cm,50.*cm),  gunZ  );
G4ThreeVector pt2(GenRandomDet(-50.*cm,50.*cm),GenRandomDet(-50.*cm,50.*cm), -gunZ);
G4ThreeVector dir=(pt2-pt1).unit();
fParticleGun->SetParticlePosition(pt1);
fParticleGun->SetParticleMomentumDirection(dir);
#endif

fParticleGun->GeneratePrimaryVertex(event);
  
}
