#include <iostream>
#include "Coordinates.h"

//#include "Properties.h"
typedef Tomography::Properties Detector;
#undef USE_EVE
#include "SetupManager.h"

int main()
{
	Coordinates c;
	Tomography::SetupManager s;
	int totalDetectors;

//	Detector *d1 = new Detector("FirstDetector",std::vector<int> channelsInDim);
//	Detector *d2 = new Detector("SecondDetector",std::vector<int> channelsInDim);
//	Detector *d3 = new Detector("ThirdDetector",std::vector<int> channelsInDim);
//	Detector *d4 = new Detector("FourthDetector",std::vector<int> channelsInDim);

	Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
	  Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	  Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", -60,-1);
	  Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", -90,-1);

	s.Register(rpc1);
	s.Register(rpc2);
	s.Register(rpc3);
	s.Register(rpc4);

	std::vector<Detector*> detVector = s.GetDetectorVector("GLASS");
	totalDetectors = detVector.size();

	Vector3D<double> temp;
	int N = totalDetectors;
	//int M = 30;
        //for(int j=0;j<4;j++){
	c.CoGenerator(totalDetectors , detVector);
	c.SetStrips();
	c.SetStripCoordinates();
	c.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < N ; i++)
	{
		temp = c.GetStripCoordinate(i+1);
		temp.Print();
		
	}
      //}
	return 0;
}
