/*
 * DetectorLayout.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#include "DetectorLayout.h"
#include <QString>
#include <QLabel>
#include "AllPlaneLayout.h"
#include "VerticalLayout.h"
#include "ScintillatorPlane.h"
DetectorLayout::DetectorLayout() {
	// TODO Auto-generated constructor stub

}

void DetectorLayout::CreateLayout(QString detName,std::vector<QString> planeStrVect){
	fDetectorLabel = new QLabel(detName);
	fDetectorLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	fAllPlanes = new AllPlaneLayout(planeStrVect);
	fHDetectoLayout->addWidget(fDetectorLabel);
	fHDetectoLayout->addLayout(fAllPlanes->GetLayout());

	//setLayout(fHDetectoLayout);

}

void DetectorLayout::CreateLayout(QString detName,std::vector<Tomography::ScintillatorPlane*> planeVect){
	fDetectorLabel = new QLabel(detName);
	fDetectorLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	fAllPlanes = new AllPlaneLayout(planeVect);
	fHDetectoLayout->addWidget(fDetectorLabel);
	//fHDetectoLayout->addLayout(fAllPlanes->GetLayout());
	fVDetectoLayout->addLayout(fAllPlanes->GetLayout());
	fHDetectoLayout->addLayout(fVDetectoLayout);

	//setLayout(fHDetectoLayout);

}

void DetectorLayout::CreateLayout(Detector *detector){
	//std::vector<QString> planeVect;
	//for(int i=0; i < detector->GetNumOfPlanes();i++)
		//planeStrVect.push_back(QString::fromStdString(detector->GetPlane(i)->GetName()));
	fEffLabel = new QLabel("EffBoth");
	fDetEffButton = new QPushButton("BOTHxy",this);
	fDetEffButton->setCheckable(true);
	fTemp->addWidget(fDetEffButton);
	fTemp->addWidget(fEffLabel);
	fVDetectoLayout->addLayout(fTemp);
	CreateLayout(QString::fromStdString(detector->GetName()), detector->GetScintillatorPlaneVector());
	connect(fDetEffButton, SIGNAL (clicked(bool)), this, SLOT (slotButtonClicked(bool)));
}

DetectorLayout::~DetectorLayout() {
	// TODO Auto-generated destructor stub
}

