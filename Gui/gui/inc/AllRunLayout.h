/*
 * AllRunLayout.h
 *
 *  Created on: Mar 24, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_ALLRUNLAYOUT_H_
#define GUI_GUI_INC_ALLRUNLAYOUT_H_

#include <QWidget>
#include <QVBoxLayout>
#include <vector>
#include <QString>
#include "Properties.h"
using Detector = Tomography::Properties;
class AllRunLayout : public QWidget{
	Q_OBJECT
private:
	QVBoxLayout *fVBoxLayout;

public:
	AllRunLayout();
	explicit AllRunLayout(std::vector<QString> runNumVect,std::vector<Detector*> detectorVect,QWidget *parent = 0): QWidget(parent) {
				fVBoxLayout = new QVBoxLayout();
				CreateLayout(runNumVect,detectorVect);
				//show();
			}

	void CreateLayout(std::vector<QString> runNumVect, std::vector<Detector*> detectorVect);
	virtual ~AllRunLayout();
};

#endif /* GUI_GUI_INC_ALLRUNLAYOUT_H_ */
