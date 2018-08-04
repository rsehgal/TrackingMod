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
#include "DetectorMapping.h"
#include "B1EventAction.hh"
#include "base/Global.h"
using Tracking::Vector3D;

//#ifdef USE_CRY
//#include "CryGeantInterface.h"
//#endif

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());

  // Set the kinetic energy of the protons to 50 keV
  // and tell the gun to emit them along the x-axis
  fParticleGun->SetParticleEnergy(50. * keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 150 * cm ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
}

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(const char *inputfile) {

#ifdef USE_CRY
  if(Tomography::effEvNo > 100)
    return;
	cryG4Interface = new CryGeantInterface();
	cryG4Interface->ForCry(inputfile);
//  ForCry(inputfile);

#endif
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete fParticleGun; }

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
 // std::cout << "EffEvNo from PrimaryGenerator : " << Tomography::effEvNo << std::endl;
   // if(Tomography::effEvNo > 100)
   //  return;
  if(Tomography::EventBreak::instance()->BreakSimulation())
        return;
  
  //fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -120 * cm));
  Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::instance();
  double gunZ = detectorMap->GetGunZ();

  std::cout << "Event No from MyPrimaryGeneratorAction : " << B1EventAction::GetEventNum() << std::endl;
if(Tomography::EventBreak::instance()->BreakSimulation())
        return;
#ifdef USE_CRY
  // if(Tomography::EventBreak::instance()->BreakSimulation())
  //       return;
	std::cout << "Generating Event using CRY @@@@@@@@@@@@@@@@@@@@ " << std::endl;
	cryG4Interface->GeneratePrimariesForCry(event);
#else

  Vector3D<double> pt1(Tracking::Global::GenRandomDet(-50.*cm,50.*cm),
                      Tracking::Global::GenRandomDet(-50.*cm,50.*cm),
                      //Tracking::Global::GenRandomDet(0.,50.)
                      //150*cm
                      gunZ
                      );

  Vector3D<double> pt2(Tracking::Global::GenRandomDet(-50.*cm,50.*cm),
                      Tracking::Global::GenRandomDet(-50.*cm,50.*cm),
                      //-150*cm
                      -gunZ);
  //std::cout<< pt.Unit().x() << pt.Unit().y() << pt.Unit().z() << std::endl;
  //pt1.Set(0.,0.,0.);
  double x = (pt2-pt1).Unit().x();
  double y = (pt2-pt1).Unit().y();
  double z = (pt2-pt1).Unit().z();

  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));

  #ifdef RANDOM_GUN
  //fParticleGun->SetParticlePosition(G4ThreeVector(Tracking::Global::GenRandomDet(-50.,50.)*cm, Tracking::Global::GenRandomDet(-50.,50.)*cm, -120 * cm));
  fParticleGun->SetParticlePosition(G4ThreeVector(pt1.x(),pt1.y(),pt1.z()));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  #endif
  fParticleGun->GeneratePrimaryVertex(event);
   //fParticleGun->SetParticlePosition(G4ThreeVector(50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);
#endif
}
