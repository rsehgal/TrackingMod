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

class AllPlaneLayout;
class QLabel;
class QString;

class DetectorLayout : public QWidget {
	Q_OBJECT
	QHBoxLayout *fHDetectoLayout;
	QLabel *fDetectorLabel;
	AllPlaneLayout *fAllPlanes;
	//QWidget window;
public:
	DetectorLayout();
	explicit DetectorLayout(QString detName,std::vector<QString> planeStrVect,QWidget *parent = 0): QWidget(parent) {
		fHDetectoLayout = new QHBoxLayout();
		CreateLayout(detName,planeStrVect);
		//show();
	}
	virtual ~DetectorLayout();
	void CreateLayout(QString detName,std::vector<QString> planeStrVect);
	QHBoxLayout* GetLayout(){return fHDetectoLayout;}
};

#endif /* GUI_GUI_INC_DETECTORLAYOUT_H_ */
