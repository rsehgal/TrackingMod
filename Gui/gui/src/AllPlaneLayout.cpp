/*
 * AllPlaneLayout.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#include "AllPlaneLayout.h"
#include <QString>
#include<iostream>

AllPlaneLayout::AllPlaneLayout() {
	// TODO Auto-generated constructor stub

}

void AllPlaneLayout::CreateLayout(std::vector<QString> planeStrVect){

	for(int i=0 ; i<planeStrVect.size() ; i++){
	//	std::cout<<"--- Adding : " << i << " ----" << std::endl;
	fVAllPlaneLayout->addLayout((new PlaneLayout(planeStrVect[i],"Efficiency"))->GetLayout());
	}
}

void AllPlaneLayout::CreateLayout(std::vector<Tomography::ScintillatorPlane*> planeVect){
	for(int i=0 ; i<planeVect.size() ; i++){
		fVAllPlaneLayout->addLayout((new PlaneLayout(planeVect[i]))->GetLayout());
	}
	//Now doing this thing in Detectot itself
	/*if(planeVect.size()==2){
		fVAllPlaneLayout->addLayout((new PlaneLayout("BothXY","EFFICIENCY"))->GetLayout());
	}*/
}

AllPlaneLayout::~AllPlaneLayout() {
	// TODO Auto-generated destructor stub
}

