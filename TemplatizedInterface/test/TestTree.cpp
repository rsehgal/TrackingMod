#include "Tree.h"
#include "base/Global.h"
#include <iostream>

int main(){

	Tracking::Tree::instance()->SetTreeDefaultParameters();
	//Writing Tree
	Tracking::Tree::instance()->TreeW();

	//reading it back
	Tracking::Tree::instance()->ReadTree("hello.root","BSC_DATA_TREE",0);
	Tracking::Tree* tr = Tracking::Tree::instance()->GetTree();
	std::cout<<tr->GetNumOfEvents() << std::endl;
	Tracking::Channel *ch = tr->GetEntry("channel",5);
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
