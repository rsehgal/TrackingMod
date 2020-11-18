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
#include "DetectorMapping.h"
using Tracking::Vector3D;

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());

  // Set the kinetic energy of the protons to 50 keV
  // and tell the gun to emit them along the x-axis
  fParticleGun->SetParticleEnergy(50. * keV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm, 80.*cm, 0 * cm ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., -1, 0));
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete fParticleGun; }

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(const char *inputfile) {

#ifdef USE_CRY
        cryG4Interface = new CryGeantInterface();
        cryG4Interface->ForCry(inputfile);
#endif
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {

   Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::instance();
   double gunZ = detectorMap->GetGunZ();
   #ifdef USE_CRY
   if(Tomography::EventBreak::instance()->BreakSimulation())
         return;
        //std::cout << "Generating Event using CRY @@@@@@@@@@@@@@@@@@@@ " << std::endl;
        cryG4Interface->GeneratePrimariesForCry(event,false);
   #else  

        Vector3D<double> pt1(Tracking::Global::GenRandomDet(-45.*cm,45.*cm),
                             gunZ,
      					   Tracking::Global::GenRandomDet(-50.*cm,50.*cm));

        Vector3D<double> pt2(Tracking::Global::GenRandomDet(-45.*cm,45.*cm),
                            -gunZ,
      					  Tracking::Global::GenRandomDet(-50.*cm,50.*cm));

        //std::cout<< pt.Unit().x() << pt.Unit().y() << pt.Unit().z() << std::endl;
        //pt1.Set(0.,0.,0.);
        double x = (pt2-pt1).Unit().x();
        double y = (pt2-pt1).Unit().y();
        double z = (pt2-pt1).Unit().z();

	#ifdef RANDOM_GUN
        fParticleGun->SetParticlePosition(G4ThreeVector(pt1.x(),pt1.y(),pt1.z()));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
  	#else
        fParticleGun->SetParticlePosition(G4ThreeVector(0.,gunZ,0.));
   #endif

   //fParticleGun->SetParticlePosition(G4ThreeVector(-50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);
  //fParticleGun->SetParticlePosition(G4ThreeVector(60.*cm, 80.*cm, 0.));
  fParticleGun->GeneratePrimaryVertex(event);
   //fParticleGun->SetParticlePosition(G4ThreeVector(50 * cm, 0., -120 * cm));
   //fParticleGun->GeneratePrimaryVertex(event);
   #endif
}
