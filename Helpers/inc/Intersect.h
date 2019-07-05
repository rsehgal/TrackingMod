/*
 * Intersect.h
 *
 *  Created on: July 5, 2019
 *  
 *  Author : Kavya Mohandas
 *  
 */

#ifndef HELPERS_INC_INTERSECT_H_
#define HELPERS_INC_INTERSECT_H_


using namespace Tomography;
using namespace Tracking;
using namespace std;

using Tomography::Track;

using Vec_t = Tracking::Vector3D<double>;
double xx1,yy1,zz1,xx2,yy2,zz2,xx3,yy3,zz3,xx4,yy4,zz4,a,b;
int countSkewed =0;
int countIntersect=0;


double s,t,denos,denot ,eq;
double o,u,c,p,q,r;
double eps=1e-9 ;






bool forXY(Vec_t p1, Vec_t p3, Vec_t dir1, Vec_t dir2)
{
  std::cout<<"-------- Entered inside XY -------------" << std::endl;
double	d1x=dir1.x();
double	d1y=dir1.y();
double	d1z=dir1.z();
double	d2x=dir2.x();
double	d2y=dir2.y();
double	d2z=dir2.z();
double	x1=p1.x();
double	y1=p1.y();
double	z1=p1.z();
double	x3=p3.x();
double	y3=p3.y();
double	z3=p3.z();


       //s = ((y1*d1z)+(z3*d1y)-d1y-(y3*d1z))/(d2y-d1y*d2z);

       denos = (d1x*d2y - d1y*d2x);
	if(denos==0.)
	   return false;
	s = (d1x*(y1 - y3) - d1y*(x1 - x3))/denos;
        t = (d2x*(y1 - y3) - d2y*(x1 - x3))/denos;
       
	
        std::cout << "S : " << s << " :: T : " << t << std::endl; 
        
	eq =( (d1z*t)-(d2z*s)+z1-z3 );
        std::cout << std::setprecision(16) << "eq value :: " << eq <<std::endl;
        
        if((eq < eps) || (eq ==0) )
        {
          
  	 o= x1+ (t*d1x);
 	 u= y1+ (t*d1y);
	 c= z1 + (t*d1z);
         std::cout<<"Point of intersection due to XY plane  :: " << o <<" "<< u <<" "<< c <<std::endl;
	return true;
         }
        else{
           std::cout<<" some error in XY " << std::endl;
	return false;
	}
	

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool forYZ(Vec_t p1, Vec_t p3, Vec_t dir1, Vec_t dir2)
{
  std::cout<<"-------- Entered inside YZ -------------" << std::endl;
double	d1x=dir1.x();
double	d1y=dir1.y();
double	d1z=dir1.z();
double	d2x=dir2.x();
double	d2y=dir2.y();
double	d2z=dir2.z();
double	x1=p1.x();
double	y1=p1.y();
double	z1=p1.z();
double	x3=p3.x();
double	y3=p3.y();
double	z3=p3.z();


       //s = ((y1*d1z)+(z3*d1y)-d1y-(y3*d1z))/(d2y-d1y*d2z);

       denos = (d1y*d2z - d1z*d2y);
	if(denos==0.)
	   return false;
	s = (d1y*(z1 - z3) - d1z*(y1 - y3))/denos;
        t = (d2y*(z1 - z3) - d2z*(y1 - y3))/denos;
       
	
        std::cout << "S : " << s << " :: T : " << t << std::endl; 
        
	eq =( d1x*t - d2x*s + x1 - x3 );
        std::cout << std::setprecision(20) << "eq value :: " << eq <<std::endl;
        
        if((eq < eps) || (eq ==0) )
        {
          
  	 o= x1+ (t*d1x);
 	 u= y1+ (t*d1y);
	 c= z1 + (t*d1z);
         std::cout<<"Point of intersection due to YZ plane  :: " << o <<" "<< u <<" "<< c <<std::endl;
 	return true;
         }
        else
 	{
           std::cout<<" some error in YZ " << std::endl;
   	   return false;
	}

	
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////





bool  forZX(Vec_t p1, Vec_t p3, Vec_t dir1, Vec_t dir2)
{
    std::cout<<"-------- Entered inside ZX -------------" << std::endl;
 double	d1x=dir1.x();
double	d1y=dir1.y();
double	d1z=dir1.z();
double	d2x=dir2.x();
double	d2y=dir2.y();
double	d2z=dir2.z();
double	x1=p1.x();
double	y1=p1.y();
double	z1=p1.z();
double	x3=p3.x();
double	y3=p3.y();
double	z3=p3.z();


       //s = ((y1*d1z)+(z3*d1y)-d1y-(y3*d1z))/(d2y-d1y*d2z);

       denos = (d1x*d2z - d1z*d2x);
	if(denos==0.)
	   return false;
	s = (d1x*(z1 - z3) - d1z*(x1 - x3))/denos;
        t = (d2x*(z1 - z3) - d2z*(x1 - x3))/denos;
       
	
        std::cout << "S : " << s << " :: T : " << t << std::endl; 
        
	eq =( d1y*t - d2y*s + y1 - y3 );
       std::cout << std::setprecision(16) << "eq value :: " << eq <<std::endl;
        
        if((eq < eps) || (eq ==0) )
        {
          
  	 o= x1+ (t*d1x);
 	 u= y1+ (t*d1y);
	 c= z1 + (t*d1z);
         std::cout<<"Point of intersection due to ZX plane  :: " << o <<" "<< u <<" "<< c <<std::endl;
		return true;
         }
        else{
           	std::cout<<" some error in ZX " << std::endl;
		return false;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Intersected(Track t1, Track t2)
{
	Vector3D<double> p1=t1.GetP1();
	Vector3D<double> p3=t2.GetP1();
	Vec_t dir1=t1.GetDirCosine();
	Vec_t dir2=t2.GetDirCosine();
	bool intersectionExist = (forXY(p1,p3,dir1,dir2) ||forYZ(p1,p3,dir1,dir2)|| forZX(p1,p3,dir1,dir2));
	return intersectionExist;
}




#endif

