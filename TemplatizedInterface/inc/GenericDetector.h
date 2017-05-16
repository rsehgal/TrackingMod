/*
 * GenericDetector.h
 *
 *  Created on: Mar 22, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_GENERICDETECTOR_H_
#define TEMPLATIZEDINTERFACE_INC_GENERICDETECTOR_H_

#include "Properties.h"

namespace Tomography {

class GenericDetector: public Properties {
public:
	GenericDetector();
	virtual ~GenericDetector();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GENERICDETECTOR_H_ */
