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

namespace Tomography{


class Update{

public:
	Update(){}
	void *handle(void *ptr) {

	          TGeoHMatrix m;
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

	            c.CoGenerator(Tomography::SetupManager::instance()->GetDetectorVector("GLASS"));
	            c.SetStrips();
	            c.SetStripCoordinates();
	            //count++;
	            TGeoBBox *shape ;
	            std::cout<<"Size : "<< hittedPixelVector.size() << std::endl;
	            if(hittedPixelVector.size()){
	              for(int i = 0 ; i < hittedPixelVector.size() ; i++){
	                Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
	              }
	            }

	            hittedPixelVector.clear();
	            for(int detNo = 0 ; detNo < Tomography::SetupManager::instance()->GetDetectorVector("GLASS").size() ; detNo++){
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
	        }
	        }

	        void RunThread() {
	          TThread *mythread = new TThread("My Thread", (void (*)(void *)) & Update::handle, (void *)this);
	          mythread->Run();
	        }


};

}

#endif

