#include "Coordinates.h"
#include <math.h>

namespace Tomography{

	Coordinates::Coordinates(){}
	//Coordinates::~Coordinates(){}

	Coordinates::Coordinates(Vector3D<double> init)
	{
		InitialPoint.SetX(init.x());
		InitialPoint.SetY(init.y());
		InitialPoint.SetZ(init.z());
		signal = 1;
	}

	//Coordinates::~Coordinates(Vector3D<double> init){}


	void Coordinates::InitializeVectors()
	{
		coordinate.resize(N);
	  //	for (int i = 0; i < N; ++i)
	  //  		coordinate[i].resize(3);

	    stripcoord.resize(N);
	   // for(int i = 0 ; i < N ; i++)
	   // 	stripcoord[i].resize(3);

	    strip.resize(N);
	   // for(int i = 0 ; i < N ; i++)
	    //	strip[i].resize(2);
	}

	void Coordinates::CoGenerator(int N1, std::vector <Detector*> det)
	{
		N = N1;
	//	M = M1;
		
	 	InitializeVectors();
		
		for(int i = 0 ; i < N ; i++)
		{
			//coordinate[i][2] = (double)((N-1-i)*M);
			coordinate[i].SetZ(det[i]->GetZPos());
		}
		srand(time(NULL));
		//coordinate[0][0] = GenRandom(-50, 50);
		//coordinate[0][1] = GenRandom(-50, 50);
		//coordinate[N-1][0] = GenRandom(-50, 50);
		//coordinate[N-1][1] = GenRandom(-50, 50);
        if(signal == 0)
        {
			InitialPoint.SetX(GenRandom(-50,50));
			InitialPoint.SetY(GenRandom(-50,50));
			InitialPoint.SetZ(det[0]->GetZPos() + 30);
        }
		//coordinate[0].SetX(GenRandom(-50,50));
		//coordinate[0].SetY(GenRandom(-50,50));
		coordinate[N-1].SetX(GenRandom(-50,50));
		coordinate[N-1].SetY(GenRandom(-50,50));

		for(int i = 0 ; i < N - 1 ; i++)
		{
			//temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			//coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			//coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
			temp = ( coordinate[N-i-2].z()- InitialPoint.z() ) / ( InitialPoint.z() - coordinate[N-1].z());
			coordinate[N-i-2].SetX((InitialPoint.x() - coordinate[N-1].x()) * temp + InitialPoint.x());
			coordinate[N-i-2].SetY((InitialPoint.y() - coordinate[N-1].y()) * temp + InitialPoint.y());
		}		
	}

	double Coordinates::GenRandom(double min, double max)
	{
		return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
	}

	Vector3D<double> Coordinates::GetSpecificCoordinate(double zpos)
	{
		Vector3D<double> vec;
		temp = ( zpos - InitialPoint.z() ) / ( InitialPoint.z() - coordinate[N-1].z());
		vec.SetX((InitialPoint.x() - coordinate[N-1].x()) * temp + InitialPoint.x());
		vec.SetY((InitialPoint.y() - coordinate[N-1].y()) * temp + InitialPoint.y());
		vec.SetZ(zpos);

		return vec;
	}



	Vector3D<double> Coordinates::GetInitialPoint()
	{
		return InitialPoint;
	}

	

	Vector3D<double> Coordinates::GetCoordinate(int detector)
	{
		Vector3D<double> temp;
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
		//	return NULL;
		}

		//for(int i = 0 ; i < 3 ; i++)
		//{
			//temp[i] = coordinate[detector - 1][i];
			temp = coordinate[detector - 1];
		//}

		return temp;
	} 

	void Coordinates::SetStripCoordinates()
	{
		for(int i =  0 ; i < N ; i++)
		{
			//stripcoord[i][0] = (double)-50 + (strip[i][0])*3.125 + 1.5625;
			//stripcoord[i][1] = (double)-50 + (strip[i][1])*3.125 + 1.5625;
			//stripcoord[i][2] = coordinate[i][2];
			stripcoord[i].SetX(-50. + (strip[i].x())*3.125 + 1.5625);
			stripcoord[i].SetY(-50. + (strip[i].y())*3.125 + 1.5625);
			stripcoord[i].SetZ(coordinate[i].z());
		}
	}

	Vector3D<double> Coordinates::GetStripCoordinate(int detector)
	{
		Vector3D<double> temp;
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
		//	return NULL;
		}

		//for(int i = 0 ; i < 3 ; i++)
			temp = stripcoord[detector - 1];

		return temp;

	}

	void Coordinates::SetStrips()
	{
		for(int i =  0 ; i < N ; i++)
		{
		//	strip[i][0] = floor( (coordinate[i][0]+(double)50) /3.125 );
		//	strip[i][1] = floor( (coordinate[i][1]+(double)50) /3.125 );
			strip[i].SetX(floor( (coordinate[i].x()+(double)50) /3.125 ));
			strip[i].SetY(floor( (coordinate[i].y()+(double)50) /3.125 ));
			strip[i].SetZ(coordinate[i].z());

		}
	}

	Vector3D<double> Coordinates::GetStrip(int detector)
	{
		Vector3D<double> temp;
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
			//return NULL;
		}

		//for(int i = 0 ; i < 2 ; i++)
			temp = strip[detector - 1];

		return temp;
	}

	void Coordinates::Print()
	{
		std::cout<<"Coordinates"<<std::endl;
		std::cout<<""<<std::endl;

		for(int i = 0 ; i < N ; i++)
		{
			coordinate[i].Print();
		}

		std::cout<<""<<std::endl;
		std::cout<<"Strips:"<<std::endl;
		std::cout<<""<<std::endl;

		for(int i = 0 ; i < N ; i++)
		{
			strip[i].Print();
		}		
	}

}//end of Tomography namespace
