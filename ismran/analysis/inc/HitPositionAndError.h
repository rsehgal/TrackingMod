/*
 * HitPositionAndError.h
 *
 *  Created on: 09-Oct-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_SIMULATIONS_INC_HITPOSITIONANDERROR_H_
#define ISMRAN_SIMULATIONS_INC_HITPOSITIONANDERROR_H_

#include "PsBar.h"

class HitPositionAndError {
public:
	Point3D fHitPosition;
	Point3D fHitPositionError;
public:
	HitPositionAndError();
	HitPositionAndError(Point3D hitPosition);
	HitPositionAndError(Point3D hitPosition, Point3D hitPositionError);
	HitPositionAndError(double X, double Y, double Z, double errX, double errY, double errZ);
	HitPositionAndError(double X, double Y, double Z);
	void SetHitPositionAndError(Point3D hitPosition, Point3D hitPositionError);
	void SetHitPositionAndError(double X, double Y, double Z, double errX, double errY, double errZ);
	void SetHitPosition(Point3D hitPosition);
	void SetHitPosition(double X, double Y, double Z);

	virtual ~HitPositionAndError();
};

#endif /* ISMRAN_SIMULATIONS_INC_HITPOSITIONANDERROR_H_ */
