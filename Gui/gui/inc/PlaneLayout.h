/*
 * PlaneLayout.h
 *
 *  Created on: Mar 23, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_PLANELAYOUT_H_
#define GUI_GUI_INC_PLANELAYOUT_H_

class QPushButton;
class QLabel;
//class QHBoxLayout;
class QString;
#include <QWidget>
#include <QHBoxLayout>
#include "ScintillatorPlane.h"
#include <QLabel>
#include <ScintillatorPlane.h>

class PlaneLayout : public QWidget{
	Q_OBJECT
private:
	QHBoxLayout* fHPlaneLayout;
	QPushButton* fPlaneButton;
	QLabel* fEffLabel;
	QString fEffValue;
	Tomography::ScintillatorPlane *fScintillatorPlane;

private slots:
			void slotButtonClicked(bool checked){
				fScintillatorPlane->SetEfficiency();
				fEffValue = QString::number(fScintillatorPlane->GetEfficiency());
				fEffLabel->setText(fEffValue);
			}

public:
	PlaneLayout();
	explicit PlaneLayout(QString planeStr, QString effStr, QWidget *parent = 0): QWidget(parent) {
		fHPlaneLayout = new QHBoxLayout();
			CreateLayout(planeStr,effStr);
			//show();

		}
	explicit PlaneLayout(Tomography::ScintillatorPlane *plane,QWidget *parent = 0): QWidget(parent) {

			fHPlaneLayout = new QHBoxLayout();
				CreateLayout(plane);
				//show();

			}

	virtual ~PlaneLayout();
	void CreateLayout(QString, QString effStr);
	void CreateLayout(Tomography::ScintillatorPlane *plane);
	QHBoxLayout* GetLayout(){return fHPlaneLayout;}
};

#endif /* GUI_GUI_INC_PLANELAYOUT_H_ */
