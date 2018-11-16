#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>
#include "CommonFunc.h"

using Tomography::VisualizationHelper;
using Tracking::Vector3D;

int main(int argc, char *argv[]){
const char* input_filename = (argc>1)?argv[1]:"tracks.txt";
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	VisualizationHelper *v = VisualizationHelper::instance();
#ifdef USE_EVE
	v->Register("Hodoscope.gdml");
    v->InitializeVisualizer();
    v->Lock();
	v->Show();
	gEve->DoRedraw3D();
	fApp->Run();
#endif

}

