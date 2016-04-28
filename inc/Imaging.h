/*
 * Imaging.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef INC_IMAGING_H_
#define INC_IMAGING_H_

#include "base/Vector3D.h"
#include "base/Global.h"

#define Vec_t Vector3D<Precision>

namespace Tracking{

class ImageReconstruction{

private:

public:
	ImageReconstruction();
	~ImageReconstruction();

	//Functions implementing Image reconstruction from projections
	Vec_t POCA( Vec_t p,  Vec_t u,  Vec_t q,  Vec_t v, Vec_t &p1, Vec_t &q1);
	void EM();

	//TODO :  other sophisticated image reconstruction algorithm,
	//        making use of RADON Transform

}; //end of Imaging class

}//end of Tracking namespace


#endif /* INC_IMAGING_H_ */
