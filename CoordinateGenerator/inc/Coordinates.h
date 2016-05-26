//#ifndef ThisClass
//#define ThisClass

#include <iostream>
#include <vector>
#include "TemplatizedInterface/inc/SetupManager.h"
#include "base/Vector3D.h"
#include "Properties.h"
typedef Tomography::Properties Detector;
using namespace Tracking;
//using std::vector;
class Coordinates{
private:
	int N;          						// Number of Detectors
	int M;
	double temp;		
	std::vector<Vector3D <double>> coordinate;
	std::vector<Vector3D <double>> stripcoord;
	std::vector<Vector3D <double>> strip;
	//vector<vector <double>> coordinate;
	//vector<vector <double>> stripcoord;    //generated coordinates from midpoints of 2-D grid of each detectors
	//vector<vector <double>> strip;

public:
	void InitializeVectors();
	void CoGenerator(int N, std::vector <Detector*> det);
	Vector3D<double> GetCoordinate(int detector);     //Enter value between 0 < detector < total detectors
	void SetStripCoordinates();
	Vector3D<double> GetStripCoordinate(int detector);
	void SetStrips();
	Vector3D<double> GetStrip(int detector);
	double GenRandom(double min, double max);
	void Print();

};
//#endif
