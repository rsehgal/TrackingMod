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
#include "DetectorMapping.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char* argv[]) {
  std::string temp_str = std::to_string(atoi(argv[1]));
  std::string daqinfofile = temp_str;
  temp_str += ".root";
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");

  std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
  std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
  std::vector<int> moduleVector = detectorMap->GetModuleVector();
  std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

  for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
  		std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
  		Detector *rpc = new GlassRpc(moduleVector[i], detectorNamesVector[i], zcoordinateVector[i], startChannelVector[i]);
  		rpc->GetX_Y_And_ClusterHistograms();
  		rpc->GetStripProfile();
  		delete rpc;
  	}

  //daqinfofile += ".info";
  detectorMap->ReadDaqInfo(daqinfofile);
  detectorMap->PrintEfficiencyVector();
  fApp->Run();
}
