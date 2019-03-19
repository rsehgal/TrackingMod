#include "Tree.h"
#include "base/Global.h"
#include <iostream>

int main() {

/*
  Tracking::Tree::instance()->SetTreeDefaultParameters();
  // Writing Tree
  Tracking::Tree::instance()->TreeW();

  // reading it back
  Tracking::Tree::instance()->ReadTree("1234.root", "BSC_DATA_TREE", 0);
  Tracking::Tree *tr = Tracking::Tree::instance()->GetTree();
  std::cout << "Num of Events : "<< tr->GetNumOfEvents() << std::endl;
*/
//  Tracking::Channel *ch = tr->GetEntry("channel", 5);
//  std::cout << ch->at(0) << std::endl;
//  std::cout << ch->at(1) << std::endl;
//  std::cout << ch->at(2) << std::endl;

  {
	  //Revision of tree creation
	  int numOfEvents = 10;
	  Tracking::Tree::Create("Raman","Sehgal");
	  Tracking::Tree* tr = Tracking::Tree::instance();
	  tr->InitializeTreeForWriting();
	  int ch;
	  tr->CreateBranch<int>("channel",ch);
	  double doubleVal;
	  tr->CreateBranch("doubleDatga",doubleVal);

	      for(int i= 0 ; i < numOfEvents ; i++){
	          	ch = i;
	          	doubleVal = (5.0*i+8.9)/2.3;
	       		tr->Fill();
	      }
	      tr->WriteToFile();
  }
}
