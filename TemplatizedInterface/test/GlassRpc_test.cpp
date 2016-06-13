/*
 * GlassRpc_test.cpp
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "Update.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

void Print(Detector *rpc1){

  int numOfPlanes = rpc1->GetNumOfPlanes();
    std::cout<<"Num Of Planes : " << numOfPlanes << std::endl;
    for(int i= 0 ; i<numOfPlanes; i++){
      std::cout<<"---------------------------------------------------------------"<<std::endl;
      int numOfChannels = rpc1->GetScintillatorPlaneVector()[i]->GetScintVector().size();
      for(int j = 0 ; j < numOfChannels ; j++){
        //std::cout<<"ChannelId : " << rpc1->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetScintId() << std::endl;
        std::cout<<"Name : " << rpc1->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetName() << std::endl;
        std::cout<<"Location : "; rpc1->GetScintillatorPlaneVector()[i]->GetScintVector()[j]->GetPlacedLocation().Print();
      }
    }
}

int main(){
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper v;
  std::vector<int> channelVector;
  channelVector.push_back(32);

  std::cout<<"===========================================================================" << std::endl;
  Detector *rpc0 = new GlassRpc(3,"SecondGlassRpc", 120,-1);
  Detector *rpc1 = new GlassRpc(3,"SecondGlassRpc", 90,-1);
  Detector *rpc2 = new GlassRpc(3,"SecondGlassRpc", 60,-1);
  Detector *rpc3 = new GlassRpc(3,"SecondGlassRpc", 30,63);
//
  Detector *rpc4 = new GlassRpc(4,"SecondGlassRpc", -30,-1);
  Detector *rpc5 = new GlassRpc(4,"SecondGlassRpc", -60,-1);
  Detector *rpc6 = new GlassRpc(4,"SecondGlassRpc", -90,-1);
  Detector *rpc7 = new GlassRpc(4,"SecondGlassRpc", -120,-1);

//  Print(rpc2);
 v.Register(rpc0);
 v.Register(rpc1);
 v.Register(rpc2);
 v.Register(rpc3);
 v.Register(rpc4);
 v.Register(rpc5);
 v.Register(rpc6);
 v.Register(rpc7);

//  Detector *topPlane = new TriggeringPlane(2,"TopPlane", -150,-1);
//  Detector *bottomPlane = new TriggeringPlane(2,"BottomPlane", 150,7);
//
//  std::cout<<"This should be cms : " << rpc1->GetDetectorType() << std::endl;
//  std::cout<<"This shoudl be glass " << rpc2->GetDetectorType() << std::endl;
//  std::cout<<"This shoudl be trg " << topPlane->GetDetectorType() << std::endl;
//
//  v.Register(topPlane);
//  v.Register(bottomPlane);


  //Tomography::SetupManager s;
  Tomography::SetupManager::instance()->Register(rpc0);
  Tomography::SetupManager::instance()->Register(rpc1);
  Tomography::SetupManager::instance()->Register(rpc2);
  Tomography::SetupManager::instance()->Register(rpc3);
  Tomography::SetupManager::instance()->Register(rpc4);
  Tomography::SetupManager::instance()->Register(rpc5);
  Tomography::SetupManager::instance()->Register(rpc6);
  Tomography::SetupManager::instance()->Register(rpc7);
//  //Tomography::SetupManager::instance()->RunThread();
  Update u;
  u.RunThread();

  v.Show();

  TTimer timer(20); // every 20 millisecond
  //// timer.SetCommand("gEve->Redraw3D(kTRUE);");
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();
  fApp->Run();
}


