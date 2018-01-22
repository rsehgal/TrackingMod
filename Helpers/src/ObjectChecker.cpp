/*
 * ObjectChecker.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: rsehgal
 */

#include "ObjectChecker.h"

namespace Tomography {

ObjectChecker::ObjectChecker() {
	// TODO Auto-generated constructor stub

}

ObjectChecker::~ObjectChecker() {
	// TODO Auto-generated destructor stub
}

void ObjectChecker::CheckAndInsert(std::string objObjectChecker){
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
if((*it).get_name() == objObjectChecker)
{
    std::cout <<"Attempt to DUPLICATE ... the histogram with name : "<< (*it).GetHist()->GetName() << "  !!! " << std::endl;
//    cout << "HistObjectChecker : "<< (*it).GetHist()->GetObjectChecker() << endl;
    return ;
}
}

std::cout<<"Inserting new Object..... " << std::endl;
ObjectCheckersSet.insert(ObjectChecker(objObjectChecker));

}

void ObjectChecker::CheckAndInsert(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh){
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
if((*it).get_name() == objName)
{
    std::cout <<"Attempt to DUPLICATE ... the histogram with name : "<< (*it).GetHist()->GetName() << "  !!! " << std::endl;
//    cout << "HistObjectChecker : "<< (*it).GetHist()->GetObjectChecker() << endl;
    return ;
}
}

std::cout<<"Inserting new Object..... " << std::endl;
ObjectCheckersSet.insert(ObjectChecker(objName,title,xbins,xlow,xhigh,ybins,ylow,yhigh));

}

void ObjectChecker::CheckAndInsert(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh,
									double x, double y, double w){
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
if((*it).get_name() == objName)
{
    //std::cout <<"Attempt to DUPLICATE ... the histogram with name : "<< (*it).GetHist()->GetName() << "  !!! .. Reusing Previously declared one.." << std::endl;
	// cout << "HistObjectChecker : "<< (*it).GetHist()->GetObjectChecker() << endl;

	/* Removed the weightage parameter(which corresponds to scattering or color),
	 * removed because this was not appropriate here, since we just wanted to know
	 * the count of POCA points in a slice of voxelized volume.
	 */
	//(*it).GetHist()->Fill(x,y,w);
	(*it).GetHist()->Fill(x,y);
    return ;
}
}

std::cout<<"Inserting new Object..... " << std::endl;
ObjectCheckersSet.insert(ObjectChecker(objName,title,xbins,xlow,xhigh,ybins,ylow,yhigh));

}

ObjectChecker::ObjectChecker(string n)
{
   name = n;
   //fHist = new TH1F(n.c_str(),n.c_str(),100,0,100);
}

ObjectChecker::ObjectChecker(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh){
	name = objName;
	fHist = new TH2F(objName.c_str(),title.c_str(),xbins,xlow,xhigh,ybins,ylow,yhigh);
}

bool ObjectChecker::operator<(const ObjectChecker& right)const
{
      return (this->name < right.name);
}

string ObjectChecker::get_name()const
{
      return name;
}


} /* namespace Tomography */
