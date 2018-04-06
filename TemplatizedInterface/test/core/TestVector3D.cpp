#include "base/Vector3D.h"
#include <iostream>
#include "Track.h"
#include "CommonFunc.h"
#include <G4ThreeVector.hh>
using Tracking::Vector3D;
using Tomography::Track;
int main(){
Vector3D<double> pt(0.832701, -0.472254, -0.261020);
pt.Unit().Print();
{
Vector3D<double> pt1(-78.125,140.625,760);
Vector3D<double> pt2(-78.125,109.375,460);
Vector3D<double> pt3(-15.625,-15.625,-440);
Vector3D<double> pt4(15.625,-78.125,-740);

Track t1(pt1,pt2);
Track t2(pt3,pt4);
Track ref(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,-1.));

std::cout << "Angle : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,t2)
		  << " : Incoming : " << CommonFunc::Functions::instance()->GetAngleInRadian(t1,ref)
		  << " : Outgoing : " << CommonFunc::Functions::instance()->GetAngleInRadian(t2,ref)
		  << std::endl;
}

{
std::cout << "------------- Using G4Three Vector --------------" << std::endl;
G4ThreeVector pt1(-78.125,140.625,760);
G4ThreeVector pt2(-78.125,109.375,460);
G4ThreeVector pt3(-15.625,-15.625,-440);
G4ThreeVector pt4(15.625,-78.125,-740);

G4ThreeVector t1 = pt2-pt1;
G4ThreeVector t2 = pt4-pt3;
G4ThreeVector ref(0.,0.,-1.);
std::cout << "Angle : " << t1.angle(t2)
		  << " : Incoming : "<< t1.angle(ref)
		  << " : Outgoing : "<< t2.angle(ref)
		  << std::endl;

}

{
std::cout << "------------- Using G4Three Vector --------------" << std::endl;
G4ThreeVector pt1(-128.675 , -30.0348 , 760);
G4ThreeVector pt2(-121.628 , 42.5673 , 460);
G4ThreeVector pt3(-100.489 , 260.374 , -440);
G4ThreeVector pt4(-93.442 , 332.976 , -740);

G4ThreeVector t1 = pt2-pt1;
G4ThreeVector t2 = pt4-pt3;
G4ThreeVector ref(0.,0.,-1.);
std::cout << "Angle : " << t1.angle(t2)
		  << " : Incoming : "<< t1.angle(ref)
		  << " : Outgoing : "<< t2.angle(ref)
		  << std::endl;

}


return 0;
}
