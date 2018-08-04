#include "GlassRpc.h"
#include "TriggeringPlane.h"
#include <iostream>
#include <TCanvas.h>
#include "Tree.h"
#include "TH2F.h"
#include "TFile.h"
#include <TApplication.h>
#include <TStyle.h>
#include "DetectorMapping.h"
//Trying to use Factory
#include "Factory.h"


typedef Tomography::Properties Detector;
using namespace Tomography;

void GenerateTimingHistogram(std::string);

int main(int argc, char **argv) {
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::string temp_str = std::to_string(atoi(argv[1]));
	GenerateTimingHistogram(temp_str);
    fApp->Run();
}

#if(0)
void GenerateTimingHistogram(){

}
#endif

#if(1) //Old Traditional way
void GenerateTimingHistogram(std::string runNum) {
  Scintillator::SetStartingId(-1);
  // ScintillatorPlane topPlane(2,8,"Top-Plane");
  // ScintillatorPlane bottomPlane(2,8,"Bottom-Plane");
  
  //Commented to try to use Factory
  //Detector *topPlane = new TriggeringPlane(2, "TopPlane", -150, -1);
  Detector *topPlane = Factory<TriggeringPlane>::MakeInstance(2, "TopPlane", -150, -1);

  Detector *bottomPlane = new TriggeringPlane(2, "BottomPlane", 150, 7);
  int nxbins = 3500;
  int xlow = -1.000;
  int xhigh = 45.500;
  int nybins = 150;
  int ylow = -10;
  int yhigh = 140;
  Tracking::Channel *trig = 0;
  Tracking::Channel *ch = 0;

  std::vector<TH2F*> histVect;

  //To disable stat box
  gStyle->SetOptStat(0);

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(2, 2);
  //c2->cd(1);
  std::string temp_str = runNum;
  temp_str += ".root";
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  Tracking::Tree *t = Tracking::Tree::instance()->GetTree();
  int numOfEvents = t->GetNumOfEvents();
  std::cout << "Total Number of Events in the Data File : " << numOfEvents << std::endl;



    std::vector<int> firedIDVector;
    std::vector<Tomography::Scintillator*> scintVector;
    std::vector<int> firedStripVector ;

	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	// std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
	// std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
	// std::vector<int> moduleVector = detectorMap->GetModuleVector();
	// std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();
	int clusterSize = 1;

    std::vector<Tomography::Mapping::Detector*> detectorVector = detectorMap->GetDetectorVector();

	//for(int detNo = 0 ; detNo < detectorNamesVector.size()-1; detNo++){
    for(int detNo = 0 ; detNo < detectorVector.size()-1; detNo++){
	//for(int detNo = 0 ; detNo < 1; detNo++){
		c2->cd(detNo+1);
	    //std::cout << "Name of Detector : " << (detNo+1) << " : " << detectorNamesVector[detNo] << std::endl;
        std::cout << "Name of Detector : " << (detNo+1) << " : " << detectorVector[detNo]->sDetectorName << std::endl;
	    //Detector *rpc = new Tomography::GlassRpc(moduleVector[detNo], detectorNamesVector[detNo], zcoordinateVector[detNo], startChannelVector[detNo]);
        Detector *rpc = new Tomography::GlassRpc(detectorVector[detNo]->sModule, detectorVector[detNo]->sDetectorName, 
                                                 detectorVector[detNo]->sZCoordinate, detectorVector[detNo]->sStartingChannel);
	    rpc->SetClusterSize(clusterSize);
	    //histVect.push_back(new TH2F(detectorNamesVector[detNo].c_str(), "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh));
        histVect.push_back(new TH2F(detectorVector[detNo]->sDetectorName.c_str(), "Timing", nxbins, xlow, 
                                    xhigh, nybins, ylow, yhigh));
	    histVect[detNo]->GetXaxis()->CenterTitle(true);
	    histVect[detNo]->GetYaxis()->CenterTitle(true);
	    //histVect[detNo]->SetTitleFontSize(10);
	    histVect[detNo]->GetXaxis()->SetTitleOffset(1.3);
	    histVect[detNo]->GetYaxis()->SetTitleOffset(1.3);
	    histVect[detNo]->GetYaxis()->SetTitle("TDC Channel Number");
	    histVect[detNo]->GetXaxis()->SetTitle("Timing (micro second)");

	    ScintillatorPlane *topScintPlane = topPlane->GetPlane(0);
	    ScintillatorPlane *bottomScintPlane = bottomPlane->GetPlane(0);

	    int multiStripCounter = 0;

	    		for (int evNo = 0; evNo < numOfEvents; evNo++) {
	    		  topScintPlane->SetFiredStripsVector(evNo);
	    		  bottomScintPlane->SetFiredStripsVector(evNo);

	    		  rpc->SetEventDetected(evNo);

	    		  if(rpc->EventDetected()){
	    		  for (int plNum = 0; plNum < rpc->GetNumOfPlanes(); plNum++) {
	    			    	firedIDVector.clear();
	    			    	firedIDVector = rpc->GetPlane(plNum)->GetFiredStripsIDVector();

	    			    	scintVector.clear();
	    			    	scintVector = rpc->GetPlane(plNum)->GetScintVector();

	    			    	firedStripVector.clear();
	    			    	firedStripVector = rpc->GetPlane(plNum)->GetFiredStripsVector();

	    			    	if(firedStripVector.size() > 1)
	    			    		multiStripCounter++;


	    			    	if(firedStripVector.size() ){
	    			    		for(int i = 0 ; i < firedStripVector.size() ; i++){
	    			    			histVect[detNo]->Fill(scintVector[firedStripVector[i]]->GetValue()*0.2/1000.,firedIDVector[i]);
	    			    		}
	    			    	}

	    		  } // end of plane loop
	    		  } // enf of EventDetection IF
#if(1)
	    		  //For Top Scintillator Plane
	    		  {
	    		  firedIDVector.clear();
	    		  firedIDVector = topScintPlane->GetFiredStripsIDVector();

	    		  scintVector.clear();
	    		  scintVector = topScintPlane->GetScintVector();

	    		  firedStripVector.clear();
	    		  firedStripVector = topScintPlane->GetFiredStripsVector();

   		  	      if(firedStripVector.size() > 0 && firedStripVector.size() <= clusterSize){
	    		   		for(int i = 0 ; i < firedStripVector.size() ; i++){
	    		  		histVect[detNo]->Fill(scintVector[firedStripVector[i]]->GetValue()*0.2/1000.,firedIDVector[i]);
	    		  }
	    		  }
	    		  }

	    		  //For Bottom Scintillator Plane
	    		  {
	    		  firedIDVector.clear();
	    		  firedIDVector = bottomScintPlane->GetFiredStripsIDVector();

	    		  scintVector.clear();
	    		  scintVector = bottomScintPlane->GetScintVector();

	    		  firedStripVector.clear();
	    		  firedStripVector = bottomScintPlane->GetFiredStripsVector();

   		  	      if(firedStripVector.size() > 0 && firedStripVector.size() <= clusterSize){
	    		   		for(int i = 0 ; i < firedStripVector.size() ; i++){
	    		  		histVect[detNo]->Fill(scintVector[firedStripVector[i]]->GetValue()*0.2/1000.,firedIDVector[i]);
	    		  }
	    		  }
	    		  }




	    		  //Trigger
	    		  {
	    		  trig = t->GetEntry("Module2_LE_CH31", evNo);
	    		  if(trig->size()==1)
	    			  histVect[detNo]->Fill(trig->at(0)*0.2/1000., 31);


	    		  }

#endif

	    		  histVect[detNo]->Draw();




	    	} // end of event loop

	    		std::cout << "MultiStripCounter for detector : " << detNo <<" : " << multiStripCounter << std::endl;
	    delete rpc;
	} // end of detector loop

	  c2->Modified();
	  c2->Update();
}
#endif
