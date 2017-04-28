/*
 * Imaging.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#include "Imaging.h"
#include "base/Vector3D.h"

#define  Precision double
#define  Vec_t Vector3D<Precision>
using Tomography::Track;
namespace Tracking{

ImageReconstruction::ImageReconstruction(){}
ImageReconstruction::~ImageReconstruction(){}

//For normal POCA we are using we are using incoming point and direction cosine and outgoing point and direction cosine
Vec_t ImageReconstruction::POCA(Vec_t p, Vec_t u, Vec_t q, Vec_t v, Vec_t &p1, Vec_t &q1){

	  Precision pDotv=p.Dot(v);
	  Precision qDotv=q.Dot(v);
	  Precision pDotu=p.Dot(u);
	  Precision qDotu=q.Dot(u);
	  Precision uDotv=u.Dot(v);
	  Precision vMag2=v.Mag2();
	  Precision uMag2=u.Mag2();


	Precision s=0.,t=0.;
	//s = (-(p.Dot(v)-q.Dot(v))/u.Dot(v))+(-1.*v.Mag2()*(u.Dot(v)*(p.Dot(u)-q.Dot(u))- (v.Mag2()*(p.Dot(v)-q.Dot(v)))))/(u.Dot(v)*(-1*(u.Dot(v) * u.Dot(v)) + v.Mag2()*u.Mag2()));
	Precision a = -(pDotv-qDotv)/uDotv;
	Precision b1 = uDotv*(pDotu-qDotu);
	Precision b2 = uMag2*(pDotv-qDotv);
	Precision b = b1 - b2;
	Precision c = (-uDotv*uDotv + vMag2*uMag2);
	Precision d = (-vMag2*b)/(uDotv*c);
	s = a+d;
	// std::cout<<"B1 : "<<b1<<" : B2 : "<<b2<<std::endl;
	// std::cout<<"A : "<<a<<" : B : "<<b<<" : C : "<<c<<" : D : "<<d<<std::endl;
	// std::cout<<"S : "<<s<<std::endl;


	t = (uDotv*(pDotu-qDotu) - uMag2*(pDotv-qDotv))/(uDotv*uDotv - uMag2*vMag2);
	// std::cout<<"T : "<<t<<std::endl;

	//Vec_t
	p1 = p + u*s;
	//Vec_t
	q1 = q + v*t;
	return (p1+q1)/2.;
	//return s;
	}

//Vec_t ImageReconstruction::POCA_Iterative(Vec_t p, Vec_t u, Vec_t q, Vec_t v, Vec_t &p1, Vec_t &q1){
// For Iterative version we are using Track as input arguement
Vec_t ImageReconstruction::POCA_Iterative(Track incoming,Track outgoing){
std::cout<<"---- Entered ITERATIVE-POCA -----" << std::endl;
	Vec_t p11(0.,0.,0.), q11(0.,0.,0.);
	Vec_t pocaPt = POCA(incoming.GetP1(),incoming.GetDirCosine(),outgoing.GetP2(),outgoing.GetDirCosine(),p11,q11);
#if(1)
        Vec_t pn(0.,0.,0.),qn(0.,0.,0.);
        while(Distance(p11,q11)> 1){
	   GetIntPoints(p11,q11,pn,qn);
	   Track trk1 = GetNewTrack(incoming.GetP2(),pn);
	   Track trk2 = GetNewTrack(qn,outgoing.GetP1());
	   pocaPt = POCA(trk1.GetP1(),trk1.GetDirCosine(),trk2.GetP2(),trk2.GetDirCosine(),p11,q11);
	}
#endif
	return pocaPt;
	

}


void ImageReconstruction::GetIntPoints(Vec_t p, Vec_t q, Vec_t &intPt1, Vec_t &intPt2 ){
	Precision dist = Distance(p,q);
	Vec_t dir = (p-q).Unit();
	intPt1 = p+dir*dist/3;
	intPt2 = q-dir*dist/3;
}

Track ImageReconstruction::GetNewTrack(Vec_t p, Vec_t q){
 return Track(p,q);
}

Precision ImageReconstruction::Distance(Vec_t p, Vec_t q){
 return (p-q).Mag(); 
}

void ImageReconstruction::EM(){}

}// end of Tracking namespace
