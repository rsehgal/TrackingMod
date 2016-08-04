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
//#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
//#include "Update.h"
#include "Tree.h"
#include "Statistics.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char* argv[]) {
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 120, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", 150, 31);

  Tomography::SetupManager::instance()->Register(rpc1);
  Tomography::SetupManager::instance()->Register(rpc2);

  rpc1->GetX_Y_And_ClusterHistograms();
  rpc1->GetStripProfile();
  rpc2->GetX_Y_And_ClusterHistograms();
  rpc2->GetStripProfile();

  fApp->Run();
}
