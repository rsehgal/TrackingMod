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
	virtual ~AllPlaneLayout();
	void CreateLayout(std::vector<QString> planeStrVect);
	QVBoxLayout* GetLayout(){return fVAllPlaneLayout;}
};

#endif /* GUI_GUI_INC_ALLPLANELAYOUT_H_ */
