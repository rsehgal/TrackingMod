/*
 * Scintillator.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: rsehgal
 */
#include "Scintillator.h"
#include <iostream>
#include <sstream>
#include <TCanvas.h>
#include <TH2F.h>
#include <TApplication.h>
#include <TFile.h>
#include <TGeoBBox.h>
#include "TGeoMatrix.h"
#ifdef USE_EVE
 #include "TEveManager.h"
 #include "Eve/EveVisualizer.h"
#else
 #include "TGeo/Visualizer.h"
#endif
///#include "Tree.h"

namespace Tracking{

/*************************************************
 ******* Definition of Scintillator class ********
 *************************************************/

int Scintillator::fId=-1;
int Scintillator::sStripNum=-1;

Scintillator::Scintillator():fLength(0),fBreadth(0),fHeight(0), fScintHit(false) ,fModuleId(0) {
  fId++;
  fScintId = fId;
  std::stringstream ss;
  ss << "Module" << fModuleId+2 <<"_LE_CH" << fScintId;
  fBName = ss.str();
  //t = new Tree("6133.root","BSC_DATA_TREE");

  //Commenting Histogram for the time being
  //h = new TH1F("h",fBName.c_str(),100,20000,21000);

}
//Setting default value corresponding the dimension of strip of RPC

Scintillator::Scintillator(int moduleId):fLength(1.5),fBreadth(50.),fHeight(0.5), fScintHit(false) ,fModuleId(moduleId) {
  fId++;
  fScintId = fId;
  sStripNum++;
  fStripNum = sStripNum;
  std::stringstream ss;
  ss << "Module" << fModuleId <<"_LE_CH" << fScintId;
  fBName = ss.str();
  //t = new Tree("6133.root","BSC_DATA_TREE");

  //Commenting Histogram for the time being
  //h = new TH1F("h",fBName.c_str(),100,20000,21000);
  #ifndef USE_EVE
    CreateScintillatorTGeoVolume();
  #endif

}

Scintillator::Scintillator(int moduleId,bool forRpc):fLength(3.),fBreadth(100.),fHeight(1.0), fScintHit(false) ,fModuleId(moduleId) {
  if(!forRpc){ // Scintillator plane
    fLength = 18.;
    fBreadth = 180.;
  }

  fId++;
  fScintId = fId;

  //Just a test, manually setting some hit strips
  //if(fScintId==35 || fScintId==38 || fScintId==42)
  //  fScintHit = true;


  sStripNum++;
  fStripNum = sStripNum;
  std::stringstream ss;
  ss << "Module" << fModuleId <<"_LE_CH" << fScintId;
  fBName = ss.str();
  //t = new Tree("6133.root","BSC_DATA_TREE");

  //Commenting Histogram for the time being
  //h = new TH1F("h",fBName.c_str(),100,20000,21000);
  #ifndef USE_EVE
    CreateScintillatorTGeoVolume();
  #endif

}

Scintillator::Scintillator(double length, double breadth, double height) :
    fLength(length), fBreadth(breadth), fHeight(height), fScintHit(false), fModuleId(0) {
  fId++;
  fScintId = fId;

  std::stringstream ss;
  ss << "Module" << fModuleId <<"_LE_CH" << fScintId;
  fBName = ss.str();
}

Scintillator::~Scintillator(){//delete t;

}

void Scintillator::DetectAndSetHit(){
/*
*  For the time being hard coding the information related to
*  trigger module and channel.
*  triggerModule : 0 , triggerChannel : 31
*
*  ModuleVector variable "modVector" should be filled after reading the ROOT file
*
*/
  ModuleVector modVector; // This should come prefilled from somewhere else. Here included just
                          // to make compiler happy, should be remove later on
  Channel *trigMultiHit = modVector[0][31];
  long trig = trigMultiHit->at(0);
  Channel *scintMultiHit = modVector[fModuleId][fScintId];
  long scintillator = scintMultiHit->at(0);
  if(  scintillator > 0){
    if(abs(trig - scintillator) < scintMax) fScintHit=true;
  }
}

Channel* Scintillator::GetEntry(int evNo){

  Tree t("6133.root","BSC_DATA_TREE");
  return t.GetEntry(fBName,evNo);
}

TH1F* Scintillator::GetHistogram(){
  Tree t("6133.root","BSC_DATA_TREE");
  int numOfEvents = t.GetNumOfEvents();
  std::cout<<"NumOf Events : "<<numOfEvents<<std::endl;
  for(int evNo= 0 ; evNo < numOfEvents ; evNo++){
    //Channel *ch = t.GetEntry(fBName,evNo);
    ch = t.GetEntry(fBName,evNo);
    if(ch->size()){
      for(int i = 0 ; i < ch->size() ; i++){
        h->Fill(ch->at(i));
      }
    }
  }

  return h;
}




void Scintillator::DetectAndSetHit(int evNo){
/*
*  For the time being hard coding the information related to
*  trigger module and channel.
*  triggerModule : 0 , triggerChannel : 31
*
*  ModuleVector variable "modVector" should be filled after reading the ROOT file
*
*/
  fScintHit = false;
  ModuleVector modVector; // This should come prefilled from somewhere else. Here included just
                          // to make compiler happy, should be remove later on
  Tree t("6133.root","BSC_DATA_TREE");
  Channel *trigMultiHit = t.GetEntry("Module2_LE_CH31",evNo);
  long trig = trigMultiHit->at(0);
  ch = t.GetEntry(fBName,evNo);
  if(ch->size()){
  long scintillator = ch->at(0);
  if(  scintillator > 0){
    if(abs(trig - scintillator) < scintMax) fScintHit=true;
  }
  }

  //std::cout<<"fScintHit : "<< fScintHit <<std::endl;
}
#ifndef USE_EVE
void Scintillator::CreateScintillatorTGeoVolume(){
  TrackingVisualizer v;
  fLength=100;
  fBreadth=3;
  fHeight=1;
  fScintTGeoVolume = v.CreateTGeoVolume(new TGeoBBox(fBName.c_str(),fLength/2., fBreadth/2., fHeight/2.));
  fScintTGeoVolume->SetVisibility(kTRUE); 
  fScintTGeoVolume->SetLineColor(kGreen);
}
#else
TGeoBBox* Scintillator::GetScintShape(){
  //return new TGeoBBox(fBName.c_str(),fLength/2., fBreadth/2., fHeight/2.);
  return new TGeoBBox(fBName.c_str(),fLength/2., fBreadth/2., fHeight/2.);
}
#endif

// template<bool ForRpc>
// void Scintillator::DetectAndSetHit(Tree &t, int evNo){
// /*
// *  For the time being hard coding the information related to
// *  trigger module and channel.
// *  triggerModule : 0 , triggerChannel : 31
// *
// *  ModuleVector variable "modVector" should be filled after reading the ROOT file
// *
// */
//   fScintHit = false;
//   ModuleVector modVector; // This should come prefilled from somewhere else. Here included just
//                           // to make compiler happy, should be remove later on
//   //Tree t("6133.root","BSC_DATA_TREE");
//   Channel *trigMultiHit = t.GetEntry("Module2_LE_CH31",evNo);
//   long trig = trigMultiHit->at(0);
//   ch = t.GetEntry(fBName,evNo);
//   if(ch->size()){
//   long scintillator = ch->at(0);
//   if(  scintillator > 0){
//     if(ForRpc){
//       long rpcData = scintillator;
//       if( rpcData >= 19550 && rpcData <= 20550) fScintHit = true;
//     }else{
//       if(abs(trig - scintillator) < scintMax) fScintHit=true;
//     }
//   }
//   }

//   //std::cout<<"fScintHit : "<< fScintHit <<std::endl;
// }


/*************************************************
 ***** Definition of ScintillatorPlane class *****
 *************************************************/

void ScintillatorPlane::GetHitMap(){
  bool verbose=true;
  int scintPlaneSize = fScintillatorPlane.size();
  for(int i = 0 ; i < scintPlaneSize ; i++){
    if(verbose){
      if(!(i%32)) std::cout<<"    ";
      if(fScintillatorPlane[i]->GetScintHit())
        std::cout<<"1 ";
      else
        std::cout<<"0 ";
    }
  }
  std::cout<<std::endl;
}

bool ScintillatorPlane::IsShowerEvent(int evNo){
  fScintTotal = 0;

  //std::cout<<"ScintTotal Before : "<<fScintTotal<<std::endl;
  int scintPlaneSize = fScintillatorPlane.size();
  for(int i = 0 ; i < scintPlaneSize ; i++){
    fScintillatorPlane[i]->DetectAndSetHit(evNo);
  }
  DetectTotalScintFired();
  //std::cout<<"ScintTotal : "<<fScintTotal<<std::endl;
  return (fScintTotal >= scint_multiplicity_max);
}

/*template<bool ForRpc>
bool ScintillatorPlane::IsShowerEvent(Tree &t, int evNo){
  fScintTotal = 0;

  //std::cout<<"ScintTotal Before : "<<fScintTotal<<std::endl;
  int scintPlaneSize = fScintillatorPlane.size();
  for(int i = 0 ; i < scintPlaneSize ; i++){
    fScintillatorPlane[i]->DetectAndSetHit<ForRpc>(t,evNo);
  }
  DetectTotalScintFired();
  //std::cout<<"ScintTotal : "<<fScintTotal<<std::endl;
  return (fScintTotal >= scint_multiplicity_max);
}*/

void ScintillatorPlane::CreateHistogram(){
  int scintPlaneSize = fScintillatorPlane.size();
  TCanvas *c1 = new TCanvas("c1",fPlaneName.c_str(),200,10,700,500);
  c1->Divide(4,2);
  for(int i = 0 ; i < scintPlaneSize ; i++){
    c1->cd(i+1);
    fScintillatorPlane[i]->GetHistogram()->Draw();
  }
}

ScintillatorPlane::ScintillatorPlane():
    fNumOfScintillators(8),
    fScintTotal(0),
    fPlaneName("Test-ScintillatorPlane"){
  InitializeScintillatorPlane();
  //CreatePlaneOfScintillators();
}

void ScintillatorPlane::CreateHistogram2D(){

  int nxbins = 1000;
  int xlow = 20000;
  int xhigh = 21000;
  int nybins = 150;
  int ylow = -10;
  int yhigh = 140;
  Channel *trig = 0;
  Channel *ch = 0;

  TCanvas *c2 = new TCanvas("c2",fPlaneName.c_str(),200,10,700,500);
  c2->Divide(1,1);
  c2->cd(1);
  Tree t("6133.root","BSC_DATA_TREE");
  int numOfEvents = t.GetNumOfEvents();

  //TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d","Timing",nxbins,xlow,xhigh,nybins,ylow,yhigh);
  for(int evNo= 0 ; evNo < numOfEvents ; evNo++){
    trig  = t.GetEntry("Module2_LE_CH31",evNo);
    h2d->Fill(trig->at(0),31);
    for (int i = 0; i < fScintillatorPlane.size(); i++) {
      ch = t.GetEntry(fScintillatorPlane[i]->GetName(), evNo);
      if (ch->size()) {
        for (int j = 0; j < ch->size(); j++) {
          h2d->Fill(ch->at(j), fScintillatorPlane[i]->GetChannelId());
        }
      }
    }
  }
  h2d->Draw();
  h2d->Print();
  //TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
  TFile::Open("hTrig.root","RECREATE");
  h2d->Write();
  c2->Modified();
  c2->Update();
}

void ScintillatorPlane::Create2DHistOfTimingAndChannels(){
 // Tree t("6133.root","BSC_DATA_TREE");
  int numOfEvents = 10; //t.GetNumOfEvents();
  int nxbins = 1000;
  int xlow = 20000;
  int xhigh = 21000;
  int nybins = 150;
  int ylow = -10;
  int yhigh= 140;
  Channel *trig = 0;
  Channel *ch = 0;

  //TApplication *fApp = new TApplication("Timing", NULL, NULL);
  //TH2F *h2d = new TH2F("h2d","Timing",nxbins,xlow,xhigh,nybins,ylow,yhigh);
  TCanvas *c1 = new TCanvas("c1",fPlaneName.c_str(),200,10,700,500);
  TH1F *h = new TH1F("h","ABCD",100,20000,21000);
  for(int evNo= 0 ; evNo < numOfEvents ; evNo++){
    //trig  = t.GetEntry("Module2_LE_CH31",evNo);
    //std::cout<<"Trig : "<< trig->at(0) <<std::endl;
    //if(trig->size())
    //h2d->Fill(trig->at(0),31);
/*
    for(int i = 0 ; i < fScintillatorPlane.size() ; i++) {
      Channel *ch = t.GetEntry(fScintillatorPlane[i]->GetName(),evNo);
      if(ch->size()){
      for(int j = 0 ; j < ch->size() ; j++){
        h2d->Fill(ch->at(j),fScintillatorPlane[i]->GetChannelId());
      }
     }
    }
*/

  }
  h->Draw();
  //h2d->Draw();
 // h2d->Write("timing.root");
 // fApp->Run();
}

ScintillatorPlane::ScintillatorPlane(int numOfScintillators,std::string planeName) :
    fNumOfScintillators(numOfScintillators),
    fScintTotal(0),
    fPlaneName(planeName) {
    InitializeScintillatorPlane();
    CreatePlaneOfScintillators();
}

ScintillatorPlane::ScintillatorPlane(int moduleId, int numOfScintillators, std::string planeName):
    fNumOfScintillators(numOfScintillators),
    fScintTotal(0),
    fPlaneName(planeName){

  InitializeScintillatorPlane();
  //CreatePlaneOfScintillators(moduleId);

}

ScintillatorPlane::ScintillatorPlane(int moduleId, int numOfScintillators,double zPos, std::string planeName):
    fNumOfScintillators(numOfScintillators),
    fScintTotal(0),
    fPlaneName(planeName),
    fLength(100.),
    fBreadth(100.){

  InitializeScintillatorPlane();
  CreatePlaneOfScintillators(moduleId,zPos);

}


ScintillatorPlane::ScintillatorPlane(int moduleId, int numOfScintillators,double zPos, bool forRpc, std::string planeName):
    fNumOfScintillators(numOfScintillators),
    fScintTotal(0),
    fPlaneName(planeName),
    fLength(100.),
    fBreadth(100.){

  InitializeScintillatorPlane();
  CreatePlaneOfScintillators(moduleId,zPos,forRpc);

}

void ScintillatorPlane::CreatePlaneOfScintillators(){
  for(int i = 0 ; i< fNumOfScintillators ; i++){
    fScintillatorPlane.push_back(new Scintillator());
  }
}

void ScintillatorPlane::CreatePlaneOfScintillators(int moduleId){
  Scintillator::SetStartingStripNum(-1);
  for(int i = 0 ; i< fNumOfScintillators ; i++){
    fScintillatorPlane.push_back(new Scintillator(moduleId));
  }
}

