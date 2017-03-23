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


class PlaneLayout : public QWidget{
	Q_OBJECT
private:
	QHBoxLayout* fHPlaneLayout;
	QPushButton* fPlaneButton;
	QLabel* fEffLabel;

private slots:
			void slotButtonClicked(bool checked){}

public:
	PlaneLayout();
	explicit PlaneLayout(QString planeStr, QString effStr, QWidget *parent = 0): QWidget(parent) {
		fHPlaneLayout = new QHBoxLayout();
			CreateLayout(planeStr,effStr);
			//show();

		}
	virtual ~PlaneLayout();
	void CreateLayout(QString, QString);
	QHBoxLayout* GetLayout(){return fHPlaneLayout;}
};

#endif /* GUI_GUI_INC_PLANELAYOUT_H_ */
