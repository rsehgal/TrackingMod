/*
 * TestXYCenterForEachScintillator.cpp
 *
 *  Created on: 24-Mar-2021
 *      Author: rsehgal
 */

#include "HardwareNomenclature.h"

int main(){
	GenerateScintMatrixXYCenters();
	for(unsigned int i = 0 ; i < vecOfScintXYCenter.size() ; i++){
			if(!(i%9))
				std::cout <<"============ Layer : "<< (i/9) << "  =====================" << std::endl;
			vecOfScintXYCenter[i].Print();
	}

	return 0;
}
