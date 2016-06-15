/*
 * GlassRpc.h
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_PROPERTIES_H_
#define TEMPLATIZEDINTERFACE_INC_PROPERTIES_H_

#include <cstring>
#include "base/Global.h"
#include <vector>
#include <algorithm>
//#include "Plane.h"
//#include <TGeoVolume.h>
//#include <Plane.h>
#include <vector>
#include "Scintillator.h"
#include "ScintillatorPlane.h"
#include "Tree.h"
#include <TCanvas.h>
#include <TH1F.h>
//#include <TApplication.h>

//#include "Tree.h"
namespace Tomography {

class Properties {
private:
  int fTotalNumOfChannels;
  std::string fName;
  double fLength;
  double fBreadth;
  double fHeight;
  double fEfficiency;
  int fDim;
  int fNumOfPlanes;
  std::vector<int> fChannelsInDim; //where vector index represent dimension number
  std::vector<ScintillatorPlane*> fScintillatorPlaneVector;
  double fZPos;
  std::string fDetectorType;

  /* Every Detector will be having different channel dimension
   * Here i am considering that each channel is rectangular in shape
   *
   */




public:
  Properties();
  Properties(std::string name,std::vector<int> channelsInDim);
  virtual ~Properties();

  void SetName(std::string name){fName = name;}
  void SetLength(double length){fLength = length;}
  void SetBreadth(double breadth){fBreadth = breadth;}
  void SetHeight(double height){fHeight = height;}
  void SetLBH(double length, double breadth, double height){
    fLength = length;
    fBreadth = breadth;
    fHeight = height;
  }
  void SetZPos(double zPos){fZPos = zPos;}
  void SetDetectorType(std::string detType){fDetectorType = detType;}
  void SetEfficiency();
  /*void SetEfficiency()
  {
    int count = 0;
    int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
    for(int i = 0 ; i <  numOfEvents ; i++)
    {
       SetFiredStripsVector(i);
       if(GetPlane(0)->GetFiredStripsVector().size() == 0 || GetPlane(1)->GetFiredStripsVector().size() == 0)
        count++;
    }
          double tmp = ((double)(numOfEvents-count))*100.; 
          fEfficiency = tmp/(double)numOfEvents;
  }
  */

  double GetZPos(){return fZPos;}
  double GetLength(){return fLength;}
  double GetBreadth(){return fBreadth;}
  double GetHeight(){return fHeight;}

  void SetChannelDimVector(std::vector<int> channelsInDim ) {fChannelsInDim = channelsInDim;}
  void SetNumOfPlanes(int numOfPlanes){fNumOfPlanes = numOfPlanes;}

  std::string GetName(){return fName;}
  void Initialize(){fTotalNumOfChannels = 0;}
  int GetTotalNumOfChannels(){return fTotalNumOfChannels;}
  int GetReadOutDimension(){return fDim;} //1, 2, or 3
  int GetNumOfChannelsInDimension(int dim){return fChannelsInDim[dim];}
  int GetNumOfPlanes(){return fNumOfPlanes;}
  std::vector<ScintillatorPlane*> GetScintillatorPlaneVector(){return fScintillatorPlaneVector;}
  ScintillatorPlane* GetPlane(int planeNo){return fScintillatorPlaneVector[planeNo];}
  std::string GetDetectorType(){return fDetectorType;}
  void GetStripProfile();
  void GetX_Y_And_ClusterHistograms();

  //void FetchInfo(Tracking::Tree t);
  void InsertPlane(ScintillatorPlane *plane){
    fScintillatorPlaneVector.push_back(plane);
    fTotalNumOfChannels += plane->GetNumOfScintillators();
  }

  void SetFiredStripsVector(int evNo){
	  for(int i=0; i < fNumOfPlanes ; i++){
		  fScintillatorPlaneVector[i]->SetFiredStripsVector(evNo);
	  }
  }

  std::vector<int> GetFiredStripsVector(int planeNo){
	  fScintillatorPlaneVector[planeNo]->GetFiredStripsVector();
  }

  std::vector<int> GetFiredStripsIDVector(int planeNo){
  	  fScintillatorPlaneVector[planeNo]->GetFiredStripsIDVector();
    }

  std::vector<std::string> GetFiredStripsNameVector(int planeNo){
    	  fScintillatorPlaneVector[planeNo]->GetFiredStripsNameVector();
    }
  double GetEfficiency(){return fEfficiency;}

};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_ */
