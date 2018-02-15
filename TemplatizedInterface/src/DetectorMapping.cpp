#include "DetectorMapping.h"
#include <fstream>

namespace Tomography{

DetectorMapping *DetectorMapping::s_instance = 0;
DetectorMapping* DetectorMapping::instance() {
        if (!s_instance){
          std::cout << "DetectorMapping Object does not exist, so creating a new one with default filename 'mapping.txt' .... " << std::endl;
          s_instance = new DetectorMapping;
        }
        return s_instance;
    }


DetectorMapping* DetectorMapping::create(std::string filename) {
/*
        if (!s_instance){
          std::cout << "DetectorMapping Object does not exist, so creating a new one with default filename 'mapping.txt' .... " << std::endl;
          s_instance = new DetectorMapping;
        }
*/
	s_instance = new DetectorMapping(filename);
        return s_instance;
    }


//Daq code will use this, coz the name of mapping file there
//will be mapping.txt
DetectorMapping::DetectorMapping(){
	fileName = "mapping.txt";
	ReadMapping();

}

//This will be used by TestProgram or Other program, if the
//name of mapping file is different
DetectorMapping::DetectorMapping(std::string filename){
	fileName = filename;
	ReadMapping();
}

void DetectorMapping::ReadMapping(){
	//std::cout<<"FileName from ReadMapping : " << fileName << std::endl;
	std::ifstream in(fileName);
	int count = 0 ;
	int detCounter = 0;
	while(1){

		if(!in.good()) break;
		std::string detectorName = "";
		int module = 0;
		int channelStart = 31;
		double zCoordinate = 0.;
		if(count == 0){
			count++;
			std::string lmodule, lchannelstart, lzcoordinate;
			in >> detectorName >> lmodule >> lchannelstart >> lzcoordinate ;
			continue;
		}else{
			detCounter++;
			in >> detectorName >> module >> channelStart >> zCoordinate ;
			fDetectorNameVector.push_back(detectorName);
			fModuleVector.push_back(module);
			fStartingChannelVector.push_back(channelStart);
			fZCoordinateVector.push_back(zCoordinate);
		}
	}
	std::cout << "Stack Length from ReadMapping : " << detCounter << std::endl;
	fNumOfDetectors = fDetectorNameVector.size();
}

}

