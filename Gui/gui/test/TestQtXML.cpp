#include "RootFilesWindow.h"
#include <QApplication>
#include "DetectorLayout.h"
#include<vector>
#include <QString>
#include "AllPlaneLayout.h"
#include "PlaneLayout.h"
#include "Setup.h"
#include "Run.h"
#include "AllRunLayout.h"
#include <QtGui>
using namespace Tomography;
int main(int argc, char **argv){
	QApplication app(argc, argv);

#if(0)
	//Working Good
	Setup *s = new Setup(argv[1]);
	std::vector<Detector*> detVect = s->GetDetectorVector();


	for(int i=0 ; i<detVect.size() ; i++){
		std::cout<<"-----------------------------------------------------"<<std::endl;
		std::vector<QString> planeStrVect;
		QString detName = QString::fromStdString(detVect[i]->GetName());
		std::cout<<"Name : " << detVect[i]->GetName() << " : No of Planes : "<< detVect[i]->GetNumOfPlanes() << std::endl;
		for(int j=0 ; j< detVect[i]->GetNumOfPlanes() ; j++){
			std::cout<<"Plane-Name : " << detVect[i]->GetPlane(j)->GetName()<<std::endl;
			planeStrVect.push_back(QString::fromStdString(detVect[i]->GetPlane(j)->GetName()));
		}
		new DetectorLayout(detName,planeStrVect);

	}

#endif

#if(1)
	QWidget* topLevelWidget = 0;
	Setup *s = new Setup(argv[1]);
	std::vector<Detector*> detVect = s->GetDetectorVector();
	std::vector<QString> runNumVect;
	runNumVect.push_back("6928");
	runNumVect.push_back("6915");
	runNumVect.push_back("6702");
	runNumVect.push_back("6742");
	runNumVect.push_back("6133");
	runNumVect.push_back("6147");
	//new Run("6915",detVect);
	AllRunLayout *editor = new AllRunLayout(runNumVect,detVect);

    QScrollArea* scroller = new QScrollArea;
    scroller->setWidget(editor);
    topLevelWidget = scroller;

    topLevelWidget->show();
    topLevelWidget->raise();



#endif


#if(0)
	 QApplication app(argc, argv);
	// new VerticalLayout;

	 std::vector<QString> planeStrVect;
	 planeStrVect.push_back("TopPlane");
	 planeStrVect.push_back("BottomPlane");

	 new DetectorLayout("MT-1",planeStrVect);

	 //new AllPlaneLayout(planeStrVect);
	 return app.exec();
#endif
	 return app.exec();
}
