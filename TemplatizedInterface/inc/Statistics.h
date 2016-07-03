#include "Coordinates.h"
#include <TGeoMatrix.h>
#include "base/Global.h"
//#include "HittedPixel.h"
#include "TGeoBBox.h"
//#include "visualizer/Eve/Singleton.h"
//#include "VisualizationHelper.h"
#include "TThread.h"
//#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"
#include "base/Global.h"
#include "Properties.h"
#include "SetupManager.h"
//#include "visualizer/Eve/Singleton.h"
#include <TCanvas.h>
#include <TH1F.h>

typedef Tomography::Properties Detector;
class Statistics{

private:
	std::vector<int> FiredXStrips;
	std::vector<int> FiredYStrips;
	std::vector<std::vector<int>> TotalCluster;
	std::vector<Detector*> detectors;
	double efficiency;
public:
	Statistics();
	void Initialize();
	void GetXPlaneHistogram();
	void GetYPlaneHistogram();
	void GetClusterHistogram();
	void StripProfile();

};
