/*
 * ReadOneEvent.cpp
 *
 *  Created on: Aug 12, 2018
 *      Author: raman
 */

#include "ReadOneEvent.h"
#include "DetectorMapping.h"
#include "Files.h"
#include <sstream>

namespace Tomography{

using Vec_t = Tracking::Vector3D<double>;

ReadOneEvent::ReadOneEvent() {
	// TODO Auto-generated constructor stub

}

ReadOneEvent::~ReadOneEvent() {
	// TODO Auto-generated destructor stub
}

ReadOneEvent::ReadOneEvent(std::string filename){
	//Read(filename);
	Tomography::DetectorMapping *detectorMap =
			Tomography::DetectorMapping::create("testMapping.txt");
	fFileName = filename;
	//std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector("GLASS");
	fNumOfDetectors = detectorMap->GetNumOfDetectors("GLASS");

}

void ReadOneEvent::Fill(){
	fActualHitPointVector.clear();
	fFittedHitPointVector.clear();
	Vec_t actHitPt, fittedHitPt;
	for (int i = 0; i < fNumOfDetectors; i++) {
		Files::instance()->FillHit(fFileName,actHitPt,fittedHitPt);
		fActualHitPointVector.push_back(actHitPt);
		fFittedHitPointVector.push_back(fittedHitPt);

	}

}

ReadOneEvent::ReadOneEvent(std::string filename, int numOfDetectors){
	fFileName = filename;
	fNumOfDetectors = numOfDetectors;
}

/*
template<bool exact>
void ReadOneEvent::Read(){
	fHitPointVector.clear();
	for(int i = 0 ; i < fNumOfDetectors ; i++){
		Files *filePtr = Files::instance();
		fHitPointVector.push_back(filePtr->ReadHit<exact>(fFileName));

	}

}
*/

}
