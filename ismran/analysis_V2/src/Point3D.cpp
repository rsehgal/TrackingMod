/*
 * Point3D.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "Point3D.h"
#include <iostream>
#include <cmath>

ClassImp(lite_interface::Point3D);

namespace lite_interface {

Point3D::Point3D() {
	// TODO Auto-generated constructor stub
	fX = 100 ; fY = 100.; fZ=100.;

}

Point3D::~Point3D() {
	// TODO Auto-generated destructor stub
}


Point3D::Point3D(double x, double y, double z) : fX(x), fY(y), fZ(z){
}

void Point3D::Print(){
	std::cout << "( " << fX << " , " << fY <<" , "<< fZ <<" )" << std::endl;
}

double Point3D::Distance(Point3D p2){
	return sqrt((p2.fX-this->fX)*(p2.fX-this->fX) + (p2.fY-this->fY)*(p2.fY-this->fY) + (p2.fZ-this->fZ)*(p2.fZ-this->fZ));
}

void Point3D::SetZero(){
		fX = 0 ;
		fY = 0 ;
		fZ = 0;
}

} /* namespace lite_interface */
