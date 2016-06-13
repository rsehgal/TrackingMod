/*
 * StripTest.cpp
 *
 *  Created on: Jun 6, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include "Properties.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "base/Vector3D.h"
//#include "Update.h"
#include "Tree.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

Tracking::Vector3D<double> GetStripCoordinate(double x, double y, double z)
    {
        int tmp = 0;
        Vector3D<double> temp;

//        temp.SetX(floor( (x+(double)50) /3.125 ));
//        temp.SetY(floor( (y+(double)50) /3.125 ));
        temp.SetX(-50. + x*3.125 + 1.5625);
        temp.SetY(-50. + y*3.125 + 1.5625);
        temp.SetZ(z);

        return temp;
    }

/*
 *
 *          stripcoord[i].SetX(-50. + (strip[i].x())*3.125 + 1.5625);
			stripcoord[i].SetY(-50. + (strip[i].y())*3.125 + 1.5625);
			stripcoord[i].SetZ(coordinate[i].z());
 */

void GenerateCoordinates(std::vector<int>xVec,std::vector<int>yVec){
	for(int i=0 ; i<xVec.size() ; i++){
		for(int j=0 ; j<yVec.size() ; j++){
			Tracking::Vector3D<double> coordinates = GetStripCoordinate(xVec[i],yVec[j],50);
			coordinates.Print();
		}
	}
}

int main(){

	Detector *rpc = new GlassRpc(2,"GlassRpc", 120,31);
	//Tracking::Tree t("6742.root", "BSC_DATA_TREE");
	Tracking::Tree::instance()->ReadTree("6742.root", "BSC_DATA_TREE", 0);
	int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	numOfEvents = 10;
    for(int evNo = 0 ; evNo < numOfEvents ; evNo++){
    	std::cout<<"======================================================" << std::endl;
    	for(int plNum = 0 ; plNum < rpc->GetNumOfPlanes() ; plNum++){
    		rpc->GetPlane(plNum)->SetFiredStripsVector(evNo);
    		int numOfStripsFired = rpc->GetPlane(plNum)->GetFiredStripsVector().size();
#define DEBUG_MSG
#ifdef DEBUG_MSG

    		std::cout<<"Num of Strips fired in Plane : "<< plNum << " : " << numOfStripsFired << std::endl;
    		for(int i = 0 ; i < numOfStripsFired ; i++)
    			std::cout<<rpc->GetPlane(plNum)->GetFiredStripsVector()[i]<<", ";
    		std::cout<<std::endl;

#endif

    	}

    	GenerateCoordinates(rpc->GetPlane(0)->GetFiredStripsVector(),rpc->GetPlane(1)->GetFiredStripsVector());
    }
}


