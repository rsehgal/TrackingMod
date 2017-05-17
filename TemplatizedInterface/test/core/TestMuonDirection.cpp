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
#include "GenericXYDetector.h"
#include "Event.h"

typedef Tomography::Properties Detector;
using namespace Tomography;


int main(int argc, char *argv[]) {
std::string temp_str = std::to_string(atoi(argv[1]));
temp_str += ".root";
int clusterSize = 1;
//TApplication *fApp = new TApplication("Test", NULL, NULL);
Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);


#if(1) //using concept of Detector
  Detector *trgPlanes = new GenericXYDetector(2,"XYTriggeringPlanes",0,-1,8,100.,100.,1);
  trgPlanes->SetClusterSize(clusterSize);
 // SetupManager::instance()->Register(trgPlanes);
  //trgPlanes->GetStripsHitPlot3D();
  trgPlanes->SetEfficiency();

  std::vector<Event*> eventVect = trgPlanes->GetEventVector();
  //std::cout<<"Size of Event vector : " << eventVect.size() << std::endl;
  int numOfTopToBottomEvents = 0;
  int numOfBottomToTopEvents = 0;
  //std::cout<<"---- Size of Event VEctor : "<< eventVect.size() << std::endl;
  for(int i=0 ; i < eventVect.size() ; i++){
	  if(eventVect[i]->IsMovingTopToBottom()){
		  numOfTopToBottomEvents++;
	  }else{
		  numOfBottomToTopEvents++;
		  std::cout<<"==== Moving Bottom to Top : Event No. : "<< eventVect[i]->GetEventNum() << " ===== Timing difference (Bottom-Top) : "
				   << (eventVect[i]->GetBottomPlaneTiming() - eventVect[i]->GetTopPlaneTiming()) << std::endl;
	  }
  }
  std::cout<<"@@@@@@ Total Num of Event where direction of motion in Bottom to Top : "<< numOfBottomToTopEvents << " @@@@@@@@@" << std::endl;
#endif

//fApp->Run();
}
