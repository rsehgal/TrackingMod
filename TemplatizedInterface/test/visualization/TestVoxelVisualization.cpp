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
#include "Track.h"
#include "TStyle.h"
#include "TColor.h"

using Tomography::VisualizationHelper;
using Vec_t = Tracking::Vector3D<double>;
int main(int argc, char *argv[]) {
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//VisualizationHelper v;
	Tomography::VisualizationHelper *v = Tomography::VisualizationHelper::instance();
	v->InitializeVisualizer();
	//gStyle->SetPalette(kRainBow);

	std::ifstream filehandle(argv[1]);
	int numOfEvents = atoi(argv[2]);
	double x = 0., y = 0., z = 0.;
	double colorSD = 0.;
	double rl = 0.;
	double intensity = 0.;
	filehandle >> x >> y >> z >> colorSD;
	Vec_t voxDim(x, y, z);

	int i = 0;

	while (numOfEvents) {
		i++;
		filehandle >> x >> y >> z >> colorSD >> rl;
		std::cout << x << " , " << y << " , " << z << " , " << colorSD << " , "
				<< rl << std::endl;
		Vec_t voxCenter(x, y, z);
		//v->Register(voxDim, voxCenter, colorSD * 1000);
		double upperRLVal = 50.;
		if(rl < upperRLVal)
			//v->Register(voxDim, voxCenter, rl);
			v->Register(voxDim, voxCenter, rl);
		numOfEvents--;
	}

	v->Lock();
	v->Show();
	fApp->Run();

	return 0;
}
