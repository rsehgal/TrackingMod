#include "Coordinates.h"
#include <math.h>


	void Coordinates::InitializeVectors()
	{
		coordinate.resize(N);
	  	for (int i = 0; i < N; ++i)
	    		coordinate[i].resize(3);

	    stripcoord.resize(N);
	    for(int i = 0 ; i < N ; i++)
	    	stripcoord[i].resize(3);

	    strip.resize(N);
	    for(int i = 0 ; i < N ; i++)
	    	strip[i].resize(2);	
	}

	void Coordinates::CoGenerator(int N1, int M1)
	{
		N = N1;
		M = M1;
		
	 	InitializeVectors();
		
		for(int i = 0 ; i < N ; i++)
		{
			coordinate[i][2] = (double)((N-1-i)*M);
		}
		srand(time(NULL));
		coordinate[0][0] = GenRandom(-50, 50);
		coordinate[0][1] = GenRandom(-50, 50);
		coordinate[N-1][0] = GenRandom(-50, 50);
		coordinate[N-1][1] = GenRandom(-50, 50);

		for(int i = 0 ; i < N - 2 ; i++)
		{
			temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
		}		
	}

	double Coordinates::GenRandom(double min, double max)
	{
		return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
	}

	

	double* Coordinates::GetCoordinate(int detector)
	{
		double* temp = new double[3];
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
			return NULL;
		}

		for(int i = 0 ; i < 3 ; i++)
			temp[i] = coordinate[detector - 1][i];

		return temp;
	} 

	void Coordinates::SetStripCoordinates()
	{
		for(int i =  0 ; i < N ; i++)
		{
			stripcoord[i][0] = (double)-50 + (strip[i][0])*3.125 + 1.5625;
			stripcoord[i][1] = (double)-50 + (strip[i][1])*3.125 + 1.5625;
			stripcoord[i][2] = coordinate[i][2];
		}
	}

	double* Coordinates::GetStripCoordinate(int detector)
	{
		double* temp = new double[3];
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
			return NULL;
		}

		for(int i = 0 ; i < 3 ; i++)
			temp[i] = stripcoord[detector - 1][i];

		return temp;

	}

	void Coordinates::SetStrips()
	{
		for(int i =  0 ; i < N ; i++)
		{
			strip[i][0] = floor( (coordinate[i][0]+(double)50) /3.125 );
			strip[i][1] = floor( (coordinate[i][1]+(double)50) /3.125 );
		}
	}

	double* Coordinates::GetStrip(int detector)
	{
		double* temp = new double[2];
		if(detector > N || detector < 1)
		{
			std::cout<<"Inappropriate Request"<<std::endl;
			return NULL;
		}

		for(int i = 0 ; i < 2 ; i++)
			temp[i] = strip[detector - 1][i];

		return temp;
	}

	void Coordinates::Print()
	{
		std::cout<<"Coordinates"<<std::endl;
		std::cout<<""<<std::endl;

		for(int i = 0 ; i < N ; i++)
		{
			std::cout<<coordinate[i][0]<<" "<<coordinate[i][1]<<" "<<coordinate[i][2]<<std::endl;
		}

		std::cout<<""<<std::endl;
		std::cout<<"Strips:"<<std::endl;
		std::cout<<""<<std::endl;

		for(int i = 0 ; i < N ; i++)
		{
			std::cout<<strip[i][0]<<" "<<strip[i][1]<<std::endl;
		}		
	}
