#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>
#include "Track.h"
#include "Point3D.h"

using Tomography::VisualizationHelper;
using Tracking::Vector3D;


int main(int argc, char *argv[]){

TApplication *fApp = new TApplication("Test", NULL, NULL);
//VisualizationHelper v;
 Tomography::VisualizationHelper *v = new Tomography::VisualizationHelper();//::instance();
  v->InitializeVisualizer();
//v.GetVisualizer().ImportFromROOTFile("atlas.root");
//v.GetVisualizer().ImportFromROOTFile("Hodoscope.root");

v->Register(argv[1]);

v->Register(new Tomography::Track(Vector3D<double>(10,50,0),Vector3D<double>(10,-50,0),6));
v->Register(new Tomography::Track(Vector3D<double>(30,50,0),Vector3D<double>(30,-50,0),2));
/*std::vector<lite_interface::Point3D*> vecPt;
vecPt.push_back(new lite_interface::Point3D(30,50,0));
vecPt.push_back(new lite_interface::Point3D(30,-50,0));
v->Register(new Tomography::Track(vecPt,2));*/


v->Show();
//
//gEve->DoRedraw3D();
fApp->Run();


}
