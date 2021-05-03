#include "HelperFunctions.h"
#include <string>
//using  lite_interface::Point3D;
int main(int argc, char *argv[]){
	
	std::cout << NumOfEventsToRead(std::string(argv[1]),std::string(argv[2]),std::string(argv[3])) << std::endl;
}
