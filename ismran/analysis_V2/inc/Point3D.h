/*
 * Point3D.h
 *
 *  Created on: 10-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_POINT3D_H_
#define ISMRAN_ANALYSIS_V2_INC_POINT3D_H_

#pragma once
#include <TROOT.h>

namespace lite_interface {

class Point3D {
	double fX;
	double fY;
	double fZ;

public:
	double x,y,z;

public:
	Point3D();
	Point3D(const Point3D &pt);
	double GetX()const{return fX;}
	double GetY()const{return fY;}
	double GetZ()const{return fZ;}
	Point3D(double x, double y, double z);
	void SetZero();
	void SetXYZ(double x, double y, double z);
	void Divide(int n);
	double Distance(Point3D p2);
	void Print();
	virtual ~Point3D();
	unsigned int GetLayer();
	Point3D* InCm();

	//Point3D operator += (Point3D obj);
	//Point3D* operator += (Point3D *obj);
	//Point3D operator /= (int n);
#ifndef FOR_SIMULATION
	ClassDef(Point3D,1)
#endif
};

} /* namespace lite_interface */

#endif /* ISMRAN_ANALYSIS_V2_INC_POINT3D_H_ */
