/*
 * HitPositionAndError.cpp
 *
 *  Created on: 09-Oct-2020
 *      Author: rsehgal
 */

#include "HitPositionAndError.h"

HitPositionAndError::HitPositionAndError() {
	// TODO Auto-generated constructor stub
	fHitPosition.SetZero();
	fHitPositionError.SetZero();

}

HitPositionAndError::HitPositionAndError(double X, double Y, double Z, double errX, double errY, double errZ){
	HitPositionAndError(Point3D(X,Y,Z), Point3D(errX, errY, errZ));
}

HitPositionAndError::HitPositionAndError(double X, double Y, double Z){
	HitPositionAndError(Point3D(X,Y,Z));
}

HitPositionAndError::HitPositionAndError(Point3D hitPosition): fHitPosition(hitPosition){
	fHitPositionError.SetZero();
}

HitPositionAndError::HitPositionAndError(Point3D hitPosition, Point3D hitPositionError): fHitPosition(hitPosition), fHitPositionError(hitPositionError){

}

HitPositionAndError::~HitPositionAndError() {
	// TODO Auto-generated destructor stub
}

void HitPositionAndError::SetHitPositionAndError(Point3D hitPosition, Point3D hitPositionError){
	fHitPosition = hitPosition;
	fHitPositionError = hitPositionError;
}

void HitPositionAndError::SetHitPositionAndError(double X, double Y, double Z, double errX, double errY, double errZ){
	SetHitPositionAndError(Point3D(X,Y,Z), Point3D(errX, errY, errZ));
}

void HitPositionAndError::SetHitPosition(Point3D hitPosition){
	fHitPosition = hitPosition;
}

void HitPositionAndError::SetHitPosition(double X, double Y, double Z){
	SetHitPosition(Point3D(X,Y,Z));
}
