#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>

using Tomography::VisualizationHelper;
using Tracking::Vector3D;


int main(int argc, char *argv[]){
const char* input_filename = (argc>1)?argv[1]:"tracks.txt";
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	VisualizationHelper v;
#ifdef USE_EVE
	v.RegisterLine(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
#endif

    Vector3D<double> pt;
    std::ifstream ft;
    ft.open(input_filename);
    double x=0.,y=0.,z=0;
    int color=0;
    Tomography::Slicer slicer(-500,500.,-100.,100.,-150.,150.);
    double deno=1.;
    while(!ft.eof()){
    	ft >> x >> y >> z >> color;
    	pt.Set(x/deno,y/deno,z/deno);
	pt.SetColor(color);
    	//v.Register(slicer,pt);
	v.Register(pt);
    
    }
	
	v.Show();
    #ifdef USE_EVE
    gEve->DoRedraw3D(); 
    #endif
    fApp->Run();
}
