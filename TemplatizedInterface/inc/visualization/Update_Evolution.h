/*
 * Update_Evolution.h
 *
 *  Created on: Aug 23, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_VISUALIZATION_UPDATE_EVOLUTION_H_
#define TEMPLATIZEDINTERFACE_INC_VISUALIZATION_UPDATE_EVOLUTION_H_

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
#include "Track.h"
#include <fstream>
#include "CommonFunc.h"
#include "EventProcessor.h"


typedef Tomography::Properties Detector;
//using Tomography::VisualizationHelper;

namespace Tomography {
namespace evolution{

class Update {

	int fDelay;
	bool firstTrack;
public:
	Update(){fDelay = 2; firstTrack=true;}
	void SetDelay(int delay){fDelay = delay;}
	int GetDelay(){return fDelay;}
#if(0)
	void *handleRoot(void *ptr) {
		std::cout<<"Handle ROOT Called..." << std::endl;

		TGeoHMatrix m;
		LinesAngle l;
		Double_t trans[3] = { 0., 0., 0. };
		m.SetTranslation(trans);

		int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
		std::cout<<"Num Of Events : " << numOfEvents << std::endl;
		Tracking::Vector3D<double> temp;
		Tracking::Vector3D<double> temp1;
		std::vector<HittedPixel *> hittedPixelVector;

		//std::vector<Detector *> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
		SetupManager *setup = Tomography::SetupManager::instance();
		std::vector<Detector *> detectors = setup->GetDetectorVector("GLASS");
		//std::vector<Detector *> trgPlaneVect = setup->GetDetectorVector("TRG");
		//std::vector<Detector *> paddleVect = setup->GetDetectorVector("PADDLE");


		std::vector<Tracking::Vector3D<double>> tempVect;
		std::vector<Tracking::Vector3D<double>> poiVect;
		int countValid = 0;
		Coordinates c;
		int evCount = 0;
		//numOfEvents = 153;
		//TH1F *hist = new TH1F("Scattering","Scattering",1000,0,M_PI/2.);
		std::ofstream fs("MuonAngles.txt");
		fs << 3000 << " " << "G4_Fe" << " " << 100 << " ";
		for (int evNo = 0; evNo < numOfEvents; evNo++) {
			tempVect.clear();
			setup->SetEventDetected("GLASS",evNo);
			if(setup->EventDetected()){
			evCount++;
			std::cout<<"Genuine event no w.r.t Full Setup : " << evCount << std::endl;


			for (int j = 0; j < detectors.size(); j++) {  // begin of detector loop
					temp1 = temp;
					for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size();	xval++) {
						for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) {

							temp = c.GetStripCoordinate(detectors[j],
											detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
											detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval],
											detectors[j]->GetZPos());

							//temp.Print();

							/*if (gEve) {
								m.SetDx(temp.x());
								m.SetDy(temp.y());
								m.SetDz(temp.z());
								//hittedPixelVector.push_back(new HittedPixel(m));
								hittedPixelVector.push_back(
										new HittedPixel(m, true, false));
							}*/
						}
					}
					/*
					 temp.Transform(detectors[j]->GetDx(), detectors[j]->GetDy(), detectors[j]->GetDTheta() );
					 std::cout<<"X Strip : " << detectors[j]->GetPlane(0)->GetFiredStripsVector()[0] << std::endl;
					 std::cout<<"Y Strip : " << detectors[j]->GetPlane(1)->GetFiredStripsVector()[0] << std::endl;
					 */
					tempVect.push_back(temp);
			} // end of detector loop

			//Tomography::Track t(tempVect[0],tempVect[tempVect.size()-1]);
			Tomography::Track incoming(tempVect[0],tempVect[1]);
			//Tomography::Track outgoing(tempVect[2],tempVect[3]);
			//Tomography::VisualizationHelper::instance()->RegisterLine(tempVect[0],tempVect[tempVect.size()-1]);

			Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
			double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
			fs << angleIncoming << " ";



#ifdef ACCUMULATE_TRACK
#else
          if (!firstTrack){
        	  Tomography::VisualizationHelper::instance()->RemoveTrack();
          	 // Tomography::VisualizationHelper::instance()->RemoveTrack();
          }
          else
        	  firstTrack = false;
#endif
            std::vector<Tomography::Track*> trackVector;
            trackVector.push_back(&incoming);
          //  trackVector.push_back(&outgoing);
			Tomography::VisualizationHelper::instance()->Register(trackVector);
			//Tomography::VisualizationHelper::instance()->Register(&outgoing);







			}
			sleep(fDelay);
		}// end of event loop
		fs.close();
	}
#endif
	void *handleRoot_V2(void *ptr) {
			TGeoHMatrix m;
			LinesAngle l;
			Double_t trans[3] = { 0., 0., 0. };
			m.SetTranslation(trans);

			int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
			std::cout<<"Num Of Events : " << numOfEvents << std::endl;
			//std::vector<HittedPixel *> hittedPixelVector;

			//std::vector<Tracking::Vector3D<double>> poiVect;
			Tomography::EventProcessor eventProcessor;
			std::ofstream fs("MuonAngles.txt");
			fs << 3000 << " " << "G4_Fe" << " " << 100 << " ";
			for (int evNo = 0; evNo < numOfEvents; evNo++) {
				eventProcessor.ProcessEvent(evNo);
				Tomography::Track incoming = eventProcessor.GetIncomingTrack();
				Tomography::Track outgoing = eventProcessor.GetOutgoingTrack();
				Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
				double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
				fs << angleIncoming << " ";



	#ifdef ACCUMULATE_TRACK
	#else
	          if (!firstTrack){
	        	  Tomography::VisualizationHelper::instance()->RemoveTrack();
	          	 // Tomography::VisualizationHelper::instance()->RemoveTrack();
	          }
	          else
	        	  firstTrack = false;
	#endif
	            std::vector<Tomography::Track*> trackVector;
	            trackVector.push_back(&incoming);
	            trackVector.push_back(&outgoing);
				Tomography::VisualizationHelper::instance()->Register(trackVector);
				//Tomography::VisualizationHelper::instance()->Register(&outgoing);

				sleep(fDelay);
			}// end of event loop
			fs.close();
		}


	void RunThread2() {
	      TThread *mythread2 = new TThread("My Thread2", (void (*)(void *)) & Update::handleRoot_V2, (void *)this);
	      mythread2->Run();
	}




}; //end of Update class

} // end of evolution namespace
} // end of Tomography namesapce




#endif /* TEMPLATIZEDINTERFACE_INC_VISUALIZATION_UPDATE_EVOLUTION_H_ */
