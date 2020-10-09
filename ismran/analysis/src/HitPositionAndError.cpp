/*
 * HitPositionAndError.cpp
 *
 *  Created on: 09-Oct-2020
 *      Author: rsehgal
 */

#include "HitPositionAndError.h"

HitPositionAndError::HitPositionAndError() {
	// TODO Auto-generated constructor stub

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
