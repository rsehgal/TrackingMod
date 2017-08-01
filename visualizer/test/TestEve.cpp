#include "VisualizationHelper.h"
#include "TApplication.h"
#include "base/Vector3D.h"
#include <iostream>
using Tomography::VisualizationHelper;
using Vec_t = Tracking::Vector3D<double>;
int main(){
    TApplication *fApp = new TApplication("Test", NULL, NULL);
    VisualizationHelper v;
    Vec_t pt(3.,4.,5.,10.);
    v.Register_V2(pt);
    v.CloseBins();

    v.Show();
    fApp->Run();

    return 0;
}