//#ifndef ThisClass
//#define ThisClass

#include <iostream>
#include <vector>
using std::vector;
class Coordinates{
private:
	int N;          						// Number of Detectors
	int M;
	double temp;		
	vector<vector <double>> coordinate;	
	vector<vector <double>> stripcoord;    //generated coordinates from midpoints of 2-D grid of each detectors
	vector<vector <double>> strip;	

public:
	void InitializeVectors();
	void CoGenerator(int N, int M);
	double* GetCoordinate(int detector);     //Enter value between 0 < detector < total detectors
	void SetStripCoordinates();
	double* GetStripCoordinate(int detector);
	void SetStrips();
	double* GetStrip(int detector);
	double GenRandom(double min, double max);
	void Print();

};
//#endif