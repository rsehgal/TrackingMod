/*
 * Point3D.cpp
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#include "Point3D.h"
#include <iostream>
#include <cmath>
#include "PsBar.h"
#ifndef FOR_SIMULATION
ClassImp(lite_interface::Point3D);
#endif
namespace lite_interface {

Point3D* Point3D::InCm(){
return (new Point3D(fX/10.,fY/10.,fZ/10.));
}

Point3D::Point3D() {
	// TODO Auto-generated constructor stub
	fX = 10000 ; fY = 10000.; fZ=10000.;

}

Point3D::Point3D(const Point3D &pt){
	fX = pt.fX;
	fY = pt.fY;
	fZ = pt.fZ;

}

Point3D::~Point3D() {
	// TODO Auto-generated destructor stub
}

void Point3D::SetXYZ(double x, double y, double z){
	fX = x;
	fY = y;
	fZ = z;
}

void Point3D::Divide(int n){
	fX /= n;
	fY /= n;
	fZ /= n;
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

unsigned int Point3D::GetLayer(){
	int tempY = (int)(fY+(numOfLayers*barHeight/2));
	return tempY/(int)barHeight;
}
/*
Point3D Point3D::operator += (Point3D obj){
	this->fX += obj.fX;
	this->fY += obj.fY;
	this->fZ += obj.fZ;
}
*/

/*Point3D* Point3D::operator += (Point3D *obj){
	this->fX += obj->fX;
	this->fY += obj->fY;
	this->fZ += obj->fZ;
}*/

/*Point3D Point3D::operator /= (int n){
	this->fX /= n;
	this->fY /= n;
	this->fZ /= n;
}*/
} /* namespace lite_interface */
