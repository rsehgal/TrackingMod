/*
 * Properties.cpp
 *
 *  Created on: May 13, 2016
 *      Author: rsehgal
 */

#include "Properties.h"
#include <TCanvas.h>
#include <TH2F.h>
namespace Tomography{

//int Properties::fClusterSize = 10;

Properties::Properties(){
  fDim = fChannelsInDim.size();
  fTotalNumOfChannels = 0;
  for(int i=0 ; i<fDim ; i++){
    fTotalNumOfChannels += fChannelsInDim[i];
  }
}

Properties::~Properties(){}

Properties::Properties(std::string name,std::vector<int> channelsInDim){
  fName = name;
  fChannelsInDim = channelsInDim;
  fDim = fChannelsInDim.size();
  fNumOfPlanes = fDim;
  fTotalNumOfChannels = 0;
  for (int i = 0; i < fDim; i++) {
    fTotalNumOfChannels += fChannelsInDim[i];
  }
}

/*void Properties::SetEventDetected(int evNo) {
  SetFiredStripsVector(evNo);
  fEventDetected = false;

for(int i = 0 ; i < fNumOfPlanes ; i++) {
#ifdef EFF_AND
#ifdef CLUSTER_SIZE
  if(i==0)
  fEventDetected = (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= GetPlane(i)->GetClusterSize());
  else
  fEventDetected &= (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= GetPlane(i)->GetClusterSize());
#else
  if(i==0)
  fEventDetected = GetPlane(i)->GetFiredStripsVector().size();
  else
  fEventDetected &= GetPlane(i)->GetFiredStripsVector().size();
#endif
#else
#ifdef CLUSTER_SIZE
  if(i==0)
  fEventDetected = (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= GetPlane(i)->GetClusterSize());
  else
  fEventDetected |=  (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= GetPlane(i)->GetClusterSize());
#else
  if(i==0)
  fEventDetected = GetPlane(i)->GetFiredStripsVector().size() 
  else
  fEventDetected |= GetPlane(i)->GetFiredStripsVector().size();
#endif
#endif
}
}*/

void Properties::SetEventDetected(int evNo) {
  SetFiredStripsVector(evNo);
  fEventDetected = false;

for(int i = 0 ; i < fNumOfPlanes ; i++) {
#ifdef EFF_AND
#ifdef CLUSTER_SIZE
  if(i==0)
  fEventDetected = (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= fClusterSize); // GetPlane(i)->GetClusterSize());
  else
  fEventDetected &= (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= fClusterSize); //GetPlane(i)->GetClusterSize());
#else
  if(i==0)
  fEventDetected = GetPlane(i)->GetFiredStripsVector().size();
  else
  fEventDetected &= GetPlane(i)->GetFiredStripsVector().size();
#endif
#else
#ifdef CLUSTER_SIZE
  if(i==0)
  fEventDetected = (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= fClusterSize); // GetPlane(i)->GetClusterSize());
  else
  fEventDetected |=  (GetPlane(i)->GetFiredStripsVector().size() > 0. && GetPlane(i)->GetFiredStripsVector().size() <= fClusterSize); //GetPlane(i)->GetClusterSize());
#else
  if(i==0)
  fEventDetected = GetPlane(i)->GetFiredStripsVector().size()
  else
  fEventDetected |= GetPlane(i)->GetFiredStripsVector().size();
#endif
#endif
}
}

