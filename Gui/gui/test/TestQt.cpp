#include "RootFilesWindow.h"
#include <QApplication>
#include "DetectorLayout.h"
#include<vector>
#include <QString>
#include "AllPlaneLayout.h"
#include "PlaneLayout.h"
int main(int argc, char **argv){

	 QApplication app(argc, argv);
	// new VerticalLayout;

	 std::vector<QString> planeStrVect;
	 planeStrVect.push_back("TopPlane");
	 planeStrVect.push_back("BottomPlane");

	 new DetectorLayout("MT-1",planeStrVect);

	 //new AllPlaneLayout(planeStrVect);
	 return app.exec();

}
