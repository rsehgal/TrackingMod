//#ifndef df
#include <tgmath.h>
#include "LinesAngle.h"
//#define PI 3.14159265359
//namespace Tomography{
double LinesAngle::GetAngle(){ return AngleDegree; }

double LinesAngle::CalculateAngle(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2, Tracking::Vector3D<double> q1, Tracking::Vector3D<double> q2)
{
	
	DotProductOfVectors = (p1-p2).Dot(q1-q2);//( (p1.x() - p2.x()) * (q1.x() - q2.x()) ) + ( (p1.y() - p2.y()) * (q1.y() - q2.y()) ) + ( (p1.z() - p2.z()) * (q1.z() - q2.z()) );
	LengthVector1 = (p1-p2).Mag();//sqrt( (p1.x() - p2.x()) * (p1.x() - p2.x()) ) + ( (p1.y() - p2.y()) * (p1.y() - p2.y()) ) + ( (p1.z() - p2.z()) * (p1.z() - p2.z() ));
	LengthVector2 = (q1-q2).Mag();//sqrt( (q1.x() - q2.x()) * (q1.x() - q2.x()) ) + ( (q1.y() - q2.y()) * (q1.y() - q2.y()) ) + ( (q1.z() - q2.z()) * (q1.z() - q2.z()) );
	AngleDegree = DotProductOfVectors / (LengthVector1 * LengthVector2);
	AngleDegree = std::acos (AngleDegree) * 180.0 / 3.14159265359;

	return AngleDegree;
}

double LinesAngle::CalculateAngle(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2){
	Tracking::Vector3D<double> temp(0.,0.,0.);
	temp.Set(p1.x(),p1.y(),-p1.z());
    return CalculateAngle(p1,temp ,p1,p2);
}

double LinesAngle::CalculateAngle(Track t1, Track t2){
	return CalculateAngle(t1.GetP2(),t1.GetP1(),t2.GetP2(),t2.GetP1());
}

double LinesAngle::GetAngleRadian(double angleDegree)
{
	return (angleDegree * 3.14159265359) / 180;
}
//}//End of Namespace Tomography
//#endif
