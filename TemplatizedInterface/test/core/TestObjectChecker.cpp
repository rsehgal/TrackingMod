/*
 * TestObjectChecker.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: rsehgal
 */

#include "ObjectChecker.h"
#include <iostream>

using Tomography::ObjectChecker;

int main()
{
ObjectChecker name;
std::vector<std::string> nameVect;
nameVect.push_back("Raman");//"Raman",100,0,100,100,0,100);
nameVect.push_back("Sehgal");//,"Sehgal",100,0,100,100,0,100);

for(int i = 0 ; i < nameVect.size() ; i++){
	string cusname = nameVect[i];
	name.CheckAndInsert(cusname,cusname,100,0,100,100,0,100);
}

//ObjectCheckersSet.insert(ObjectChecker("raman"));
name.CheckAndInsert("raman","raman",100,0,100,100,0,100);

std::cout<<"------------------ Trying to duplicate -------------------" << std::endl;
name.CheckAndInsert("Raman","Raman",100,0,100,100,0,100);

std::set<ObjectChecker> ObjectCheckersSet = name.GetSet();

std::cout<<"-------------- Printing All histograms names -------------" << std::endl;
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
std::cout<<"HistName : "<< (*it).GetHist()->GetName() << std::endl;
}

   return 0;
}





