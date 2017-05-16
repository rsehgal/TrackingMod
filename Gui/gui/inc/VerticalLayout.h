/*
 * VerticalLayout.h
 *
 *  Created on: Mar 20, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_VERTICALLAYOUT_H_
#define GUI_GUI_INC_VERTICALLAYOUT_H_


#include "HorizontalLayout.h"
#include <QVBoxLayout>

class QVBoxLayout;
class QString;

class VerticalLayout: public QWidget {
QVBoxLayout *fVLayout;
public:
	//VerticalLayout();
explicit VerticalLayout(QWidget *parent = 0): QWidget(parent) {
	// TODO Auto-generated constructor stub
	fVLayout = new QVBoxLayout(this);
	CreateLayout();
	//fVLayout->show();
	show();

}
	virtual ~VerticalLayout();
	void CreateLayout();
	QVBoxLayout* GetLayout(){return fVLayout;}
};

#endif /* GUI_GUI_INC_VERTICALLAYOUT_H_ */
