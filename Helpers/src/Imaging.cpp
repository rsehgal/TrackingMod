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


	double numer = (uDotv*(pDotu-qDotu) - uMag2*(pDotv-qDotv));
	double deno = (uDotv*uDotv - uMag2*vMag2);
	t = numer / deno;
	// std::cout<<"T : "<<t<<std::endl;

	//Vec_t
	p1 = p + u*s;
	//Vec_t
	q1 = q + v*t;

	std::cout <<"S : " << s <<" : T : " << t << std::endl;
	return (p1+q1)/2.;
	//return s;
	}


Vec_t ImageReconstruction::POCA_V4( Track incoming, Track outgoing){
	//Algo from : http://etheses.whiterose.ac.uk/10432/1/Tim_Blackwell_PhD_Thesis.pdf
	Vec_t u = (incoming.GetP2() - incoming.GetP1()).Unit();
	Vec_t v = (outgoing.GetP1() - outgoing.GetP2()).Unit();
	Vec_t w = (incoming.GetP1() - outgoing.GetP2());
	double a = u.Dot(u);
	double b = u.Dot(v);
	double c = v.Dot(v);
	double d = u.Dot(w);
	double e = v.Dot(w);
	double deno = (a*c-b*b);
//	if(!deno)
//		deno = 1e-9;
	//double s = (b*e-c*d)/(a*c-b*b);
	double s = (b*e-c*d)/deno;
	//double t = (a*e-b*d)/(a*c-b*b);
	double t = (a*e-b*d)/deno;
	Vec_t Ps = incoming.GetP1() + u*s;
	Vec_t Qt = outgoing.GetP2() + v*t;
	std::cout << "S : " << s << " : T : " << t << std::endl;
	return (Ps+Qt)/2.;

}

double ImageReconstruction::cpa_time(Track Tr1, Track Tr2){
	Vec_t dv = Tr1.GetDirCosine() - Tr2.GetDirCosine();
	double dv2 = dv.Dot(dv);
	if(dv2 < 1e-9)
		return 0.;

	Vec_t w0 = Tr1.GetP1() - Tr2.GetP1();
	double cpatime = w0.Dot(dv)/dv2;
	return cpatime;
}

Vec_t ImageReconstruction::POCA_V3( Track incoming, Track outgoing){
	double ctime = cpa_time(incoming,outgoing);
	Vec_t P1 = incoming.GetP1() + incoming.GetDirCosine()*ctime;
	Vec_t P2 = outgoing.GetP1() + outgoing.GetDirCosine()*ctime;
	std::cout << "CTIME : " << ctime << std::endl;
	std::cout << "P1 : " ; P1.Print();
	std::cout << "P2 : " ; P2.Print();
	double doca =  Distance(P1,P2);
	return (P1+P2)/2.;
}

Vec_t ImageReconstruction::POCA_V2(Vec_t p, Vec_t u, Vec_t q, Vec_t v, Vec_t &p1, Vec_t &q1){

}

Vec_t ImageReconstruction::POCA( Track incoming, Track outgoing){
	Vec_t p1(0.,0.,0.),q1(0.,0.,0.);
	//std::cout<<"INComing Track : "; incoming.Print();
	//std::cout<<"OUTGoing Track : "; outgoing.Print();
	return POCA(incoming.GetP1(),incoming.GetDirCosine(),outgoing.GetP1(), outgoing.GetDirCosine(),p1,q1);
	//return POCA_V3(incoming,outgoing);
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
