/*
 * HistogramCountingTest.cpp
 *
 *  Created on: Jan 10, 2018
 *      Author: rsehgal
 */

#include "Hist2D.h"
#undef NDEBUG
#include <iostream>
#include <cassert>
#include "Coordinates.h"


using Tomography::Hist2D;
using Tomography::Coordinates;

int main(){
	Hist2D *h1 = new Hist2D("My2DHist", 4, 0., 4., 4, 0., 4.);
	h1->SetCutOnNumOfEntriesInABin(4532);
	assert(h1->GetNumberOfBins()==16);
	Coordinates c;

	int numOfEntries = 150000;
	for(int i = 0 ; i < numOfEntries ; i++){
		double x = c.GenRandom(0,4);
		double y = c.GenRandom(0,4);
		if(h1->GetBinCount(h1->GetBinNum(x,y)) < h1->GetCutOnNumOfEntriesInABin())
			h1->Fill(x,y);

	}

	h1->Fill(3.5,3.1);


	int totalCount = 0;
	for(int i = 0 ; i < h1->GetNumberOfBins() ; i++){
		std::cout << "Entries in Bin Num : " << i << " : " <<  h1->GetBinCount(i) << std::endl;
		totalCount += h1->GetBinCount(i) ;
	}



	std::cout<<"TotalCount : " << totalCount << std::endl;
	assert(totalCount==numOfEntries);
	std::cout << "Histogram Counting Test Passed...." << std::endl;
	return 0;

}


