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
#include "Tree.h"
 #include "Statistics.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main() {
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tomography::VisualizationHelper v;
  std::vector<int> channelVector;
  channelVector.push_back(32);

  Tracking::Tree::instance()->ReadTree("6915.root", "BSC_DATA_TREE", 0);
  std::cout << "===========================================================================" << std::endl;
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 120, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", -120, 31);

  //This line should be added after creation of all RPCs
  Tomography::ScintillatorPlane::SetClusterSize(1);

  v.Register(rpc1);
  Tomography::SetupManager::instance()->Register(rpc1);
  v.Register(rpc2);
  Tomography::SetupManager::instance()->Register(rpc2);


  Update u;
  //Statistics s;
  //if required delay can be Set using SetDelay() function
  //Default value is set to 2 sec
  u.SetDelay(2);
  u.RunThread2();
  //s.Initialize();
  //s.GetXPlaneHistogram();
  v.Show();

  TTimer timer(20); // every 20 millisecond
  timer.SetCommand("gEve->DoRedraw3D();");
  timer.TurnOn();
  fApp->Run();
}
