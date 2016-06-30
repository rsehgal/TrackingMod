/*
 * Scintillator.h
 *
 *  Created on: June 02, 2016
 *      Author: rsehgal
 */

#ifndef INC_UPDATE_H_
#define INC_UPDATE_H_

#include "Coordinates.h"
#include <TGeoMatrix.h>
#include "base/Global.h"
#include "HittedPixel.h"
#include "TGeoBBox.h"
#include "visualizer/Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
 #include "visualizer/Eve/Singleton.h"
 typedef Tomography::Properties Detector;

namespace Tomography {

class Update {

  TEveStraightLineSet *ls;
  int fDelay;

public:
  Update() {fDelay = 2;}
  void SetDelay(int delay){fDelay = delay;}

  void *handleRoot(void *ptr) {
    TGeoHMatrix m;
    LinesAngle l;
    Double_t trans[3] = {0., 0., 0.};
    m.SetTranslation(trans);
    //Coordinates c(Tomography::SetupManager::instance()->GetDetectorVector("GLASS"));

    int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
    Tracking::Vector3D<double> temp;
    Tracking::Vector3D<double> temp1;
    std::vector<HittedPixel *> hittedPixelVector;

    //std::vector<Detector *> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
    SetupManager *setup = Tomography::SetupManager::instance();
    std::vector<Detector *> detectors = setup->GetDetectorVector("GLASS");
    std::vector<Detector *> trgPlaneVect = setup->GetDetectorVector("TRG");
    int evCount = 0;
#ifdef ANG_DIST
    TCanvas *canvas = new TCanvas("AngDist", "AngularDistribution", 800, 600);
    canvas->Divide(1,1);
    TH1F  *angHist = new TH1F("AD", "Angular Distribution", 50., 0, 90);
#endif
    std::vector<Tracking::Vector3D<double>> tempVect;
    std::vector<Tracking::Vector3D<double>> poiVect;
    int countValid = 0;
    Coordinates c;

    for (int evNo = 0; evNo < numOfEvents; evNo++) {
    	tempVect.clear();
    	poiVect.clear();
    	setup->SetEventDetected("TRG",evNo);
    	if(setup->EventDetected()){
      std::cout << "======================================================" << std::endl;

      setup->SetEventDetected("GLASS",evNo);
#ifdef EFF_SETUP_AND
      if(setup->EventDetected())
#endif
      {
    	  evCount++;
    	  std::cout<<"Genuine event no w.r.t Full Setup : " << evCount << std::endl;
#ifdef ACCUMULATE_TRACK
#else
          if (ls)
            Tracking::Singleton::instance()->RemoveElement();
#endif
      for (int j = 0; j < detectors.size(); j++) {
        //detectors[j]->SetFiredStripsVector(evNo);
    	temp1=temp;
        for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) {
          for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) {

//            temp = GetStripCoordinate(detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
//                                      detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval], detectors[j]->GetZPos());

        	  temp = c.GetStripCoordinate(detectors[j],detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
        	                                        detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval], detectors[j]->GetZPos());

            //temp.Print();

            if (gEve) {
              m.SetDx(temp.x());
              m.SetDy(temp.y());
              m.SetDz(temp.z());
              //hittedPixelVector.push_back(new HittedPixel(m));
              hittedPixelVector.push_back(new HittedPixel(m,true,false));

//              Tracking::Singleton::instance()->AddElement(
//                  hittedPixelVector[hittedPixelVector.size() - 1]->GetEveGeoShape());
            }
          }
        }
        tempVect.push_back(temp);
      }


      //Code block for Track Validation
      //Coordinates c;
      c.SetPoints(tempVect);
      std::cout<<"TopPlaneIntersection : ";c.GetPOI(trgPlaneVect[0],false).Print();
      std::cout<<"BottomPlaneIntersection : ";c.GetPOI(trgPlaneVect[1],true).Print();
      poiVect.push_back(c.GetPOI(trgPlaneVect[0],false));
      poiVect.push_back(c.GetPOI(trgPlaneVect[1],true));
      int realNo = 10000, calcNo = 10000;
      bool valid = Validate(poiVect,realNo,calcNo);
      std::cout << "Validity : " << valid << std::endl;
      if (valid){
    	  std::cout<<"Found Valid Track for Event No : " << evNo << std::endl;
        countValid++;
      }


      // Displaying all illuminated pixel
      for (int i = 0; i < hittedPixelVector.size(); i++)
        Tracking::Singleton::instance()->AddElement(hittedPixelVector[i]->GetEveGeoShape());

      ls = new TEveStraightLineSet();
      ls->SetLineColor(5);
      Vector3D<double> dir = (temp1-temp)/(temp1-temp).Mag();
      Vector3D<double>temp11 = temp1+dir*80;
      Vector3D<double>temp22 = temp-dir*80;
      AddLine(temp11, temp22);
      Tracking::Singleton::instance()->AddElement(ls);

      std::cout << "Angle : "
                << l.CalculateAngle(temp1, temp)
                << std::endl;
#ifdef ANG_DIST
      angHist->Fill(l.CalculateAngle(temp1, temp));
#endif

      //sleep(fDelay);
      bool skipDelay = true;
      for (int j = 0; j < detectors.size(); j++) {
        skipDelay &= detectors[j]->GetPlane(0)->GetFiredStripsVector().size()==0 &&
                     detectors[j]->GetPlane(1)->GetFiredStripsVector().size()==0;
        //sleep(fDelay);
      }

      if(!skipDelay)
        sleep(fDelay);
      std::cout << "Size : " << hittedPixelVector.size() << std::endl;
      //if(ls)
    	//  Tracking::Singleton::instance()->RemoveElement();
    }



