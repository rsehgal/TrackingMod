/*
 * ObjectChecker.h
 *
 *  Created on: Jul 24, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_OBJECTCHECKER_H_
#define HELPERS_INC_OBJECTCHECKER_H_

#include <iostream>
#include <set>
#include <string>
#include <cassert>
#include <TH2F.h>
using namespace std;

namespace Tomography {

class ObjectChecker {
	TH2F *fHist;
	string name;
	std::set<ObjectChecker> ObjectCheckersSet;

public:
	ObjectChecker();
	virtual ~ObjectChecker();

    ObjectChecker(string n);
    ObjectChecker(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh);
    TH2F* GetHist() const {return fHist;}
    bool operator<(const ObjectChecker& right)const;
    string get_name()const;
    void CheckAndInsert(std::string objObjectChecker);
    void CheckAndInsert(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh);
    void CheckAndInsert(string objName,string title,int xbins, double xlow, double xhigh,int ybins, double ylow, double yhigh,double x, double y, double w);
    std::set<ObjectChecker> GetSet(){return ObjectCheckersSet;}


//  private:
//     string name;
//     std::set<ObjectChecker> ObjectCheckersSet;
};

} /* namespace Tomography */

#endif /* HELPERS_INC_OBJECTCHECKER_H_ */
