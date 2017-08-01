/*
 * RootFilesWindow.h
 *
 *  Created on: Mar 20, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_ROOTFILESWINDOW_H_
#define GUI_GUI_INC_ROOTFILESWINDOW_H_

#include "VerticalLayout.h"


class RootFilesWindow  : public QWidget {
public:
	explicit RootFilesWindow(QWidget *parent = 0): QWidget(parent) {

		new VerticalLayout();
		//show();

	}
	virtual ~RootFilesWindow();
};

#endif /* GUI_GUI_INC_ROOTFILESWINDOW_H_ */