      if (hittedPixelVector.size()) {
        for (int i = 0; i < hittedPixelVector.size(); i++) {
          Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());

        }
      }
      hittedPixelVector.clear();
    }
    }//end of event loop

    std::cout<<"Total Number of Valid Tracks : " << countValid << std::endl;
    //angHist->SaveAs("AngDist.gif");
#ifdef ANG_DIST
    canvas->cd(1);
    angHist->Draw();
    canvas->SaveAs("AngularDistribution.gif");
#endif

  }


  bool Validate(std::vector<Tracking::Vector3D<double>> tempVect,int& realNo, int& calcNo){
  	bool valid=true;
      SetupManager *setup = SetupManager::instance();
      std::vector<Detector *> trgPlaneVect = setup->GetDetectorVector("TRG");
      for(int i = 0 ; i < trgPlaneVect.size() ; i++){
       int scintNo = 10000;
       int calcScintNo = 10000;
       //std::cout<<"Name : "<< trgPlaneVect[i]->GetName() << std::endl;
       if(trgPlaneVect[i]->GetPlane(0)->GetFiredStripsVector().size())
       scintNo = trgPlaneVect[i]->GetPlane(0)->GetFiredStripsVector()[0];
       realNo = scintNo;
       //std::cout<<"Breadth : " << trgPlaneVect[i]->GetLength() << std::endl;
       calcScintNo = floor((trgPlaneVect[i]->GetLength()/2 - tempVect[i].x())/18.);
       calcNo = calcScintNo;
       valid &= (scintNo == calcScintNo);
       std::cout<<"Real Scint No : " << scintNo <<"  :: Calculated Scint Num : " << calcScintNo << std::endl;
      }
      return valid;

    }

  void *handle(void *ptr) {

    TGeoHMatrix m;
    LinesAngle l;
    Double_t trans[3] = {0., 0., 0.};
    m.SetTranslation(trans);
    m.SetDx(rand() % 50);
    m.SetDy(rand() % 50);
    m.SetDz(75);

    Coordinates c(Tomography::SetupManager::instance()->GetUpperLayerDetectorVector("GLASS"));
    Tracking::Vector3D<double> temp;
    std::vector<HittedPixel *> hittedPixelVector;
    while (true) {

      c.CoGenerator();
      c.SetStrips();
      c.SetStripCoordinates();

      Vector3D<double> temp2 = c.GetSpecificCoordinate(10.);
      Coordinates c2(Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS"),temp2);
      c2.CoGenerator();
      c2.SetStrips();
      c2.SetStripCoordinates();

      TGeoBBox *shape;
      std::cout << "Size : " << hittedPixelVector.size() << std::endl;
      if (hittedPixelVector.size()) {
        for (int i = 0; i < hittedPixelVector.size(); i++) {
          Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
        }
      }

      hittedPixelVector.clear();
      // For top half detectors
      for (int detNo = 0; detNo < Tomography::SetupManager::instance()->GetLowerLayerStartIndex(); detNo++) {
        temp = c.GetStripCoordinate(detNo + 1);
        temp.Print();
        m.SetDx(temp.x());
        m.SetDy(temp.y());
        m.SetDz(temp.z());
        // Add some element
        if (gEve) {
          hittedPixelVector.push_back(new HittedPixel(m));
          Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
        }
      }

      // For bottom half detectors
      for (int detNo = Tomography::SetupManager::instance()->GetLowerLayerStartIndex();
           detNo < Tomography::SetupManager::instance()->GetDetectorVector("GLASS").size(); detNo++) {
        temp = c2.GetStripCoordinate(detNo - Tomography::SetupManager::instance()->GetLowerLayerStartIndex() + 1);
        temp.Print();
        m.SetDx(temp.x());
        m.SetDy(temp.y());
        m.SetDz(temp.z());

        // Add some element
        if (gEve) {
          hittedPixelVector.push_back(new HittedPixel(m));
          Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
        }
      }

      std::cout << "Pt : ";
      c.GetCoordinate(1).Print();

      ls = new TEveStraightLineSet();
      ls->SetLineColor(5);
      AddLine(c);
      AddLine(c2);

      Tracking::Singleton::instance()->AddElement(ls);

      std::cout << "Angle : "
                << l.CalculateAngle(c.GetCoordinate(1), c.GetCoordinate(2), c2.GetCoordinate(1), c2.GetCoordinate(2))
                << std::endl;
      sleep(2);
      Tracking::Singleton::instance()->RemoveElement();
    }
  }


Tracking::Vector3D<double> GetStripCoordinate(double x, double y, double z)
    {
        int tmp = 0;
        Vector3D<double> temp;
        temp.SetX(-50. + x*3.125 + 1.5625);
        temp.SetY(-50. + y*3.125 + 1.5625);
        temp.SetZ(z);

        return temp;
    }

void GenerateCoordinates(std::vector<int>xVec,std::vector<int>yVec){
	for(int i=0 ; i<xVec.size() ; i++){
		for(int j=0 ; j<yVec.size() ; j++){
			Tracking::Vector3D<double> coordinates = GetStripCoordinate(xVec[i],yVec[j],50);
			coordinates.Print();
		}
	}
}


void AddLine(Coordinates c) {
    int startDetIndex = 1;
    int lastDetIndex = c.GetLength();
    //std::cout<<"LAST INDEX : "<< lastDetIndex << std::endl;
    ls->AddLine(c.GetCoordinate(startDetIndex).x(), c.GetCoordinate(startDetIndex).y(),
                c.GetCoordinate(startDetIndex).z(), c.GetCoordinate(lastDetIndex).x(),
                c.GetCoordinate(lastDetIndex).y(), c.GetCoordinate(lastDetIndex).z());
    AddMarkers(c);
  }

void AddLine(Vector3D<Precision>p1, Vector3D<Precision>p2) {
    int startDetIndex = 1;
    //int lastDetIndex = c.GetLength();
    //std::cout<<"LAST INDEX : "<< lastDetIndex << std::endl;
    std::cout<<"P1 : " ; p1.Print();
    std::cout<<"P2 : " ; p2.Print();
    AddMarkers(p1);
    AddMarkers(p2);
    ls->AddLine(p1.x(), p1.y(), p1.z(),
    		    p2.x(), p2.y(), p2.z());
    //AddMarkers(c);
  }

  void AddMarkers(Coordinates c) {
    for (int i = 0; i < c.GetLength(); i++) {
      ls->AddMarker(c.GetCoordinate(i + 1).x(), c.GetCoordinate(i + 1).y(), c.GetCoordinate(i + 1).z());
    }

    ls->SetMarkerSize(1.3);
    ls->SetMarkerStyle(4);
  }

  void AddMarkers(Vector3D<Precision> p) {
      ls->AddMarker(p.x(), p.y(), p.z());
      ls->SetMarkerSize(1.3);
      ls->SetMarkerStyle(4);
    }

  void RunThread() {
    TThread *mythread = new TThread("My Thread", (void (*)(void *)) & Update::handle, (void *)this);
    mythread->Run();
  }

  void RunThread2() {
      TThread *mythread2 = new TThread("My Thread2", (void (*)(void *)) & Update::handleRoot, (void *)this);
      mythread2->Run();
    }
};
}

#endif
