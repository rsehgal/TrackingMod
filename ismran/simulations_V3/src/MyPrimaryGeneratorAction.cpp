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
#include "base/Vector3D.h"
#include "MuonReader.h"
#include "B1RunAction.hh"
#include "MySD.h"
using Tracking::Vector3D;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  /*fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());

  // Set the kinetic energy of the protons to 50 keV
  // and tell the gun to emit them along the x-axis
  fParticleGun->SetParticleEnergy(50. * keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -120 * cm ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));*/
}
MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete fParticleGun; }

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
   //fParticleGun->SetParticlePosition(G4ThreeVector(-50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);
  
  /*fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -120 * cm));
  fParticleGun->GeneratePrimaryVertex(event);
  */
   //fParticleGun->SetParticlePosition(G4ThreeVector(50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);
  double gunZ = 50*cm;
  Muon *muon = new Muon(*lite_interface::MuonReader::instance()->GetMuon());
  B1RunAction::fEvNo++;
  if(!(B1RunAction::fEvNo % 10000))
	  std::cout << "Processed : " << B1RunAction::fEvNo << " events..." << std::endl;
   Vector3D<double> pt1(Tracking::Global::GenRandomDet(-45.*cm,45.*cm),
                        gunZ,
                        Tracking::Global::GenRandomDet(-50.*cm,50.*cm));
   fParticleGun->SetParticlePosition(G4ThreeVector(pt1.x(),pt1.y(),pt1.z()));
   G4ThreeVector dir(muon->angleX,
                     muon->angleY,
                     muon->angleZ);
   G4ThreeVector ref(0.,-1.,0.);
   double angle = dir.angle(ref);
   B1RunAction::fAngleVec.push_back(angle);
   MySD::angle = angle;
   fParticleGun->SetParticleMomentumDirection(dir);
   fParticleGun->SetParticleEnergy(muon->energy * MeV);
   /*if(MySD::GetEvNo()  <20){
	   std::cout << "EvNo : " << MySD::GetEvNo() << std::endl;
	   std::cout << "Injected Energy : " << (muon->energy * MeV) << std::endl;
   }*/
   fParticleGun->GeneratePrimaryVertex(event);
}
