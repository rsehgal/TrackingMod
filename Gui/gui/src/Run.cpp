/*
 * Run.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#include "Run.h"
#include "Tree.h"

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

void Run::CreateLayout2(QString runNum, std::vector<Detector*> detectorVect){

	//fRunLabel = new QLabel(runNum);
	//fHRunLayout->addWidget(fRunLabel);
	fRunNum = runNum;
	fRunNumRadioButton = new QRadioButton(runNum);
	fRunNumCheckbox = new QCheckBox(runNum);
	fHRunLayout->addWidget(fRunNumCheckbox);
	fHRunLayout->addWidget((new VSeparator)->GetSeparator());
	fVRunLayout->addWidget((new HSeparator)->GetSeparator());
	for(int i=0;i<detectorVect.size();i++){

		fVRunLayout->addLayout((new DetectorLayout(detectorVect[i]))->GetLayout());
		fVRunLayout->addWidget((new HSeparator)->GetSeparator());
	}
	//fVRunLayout->addWidget(line);
	fHRunLayout->addLayout(fVRunLayout);
	connect(fRunNumCheckbox, SIGNAL (clicked(bool)), this, SLOT (slotCheckBoxClicked(bool)));
	//setLayout(fHRunLayout);

}

/*void Run::CreateLayout3(std::vector<QString> runNumVect,std::vector<Detector*> detectorVect){
	for(int i=0;i<runNumVect.size();i++){
		CreateLayout2(runNumVect[i],detectorVect);
		fVAllRunLayout->addLayout(fHRunLayout);
	}
	setLayout(fVAllRunLayout);
}*/

void Run::ReadData(QString runNum){
	std::cout<<"----- Read Data Called ----- " << std::endl;
	std::string temp_str = runNum.toUtf8().constData();
	temp_str += ".root";
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

}
