/*
 * DetectorLayout.h
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_DETECTORLAYOUT_H_
#define GUI_GUI_INC_DETECTORLAYOUT_H_

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Properties.h"
#include "ScintillatorPlane.h"
#include <QString>
#include <QPushButton>
#include <QLabel>
using Detector = Tomography::Properties;

class AllPlaneLayout;
class QLabel;
class QString;

class DetectorLayout : public QWidget {
	Q_OBJECT
	QHBoxLayout *fHDetectoLayout;
	QVBoxLayout *fVDetectoLayout;
	QLabel *fDetectorLabel;
	AllPlaneLayout *fAllPlanes;

	QString fEffValue;
	QHBoxLayout  *fTemp;
	QLabel *fEffLabel;
	QPushButton *fDetEffButton;
	Detector *fDetector;
	//QWidget window;
private slots:
	void slotButtonClicked(bool checked){
		fDetector->SetEfficiency();
		fEffValue = QString::number(fDetector->GetEfficiency());
		fEffLabel->setText(fEffValue);
	}

public:
	DetectorLayout();
	explicit DetectorLayout(QString detName,std::vector<QString> planeStrVect,QWidget *parent = 0): QWidget(parent) {
		fHDetectoLayout = new QHBoxLayout();
		CreateLayout(detName,planeStrVect);
		//show();
	}
	explicit DetectorLayout(Detector *detector,QWidget *parent = 0): QWidget(parent) {
		 	fDetector = detector;
			fHDetectoLayout = new QHBoxLayout();
			fTemp = new QHBoxLayout();
			fVDetectoLayout = new QVBoxLayout();
			CreateLayout(detector);
			//show();
		}
	virtual ~DetectorLayout();
	void CreateLayout(QString detName,std::vector<QString> planeStrVect);
	void CreateLayout(Detector *detector);
	void CreateLayout(QString detName,std::vector<Tomography::ScintillatorPlane*> planeVect);
	QHBoxLayout* GetLayout(){return fHDetectoLayout;}
};

#endif /* GUI_GUI_INC_DETECTORLAYOUT_H_ */
