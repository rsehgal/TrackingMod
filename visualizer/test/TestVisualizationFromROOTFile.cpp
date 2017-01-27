#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>

using Tomography::VisualizationHelper;
using Tracking::Vector3D;


int main(int argc, char *argv[]){

TApplication *fApp = new TApplication("Test", NULL, NULL);
VisualizationHelper v;
v.GetVisualizer().ImportFromROOTFile("atlas.root");
v.Show();
//gEve->DoRedraw3D();
fApp->Run();


}
