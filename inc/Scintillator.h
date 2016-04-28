/*
 * Scintillator.h
 *
 *  Created on: Mar 9, 2016
 *      Author: rsehgal
 */

#ifndef INC_SCINTILLATOR_H_
#define INC_SCINTILLATOR_H_


#define scint_top1 0
#define scint_top2 1
#define scint_top3 2
#define scint_top4 3
#define scint_top5 4
#define scint_top6 5
#define scint_top7 6
#define scint_top8 7

#define scint_bot1 8
#define scint_bot2 9
#define scint_bot3 10
#define scint_bot4 11
#define scint_bot5 12
#define scint_bot6 13
#define scint_bot7 14
#define scint_bot8 15

#include <vector>
#include <algorithm>
#include"base/Global.h"
#include "Tree.h"
#include <TH1F.h>
#include <iostream>
#include "base/Vector3D.h"
#include "TGeoVolume.h"
//#include "Singleton.h"

#ifdef USE_EVE
#include "Eve/EveVisualizer.h"
typedef EveVisualizer TrackingVisualizer;
//typedef Singleton::instance()->GetEveVisualizer() TrackingVisualizer;

#else
#include "TGeo/Visualizer.h"
typedef Visualizer TrackingVisualizer;
#endif

class TGeoBBox;


namespace Tracking{

class Scintillator{
private:
  //Dimension of Scintillator, not required for the time being
  //May be required in future
  double fLength;
  double fBreadth;
  double fHeight;

  int fModuleId; //represents the id of Module at which it is connected
  int fScintId; //represents it id of channel on a Module
  static int fId;//Static variable to increase whenever a new object is created.
  bool fScintHit;
  TGeoVolume *fScintTGeoVolume;
  static int sStripNum;
  int fStripNum;
  Channel *ch;// Data structure to hold data for the scintillator
  std::string fBName; // branch name in ROOT data Tree
  TH1F *h; //Histogram for the scillator

  //DATA tree //should be kept some where else
  Tree *t;




public:
  Scintillator();//:fLength(0),fBreadth(0),fHeight(0), fScintHit(false) {}
  Scintillator(int moduleId);
  Scintillator(int moduleId,bool forRpc);
  Scintillator(double length, double breadth, double height=1);// :
    //fLength(length), fBreadth(breadth), fHeight(height), fScintHit(false) {}
  ~Scintillator();


  TRACKING_INLINE
  static void SetStartingId(int sId){fId = sId;}

  TRACKING_INLINE
  static void SetStartingStripNum(int sId){sStripNum = sId;}

  TRACKING_INLINE
  void SetModuleId(int moduleId){fModuleId = moduleId;}
  TRACKING_INLINE
  int GetModuleId(){return fModuleId;}
  TRACKING_INLINE
  void SetChannelId(int channelId){fScintId = channelId;}
  TRACKING_INLINE
  int GetChannelId(){return fScintId;}
  TRACKING_INLINE
  bool GetScintHit(){return fScintHit;}
  TRACKING_INLINE
  std::string GetName(){return fBName;} //will return the associated branch Name

  Channel* GetEntry(int evNo); //Get Scintillator data for a particular event
  TH1F* GetHistogram(); //Function to generate Histogram for Scintillator

  void DetectAndSetHit();
  void DetectAndSetHit(int evNo);

  template<bool ForRpc>
  void DetectAndSetHit(Tree&t, int evNo);

  Vector3D<Precision> GetDimensions(){
    return Vector3D<Precision>(fLength,fBreadth,fHeight);
  }

  Vector3D<Precision> GetStripPlacement(){

  }

  void CreateScintillatorTGeoVolume();
  void Draw();
  TGeoVolume* GetScintillatorTGeoVolume(){return fScintTGeoVolume;}
  TGeoBBox* GetScintShape();

  double GetLength(){return fLength;}
  double GetBreadth(){return fBreadth;}
  double GetHeight(){return fHeight;}

};//end of Scintillator class


class ScintillatorPlane{
private:
  int fScintTotal;
  int fNumOfScintillators;
  std::vector<Scintillator*> fScintillatorPlane;
  std::string fPlaneName;

  //Plane dimensions
  int fLength;
  int fBreadth;
  int fHeight;

  TGeoVolume *fPlaneTGeoVolume;
  TrackingVisualizer fEve;

public:
  ScintillatorPlane();//: fScintTotal(0), fNumOfScintillators(8){}
  ScintillatorPlane(int numOfScintillators, std::string planeName="Test-ScintillatorPlane");
  ScintillatorPlane(int moduleId, int numOfScintillators, std::string planeName="Test-ScintillatorPlane");
  ScintillatorPlane(int moduleId, int numOfScintillators, double zPos, std::string planeName="Test-ScintillatorPlane");
  ScintillatorPlane(int moduleId, int numOfScintillators, double zPos, bool forRpc, std::string planeName="Test-ScintillatorPlane");

  ~ScintillatorPlane(){}

