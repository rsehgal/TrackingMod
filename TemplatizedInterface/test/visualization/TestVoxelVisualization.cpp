/* This file will read Voxels.txt and visualize voxels with
*  the color corresponding to the standard deviation 
*  (bettter will be with RADIATION LENGTH )
*
*/

#include "VisualizationHelper.h"
#include "TApplication.h"
#include "base/Vector3D.h"
#include <iostream>
#include <fstream>

using Tomography::VisualizationHelper;
using Vec_t = Tracking::Vector3D<double>;
int main(int argc, char *argv[]){
    TApplication *fApp = new TApplication("Test", NULL, NULL);
    VisualizationHelper v;
    //Vec_t pt(3.,4.,5.,10.);
    v.InitializeVisualizer();
    //v.Register(pt);

//Reading Voxels.txt
#if(1)
std::ifstream filehandle(argv[1]);
int numOfEvents = atoi(argv[2]);
double x = 0., y = 0., z = 0.;
double color = 0.;
filehandle >> x >> y >> z;
Vec_t voxDim(x,y,z);
int i = 0 ;

while(numOfEvents){
    i++;
    filehandle >> x >> y >> z >> color;
    //voxCenter.Set(x*1.,y*1.,z*1.);
    Vec_t voxCenter(x,y,z);
    v.Register(voxDim,voxCenter,color);
    numOfEvents--;
}


#endif


#if(0) // Demonstration
    double color =10;
    //Now tryig to register a voxel
    Vec_t voxDim(100.,100.,90.);

    for(int i = 0 ;  i < 4 ; i++){
    Vec_t voxCenter(100.*i,0.,0.);
    v.Register(voxDim,voxCenter,i+1);
    }

/*
    voxCenter.Set(10.,0.,0.);
    v.Register(voxDim,voxCenter,2);

    voxCenter.Set(0.,10.,0.);
    v.Register(voxDim,voxCenter,4);

    voxCenter.Set(10.,10.,0.);
    v.Register(voxDim,voxCenter,6);
*/

    
    //rev.CloseBins();
#endif
    v.Lock();
    v.Show();
    fApp->Run();

    return 0;
}
