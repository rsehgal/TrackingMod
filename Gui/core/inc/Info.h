#ifndef __GUI_INFO_H
#define __GUI_INFO_H
#include <iostream>
#include <map>

class Info{
std::map<int,double> fInfoMap;
public:
Info();
~Info();
void FillInfoMap(std::string dir="",std::string infoFileName="info.txt");
std::map<int,double> GetMap(){return fInfoMap;}
};
#endif
