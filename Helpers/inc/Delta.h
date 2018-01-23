/*
 * Delta.h
 *
 *  Created on: Jul 14, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_DELTA_H_
#define HELPERS_INC_DELTA_H_

#include "Track.h"
#include "base/Vector3D.h"
#include "LinesAngle.h"
#include <iostream>
#include "CommonFunc.h"

using Tomography::Track;
//using Vector3D<double> = Tracking::Vector3D<double>;
using Tracking::Vector3D;//<double>;

#ifdef VERBOSE
bool verbose = true;
#else
bool verbose = false;
#endif

/* if xdir set to true then this will return the
 * displacement in X direction, otherwise in
 * Y direction
 *
 * NOTE: Here incoming track is the undeviated track which
 * is formed by extenting the incoming tracks to the topmost
 * detector in bottom plane.
 *
 * This calculation of incoming track should be done in Simulation
 * or analysis code which read real data from experimental setup.
 *
 * Basically extend the incoming track and find the point of
 * intersection with topmost detector in bottom detectors, that
 * means Z is already known, which is Z position first bottom
 * detector
 */
class Delta{
public:
template<bool xdir>
static double DeltaLinear(Track incoming, Track outgoing){
	if(xdir){
		return outgoing.GetP1().x() - incoming.GetP2().x();
	}else{
		return outgoing.GetP1().y() - incoming.GetP2().y();
	}
}

/* if xdir set to true then this will return the
 * displacement in X direction, otherwise in
 * Y direction.
 *
 * Here we have one more argument, which is the Z position of
 * first detector in bottom plane, hence allow us to do
 * calculation here itself.
 */

template<bool xdir>
static double DeltaLinear(Track incoming, Track outgoing, double FirstBottomDetectorZ){


	Track newIncoming(incoming.GetP1(), Vector3D<double>(0.,0.,0.));
	double L = (FirstBottomDetectorZ - incoming.GetP2().z())/incoming.GetDirection().z();
	if(verbose)
		std::cout << "DistaneTravelled : " << L << std::endl;
	Vector3D<double> p2(incoming.GetP2().x() + incoming.GetDirection().x() * L,
			incoming.GetP2().y() + incoming.GetDirection().y() * L,
			FirstBottomDetectorZ);
	newIncoming.SetP2(p2);
	return DeltaLinear<xdir>(newIncoming,outgoing);
}

/* if xdir set to true then this will return the
 * Angular deviation in X direction, otherwise in
 * Y direction
 */
template<bool xdir>
static double DeltaAngular(Track incoming, Track outgoing){

	Track newIncoming(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
	Track newOutgoing(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
	Track refIncoming(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
	Track refOutgoing(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
	if(xdir){
		newIncoming.SetP1(Vector3D<double>(incoming.GetP1().x(),0.,incoming.GetP1().z()));
		newIncoming.SetP2(Vector3D<double>(incoming.GetP2().x(),0.,incoming.GetP2().z()));
		newOutgoing.SetP1(Vector3D<double>(outgoing.GetP1().x(),0.,outgoing.GetP1().z()));
		newOutgoing.SetP2(Vector3D<double>(outgoing.GetP2().x(),0.,outgoing.GetP2().z()));

	}else{
		newIncoming.SetP1(Vector3D<double>(0.,incoming.GetP1().y(),incoming.GetP1().z()));
		newIncoming.SetP2(Vector3D<double>(0.,incoming.GetP2().y(),incoming.GetP2().z()));
		newOutgoing.SetP1(Vector3D<double>(0.,outgoing.GetP1().y(),outgoing.GetP1().z()));
		newOutgoing.SetP2(Vector3D<double>(0.,outgoing.GetP2().y(),outgoing.GetP2().z()));

	}
	refIncoming.SetP1(Vector3D<double>(newIncoming.GetP2().x(),newIncoming.GetP2().y(),newIncoming.GetP1().z()));
	refIncoming.SetP2(newIncoming.GetP2());
	refOutgoing.SetP1(newOutgoing.GetP1());
	refOutgoing.SetP2(Vector3D<double>(newOutgoing.GetP1().x(),newOutgoing.GetP1().y(),newOutgoing.GetP2().z()));


/*
 *  // For the time being, not using LinesAngle class, instead now we use
 *  // Function from CommonFunc::Functions class, that contains many such
 *  // function to do similar work using ROOT function.
 *
	LinesAngle la;
	double thetaIncoming = la.CalculateAngle(newIncoming,refIncoming);
	double thetaOutgoing = la.CalculateAngle(newOutgoing,refOutgoing);
*/

	double thetaIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(newIncoming,refIncoming);
	double thetaOutgoing = CommonFunc::Functions::instance()->GetAngleInRadian(newOutgoing,refOutgoing);

	if(verbose)
		std::cout<<"ThetaIncoming : " <<  thetaIncoming <<" : ThetaOutgoing : " << thetaOutgoing << std::endl;
	return (thetaOutgoing-thetaIncoming) ;
}


//Functions to detect Candidate Voxels that influenced the Muon direction
  /*
   * @Input : Track
   * @Input : Z value to detect the intersection at
   * @Input : An integer defining following
   * 		  1) Incoming Track (to detect incoming Hit Point at Voxelized Volume)
   * 		  2) Outgoing Track (to detect outgoing Hit Point at Voxelized Volume)
   * 		  3) Intersection with real Voxels
   *
   */


static Vector3D<double> GetIntersection(Track t, double zVal, int select){
	Vector3D<double> ptOfIntersection;
	double x = 0.,y = 0., z = 0., dist = 0. ;

	if(select == 1){
		dist = (zVal - t.GetP2().z())/t.GetDirection().z();
		ptOfIntersection = t.GetP2() + t.GetDirection()*dist;
	}

	if(select == 2){
		dist = (zVal - t.GetP1().z())/(-t.GetDirection().z());
		ptOfIntersection = t.GetP1() - t.GetDirection()*dist;
	}

	if(select == 3){
		dist = (zVal - t.GetP1().z())/t.GetDirection().z();
		ptOfIntersection = t.GetP1() + t.GetDirection()*dist;
	}

	return ptOfIntersection;
}

};



#endif /* HELPERS_INC_DELTA_H_ */
