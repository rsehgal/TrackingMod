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
#include <TH1F.h>

typedef Tomography::Properties Detector;
using namespace Tomography;


int main(int argc, char *argv[]) {
std::string temp_str = std::to_string(atoi(argv[1]));
temp_str += ".root";
int clusterSize = 1;
TApplication *fApp = new TApplication("Test", NULL, NULL);
Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
c2->Divide(3,1);

#if(1) //using concept of Detector
  Detector *trgPlanes = new GenericXYDetector(2,"XYTriggeringPlanes",0,-1,8,100.,100.,1);
  trgPlanes->SetClusterSize(clusterSize);
  SetupManager::instance()->Register(trgPlanes);
  trgPlanes->GetStripsHitPlot3D();
//  trgPlanes->SetEfficiency();

  std::vector<Event*> eventVect = trgPlanes->GetEventVector();
  //std::cout<<"Size of Event vector : " << eventVect.size() << std::endl;
  int numOfTopToBottomEvents = 0;
  int numOfBottomToTopEvents = 0;
  std::cout<<"---- Size of Event VEctor : "<< eventVect.size() << std::endl;
  TH1F *tdiff = new TH1F("All","AllEvents",1500,-500,1000);
  TH1F *tdiffPos = new TH1F("Moving Down","Moving Down",500,-250,250);
  TH1F *tdiffNeg = new TH1F("Moving Up","Moving Up",500,-250,250);
  int diff=0;
  for(int i=0 ; i < eventVect.size() ; i++){

	  diff = (eventVect[i]->GetBottomPlaneTiming() - eventVect[i]->GetTopPlaneTiming());

	  if(eventVect[i]->IsMovingTopToBottom()){
		  tdiffPos->Fill(diff);
		  numOfTopToBottomEvents++;
	  }else{
		  tdiffNeg->Fill(diff);
		  numOfBottomToTopEvents++;
		  std::cout<<"==== Moving Bottom to Top : Event No. : "<< eventVect[i]->GetEventNum() << " ===== Timing difference (Bottom-Top) : "
				   << diff << std::endl;

	  }

	  tdiff->Fill(diff);
  }

  c2->cd(1);
  tdiffPos->Draw();
  c2->cd(2);
  tdiffNeg->Draw();
  c2->cd(3);
  tdiff->Draw();


  std::cout<<"@@@@@@ Total Num of Event where direction of motion in Bottom to Top : "<< numOfBottomToTopEvents << " @@@@@@@@@" << std::endl;
#endif

fApp->Run();
}
