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
//#include "Update.h"
#include "Update_Evolution.h"
#include "Tree.h"
 #include "Statistics.h"
#include "Paddle.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {

  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper *v = Tomography::VisualizationHelper::instance();
  v->InitializeVisualizer();
  std::vector<int> channelVector;
  channelVector.push_back(32);

  //Tracking::Tree::instance()->ReadTree("6915.root", "BSC_DATA_TREE", 0);
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  std::cout << "===========================================================================" << std::endl;
/*  Detector *topPlane = new TriggeringPlane(2,"TopPlane", 0, 0,105, 0.,-1);
   Detector *bottomPlane = new TriggeringPlane(2,"BottomPlane",0.,0., -105, 0., 7);
  Detector *rpc1 = new GlassRpc(4, "FirstGlassRpc",0.,0., 45, 0.,31);
  Detector *rpc2 = new GlassRpc(2, "SecondGlassRpc",0.,0., -75,0., 31);*/

//  Detector *topPlane = new TriggeringPlane(2,"TopPlane", 105, -1);
//  topPlane->SetClusterSize(1);
//  Detector *bottomPlane = new TriggeringPlane(2,"BottomPlane",-105, 7);
//  bottomPlane->SetClusterSize(1);
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", -75, 31);
  rpc1->SetClusterSize(1);
  Detector *rpc2 = new GlassRpc(3, "SecondGlassRpc",-45, 31);
  rpc2->SetClusterSize(1);
  Detector *rpc3 = new GlassRpc(4, "FirstGlassRpc", 45, 31);
    rpc3->SetClusterSize(1);
    Detector *rpc4 = new GlassRpc(5, "SecondGlassRpc",75, 31);
    rpc4->SetClusterSize(1);

  //Creating and Registering Paddle
//  Detector *paddle = new Paddle(2,"Paddle", -15, 15, 18., 66.);
//  paddle->GetPlane(0)->GetScintVector()[0]->SetPlacedLocation(Tracking::Vector3D<double>(0.,0.,-15));
//  v.Register(paddle);
//  Tomography::SetupManager::instance()->Register(paddle);

//  std::cout<<"GET PLACED LOCATION of PADDLE : " ;
//  paddle->GetPlane(0)->GetScintVector()[0]->GetPlacedLocation().Print();
  //This line should be added after creation of all RPCs
  //Tomography::ScintillatorPlane::SetClusterSize(1);
  //std::cout<<"CLUSTER : " << Tomography::ScintillatorPlane::GetClusterSize() << std::endl;

  v->Register(rpc1);
  Tomography::SetupManager::instance()->Register(rpc1);
  v->Register(rpc2);
  Tomography::SetupManager::instance()->Register(rpc2);
  v->Register(rpc3);
    Tomography::SetupManager::instance()->Register(rpc3);
    v->Register(rpc4);
    Tomography::SetupManager::instance()->Register(rpc4);

//  v.Register(topPlane);
//  Tomography::SetupManager::instance()->Register(topPlane);
//  v.Register(bottomPlane);
//  Tomography::SetupManager::instance()->Register(bottomPlane);



  Tomography::evolution::Update u;
  //Statistics s;
  //if required delay can be Set using SetDelay() function
  //Default value is set to 2 sec
  u.SetDelay(1);
  u.RunThread2();
  //s.Initialize();
  //s.GetXPlaneHistogram();
  v->Show();

  TTimer timer(20); // every 20 millisecond
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();
  fApp->Run();
}
