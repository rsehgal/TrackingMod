/*
 * TrackFinder.cpp
 *
 *  Created on: Aug 7, 2018
 *      Author: raman
 */

#include "TrackFinder.h"
#include "Pixel.h"
#include "Fit2DLinear.h"

namespace Tomography {

using Vec_t = Tracking::Vector3D<double>;

TrackFinder::TrackFinder() {
	// TODO Auto-generated constructor stub

}

TrackFinder::TrackFinder(std::vector<Vec_t> pixelCenterVector){

	SetPixelCenterVector(pixelCenterVector);

}

void TrackFinder::SetPixelCenterVector(std::vector<Vec_t> pixelCenterVector){
	//Fill the Pixel Vector
		fPixelVector.clear();
		for(int i = 0 ; i < pixelCenterVector.size(); i++)
			fPixelVector.push_back(new Pixel(pixelCenterVector[i]));

		FindTrack_Algo1();
}

TrackFinder::~TrackFinder() {
	// TODO Auto-generated destructor stub
}

void TrackFinder::FindTrack_Algo1(){
	//Access fPixelVector to get the Track using Algo1
	std::vector<Vec_t> randomPtVector;
	for(int i = 0 ; i < fPixelVector.size() ; i++){
		randomPtVector.push_back(fPixelVector[i]->GetRandomPoint());
	}
	Tomography::Fit2DLinear fitter;
	fFittedTrackPointVector = fitter.GetFittedTrack(randomPtVector);
}

void TrackFinder::FindTrack_Algo2(){
	//Access fPixelVector to get the Track using Algo2
}

} /* namespace Tomography */
