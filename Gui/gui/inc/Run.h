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
#include <QFrame>
#include <QRadioButton>
#include <QCheckBox>
using Detector = Tomography::Properties;
class Run : public QWidget{
	Q_OBJECT
private:
	//QVBoxLayout *fVAllRunLayout;
	QVBoxLayout *fVRunLayout;
	QHBoxLayout *fHRunLayout;
	QLabel *fRunLabel;
	//DetectorLayout *fDetectorLayout;
	QString fRunNum;
	QRadioButton *fRunNumRadioButton;
	QCheckBox *fRunNumCheckbox;

private slots:
	void slotCheckBoxClicked(bool checked){
		ReadData(fRunNum);
	}


public:
	Run();
	explicit Run(QString runNum,std::vector<Detector*> detectorVect,QWidget *parent = 0): QWidget(parent) {
			//fVAllRunLayout = new QVBoxLayout();
			fVRunLayout = new QVBoxLayout();
			fHRunLayout = new QHBoxLayout();
			//CreateLayout(runNum,detectorVect);
			//ReadData(runNum);
			CreateLayout2(runNum,detectorVect);
			//show();
		}
	/*explicit Run(std::vector<QString> runNumVect,std::vector<Detector*> detectorVect,QWidget *parent = 0): QWidget(parent) {
				fVAllRunLayout = new QVBoxLayout();
				fVRunLayout = new QVBoxLayout();
				fHRunLayout = new QHBoxLayout();
				//CreateLayout(runNum,detectorVect);
				//ReadData(runNum);
				CreateLayout3(runNumVect,detectorVect);
				show();
			}
*/
	/*explicit Run(QString runNum,std::vector<Detector*> detectorVect,QWidget *parent = 0): QWidget(parent) {
				fVRunLayout = new QVBoxLayout();
				fHRunLayout = new QHBoxLayout();
				CreateLayout(runNum,detectorVect);
				show();
			}
	*/
	virtual ~Run();
	void ReadData(QString runNum);
	void CreateLayout(QString runNum, std::vector<Detector*> detectorVect);
	void CreateLayout2(QString runNum, std::vector<Detector*> detectorVect);
//	void CreateLayout3(std::vector<QString> runNumVect,std::vector<Detector*> detectorVect);
	QHBoxLayout* GetLayout(){return fHRunLayout;}
};

class HSeparator : public QWidget{
	private:
	QFrame* line;
	public:
	HSeparator(){
		line=new QFrame();
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
	}
	QFrame* GetSeparator(){return line;}
};

class VSeparator : public QWidget{
	private:
	QFrame* line;
	public:
	VSeparator(){
		line=new QFrame();
		line->setFrameShape(QFrame::VLine);
		line->setFrameShadow(QFrame::Sunken);
	}
	QFrame* GetSeparator(){return line;}
};
#endif /* TEMPLATIZEDINTERFACE_INC_RUN_H_ */