  void Properties::SetEfficiency()
   {
     int count = 0;
     int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
     for(int i = 0 ; i <  numOfEvents ; i++)
     {
         SetEventDetected(i);
         if(fEventDetected)
           count++;
        /*SetFiredStripsVector(i);
        #ifdef EFF_AND
        if(GetPlane(0)->GetFiredStripsVector().size() && GetPlane(1)->GetFiredStripsVector().size())
         count++;
        #else
        if(GetPlane(0)->GetFiredStripsVector().size() || GetPlane(1)->GetFiredStripsVector().size())
         count++;
        #endif*/
     }
           //double tmp = ((double)(numOfEvents-count))*100.;
           fEfficiency = count/(double)numOfEvents*100;
   }

Tracking::Vector3D<double> Properties::GetStripCoordinate(int x, int y, int z) {
  Tracking::Vector3D<double> temp;
  double stripLength = GetPlane(0)->GetScintVector()[0]->GetLength()/GetPlane(0)->GetNumOfScintillators();
  double stripBreadth = GetPlane(1)->GetScintVector()[0]->GetBreadth()/GetPlane(0)->GetNumOfScintillators();
  //std::cout<<"StripLenght : " << stripLength << " :  StripBredth : " << stripBreadth << std::endl;
  temp.SetX(-GetLength()/2. + (31-x) * stripLength + stripLength/2.);
  temp.SetY(-GetBreadth()/2. + y * stripBreadth + stripBreadth/2.);
  temp.SetZ(z);

  return temp;
}


void Properties::GetHitPlot(){
  
  TCanvas *cHitPlot = new TCanvas(GetName().c_str(), GetName().c_str(), 800, 600);
  TH2F *h2dHitPlot = new TH2F("h2dHitPlot", "HitPlot", 500, -fLength, fLength, 500, -fBreadth, fBreadth);
  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
  std::vector<int> topPlaneFiredStripVector;
  std::vector<int> bottomPlaneFiredStripVector;
  std::vector<Tracking::Vector3D<double>> pixelVect;
  int count=0;
  for(int evNo = 0 ; evNo < numOfEvents ; evNo++){
    pixelVect.clear();
    SetEventDetected(evNo);
    topPlaneFiredStripVector = GetPlane(0)->GetFiredStripsVector();
    bottomPlaneFiredStripVector = GetPlane(1)->GetFiredStripsVector();
    if(fEventDetected){
    //if(topPlaneFiredStripVector.size() && bottomPlaneFiredStripVector.size()){
      for(int xval = 0  ; xval < topPlaneFiredStripVector.size() ; xval++){
        for(int yval = 0  ; yval < bottomPlaneFiredStripVector.size() ; yval++){
          count++;
          pixelVect.push_back(GetStripCoordinate(xval,yval,GetZPos()));	  
          pixelVect[count].Print();
        }
      }
    }
    if(pixelVect.size()){
      for(int i = 0 ;  i < pixelVect.size() ; i++){
        h2dHitPlot->Fill(pixelVect[i].x(), pixelVect[i].y());
      }
    }

  }

  std::cout<<"Total Num of Hit Point for Detector  : "<< GetName() << " : " << count << std::endl;
  std::cout<<"==================================================================="<< std::endl;
  
  h2dHitPlot->Draw();
  
}

void Properties::GetX_Y_And_ClusterHistograms()
{
 // TApplication *fApp = new TApplication("Histograms", NULL, NULL);
  TCanvas *c = new TCanvas((GetName()+"Cluster").c_str(), (GetName() + " ClusterSize").c_str(), 800, 600);
  c->Divide(2,2);

  int nxbins = 32;
  int nybins = 32;
  int pbins = 1024;
  int xlow = 0;
  int xhigh = 32;
  int ylow = 0;
  int yhigh = 32;
 
  TH1F *histogram_x = new TH1F(GetPlane(0)->GetName().c_str(), "Cluster Size of X Plane", nxbins, xlow, xhigh);
  TH1F *histogram_y = new TH1F(GetPlane(1)->GetName().c_str(), "Cluster Size of Y Plane", nybins, ylow, yhigh);
  TH1F *histogram_pixel = new TH1F(("Pixels-"+GetName()).c_str(), "Cluster Size of Pixels", pbins, 0, 1024);
 
  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
 
  for (int evNo = 0; evNo < numOfEvents; evNo++) 
  {
      SetFiredStripsVector(evNo);
      histogram_x->Fill(GetPlane(0)->GetFiredStripsVector().size());
      histogram_y->Fill(GetPlane(1)->GetFiredStripsVector().size());
      histogram_pixel->Fill(GetPlane(0)->GetFiredStripsVector().size() * GetPlane(1)->GetFiredStripsVector().size());
  }
      
  c->cd(1);
  histogram_x->GetXaxis()->SetRangeUser(0,10);
  histogram_x->Draw();

  c->cd(2);
  histogram_y->GetXaxis()->SetRangeUser(0,10);
  histogram_y->Draw();


  c->cd(3);
  histogram_pixel->GetXaxis()->SetRangeUser(0,10);
  histogram_pixel->Draw();

  c->SaveAs((GetName()+"ClusterSize.gif").c_str());
//  fApp->Run();
}

void Properties::GetStripProfile()
{
  //TApplication *fApp = new TApplication("Histogram", NULL, NULL);
  TCanvas *c = new TCanvas((GetName()+"StripProfile").c_str(),  (GetName() + " Strip Profile").c_str(), 800, 600);
  int nxbins = 64;
  int xlow = 0;
  int xhigh = 64;
  
  TH1F *histogram = new TH1F(GetName().c_str(), "Strip Profile of Full Detector", nxbins, xlow, xhigh);

  int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();

  for (int evNo = 0; evNo < numOfEvents; evNo++) 
  {
     SetFiredStripsVector(evNo);
       
     for(int k = 0 ; k < GetNumOfPlanes() ; k++)
     {
        for(int l = 0 ; l < GetPlane(k)->GetNumOfScintillators() ; l++)
        {
            if(Tracking::Tree::instance()->GetEntry(GetPlane(k)->GetScintVector()[l]->GetName(),evNo)->size())
            {
              histogram->Fill(GetPlane(k)->GetScintVector()[l]->GetStripNum());
            }
        } 
      }
  }
      
    histogram->Draw();
    c->SaveAs((GetName()+"StripProfile.gif").c_str());
   // fapp->Run();
}



}//end of Tomography namespace


