/*
 * PlaneLayout.cpp
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#include "PlaneLayout.h"
#include "HorizontalLayout.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <iostream>


PlaneLayout::PlaneLayout() {
	// TODO Auto-generated constructor stub

}

void PlaneLayout::CreateLayout(QString planeStr, QString effStr){
	//std::cout <<"Eff : " << effNum.toUtf8().constData()<<" : Voltage : " << voltage.toUtf8().constData() << std::endl ;

	fPlaneButton = new QPushButton(planeStr,this);
	fPlaneButton->setCheckable(true);
	fPlaneButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	fEffLabel = new QLabel(effStr);
	fEffLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	fHPlaneLayout->addWidget(fPlaneButton);
	fHPlaneLayout->addWidget(fEffLabel);
	//connect(fEffButton, SIGNAL (clicked(bool)), this, SLOT (slotButtonClicked(bool)));
}


PlaneLayout::~PlaneLayout() {
	// TODO Auto-generated destructor stub
}

