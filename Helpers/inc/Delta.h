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

using Tomography::Track;
using Vec_t = Tracking::Vector3D<double>;

bool verbose = true;

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
template<bool xdir>
double DeltaLinear(Track incoming, Track outgoing){
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
double DeltaLinear(Track incoming, Track outgoing, double FirstBottomDetectorZ){


	Track newIncoming(incoming.GetP1(), Vec_t(0.,0.,0.));
	double L = (FirstBottomDetectorZ - incoming.GetP1().z())/incoming.GetDirection().z();
	if(verbose)
		std::cout << "DistaneTravelled : " << L << std::endl;
	Vec_t p2(incoming.GetP1().x() + incoming.GetDirection().x() * L,
			incoming.GetP1().y() + incoming.GetDirection().y() * L,
			FirstBottomDetectorZ);
	newIncoming.SetP2(p2);
	return DeltaLinear<xdir>(newIncoming,outgoing);
}

/* if xdir set to true then this will return the
 * Angular deviation in X direction, otherwise in
 * Y direction
 */
template<bool xdir>
double DeltaAngular(Track incoming, Track outgoing){

	Track newIncoming(Vec_t(0.,0.,0.),Vec_t(0.,0.,0.));
	Track newOutgoing(Vec_t(0.,0.,0.),Vec_t(0.,0.,0.));
	Track refIncoming(Vec_t(0.,0.,0.),Vec_t(0.,0.,0.));
	Track refOutgoing(Vec_t(0.,0.,0.),Vec_t(0.,0.,0.));
	if(xdir){
		newIncoming.SetP1(Vec_t(incoming.GetP1().x(),0.,incoming.GetP1().z()));
		newIncoming.SetP2(Vec_t(incoming.GetP2().x(),0.,incoming.GetP2().z()));
		newOutgoing.SetP1(Vec_t(outgoing.GetP1().x(),0.,outgoing.GetP1().z()));
		newOutgoing.SetP2(Vec_t(outgoing.GetP2().x(),0.,outgoing.GetP2().z()));

	}else{
		newIncoming.SetP1(Vec_t(0.,incoming.GetP1().y(),incoming.GetP1().z()));
		newIncoming.SetP2(Vec_t(0.,incoming.GetP2().y(),incoming.GetP2().z()));
		newOutgoing.SetP1(Vec_t(0.,outgoing.GetP1().y(),outgoing.GetP1().z()));
		newOutgoing.SetP2(Vec_t(0.,outgoing.GetP2().y(),outgoing.GetP2().z()));

	}
	refIncoming.SetP1(Vec_t(newIncoming.GetP2().x(),newIncoming.GetP2().y(),newIncoming.GetP1().z()));
	refIncoming.SetP2(newIncoming.GetP2());
	refOutgoing.SetP1(newOutgoing.GetP1());
	refOutgoing.SetP2(Vec_t(newOutgoing.GetP1().x(),newOutgoing.GetP1().y(),newOutgoing.GetP2().z()));

	LinesAngle la;
	double thetaIncoming = la.CalculateAngle(newIncoming,refIncoming);
	double thetaOutgoing = la.CalculateAngle(newOutgoing,refOutgoing);
	if(verbose)
		std::cout<<"ThetaIncoming : " <<  thetaIncoming <<" : ThetaOutgoing : " << thetaOutgoing << std::endl;
	return (thetaOutgoing-thetaIncoming) ;
}



#endif /* HELPERS_INC_DELTA_H_ */
