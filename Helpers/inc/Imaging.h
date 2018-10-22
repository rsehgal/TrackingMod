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
#include "Track.h"
//#define Vec_tVector3D<Precision>
using Vec_t = Tracking::Vector3D<Precision>;
using Tomography::Track;
namespace Tracking{

class ImageReconstruction{

private:

protected:
	double cpa_time(Track Tr1, Track Tr2);
public:
	ImageReconstruction();
	~ImageReconstruction();

	//Functions implementing Image reconstruction from projections
	Vec_t POCA( Vec_t p,  Vec_t u,  Vec_t q,  Vec_t v, Vec_t &p1, Vec_t &q1);
	Vec_t POCA_V2( Vec_t p,  Vec_t u,  Vec_t q,  Vec_t v, Vec_t &p1, Vec_t &q1);
	Vec_t POCA( Track incoming, Track outgoing);
	Vec_t POCA_V3( Track incoming, Track outgoing);
	Vec_t POCA_V4( Track incoming, Track outgoing);
	//Vec_t POCA_Iterative( Vec_t p,  Vec_t u,  Vec_t q,  Vec_t v, Vec_t &p1, Vec_t &q1);
	Vec_t POCA_Iterative(Track incoming,Track outgoing);
	void GetIntPoints(Vec_t p, Vec_t q, Vec_t &intPt1, Vec_t &intPt2 );
	Track GetNewTrack(Vec_t p, Vec_t q);
  	Precision Distance(Vec_t p, Vec_t q);

	void EM();

	//TODO :  other sophisticated image reconstruction algorithm,
	//        making use of RADON Transform

}; //end of Imaging class

}//end of Tracking namespace


#endif /* INC_IMAGING_H_ */