  void CreatePlaneOfScintillators();
  void CreatePlaneOfScintillators(int moduleId);


  void CreatePlaneOfScintillators(int moduleId,double zPos);
  void CreatePlaneOfScintillators(int moduleId,double zPos,bool forRpc);
  void ReadScintMapFileAndCreatePlane();

  TRACKING_INLINE
  int GetLength(){return fLength;}
  TRACKING_INLINE
  int GetBreadth(){return fBreadth;}
  TRACKING_INLINE
  int GetHeight(){return fHeight;}


  TRACKING_INLINE
  void SetNumOfScintillators(int numOfScintillators){fNumOfScintillators = numOfScintillators;}

  TRACKING_INLINE
  int GetNumOfScintillators(){return fNumOfScintillators;}

  TRACKING_INLINE
  void SetPlaneName(std::string planeName){fPlaneName = planeName;}

  TRACKING_INLINE
  std::string GetName(){return fPlaneName;}

  TRACKING_INLINE
  std::vector<Scintillator*> GetScintillatorPlane(){return fScintillatorPlane;}

  void Print();

  void DetectTotalScintFired();

  void InitializeScintillatorPlane();

  void CreateHistogram();
  void CreateHistogram2D();
  void Create2DHistOfTimingAndChannels();

  bool IsShowerEvent(int evNo);

  template<bool ForRpc>
  bool IsShowerEvent(Tree &t, int evNo);

  // This function returns that how many scintillators were fired in an event.
  int GetShowerCount(){return fScintTotal;}

  void GetHitMap();

  void PrintStrips(){
    std::cout<<"-----------------------------------------------------"<<std::endl;
         for(int i = 0 ; i < fScintillatorPlane.size() ; i++){
           std::cout<<" Name : "<< fScintillatorPlane[i]->GetName() <<std::endl;
         }
  }

  void CreatePlaneTGeoVolume();
  void CreatePlaneTGeoVolume(double dZ);
  void Draw();
  TGeoVolume* GetPlaneTGeoVolume(){return fPlaneTGeoVolume;}

  #ifdef USE_EVE
  void CreateEvePlane();
  void CreateEvePlane(double dZ);
  //void CreateEvePlane(double dZ,bool forRpc){}
  void Show(){fEve.Show();}
  //void Show(){Singleton::instance()->GetEveVisualizer()->ShowEve();}
  #endif


};//end of ScintillatorPlane class

template<bool ForRpc>
void Scintillator::DetectAndSetHit(Tree &t, int evNo){
/*
*  For the time being hard coding the information related to
*  trigger module and channel.
*  triggerModule : 0 , triggerChannel : 31
*
*  ModuleVector variable "modVector" should be filled after reading the ROOT file
*
*/
  fScintHit = false;
  //ModuleVector modVector; // This should come prefilled from somewhere else. Here included just
                          // to make compiler happy, should be remove later on
  //Tree t("6133.root","BSC_DATA_TREE");
  Channel *trigMultiHit = t.GetEntry("Module2_LE_CH31",evNo);
  long trig = 0;
  trig = trigMultiHit->at(0);
  ch = 0;
  ch = t.GetEntry(fBName,evNo);
  if(ch->size()){
  long scintillator = 0;
  scintillator = ch->at(0);
  //std::cout<<"Name : "<<fBName << "  :: Value : "<<scintillator<< "  :: Trig Value : "<< trig << std::endl;

  if(  scintillator > 0){
    if(ForRpc){
      //std::cout<<"BranchName : "<<fBName<<std::endl;
      long rpcData = scintillator;
      //std::cout<< "Rpc Data : "<<rpcData<<std::endl;
      if( rpcData >= 19450 && rpcData <= 20550
          //&& (fScintId < 64)
          ) fScintHit = true;
      //if( rpcData >= 19550 && rpcData <= 19875) fScintHit = true;
    }else{
      if(abs(trig - scintillator) < scintMax) fScintHit=true;
    }
  }
  }

  //std::cout<<"fScintHit : "<< fScintHit <<std::endl;
}

template<bool ForRpc>
bool ScintillatorPlane::IsShowerEvent(Tree &t, int evNo){
  fScintTotal = 0;

  //std::cout<<"ScintTotal Before : "<<fScintTotal<<std::endl;
  int scintPlaneSize = fScintillatorPlane.size();
  for(int i = 0 ; i < scintPlaneSize ; i++){
    fScintillatorPlane[i]->DetectAndSetHit<ForRpc>(t,evNo);  
  }
  DetectTotalScintFired();  
  if(ForRpc){
    //For one event fScintTotal for detector can never be > 1 (in principle)
    return (fScintTotal > 0);  
  }else{

  //std::cout<<"ScintTotal : "<<fScintTotal<<std::endl;
  return (fScintTotal >= scint_multiplicity_max);  
}
}    

}//end of Tracking namespace


#endif /* INC_SCINTILLATOR_H_ */

