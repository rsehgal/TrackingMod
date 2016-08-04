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
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {

  Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
  Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", -30, 31);

  bool verbose = true;

  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";

  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  rpc1->SetEfficiency();
  rpc1->GetPlane(0)->SetEfficiency();
  rpc1->GetPlane(1)->SetEfficiency();
  if (verbose) {
    std::cout << "Eff of Det-1 : " << rpc1->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 1 : " << rpc1->GetPlane(0)->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 2 : " << rpc1->GetPlane(1)->GetEfficiency() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
  }
  rpc2->SetEfficiency();
  rpc2->GetPlane(0)->SetEfficiency();
  rpc2->GetPlane(1)->SetEfficiency();
  if (verbose) {
    std::cout << "Eff of Det-2 : " << rpc2->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 1 : " << rpc2->GetPlane(0)->GetEfficiency() << std::endl;
    std::cout << "Eff w.r.t Plane 2 : " << rpc2->GetPlane(1)->GetEfficiency() << std::endl;
  }
}
