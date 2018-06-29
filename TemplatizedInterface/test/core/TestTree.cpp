#include "Tree.h"
#include "base/Global.h"
#include <iostream>

int main() {

  Tracking::Tree::instance()->SetTreeDefaultParameters();
  // Writing Tree
  Tracking::Tree::instance()->TreeW();

  // reading it back
  Tracking::Tree::instance()->ReadTree("hello.root", "BSC_DATA_TREE", 0);
  Tracking::Tree *tr = Tracking::Tree::instance()->GetTree();
  std::cout << "Num of Events : "<< tr->GetNumOfEvents() << std::endl;
  Tracking::Channel *ch = tr->GetEntry("channel", 5);
  std::cout << ch->at(0) << std::endl;
  std::cout << ch->at(1) << std::endl;
  std::cout << ch->at(2) << std::endl;

  std::cout<<"---- Processing 375.root -----" << std::endl;
  Tracking::Tree::instance()->ReadTree("379.root", "BSC_DATA_TREE", 0);
  tr = Tracking::Tree::instance()->GetTree();
  std::cout << "Num of Events : "<< tr->GetNumOfEvents() << std::endl;


  for(int i = 8 ; i < 16 ; i++){
	  std::string  brName = "Module2_LE_CH"+std::to_string(i);
	  Tracking::Channel *ch2 = tr->GetEntry(brName,99761);
	  std::cout << "Multihit size : " << ch2->size() << std::endl;
  }


}
