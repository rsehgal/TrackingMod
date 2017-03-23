/*
 * Run.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#include "Run.h"

Run::Run() {
	// TODO Auto-generated constructor stub

}

Run::~Run() {
	// TODO Auto-generated destructor stub
}

void Run::CreateLayout(QString runNum, std::vector<Detector*> detectorVect){
	fRunLabel = new QLabel(runNum);
	fHRunLayout->addWidget(fRunLabel);
	for(int i=0;i<detectorVect.size();i++){
		//std::cout<<"--------- Added ----------- " << (i+1) << std::endl;
		std::vector<QString> planeStrVect;
		QString detName = QString::fromStdString(detectorVect[i]->GetName());
		for(int j=0;j<detectorVect[i]->GetNumOfPlanes();j++){
			planeStrVect.push_back(QString::fromStdString(detectorVect[i]->GetPlane(j)->GetName()));
	    }
		if(detectorVect[i]->GetNumOfPlanes() == 2){
			planeStrVect.push_back("Both");
		}
		fVRunLayout->addLayout((new DetectorLayout(detName,planeStrVect))->GetLayout());
	}
	fHRunLayout->addLayout(fVRunLayout);
	setLayout(fHRunLayout);
}
