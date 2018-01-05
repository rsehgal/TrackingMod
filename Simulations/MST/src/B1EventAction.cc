//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1Run.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "LinesAngle.h"
#include "Tree.h"
#include "TInterpreter.h"
#include "Delta.h"

#include "CommonFunc.h"
//#include "Voxel.h"

#include "Imaging.h"

int B1EventAction::eventNo = 0 ;
using Tracking::ImageReconstruction;
using VectorOfVoxelsForAnEvent = std::vector<int>; // This represent std::vector of candidate voxel num which can influenced the muon track.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.),verbose(false),fScatteringAngle(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  eventNo++;
  fEdep = 0.;
  fScatteringAngle = 0.;
  hitVect.clear();
  if(verbose)
    std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int B1EventAction::IfVoxelExist(int voxelNum){
	B1Run* run
	    = static_cast<B1Run*>(
	        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	std::cout<< "VoxelVectorSize : " << run->fVoxelVector.size() << std::endl;
	if(run->fVoxelVector.size()){
		for(int i = 0 ; i < run->fVoxelVector.size() ; i++){
			if(run->fVoxelVector[i]->GetVoxelNum() == voxelNum){
				std::cout<<"Voxel Hit.........Found previously created Voxel... :  "  << run->fVoxelVector[i]->GetVoxelNum() << std::endl;
				return i;
			}
		}
		return -1;
	}else{
		return -1;
	}
}

void B1EventAction::EndOfEventAction(const G4Event*)
{   

  // accumulate statistics in B1Run
  B1Run* run 
    = static_cast<B1Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    run->FillPhysicalTrackVector(hitVect);
  run->AddEdep(fEdep);
  CalcScatteringAngle();

  /* Put some cut condition IF Required
  * if(fScatteringAngle*1000 > 20. && fScatteringAngle*1000 < 100.)
  * if(fScatteringAngle*1000 > 5.)
  */
  //if(fScatteringAngle*1000 > 5.)
  if(fScatteringAngle > 0.)
  {
  GenerateIncomingTrack();
  GenerateOutgoingTrack();
  CalculatePOCA();

  run->GetInComing() = incoming;
  run->GetOutGoing() = outgoing;
  run->SetScattering(fScatteringAngle);//*1000);

  run->GetTreeInstance()->Fill();

  int val = IfVoxelExist(run->GetVoxelator().GetVoxelNumber(fPocaPt));
  std::cout << "Initial Voxel Number : " << run->GetVoxelator().GetVoxelNumber(fPocaPt) << std::endl;
  std::cout<<"Val : " << val << std::endl;
  if(val < 0.)
	  (run->fVoxelVector).push_back(new Voxel(fPocaPt,run->GetVoxelator().GetVoxelNumber(fPocaPt)));
  else
	  (run->fVoxelVector)[val]->Insert(fPocaPt,run->GetVoxelator().GetVoxelNumber(fPocaPt));
  if(val >= 0)
	  std::cout<<"Voxel Exist.......... : VoxelNum : " << val <<  std::endl;


#undef FIND_CANDIDATE_VOXEL
#ifdef FIND_CANDIDATE_VOXEL
	VectorOfVoxelsForAnEvent vectOfVoxels;
  //Detecting HitPoints in Voxelized Volume
  Tracking::Vector3D<double> inComingHitPt = GetIntersection(incoming,-1.*(run->GetVoxelator().GetVoxelizedVolumeDim().z()/2.),1);
  Tracking::Vector3D<double> outGoingHitPt = GetIntersection(incoming,run->GetVoxelator().GetVoxelizedVolumeDim().z()/2.,2);
  std::cout << "InComingHitPoint : ";
  inComingHitPt.Print();
  std::cout << "OutgoingHitPoint : " ;
  outGoingHitPt.Print();

  Tomography::Track tr(inComingHitPt,outGoingHitPt);
  for(int i = 0 ; i < (run->GetVoxelator().GetVoxelatorDim().z()) ; i++){
	  Tracking::Vector3D<double> voxelHitPt = GetIntersection(tr,
			  	  	  	  -1.*(run->GetVoxelator().GetVoxelizedVolumeDim().z()/2.)+(i*run->GetVoxelator().GetEachVoxelDim().z()),
						  1);
	  voxelHitPt.SetZ(voxelHitPt.z()+1);

	  std::cout<<"Voxel Hit Point for Slice : " << i << " : VoxelNumber : " << run->GetVoxelator().GetVoxelNumber(voxelHitPt)
				<< " : PocaPt_VoxelNumber : " << run->GetVoxelator().GetVoxelNumber(fPocaPt) << " : Voxel Point ";
	  voxelHitPt.Print();
	  vectOfVoxels.push_back(run->GetVoxelator().GetVoxelNumber(voxelHitPt));
  }
  //std::cout<<"EventVoxelVectorSize : " << vectOfVoxels.size() << std::endl;
  run->InsertVectorOfVoxels(vectOfVoxels);
  //run->GetVectorOfVoxelForRun().push_back(vectOfVoxels);
  //std::cout<<"RunVoxel-Vector-Size : "<< run->GetVectorOfVoxelForRun().size() << std::endl;
#endif

  }




  /*
   * Caching the above calculated values (IF REQUIRED) in stl vector defined in RUN
   */
  //if(fScatteringAngle*1000 > 5.)
  if(fScatteringAngle > 0.)
  {
  run->FillScatteringAngleVector(fScatteringAngle);
  run->FillIncomingTrackVector(incoming);
  run->FillOutgoingTrackVector(outgoing);
  run->FillPocaPtVector(fPocaPt);
  }

  // 

  std::cout<<"Ending Event no : " << eventNo << std::endl;
/*  if(eventNo == 50){
  const B1RunAction* runAction
      = static_cast<const B1RunAction*>(
          G4RunManager::GetRunManager()->GetUserRunAction());

  G4Run* g4run
      =  G4RunManager::GetRunManager()->GetNonConstCurrentRun();

  std::cout<<"@@@@@@@@@@ Going to call end of Run Action @@@@@@@@@@@ " << std::endl;
  //runAction
  G4RunManager::GetRunManager()->GetUserRunAction()->EndOfRunAction(g4run);
  }*/


}

void B1EventAction::CalcScatteringAngle(){
  int size = hitVect.size();
  int hSize = size/2;

  LinesAngle l;
if(size){
  if(size == 3){
	  //fScatteringAngle = l.GetAngleRadian(l.CalculateAngle(hitVect[0],hitVect[hSize],hitVect[hSize],hitVect[size-1]));
	  fScatteringAngle = 0.;// CommonFunc::Functions::instance()->GetAngleInRadian(hitVect[0],hitVect[hSize],hitVect[hSize],hitVect[size-1]);
  }else{
	  //fScatteringAngle = l.GetAngleRadian(l.CalculateAngle(hitVect[0],hitVect[hSize-1],hitVect[hSize],hitVect[size-1]));
	  fScatteringAngle = CommonFunc::Functions::instance()->GetAngleInRadian(hitVect[0],hitVect[hSize-1],hitVect[hSize],hitVect[size-1]);
  }
}

}

void B1EventAction::GenerateIncomingTrack(){
  //std::cout<<"-----------------------------------------------------"<<std::endl;
  int size = hitVect.size();
  //std::cout<<"Size : must be Six : " << size << std::endl;
  int hSize = size/2;

  incoming.SetP1(hitVect[0]);
  if(size == 3){
	  incoming.SetP2(hitVect[hSize]);
  }else{
	  incoming.SetP2(hitVect[hSize-1]);
  }
  //std::cout<<"INComing : "; hitVect[0].Print() ; std::cout<<" : ";  hitVect[hSize-1].Print();
  //std::cout<< std::endl;
  
}

void B1EventAction::GenerateOutgoingTrack(){
  int size = hitVect.size();
  int hSize = size/2;
  outgoing.SetP1(hitVect[hSize]);
  outgoing.SetP2(hitVect[size-1]);
  //std::cout<<"OUTGoing : "; hitVect[hSize].Print(); std::cout<< " : "; hitVect[size-1].Print();
  //std::cout<< std::endl;

}

void B1EventAction::CalculatePOCA(){
  Tracking::Vector3D<double> p1(0.,0.,0.), q1(0.,0.,0.);
#if(1)

  // Working logic
  fPocaPt = fIm.POCA(incoming.GetP1(),
                     incoming.GetDirCosine(),
                     outgoing.GetP1(),
                     outgoing.GetDirCosine(),p1,q1);
#endif
#if(0)
//Trying to use POCA_Iterative
 fPocaPt = fIm.POCA_Iterative(incoming,outgoing);
#endif
//  fPocaPt.SetColor((int(fScatteringAngle*1000)%6));
//fPocaPt.SetColor((int(fScatteringAngle*1000)/10));
  fPocaPt.SetColor((int(fScatteringAngle*1000)));
  
  //std::cout<<"PocaPt : "; fPocaPt.Print() ; std::cout<<" : ScatteringAngle : "<< fScatteringAngle <<  " : Color : "<< fPocaPt.GetColor() << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
