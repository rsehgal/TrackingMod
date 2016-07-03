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
  Detector *MT1 = new GlassRpc(4, "FirstGlassRpc", 45, 31);
  Detector *MT2 = new GlassRpc(2, "SecondGlassRpc", -75, 31);
  MT1->SetClusterSize(1);
  MT2->SetClusterSize(1);
  MT1->GetHitPlot();
  MT2->GetHitPlot();
  fApp->Run();
}