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
	Point3D fHitPosition;
	Point3D fHitPositionError;
public:
	HitPositionAndError();
	HitPositionAndError(Point3D hitPosition, Point3D hitPositionError);
	void SetHitPositionAndError(Point3D hitPosition, Point3D hitPositionError);
	virtual ~HitPositionAndError();
};

#endif /* ISMRAN_SIMULATIONS_INC_HITPOSITIONANDERROR_H_ */
