/*
 * TestSetupHitDistribution.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: raman
 */

#include "SetupHitDistribution.h"
#include <iostream>
#include "DetectorMapping.h"
#include <vector>
#include "base/Vector3D.h"
#include "ReadOneEvent.h"
#include "Files.h"

using Vec_t = Tracking::Vector3D<double>;

int main(){



	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector("GLASS");
	int numOfRpcs = detectorMap->GetNumOfDetectors("GLASS");

	//Creating the histograms for all the pixel for all the detectors
	Tomography::SetupHitDistribution s(detNamesVector);


	/* Read one Event and fill the hits in the histogram for hitted pixel
	 * for the detector
	 */
	std::string filename = "ActualAndFittedHits.txt";
	//Opening the required file
	Tomography::Files::instance()->Open(filename, Tomography::operation::read);
	//Creating object of Event Reader
	//Tomography::ReadOneEvent r(filename);
	Tomography::ReadOneEvent r(filename,numOfRpcs);

	//Accessing 10 events, but only one at a time
	for (int i = 0; i < 10; i++) {
		std::cout << "==================================" << std::endl;
		r.Read();
		r.Print();
		std::vector<Vec_t> hitPtVector = r.GetHitPointVector();
		std::cout<<"Hit Point Vector size : " << hitPtVector.size() << std::endl;
		s.FillHitPointVector(hitPtVector);

	}

	//Closing the file
	Tomography::Files::instance()->Close(filename);
	s.Write();


	return 0;
}



