#include <iostream>
#include "Coordinates.h"
int main()
{
	Coordinates c;
	double* temp;
	int N = 5;
	int M = 30;
        for(int j=0;j<4;j++){
	c.CoGenerator(N,M);
	c.SetStrips();
	c.SetStripCoordinates();
	//c.Print();
	std::cout<<""<<std::endl;
	std::cout<<"Printing MidPoint of all the strips where intersection happens:"<<std::endl;
	std::cout<<""<<std::endl;

	for(int i = 0 ; i < N ; i++)
	{
		temp = c.GetStripCoordinate(i+1);
		std::cout<<*temp<<" "<<*(temp+1)<<" "<<*(temp+2)<<std::endl; 
		
	}
      }
	return 0;
}
