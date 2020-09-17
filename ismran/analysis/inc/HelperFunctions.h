/*
 * HelperFunctions.h
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_
#define ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_

#include <base/Vector3D.h>

class Point3D;

extern Tracking::Vector3D<double> ConvertToTomoVector3D(Point3D pt);

class HelperFunctions {
public:
	HelperFunctions();
	virtual ~HelperFunctions();
};

#endif /* ISMRAN_ANALYSIS_INC_HELPERFUNCTIONS_H_ */
