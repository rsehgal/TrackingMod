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
#include "DetectorMapping.h"
#include "GenericXYDetector.h"
#include "TriggeringPlane.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]) {
  std::string temp_str = std::to_string(atoi(argv[1]));
  std::string daqinfofile = temp_str;
  temp_str += ".root";
  int clusterSize = 10;
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

  //Reading Detector data from file, instead of doing hard coding here.
  Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
  std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
  std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
  std::vector<int> moduleVector = detectorMap->GetModuleVector();
  std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

  for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
    std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
    Detector *rpc = new GlassRpc(moduleVector[i], detectorNamesVector[i], zcoordinateVector[i], startChannelVector[i]);
    rpc->SetClusterSize(1);
    rpc->GetHitPlot();
    rpc->GetHitPlot3D_V2();
    rpc->GetX_Y_And_ClusterHistograms();
    rpc->GetStripProfile();
    delete rpc;
  }

  Detector *trgPlanes = new Tomography::GenericXYDetector(2,"XYTriggeringPlanes",0,-1,8,100.,100.,1);
  trgPlanes->SetClusterSize(2);
  trgPlanes->SetClusterSize(clusterSize);
  SetupManager::instance()->Register(trgPlanes);
  //TCanvas *cPattern = new TCanvas("cPattern", "cPattern", 200, 10, 700, 500);
  trgPlanes->GetStripsHitPlot3D();//->Draw("LEGO2");

  detectorMap->ReadDaqInfo(daqinfofile);
  detectorMap->PrintEfficiencyVector();
  fApp->Run();
}
