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

namespace Tracking{

ImageReconstruction::ImageReconstruction(){}
ImageReconstruction::~ImageReconstruction(){}

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
	std::cout<<"B1 : "<<b1<<" : B2 : "<<b2<<std::endl;
	std::cout<<"A : "<<a<<" : B : "<<b<<" : C : "<<c<<" : D : "<<d<<std::endl;
	std::cout<<"S : "<<s<<std::endl;


	t = (uDotv*(pDotu-qDotu) - uMag2*(pDotv-qDotv))/(uDotv*uDotv - uMag2*vMag2);
	std::cout<<"T : "<<t<<std::endl;

	//Vec_t
	p1 = p + u*s;
	//Vec_t
	q1 = q + v*t;
	return (p1+q1)/2.;
	//return s;
	}


void ImageReconstruction::EM(){}

}// end of Tracking namespace