  void ScintillatorPlane::CreatePlaneOfScintillators(int moduleId,double zPos){
    Scintillator::SetStartingStripNum(-1);
    fLength=144.;
    fBreadth = 180.;
    for(int i = 0 ; i< fNumOfScintillators ; i++){
      fScintillatorPlane.push_back(new Scintillator(moduleId));
    }

  #ifndef USE_EVE
  //CreatePlaneTGeoVolume();
  CreatePlaneTGeoVolume(zPos);
  #else
  CreateEvePlane(zPos);
  #endif
}


  void ScintillatorPlane::CreatePlaneOfScintillators(int moduleId,double zPos, bool forRpc){
    Scintillator::SetStartingStripNum(-1);
    if(forRpc){
      fLength = 100.;
      fBreadth = 100.;
    }
    else{
      fLength=144.;
      fBreadth = 180.;
    }

    for(int i = 0 ; i< fNumOfScintillators ; i++){
      //fScintillatorPlane.push_back(new Scintillator(moduleId));
      fScintillatorPlane.push_back(new Scintillator(moduleId,forRpc));
    }

  #ifndef USE_EVE
  //CreatePlaneTGeoVolume();
  CreatePlaneTGeoVolume(zPos);
  #else
  //CreateEvePlane(zPos,forRpc);
  CreateEvePlane(zPos);
  #endif
}



void ScintillatorPlane::Print(){
    std::cout<<"Plane-Name : "<<fPlaneName<<std::endl
             <<"Num Of Scintillators : "<<fNumOfScintillators<<std::endl;
}

//This function is basically used to detect shower event
void ScintillatorPlane::DetectTotalScintFired(){

  for(int i=0; i < fScintillatorPlane.size(); i++){
    if(fScintillatorPlane[i]->GetScintHit())
      fScintTotal++;
  }


}

void ScintillatorPlane::InitializeScintillatorPlane(){
  fScintTotal = 0;
}

#ifdef USE_EVE
void ScintillatorPlane::CreateEvePlane(){
// TEveManager::Create();
 TGeoBBox *box = fScintillatorPlane[0]->GetScintShape();
 TGeoHMatrix m;
 Double_t trans[3] = { 0., 0., 0. };
 m.SetTranslation(trans);

 for(int i=0; i < fScintillatorPlane.size(); i++){
   std::cout<<" X trans : "<< (-fLength/2.+i*1.6) << std::endl;
   m.SetDx(-fLength/2.+i*1.6);
  fEve.AddEveShape(fScintillatorPlane[i]->GetName(), box, m );
  // Singleton::instance()->GetEveVisualizer()->AddEveShape(fScintillatorPlane[i]->GetName(), box, m );
 }
}
/*  Commenting for the time being
void ScintillatorPlane::CreateEvePlane(double dZ){
 //TEveManager::Create();
 //TGeoBBox *box = fScintillatorPlane[0]->GetScintShape();
 TGeoHMatrix m;
 Double_t trans[3] = { 0., 0., 0. };
 m.SetTranslation(trans);

 for(int i=0; i < fScintillatorPlane.size(); i++){

   //std::cout<<"fLength : "<< fLength << " :  X trans : "<< (-fLength/2.+1.6+i*1.6) << std::endl;
   //m.SetDx(-fLength/2.+i*1.6);
   m.SetDx(-fLength/2.+1.6+i*1.6);
   //m.SetDx(20.);
   m.SetDz(dZ);
   int channelId = fScintillatorPlane[i]->GetChannelId();
   if(channelId > 40 && channelId < 53)
     fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[0]->GetScintShape(),2, m );
     //Singleton::instance()->GetEveVisualizer()->AddEveShape(fScintillatorPlane[i]->GetName(), box,2, m );
   else
     fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[0]->GetScintShape(),3, m );
     //Singleton::instance()->GetEveVisualizer()->AddEveShape(fScintillatorPlane[i]->GetName(), box,3, m );
  //fEve.AddEveShape(fScintillatorPlane[i]->GetName(), box, m );
 }
 std::cout<<"=================================================================================" << std::endl;
}
*/
//void ScintillatorPlane::CreateEvePlane(double dZ, bool forRpc){
void ScintillatorPlane::CreateEvePlane(double dZ){
 //TEveManager::Create();
 //TGeoBBox *box = fScintillatorPlane[0]->GetScintShape();
 TGeoHMatrix m;
 Double_t trans[3] = { 0., 0., 0. };
 m.SetTranslation(trans);

 for(int i=0; i < fScintillatorPlane.size(); i++){
     m.SetDx(-fLength/2.+i*fScintillatorPlane[i]->GetLength());
     m.SetDz(dZ);
     int channelId = fScintillatorPlane[i]->GetChannelId();
     fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[i]->GetScintShape(),3, m );
/*
     if(fScintillatorPlane[i]->GetScintHit())
       fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[i]->GetScintShape(),2, m );
     else
       fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[i]->GetScintShape(),3, m );
*/

     //fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[0]->GetScintShape(),3, m );
/*
   if(channelId > 40 && channelId < 53)
     fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[0]->GetScintShape(),2, m );
     //Singleton::instance()->GetEveVisualizer()->AddEveShape(fScintillatorPlane[i]->GetName(), box,2, m );
   else
     fEve.AddEveShape(fScintillatorPlane[i]->GetName(), fScintillatorPlane[0]->GetScintShape(),3, m );
*/
     //Singleton::instance()->GetEveVisualizer()->AddEveShape(fScintillatorPlane[i]->GetName(), box,3, m );
  //fEve.AddEveShape(fScintillatorPlane[i]->GetName(), box, m );
 }
 std::cout<<"=================================================================================" << std::endl;
}
#else
void ScintillatorPlane::CreatePlaneTGeoVolume(){
  TrackingVisualizer v;
  fLength=100;
  fBreadth=100;
  fHeight=1;
  fPlaneTGeoVolume = v.CreateTGeoVolume(new TGeoBBox(fPlaneName.c_str(),fLength/2., fBreadth/2., fHeight/2.));
  fPlaneTGeoVolume->SetVisibility(kTRUE);
  fPlaneTGeoVolume->SetVisDaughters(kTRUE);
  fPlaneTGeoVolume->SetTransparency(90);  
  for(int i=0; i < fScintillatorPlane.size(); i++){
    fPlaneTGeoVolume->AddNode(fScintillatorPlane[i]->GetScintillatorTGeoVolume(),i+1,(new TGeoTranslation( 0,-fLength/2. + 3.2*i + 3.2, 0.)));
    //std::cout<<"Value : " << (-fLength/2. + 3*i + 3) << std::endl;
  }
}

