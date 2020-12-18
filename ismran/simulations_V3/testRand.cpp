#include<iostream>
#include<random>
#include <time.h>
#include <TVector3.h>
#include "HelperFunctions.h"
#include <TF1.h>
#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>
int main(){
 srand( (unsigned)time( NULL ) );
 //double theta = 0.78539816;
 TF1* samplingFormula = GetCos2ThetaFormula();
 double h_r = 120.;
 double h_g = 120.*cm;

// for(unsigned int i = 0 ; i < 10 ; i++)
{
 double theta = 0.785066;// samplingFormula->GetRandom();
 //double randSample = (double)rand()/RAND_MAX;
 //double randPhi = randSample*(2*M_PI);
 double randPhi = GenerateRandom()*(2*M_PI);
 double r_r = h_r*tan(theta);
 double r_g = h_g*tan(theta);
 TVector3 pt1_r(-163.263792,600.000000,66.954600);//(0.,60,0);
 G4ThreeVector pt1_g(-163.263792,600.000000,66.954600);//(0.,60*cm,0);
 TVector3 pt2_r(-1144.495300,-600.000000,358.078688);//(r_r*cos(randPhi),-60.,r_r*sin(randPhi));
 G4ThreeVector pt2_g(-1144.495300,-600.000000,358.078688);//(r_g*cos(randPhi),-60*cm,r_g*sin(randPhi));
 TVector3 ref_r(0.,-1.,0.);
 G4ThreeVector ref_g(0.,-1.,0.);
 TVector3 dir_r = (pt2_r-pt1_r).Unit();
 G4ThreeVector dir_g = (pt2_g-pt1_g).unit();
 std::cout << "ROOT version DIR : " ; dir_r.Print();
 std::cout << "Geant verson DIR : " << dir_g << std::endl;
 double angle_r = dir_r.Angle(ref_r);
 double angle_g = dir_g.angle(ref_g);
 std::cout <<"ROOT version starting angle : " << theta <<" : Phi : " << randPhi << " : Recalc angle : " << angle_r << std::endl;
 std::cout <<"Geant verson starting angle : " << theta <<" : Phi : " << randPhi << " : Recalc angle : " << angle_g << std::endl;
 }
 /*
 for(unsigned int i = 0 ; i < 100 ; i++){
  double randSample = (double)rand()/RAND_MAX;
  std::cout << randSample <<" : " << randSample*2*M_PI << std::endl;
 }
*/

 return 0;
}
