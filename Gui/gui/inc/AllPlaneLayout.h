/*
 * AllPlaneLayout.h
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_ALLPLANELAYOUT_H_
#define GUI_GUI_INC_ALLPLANELAYOUT_H_

#include <QWidget>
#include <QVBoxLayout>
#include "PlaneLayout.h"
#include <vector>
#include "ScintillatorPlane.h"

class QString;

class AllPlaneLayout : public QWidget {
	Q_OBJECT
private:
	QVBoxLayout *fVAllPlaneLayout;
public:
	AllPlaneLayout();
	explicit AllPlaneLayout(std::vector<QString> planeStrVect,QWidget *parent = 0): QWidget(parent) {
		fVAllPlaneLayout = new QVBoxLayout();
		CreateLayout(planeStrVect);
		//show();
	}
	explicit AllPlaneLayout(std::vector<Tomography::ScintillatorPlane*> planeVect,QWidget *parent = 0): QWidget(parent) {
			fVAllPlaneLayout = new QVBoxLayout();
			CreateLayout(planeVect);
			//show();
		}
	virtual ~AllPlaneLayout();
	void CreateLayout(std::vector<QString> planeStrVect);
	void CreateLayout(std::vector<Tomography::ScintillatorPlane*> planeVect);
	QVBoxLayout* GetLayout(){return fVAllPlaneLayout;}
};

#endif /* GUI_GUI_INC_ALLPLANELAYOUT_H_ */
