#include <TApplication.h>
#include "VisualizationHelper.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>

using Tomography::VisualizationHelper;
using Tracking::Vector3D;
#if(0)
int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	std::ifstream ft;
	ft.open(argv[1]);
	double x,y,z;
	int color;
	TH1F *hist = new TH1F("test","Scattering",100,0.,100.);
	while(!ft.eof()){
		ft >> x >> y >> z >> color;
		hist->Fill(color);
	}
	hist->Draw();
	fApp->Run();
}
#endif
#if(1)
int main(int argc, char *argv[]){
const char* input_filename = (argc>1)?argv[1]:"tracks.txt";
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	VisualizationHelper v;
#ifdef USE_EVE
	v.Register("Hodoscope.gdml");
	//v.Register("VoxelizedVolume.gdml");
	// v.RegisterLine(Vector3D<double>(0.,0.,0.),Vector3D<double>(0.,0.,0.));
 //    v.CreatePointSetArray();
    v.InitializeVisualizer();
#endif

    Vector3D<double> pt;
    std::ifstream ft;
    ft.open(input_filename);
    double x=0.,y=0.,z=0;
    double color=0;

    //TH1F *hist = new TH1F("test","Scattering",100,0.,100.);

    Tomography::Slicer slicer(-500,500.,-100.,100.,-150.,150.);
    double deno=10.;
    int count = atoi(argv[2]);
    //while(!ft.eof()){

   

    while(count){
	count--;
    	ft >> x >> y >> z >> color;
    //	hist->Fill(color);
    	//if((z/deno) > 15. || (z/deno) < 5.) continue;
    	pt.Set(x/deno,y/deno,z/deno);
	pt.SetColor(color);
    	//v.Register(slicer,pt);
    //v.Register(pt);
/*
#ifdef USE_EVE
	v.Register_V2(pt);
#else
*/
	v.Register(pt);
///#endif
    
    }
#ifdef USE_EVE
    v.Lock();
    //fPointSetArray->CloseBins();
#endif

	//hist->Draw();
	
	v.Show();
    #ifdef USE_EVE
    gEve->DoRedraw3D(); 
    #endif
    fApp->Run();
}
#endif
