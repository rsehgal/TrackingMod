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
#include "Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"

namespace Tomography{


class Update{

public:
	Update(){}
	void *handle(void *ptr) {

	          TGeoHMatrix m;
	          LinesAngle l;
	          Double_t trans[3] = {0., 0., 0.};
	          m.SetTranslation(trans);
	          m.SetDx(rand()%50);
	          m.SetDy(rand()%50);
	          m.SetDz(75);

	          Coordinates c;
	          Tracking::Vector3D<double> temp;
	          std::vector<HittedPixel*> hittedPixelVector;
	          while(true){
	            sleep(2);

	            //c.CoGenerator(Tomography::SetupManager::instance()->GetDetectorVector("GLASS"));
	            c.CoGenerator(0);
	            c.SetStrips();
	            c.SetStripCoordinates();

	            Vector3D<double>temp2 = c.GetSpecificCoordinate(10.);
	            Coordinates c2(temp2);
	            c2.CoGenerator(1);
	            c2.SetStrips();
	            c2.SetStripCoordinates();

	            //count++;
	            TGeoBBox *shape ;
	            std::cout<<"Size : "<< hittedPixelVector.size() << std::endl;
	            if(hittedPixelVector.size()){
	              for(int i = 0 ; i < hittedPixelVector.size() ; i++){
	                Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
	              }
	            }

	            hittedPixelVector.clear();
	            for(int detNo = 0 ; detNo < Tomography::SetupManager::instance()->GetLowerLayerStartIndex() ; detNo++){
	              temp = c.GetStripCoordinate(detNo+1);
	              temp.Print();
	              m.SetDx(temp.x());
	              m.SetDy(temp.y());
	              m.SetDz(temp.z());

	            //m.SetDx(*temp);
				//m.SetDy(*(temp + 1));
				//m.SetDz(*(temp + 2));

	            //Add some element
	            if(gEve){
	              hittedPixelVector.push_back(new HittedPixel(m));
	              Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
	             }
	            }

	            for(int detNo = Tomography::SetupManager::instance()->GetLowerLayerStartIndex() ; detNo < Tomography::SetupManager::instance()->GetDetectorVector("GLASS").size() ; detNo++){
	            //for(int detNo = 0 ; detNo < Tomography::SetupManager::instance()->GetLowerLayerStartIndex() ; detNo++){
	            	              temp = c2.GetStripCoordinate(detNo-Tomography::SetupManager::instance()->GetLowerLayerStartIndex()+1);
	            	              temp.Print();
	            	              m.SetDx(temp.x());
	            	              m.SetDy(temp.y());
	            	              m.SetDz(temp.z());

	            	            //m.SetDx(*temp);
	            				//m.SetDy(*(temp + 1));
	            				//m.SetDz(*(temp + 2));

	            	            //Add some element
	            	            if(gEve){
	            	              hittedPixelVector.push_back(new HittedPixel(m));
	            	              Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
	            	             }
	            	            }

	           std::cout<<"Angle : "<< l.CalculateAngle(c.GetCoordinate(1), c.GetCoordinate(2), c2.GetCoordinate(1), c2.GetCoordinate(2))
	        		   << std::endl;


	        }
	        }

	        void RunThread() {
	          TThread *mythread = new TThread("My Thread", (void (*)(void *)) & Update::handle, (void *)this);
	          mythread->Run();
	        }


};

}

#endif

