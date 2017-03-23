/*
 * Run.h
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_RUN_H_
#define TEMPLATIZEDINTERFACE_INC_RUN_H_

#include <QWidget>
#include "DetectorLayout.h"
#include <vector>
#include "Properties.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
using Detector = Tomography::Properties;
class Run : public QWidget{
	Q_OBJECT
private:
	QVBoxLayout *fVRunLayout;
	QHBoxLayout *fHRunLayout;
	QLabel *fRunLabel;
	//DetectorLayout *fDetectorLayout;

public:
	Run();
	explicit Run(QString runNum,std::vector<Detector*> detectorVect,QWidget *parent = 0): QWidget(parent) {
			fVRunLayout = new QVBoxLayout();
			fHRunLayout = new QHBoxLayout();
			CreateLayout(runNum,detectorVect);
			show();
		}
	virtual ~Run();
	void CreateLayout(QString detName, std::vector<Detector*> detectorVect);
	QVBoxLayout* GetLayout(){return fVRunLayout;}
};

#endif /* TEMPLATIZEDINTERFACE_INC_RUN_H_ */
