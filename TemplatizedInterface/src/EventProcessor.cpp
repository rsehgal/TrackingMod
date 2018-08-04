/*
 * EventProcessor.cpp
 *
 *  Created on: Aug 24, 2017
 *      Author: rsehgal
 */

#include "EventProcessor.h"
#include "Fit2DLinear.h"

namespace Tomography {

EventProcessor::EventProcessor() {
	// TODO Auto-generated constructor stub
	fGenuineEvent = false;

}

EventProcessor::~EventProcessor() {
	// TODO Auto-generated destructor stub
}

void EventProcessor::ProcessEvent(int evNo){

	fGenuineEvent = false;
    SetupManager *setup = Tomography::SetupManager::instance();
    std::vector<Detector *> detectors = setup->GetDetectorVector("GLASS");
    std::vector<Tracking::Vector3D<double>> tempVect;
    Tracking::Vector3D<double> temp(0.,0.,0.);
    Coordinates c;
    tempVect.clear();
    fHitPointVector.clear();
    setup->SetEventDetected("GLASS",evNo);
    if(setup->EventDetected()){
        fGenuineEvent = true;
        for (int j = 0; j < detectors.size(); j++) {  // begin of detector loop
                    //temp1 = temp;
                    for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) {
                        for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) {

                        	std::cout << detectors[j]->GetPlane(0)->GetFiredStripsNameVector()[xval] << " : "
                        			  << detectors[j]->GetPlane(1)->GetFiredStripsNameVector()[yval] << " : "
									  << detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval] << " : "
									  << detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval] << std::endl;

                            temp = c.GetStripCoordinate(detectors[j],
                                            detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
                                            detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval],
                                            detectors[j]->GetZPos());

                        }
                    }
                    /*
                     temp.Transform(detectors[j]->GetDx(), detectors[j]->GetDy(), detectors[j]->GetDTheta() );
                     std::cout<<"X Strip : " << detectors[j]->GetPlane(0)->GetFiredStripsVector()[0] << std::endl;
                     std::cout<<"Y Strip : " << detectors[j]->GetPlane(1)->GetFiredStripsVector()[0] << std::endl;
                     */
                    tempVect.push_back(temp);
                    fHitPointVector.push_back(temp);
            } // end of detector loop


    int numOfDetectors = detectors.size();
    if(numOfDetectors == 3){
    	fIncoming.Set(tempVect[0],tempVect[1]);
    	fOutgoing.Set(tempVect[1],tempVect[2]);
    }else{
            fIncoming.Set(tempVect[0],tempVect[numOfDetectors/2 - 1]);
            fOutgoing.Set(tempVect[numOfDetectors/2],tempVect[numOfDetectors-1]);
    }
    }

/*
    Tomography::Fit2DLinear f;
    f.CalculateResiduals(tempVect);
    f.PrintResiduals();
*/

}

} /* namespace Tomography */
