/*
 * TestBranchNames.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: rsehgal
 */

#include "Tree.h"

int main(){

	Tracking::Tree::instance()->ReadTree("6853.root", "BSC_DATA_TREE", 0);
	Tracking::Tree::instance()->PrintBranchNames();
	return 0;
}


