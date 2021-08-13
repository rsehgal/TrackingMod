#include "EveVisualizer.h"
#include "TApplication.h"
#include "base/Vector3D.h"
#include <iostream>
#include "VisualizationHelper.h"
using Tomography::VisualizationHelper;
using Vec_t = Tracking::Vector3D<double>;
int main(){
    TApplication *fApp = new TApplication("Test", NULL, NULL);
    //VisualizationHelper v;
    Tomography::VisualizationHelper *v = new Tomography::VisualizationHelper();//::instance();
    //Tracking::EveVisualizer *v = new Tracking::EveVisualizer();
    Vec_t pt(3.,4.,5.,10.);
    v->InitializeVisualizer();
    v->Register(pt);

    double color =10;
    //Now tryig to register a voxel
    Vec_t voxDim(10.,10.,10.);
    Vec_t voxCenter(0.,0.,0.);
    /*v->Register(voxDim,voxCenter,color);

    voxCenter.Set(10.,0.,0.);
    v->Register(voxDim,voxCenter,2);

    voxCenter.Set(0.,10.,0.);
    v->Register(voxDim,voxCenter,4);

    voxCenter.Set(10.,10.,0.);
    v->Register(voxDim,voxCenter,6);
*/
    v->Lock();
    //rev.CloseBins();

    v->Show();
    fApp->Run();

    return 0;
}
