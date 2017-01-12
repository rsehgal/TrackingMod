#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>

using Tomography::VisualizationHelper;
using Tracking::Vector3D;


int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	VisualizationHelper v;
	v.RegisterLine(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));

    Vector3D<double> pt;
    std::ifstream ft;
    ft.open("tracks.txt");
    double x=0.,y=0.,z=0;
    while(!ft.eof()){
    	ft >> x >> y >> z;
    	pt.Set(x,y,z);
    	v.Register(pt);
    
    }
	
	v.Show();
    #ifdef USE_EVE
    gEve->DoRedraw3D(); 
    #endif
    fApp->Run();
}
