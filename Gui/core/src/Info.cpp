#include "Info.h"
#include "fstream"

Info::Info(){}

Info::~Info(){}

void Info::FillInfoMap(std::string dir, std::string infoFileName){

std::string fullFileName="";
if(dir!="")
 fullFileName = dir+"/"+infoFileName;
else
 fullFileName = infoFileName;

std::cout<<"Full file Name : "<< fullFileName << std::endl;
int runNo=0;
double voltage=0.;
 std::ifstream infofile(fullFileName);
  if (infofile.is_open())
  {
    while(!infofile.eof()){
	infofile >> runNo >> voltage;
	fInfoMap.insert(std::pair<int,double>(runNo,voltage));
    }
  }else{
    std::cerr <<"File Does not exist..... " << std::endl;
  }
}
