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

DetectorLayout::~DetectorLayout() {
	// TODO Auto-generated destructor stub
}

