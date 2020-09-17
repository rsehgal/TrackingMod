/*
 * HelperFunctions.cpp
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#include "HelperFunctions.h"
#include "PsBar.h"

Tracking::Vector3D<double> ConvertToTomoVector3D(Point3D pt){
	return Tracking::Vector3D<double>(pt.x,pt.y,pt.z);
}

HelperFunctions::HelperFunctions() {
	// TODO Auto-generated constructor stub

}

HelperFunctions::~HelperFunctions() {
	// TODO Auto-generated destructor stub
}

