#include "Tree.h"
#include "base/Global.h"
#include <iostream>

int main(){
	Tracking::Tree tw("hello.root","BSC_DATA_TREE",1);

	//Writing Tree
	tw.TreeW();

	//reading it back
	Tracking::Tree tr("hello.root","BSC_DATA_TREE",0);
	Tracking::Channel *ch = tr.GetEntry("channel",5);
	std::cout << ch->at(0) << std::endl;
	std::cout << ch->at(1) << std::endl;
	std::cout << ch->at(2) << std::endl;


	/*
	Tracking::Channel ch;
	int numOfEvents = 10;
	for(int i=0 ; i<numOfEvents ; i++){
#ifdef MULTIHIT
#else
	ch.push_back(10);
#endif
	tw.TreeW();//("HelloBranch",ch);

	}
	*/

}
