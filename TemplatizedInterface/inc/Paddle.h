/*
 * Paddle.h
 *
 *  Created on: Jul 1, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_PADDLE_H_
#define TEMPLATIZEDINTERFACE_INC_PADDLE_H_

#include "Properties.h"

namespace Tomography {

class Paddle : public Properties {
public:
  Paddle(int moduleId, std::string detName, double zPos, int startId , double length, double breadth) {
	  SetDetectorType("PADDLE");
	  Scintillator::SetStartingId(startId);
	  int numOfPlanes = 1;
	  SetNumOfPlanes(numOfPlanes);
	  SetName(detName);
	  SetZPos(zPos);
	  SetLBH(length,breadth,1);
	  Initialize();
	  InsertPlane(new ScintillatorPlane(moduleId,1,zPos,GetLength(),GetBreadth(),GetHeight(),0.,false));

	}

  Paddle();
  ~Paddle(){}
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_PADDLE_H_ */
