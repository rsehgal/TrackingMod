/*
 * VisualizeTracksFromIsmran.cpp
 *
 *  Created on: 28-Aug-2020
 *      Author: rsehgal
 */

#include "Coordinates.h"
#include <TGeoMatrix.h>
#include "base/Global.h"
#include "HittedPixel.h"
#include "TGeoBBox.h"
#include "visualizer/Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
 #include "visualizer/Eve/Singleton.h"
#include "Track.h"
#include <fstream>
#include "CommonFunc.h"

using Tomography::Coordinates;
int main(){

	TApplication *fApp = new TApplication("Test", NULL, NULL);
	Tomography::VisualizationHelper *v = Tomography::VisualizationHelper::instance();
	v->InitializeVisualizer();

	unsigned int numOfEvents = 100;
	Coordinates c;
	double ztop = 50.;
	double zbot = -50.;
	for (int evNo = 0; evNo < numOfEvents; evNo++) {
		double x = c.GenRandom(-50.,50);
		double y = c.GenRandom(-50.,50);
		Tracking::Vector3D<double> p1(x,y,ztop);
		x = c.GenRandom(-50.,50);
		y = c.GenRandom(-50.,50);
		Tracking::Vector3D<double> p2(x,y,zbot);
		Tomography::Track t(p1,p2);
		v->Register(&t);
		//sleep(1);
		v->Show();

	}

	 v->Show();
	fApp->Run();
}


