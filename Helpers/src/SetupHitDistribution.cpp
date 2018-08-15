/*
 * SetupHitDistribution.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: raman
 */

#include "SetupHitDistribution.h"
#include <fstream>

namespace Tomography {

using Vec_t = Tracking::Vector3D<double>;

SetupHitDistribution::SetupHitDistribution() {
	// TODO Auto-generated constructor stub

}

SetupHitDistribution::~SetupHitDistribution() {
	// TODO Auto-generated destructor stub
}

SetupHitDistribution::SetupHitDistribution(std::vector<std::string> detectorNameVector){
	for(int i = 0 ; i  < detectorNameVector.size() ; i++){
		fDetectorNameVector.push_back(detectorNameVector[i]);
		fHitDistributionVector.push_back(new HitDistribution(fDetectorNameVector[i]));
	}

}

SetupHitDistribution::SetupHitDistribution(std::string detectorName){

}

void SetupHitDistribution::Write(){
	for(int i = 0 ; i  < fDetectorNameVector.size() ; i++){
		fHitDistributionVector[i]->Write(fDetectorNameVector[i]);
	}
}

void SetupHitDistribution::Delete(){
	for(int i = 0 ; i < fHitDistributionVector.size() ; i++){
		delete fHitDistributionVector[i];
	}
}

void SetupHitDistribution::FillHitPointVector(std::vector<Vec_t> hitPtVector){
	/* Here we are considering that all the detectors fired
	 * so the index of hitPtVector corresponds to ID of hitted
	 * detector, but actually its not, So we should have some
	 * information about the ID of hitted detector
	 */
	std::ofstream outfile;
	outfile.open("filledPixels.txt",std::ios::app);
	for(int i = 0 ; i < hitPtVector.size() ; i++){
		fHitDistributionVector[i]->Fill(hitPtVector[i].x(),hitPtVector[i].y());
#define DEB
#ifdef DEB
		double xval = hitPtVector[i].x();
		double yval = hitPtVector[i].y();
		double stripWidth = 31.25;
		int numOfStrips = 32;
		int xStripNum = (xval + 500)/stripWidth;
		int yStripNum = (yval + 500)/stripWidth;
		int pixelIndex = numOfStrips*xStripNum+yStripNum ;
		outfile << i << " : " << pixelIndex << std::endl;
#endif
	}
	outfile << "==============================" << std::endl;
	outfile.close();
}

} /* namespace Tomography */