void ScintillatorPlane::CreatePlaneTGeoVolume(double dZ){
  TrackingVisualizer v;
  fLength=100;
  fBreadth=100;
  fHeight=1;
  fPlaneTGeoVolume = v.CreateTGeoVolume(new TGeoBBox(fPlaneName.c_str(),fLength/2., fBreadth/2., fHeight/2.));
  fPlaneTGeoVolume->SetVisibility(kTRUE);
  fPlaneTGeoVolume->SetVisDaughters(kTRUE);
  fPlaneTGeoVolume->SetTransparency(90);
  for(int i=0; i < fScintillatorPlane.size(); i++){
    fPlaneTGeoVolume->AddNode(fScintillatorPlane[i]->GetScintillatorTGeoVolume(),i+1,(new TGeoTranslation( 0,-fLength/2. + i*fScintillatorPlane[i]->GetBreadth(), dZ)));
    //fPlaneTGeoVolume->AddNode(fScintillatorPlane[i]->GetScintillatorTGeoVolume(),i+1,(new TGeoTranslation( 0,-fLength/2. + 3*i + 3, dZ)));
    //std::cout<<"Value : " << (-fLength/2. + 3*i + 3) << std::endl;
  }
}
#endif
void ScintillatorPlane::Draw(){

}

}//end of Tracking namespace


