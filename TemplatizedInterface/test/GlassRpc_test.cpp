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
#include "TApplication.h"
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

  //Properties *rpc1 = new GlassRpc("FirstRpc",channelVector);
  Detector *rpc1 = new CmsRpc(3,"FirstGlassRpc", -75,-1);
  //Detector *rpc2 = new GlassRpc(3,"FirstGlassRpc", 105,-1);
  std::cout<<"Name : "<< rpc1->GetName() << std::endl;
  std::cout<<"Total Num of Channels : "<< rpc1->GetTotalNumOfChannels() << std::endl;
  Print(rpc1);

  //v.Register(rpc1);

  std::cout<<"===========================================================================" << std::endl;
  Detector *rpc2 = new GlassRpc(3,"SecondGlassRpc", 75,63);
  Print(rpc2);
  v.Register(rpc2);

  Detector *topPlane = new TriggeringPlane(2,"TopPlane", -105,-1);
  Detector *bottomPlane = new TriggeringPlane(2,"BottomPlane", 105,7);

  std::cout<<"This should be cms : " << rpc1->GetDetectorType() << std::endl;
  std::cout<<"This shoudl be glass " << rpc2->GetDetectorType() << std::endl;
  std::cout<<"This shoudl be trg " << topPlane->GetDetectorType() << std::endl;

  //v.Register(topPlane);
  //v.Register(bottomPlane);


  Tomography::SetupManager s;
  s.Register(rpc1);
  s.RunThread();

  v.Show();

  TTimer timer(20); // every 20 millisecond
  // timer.SetCommand("gEve->Redraw3D(kTRUE);");
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();
  fApp->Run();
}


