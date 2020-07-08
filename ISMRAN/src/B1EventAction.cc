#include "B1EventAction.hh"
#include "B1Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
//#include "Tree.h"
//#include "B1RunAction.hh"
//#include <TVector3.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
int B1EventAction::evNo = 0;
std::vector<G4ThreeVector> B1EventAction::position({G4ThreeVector(0.,0.,0.)});
std::vector<G4ThreeVector> B1EventAction::smearedPosition({G4ThreeVector(0.,0.,0.)});

B1EventAction::B1EventAction()
: G4UserEventAction(){} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{ evNo++;

  std::cout <<"=========== Event No : " << evNo <<" started ==========" << std::endl;

  if(!(evNo%1000))
    std::cout << "========  " << evNo << " Event Processed ========= " << std::endl;
  position.clear();
  //B1RunAction::fScatteringAngle = 0.;

}

G4ThreeVector B1EventAction::POCA(G4ThreeVector p, G4ThreeVector u, G4ThreeVector q, G4ThreeVector v){

    double pdotv=p.dot(v);
    double qdotv=q.dot(v);
    double pdotu=p.dot(u);
    double qdotu=q.dot(u);
    double udotv=u.dot(v);
    double vmag2=v.mag2();
    double umag2=u.mag2();


  double s=0.,t=0.;
  double a = -(pdotv-qdotv)/udotv;
  double b1 = udotv*(pdotu-qdotu);
  double b2 = umag2*(pdotv-qdotv);
  double b = b1 - b2;
  double c = (-udotv*udotv + vmag2*umag2);
  double d = (-vmag2*b)/(udotv*c);
  s = a+d;
 
  double numer = (udotv*(pdotu-qdotu) - umag2*(pdotv-qdotv));
  double deno = (udotv*udotv - umag2*vmag2);
  t = numer / deno;
 
  G4ThreeVector p1 = p + u*s;
  G4ThreeVector q1 = q + v*t;

  G4ThreeVector poca = (p1+q1)/2.;
  return poca;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event*)
{  
  B1Run* run 
    = static_cast<B1Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  //std::cout << "Printing from EndOfEventAction..........."<<std::endl;

  //std::cout<<"Position Vector Size : "<< position.size() << std::endl;
  G4ThreeVector incoming = position[2]-position[0];
  G4ThreeVector outgoing = position[5]-position[3];
  G4ThreeVector ref(0.,0.,-1.);

  //Filling Tracks
  
  double angleIncoming = incoming.angle(ref);
  double angleOutgoing = outgoing.angle(ref);
  double fScatteringAngle = angleIncoming-angleOutgoing; //outgoing.angle(incoming);
  //std::cout<<"Scattering Angle : " << fScatteringAngle << std::endl;
  if(std::abs(fScatteringAngle) > .0001)
  {
    run->FillScatteringAngleVector(fScatteringAngle);
    G4ThreeVector poCA = POCA(position[0],incoming.unit(),position[3],outgoing.unit());
    run->InsertPocaPoint(poCA);

  }
  
  std::cout <<"=========== Event No : " << evNo <<" Ended ==========" << std::endl;
}


