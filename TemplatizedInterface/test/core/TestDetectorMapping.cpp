/*
 * TestDetectorMapping.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: rsehgal
 */

#include "DetectorMapping.h"
#include <iostream>
/*
int main(){
	//Tomography::DetectorMapping *detectorMap = new Tomography::DetectorMapping("testMapping.txt");
//	std::cout << "Stack Length : " << Tomography::DetectorMapping::instance()->GetNumOfDetectors() << std::endl;

	std::cout << "User Defined file Stack Length : " << Tomography::DetectorMapping::create("testMapping.txt")->GetNumOfDetectors() << std::endl;
	std::vector<std::string> detectorNamesVector = Tomography::DetectorMapping::instance()->GetDetectorNamesVector();

	for(int i = 0 ; i < detectorNamesVector.size(); i++){
		std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
	}

	return 0;
}
*/

int main(){
       
	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
  	int numOfRpcs = detectorMap->GetNumOfDetectors("GLASS");
	std::vector<std::string> detNamesVector = detectorMap->GetDetectorNamesVector("GLASS");
	 std::cout <<"=======================================================" << std::endl;
    	std::cout << "Num Of Rpc :  "<< numOfRpcs << std::endl;
	for(auto &detName : detNamesVector){
		std::cout <<"DetectorName : " << detName << std::endl;



	}


return 0;
}


