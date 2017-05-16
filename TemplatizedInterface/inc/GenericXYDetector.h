/*
 * GenericXYDetector.h
 *
 *  Created on: Apr 7, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_GENERICXYDETECTOR_H_
#define TEMPLATIZEDINTERFACE_INC_GENERICXYDETECTOR_H_

#include "Properties.h"

namespace Tomography {

class GenericXYDetector: public Properties {
public:
	GenericXYDetector();
	GenericXYDetector(int moduleId,std::string detName,double zPos,int startId, int numOfStrips, double length, double breadth, double height);
	virtual ~GenericXYDetector();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GENERICXYDETECTOR_H_ */
