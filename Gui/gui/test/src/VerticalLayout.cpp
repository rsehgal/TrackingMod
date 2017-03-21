/*
 * VerticalLayout.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: rsehgal
 */

#include "VerticalLayout.h"
#include "Info.h"
#include <map>
#include <string>
#include <QString>
#include <QVBoxLayout>

/*
VerticalLayout::VerticalLayout() {
	// TODO Auto-generated constructor stub
	fVLayout = new QVBoxLayout;
	CreateLayout();

}
*/

VerticalLayout::~VerticalLayout() {
	// TODO Auto-generated destructor stub
}

void VerticalLayout::CreateLayout(){
	Info f;
	f.FillInfoMap();
	std::map<int,double> mymap = f.GetMap();
	std::map<int,double>::iterator it;

	for (it=mymap.begin(); it!=mymap.end(); ++it)
	{
	  fVLayout->addLayout((new HorizontalLayout(QString::number(it->first),QString::number(it->second)))->GetLayout());
	}
}

