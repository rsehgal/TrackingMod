#include <iostream>
#include <fstream>

int main(int argc, char **argv){

	std::ifstream input;
	input.open(argv[1]);


	std::ofstream output;
	output.open("ThreeD.txt");

	double x=0,y=0,z=0.,color=1;

	if(input.is_open()){
		while(!input.eof()){
			input >> x >> y >> z;
			output << x << " " << y << " " << z << " " << color <<std::endl;
		}
	}

	input.close();
	output.close();

}
