/*
 * TestGenTree.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: rsehgal
 */

#include "Tree.h"
#include "base/Global.h"
#include "base/Vector3D.h"
#include "Track.h"
#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"



int main() {
	  std::cout<<"========= Entered main ======== " << std::endl;
	  Tracking::Tree *tree = Tracking::Tree::instance();
#if(0)
	  tree->SetTreeDefaultParameters();
	  tree->InitializeTreeForWriting();

	  Tracking::Vector3D<double> p1,p2;
	  Tomography::Track t;
	  tree->CreateBranch<Tracking::Vector3D<double>>("P1", p1);
	  tree->CreateBranch<Tracking::Vector3D<double>>("P2", p2);
	  tree->CreateBranch<Tomography::Track>("Track",t);

	  for(int i = 0 ; i< 100; i++){
		  p1.Set(1*i,2*i,3*i);
		  p2.Set(4*i,5*i,6*i);
		  t.SetP1(p1);
		  t.SetP2(p2);
		  tree->Fill();
	  }

	  tree->WriteToFile();
#endif

#if(1)
	  //Doing some read test using TTreeReader
	  tree->NewReadTree("hello.root","BSC_DATA_TREE");
	  std::cout<<"========= reached here ======== " << std::endl;
	  //tree->GetEntry(tree->ConnectToBranch<Tomography::Track>("Track"),1);//.Print();
	  //(tree->GetEntry<Tomography::Track>("Track",11)).Print();
	  std::cout<< "Value : " << (tree->GetEntry<double>("ScatteringAngle",11)) << std::endl;//.Print();
#endif

#if(0)
   TFile *myFile = TFile::Open("hello.root");
   TTreeReader myReader("BSC_DATA_TREE", myFile);
   TTreeReaderValue<Tomography::Track> myTrack(myReader, "Track");
   TTreeReaderValue<Tracking::Vector3D<double>> myVec(myReader, "P1");

   myReader.SetEntry(9);
   (*myTrack).Print();

   //(*(myTrack+1)).Print();
   //myTrack.SetEntry(1);
   //myReader.Next();
   //(*myTrack).Print();
   /*while (myReader.Next()) {
      //myHist->Fill(*myPx + *myPy);
      (*myTrack).Print();
   }*/

#endif
	  return 0;
}




