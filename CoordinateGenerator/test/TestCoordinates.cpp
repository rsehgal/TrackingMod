#include <iostream>
#include "Coordinates.h"

#include "Properties.h"
typedef Tomography::Properties Detector;
#undef USE_EVE
#include "SetupManager.h"
#include "LinesAngle.h"

int main()
{
	Vector3D<double> temp;
	Tomography::Coordinates c1;

	Tomography::SetupManager s1;
	Tomography::SetupManager s2;
	LinesAngle l;
	int totalDetectors_s1;
	int totalDetectors_s2;
	double angle;

//	Detector *d1 = new Detector("FirstDetector",std::vector<int> channelsInDim);
//	Detector *d2 = new Detector("SecondDetector",std::vector<int> channelsInDim);
//	Detector *d3 = new Detector("ThirdDetector",std::vector<int> channelsInDim);
//	Detector *d4 = new Detector("FourthDetector",std::vector<int> channelsInDim);

	Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
	  Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 60,-1);
	  Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", 30,-1);
	  Detector *rpc4 = new Tomography::GlassRpc(3,"FourthGlassRpc", 0,-1);

	s1.Register(rpc1);
	s1.Register(rpc2);
	s1.Register(rpc3);
	s1.Register(rpc4);



	std::vector<Detector*> detVector_s1 = s1.GetDetectorVector("GLASS");
	totalDetectors_s1 = detVector_s1.size();




	//int N = totalDetectors;
	//int M = 30;
        //for(int j=0;j<4;j++){

	std::cout<<"Detectors Above Object:"<<std::endl;
	std::cout<<""<<std::endl;

	c1.CoGenerator(totalDetectors_s1 , detVector_s1);
	c1.SetStrips();
	c1.SetStripCoordinates();
	c1.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < totalDetectors_s1 ; i++)
	{
		temp = c1.GetStripCoordinate(i+1);
		temp.Print();
		
	}
	std::cout<<""<<std::endl;
	std::cout<<"Initial Point"<<std::endl;
	std::cout<<""<<std::endl;
	temp = c1.GetInitialPoint();
	temp.Print();
      //}
	//////////////////////////
		std::cout<<""<<std::endl;
		std::cout<<"Detectors Beneath Object:"<<std::endl;
		std::cout<<""<<std::endl;
	//////////////////////////

		temp = c1.GetSpecificCoordinate(-15.0);
			Tomography::Coordinates c2(temp);

			Detector *rpc5 = new Tomography::GlassRpc(3,"FirstGlassRpc", -30,-1);
					  Detector *rpc6 = new Tomography::GlassRpc(3,"SecondGlassRpc", -60,-1);
					  Detector *rpc7 = new Tomography::GlassRpc(3,"ThirdGlassRpc", -90,-1);
					  Detector *rpc8 = new Tomography::GlassRpc(3,"FourthGlassRpc", -120,-1);

					s2.Register(rpc5);
					s2.Register(rpc6);
					s2.Register(rpc7);
					s2.Register(rpc8);

					std::vector<Detector*> detVector_s2 = s2.GetDetectorVector("GLASS");
							totalDetectors_s2 = detVector_s2.size();

	c2.CoGenerator(totalDetectors_s2 , detVector_s2);
	c2.SetStrips();
	c2.SetStripCoordinates();
	c2.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < totalDetectors_s2 ; i++)
	{
		temp = c2.GetStripCoordinate(i+1);
		temp.Print();

	}
	std::cout<<""<<std::endl;
	std::cout<<"Initial Point"<<std::endl;
	std::cout<<""<<std::endl;
	temp = c2.GetInitialPoint();
	temp.Print();

	///////////////////////////
	std::cout<<""<<std::endl;
	std::cout<<"Angle Between Lines In Degree :"<<std::endl;
	std::cout<<""<<std::endl;

	angle = l.CalculateAngle(c1.GetCoordinate(1), c1.GetCoordinate(2), c2.GetCoordinate(1), c2.GetCoordinate(2));
	std::cout<<angle<<std::endl;

	return 0;
}
