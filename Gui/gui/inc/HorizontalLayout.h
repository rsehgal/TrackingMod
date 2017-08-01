/*
 * HorizontalLayout.h
 *
 *  Created on: Mar 20, 2017
 *      Author: rsehgal
 */

#ifndef GUI_GUI_INC_HORIZONTALLAYOUT_H_
#define GUI_GUI_INC_HORIZONTALLAYOUT_H_

class QPushButton;
class QLabel;
class QHBoxLayout;
class QString;
#include <QWidget>
#include <QHBoxLayout>

class HorizontalLayout: public QWidget {
	Q_OBJECT
private:
	QHBoxLayout* fHLayout;
	QPushButton* fEffButton;
	QLabel* fVoltageLabel;

private slots:
		void slotButtonClicked(bool checked);
public:
		//Added the required parameters to this constructor, and pass them to CreateLayout function
	explicit HorizontalLayout(QString effStr, QString voltageStr, QWidget *parent = 0): QWidget(parent) {
		fHLayout = new QHBoxLayout;
		CreateLayout(effStr,voltageStr);

	}
	virtual ~HorizontalLayout();
	void CreateLayout(QString, QString);
	QHBoxLayout* GetLayout(){return fHLayout;}
};

#endif /* GUI_GUI_INC_HORIZONTALLAYOUT_H_ */
