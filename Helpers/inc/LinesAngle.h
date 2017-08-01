#ifndef HELPERS_INC_LINESANGLE_H_
#define HELPERS_INC_LINESANGLE_H_

#include <iostream>
#include "base/Vector3D.h"
#include "Track.h"
using Tomography::Track;

//namespace Tomography{
class LinesAngle
{
private:
	double AngleDegree;
	double AngleRadian;
	double DotProductOfVectors;
	double LengthVector1;
	double LengthVector2;
public:
	double GetAngle();
	double CalculateAngle(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2, Tracking::Vector3D<double> q1, Tracking::Vector3D<double> q2);
	double CalculateAngle(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2);
	double CalculateAngle(Track t1, Track t2);
	double GetAngleRadian(double angleDegree);

};
//}// end of namespace Tomography

#endif
