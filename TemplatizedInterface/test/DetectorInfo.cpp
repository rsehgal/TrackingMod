/*
 * DetectorInfo.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: rsehgal
 */


#include "GlassRpc.h"
#include "Properties.h"
#include <iostream>
typedef Tomography::Properties Detector;
using namespace Tomography;
int main(){
	Detector *rpc = new GlassRpc(3,"SecondGlassRpc", 120,-1);
	for(int i=0 ; i < rpc->GetNumOfPlanes() ;i++){
		for (int j=0 ; j < rpc->GetPlane(i)->GetNumOfScintillators() ; j++){
			std::cout<< "ChannelName : " << rpc->GetPlane(i)->GetScintVector()[j]->GetName() << std::endl;
		}
	}

	return 0;
}

