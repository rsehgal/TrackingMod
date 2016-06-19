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
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
  Tracking::Tree::instance()->ReadTree("6915.root", "BSC_DATA_TREE", 0);
  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", -30, 31);
  Tomography::SetupManager::instance()->Register(rpc1);
  Tomography::SetupManager::instance()->Register(rpc2);
  Tomography::SetupManager::instance()->SetEfficiency("GLASS");
  std::cout<<"Setup Efficiency : " << Tomography::SetupManager::instance()->GetEfficiency() << std::endl;
  return 0;
}