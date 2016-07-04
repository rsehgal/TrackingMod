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

int main(int argc, char *argv[]) {
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  int clusterSize = 1;
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  Detector *ScintTop = new TriggeringPlane(2, "ScintTop", 105, -1);
  ScintTop->SetClusterSize(1);
  Detector *ScintBottom = new TriggeringPlane(2, "ScintBottom", -105, 7);
  ScintBottom->SetClusterSize(1);

  Detector *MT1 = new GlassRpc(4, "FirstGlassRpc", 45, 31);
  Detector *MT2 = new GlassRpc(2, "SecondGlassRpc", -75, 31);
  SetupManager *setup = SetupManager::instance();
  MT1->SetClusterSize(clusterSize);
  MT2->SetClusterSize(clusterSize);
  setup->Register(ScintTop);
  setup->Register(ScintBottom);
  setup->Register(MT1);
  setup->Register(MT2);
  MT1->GetHitPlot();
  MT2->GetHitPlot();
  setup->GetHitPlot("GLASS");
  fApp->Run();
}
