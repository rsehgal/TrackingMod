#include "MyPrimaryGeneratorAction.h"

#include "G4Box.hh"
#include "G4Electron.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Proton.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "base/Global.h"
#include "base/Vector3D.h"
using Tracking::Vector3D;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());

  // Set the kinetic energy of the protons to 50 keV
  // and tell the gun to emit them along the x-axis
  fParticleGun->SetParticleEnergy(50. * keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 120 * cm ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
}
MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete fParticleGun; }

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
   //fParticleGun->SetParticlePosition(G4ThreeVector(-50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);

  //Tracking::Global::GenRandomDet(-fLength/2.,fLength/2.),
    //                              Tracking::Global::GenRandomDet(-fLength/2.
  
  //fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -120 * cm));
  Vector3D<double> pt1(Tracking::Global::GenRandomDet(-15.*cm,15.*cm),
                      Tracking::Global::GenRandomDet(-15.*cm,15.*cm),
                      //Tracking::Global::GenRandomDet(0.,50.)
                      120*cm);

  Vector3D<double> pt2(Tracking::Global::GenRandomDet(-15.*cm,15.*cm),
                      Tracking::Global::GenRandomDet(-15.*cm,15.*cm),
                      -120*cm);
  //std::cout<< pt.Unit().x() << pt.Unit().y() << pt.Unit().z() << std::endl;
  //pt1.Set(0.,0.,0.);
  double x = (pt1-pt2).Unit().x();
  double y = (pt1-pt2).Unit().y();
  double z = (pt1-pt2).Unit().z();

  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));

  #ifdef RANDOM_GUN
  //fParticleGun->SetParticlePosition(G4ThreeVector(Tracking::Global::GenRandomDet(-50.,50.)*cm, Tracking::Global::GenRandomDet(-50.,50.)*cm, -120 * cm));
  fParticleGun->SetParticlePosition(G4ThreeVector(pt2.x(),pt2.y(),pt2.z()));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  #endif
  fParticleGun->GeneratePrimaryVertex(event);
   //fParticleGun->SetParticlePosition(G4ThreeVector(50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);

}
