#include "Info.h"
#include <iostream>
#include <string>


int main(int argc, char *argv[]){
Info f;
if(argc==2)
f.FillInfoMap(std::string(argv[1]));
else
f.FillInfoMap();

return 1;
}
