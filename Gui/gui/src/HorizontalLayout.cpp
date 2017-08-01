/*
 * HorizontalLayout.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: rsehgal
 */

#include "HorizontalLayout.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <iostream>
HorizontalLayout::~HorizontalLayout() {
	// TODO Auto-generated destructor stub
}

void HorizontalLayout::CreateLayout(QString effNum, QString voltage){
	std::cout <<"Eff : " << effNum.toUtf8().constData()<<" : Voltage : " << voltage.toUtf8().constData() << std::endl ;
	//fHLayout = new QHBoxLayout;
	fEffButton = new QPushButton(effNum,this);
	fEffButton->setCheckable(true);
	fVoltageLabel = new QLabel(voltage);
	fHLayout->addWidget(fEffButton);
	fHLayout->addWidget(fVoltageLabel);
	connect(fEffButton, SIGNAL (clicked(bool)), this, SLOT (slotButtonClicked(bool)));
}

void HorizontalLayout::slotButtonClicked(bool checked)
{
	std::cout<<"Checked : "<< checked << std::endl;
 if (checked) {
 fVoltageLabel->setText("Checked");
 } else {
 fVoltageLabel->setText("Hello World");
 }
 system("./JustEfficiencyXML ../setup.xml 6928");
}


