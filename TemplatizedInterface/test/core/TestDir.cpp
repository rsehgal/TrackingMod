/*
 * TestDir.cpp
 *
 *  Created on: July 5, 2019
 *  
 *  Author : Kavya Mohandas
 *  
 */

#include "Track.h"
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Intersect.h"


int main()

{


std::ifstream infile("TrackExact.txt");
std::ofstream ifile("intersect.txt");
std::ofstream sfile("skewed.txt");


double xx1,yy1,zz1,xx2,yy2,zz2,xx3,yy3,zz3,xx4,yy4,zz4,a,b;
int countSkewed =0;
int countIntersect=0;
while(!infile.eof()){
	infile >> xx1 >> yy1 >> zz1 >> xx2 >> yy2 >> zz2 >> xx3 >> yy3 >> zz3 >> xx4 >> yy4 >> zz4 >> a >> b;
	Vector3D<double> p1(xx1,yy1,zz1);
	Vector3D<double> p2(xx2,yy2,zz2);
	Vector3D<double> p3(xx3,yy3,zz3);
	Vector3D<double> p4(xx4,yy4,zz4);
	std::cout <<"-------------------------------"<<std::endl;
	Track t1(p1,p2);
	Track t2(p3,p4);
        t1.Print(); t2.Print();
	
	if(Intersected(t1,t2))
        {
	   countIntersect++;
	   std::cout << "Intersection = YES " << std::endl;
        ifile << xx1 << " "<< yy1 << " "<< zz1 <<" "<<  xx2 << " "<< yy2 <<" "<<  zz2 <<" "<<  xx3 <<" "<<  yy3 <<" "<<  zz3 <<" "<<  xx4 <<" "<<  yy4 <<" "<<  zz4 <<" "<<  a <<" "<<  b << std::endl; 
         
	}
	else {
	countSkewed++;
   	std::cout << "Intersection = NO " <<std::endl;
        sfile << xx1 << " "<< yy1 << " "<< zz1 <<" "<<  xx2 << " "<< yy2 <<" "<<  zz2 <<" "<<  xx3 <<" "<<  yy3 <<" "<<  zz3 <<" "<<  xx4 <<" "<<  yy4 <<" "<<  zz4 <<" "<<  a <<" "<<  b << std::endl; 
	}

	std::cout << "Skewed tracks : " << countSkewed <<" : Intersected Tracks : " << countIntersect << std::endl;



}

infile.close();
ifile.close();
sfile.close();

return 0;

}
