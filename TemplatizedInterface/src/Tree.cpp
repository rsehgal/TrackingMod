#include "Tree.h"
#include <TFile.h>
#include <TBranch.h>
#include <iostream>

namespace Tracking{

Tree *Tree::s_instance = 0;
Tree* Tree::instance() {
        if (!s_instance)
          s_instance = new Tree;
        return s_instance;
    }

Tree::Tree(){
	numOfEvents = 100;
	rootFile = "test.root";
	//sRootFile = rootFile;
}

Tree::Tree(std::string rootFl){

  rootFile = rootFl;
  f = TFile::Open(rootFile.c_str(),"READ");
  if (!f) { return; }
  f->GetObject("BSC_DATA_TREE",t);
  numOfEvents = t->GetEntries();
  //f->Close();

}

void Tree::ReadTree(std::string rootFl, std::string treeName, int rw){
	rootFile = rootFl;
	  fTreeName = treeName;
	  //t = new TTree(treeName.c_str(),"A Tree with STL vectors");
	  if (rw)
	    f = TFile::Open(rootFile.c_str(), "RECREATE");
	  else {
	    f = TFile::Open(rootFile.c_str(), "READ");
	    if (!f) {
	      return;
	    }
	    f->GetObject(treeName.c_str(), t);
	    numOfEvents = t->GetEntries();
	  }
}

Tree::Tree(std::string rootFl, std::string treeName, int rw){

  rootFile = rootFl;
  fTreeName = treeName;
  //t = new TTree(treeName.c_str(),"A Tree with STL vectors");
  if (rw)
    f = TFile::Open(rootFile.c_str(), "RECREATE");
  else {
    f = TFile::Open(rootFile.c_str(), "READ");
    if (!f) {
      return;
    }
    f->GetObject(treeName.c_str(), t);
    numOfEvents = t->GetEntries();
  }
  //f->Close();

}


Tree::~Tree(){ f->Close(); }

void Tree::TreeW(){

	//TFile f(rootFile.c_str(),"RECREATE");
	f = TFile::Open(rootFile.c_str(), "RECREATE");
	Channel ch;
	TTree t(fTreeName.c_str(),"A Tree with STL vectors");
    t.Branch("channel",&ch);

    numOfEvents =10;
    for(int i= 0 ; i < numOfEvents ; i++){
    	ch.clear(); // clearing the vector
    	ch.push_back(1);
    	ch.push_back(8);
   		ch.push_back(3);

    	t.Fill();
    }

    f->Write();
 
}

void Tree::TreeW(std::string branchName, Channel ch){

	t->Branch(branchName.c_str(),&ch);
    t->Fill();
}

void Tree::WriteToFile(){
	f->Write();
}

void Tree::TreeR(){

	Channel *ch;
	f = new TFile(rootFile.c_str());
	t = (TTree*)f->Get("testTree");
	TBranch *b = t->GetBranch("channel");
	t->SetBranchAddress("channel",&ch,&b);
	Int_t nentries = t->GetEntries();
	for(Int_t i = 0 ; i < nentries ; i++){
		b->GetEntry(i);
		std::cout<<"val : "<<ch->at(1)<<std::endl;
	}


}

void Tree::TreeR_V2(std::string bName, int entry){
//TFile *
//   f = TFile::Open(rootFile.c_str(),"READ");
//   if (!f) { return; }
//   f->GetObject("testTree",t);
   Channel *vpx = 0;
   TBranch *bvpx = 0;
   t->SetBranchAddress(bName.c_str(),&vpx,&bvpx);
   Long64_t tentry = t->LoadTree(entry);
   bvpx->GetEntry(tentry);
   for (UInt_t j = 0; j < vpx->size(); ++j) {
         std::cout<<"value : "<<vpx->at(j)<<" , ";         
   }
   std::cout<<std::endl;
   t->ResetBranchAddresses();

}

Channel* Tree::GetEntry(std::string bName, int evNo){

  Channel *vpx = 0;
  TBranch *bvpx = 0;
  t->SetBranchAddress(bName.c_str(),&vpx,&bvpx);
  Long64_t tentry = t->LoadTree(evNo);
  bvpx->GetEntry(tentry);
  t->ResetBranchAddresses();
  return vpx;

}
} /*end of Tomography namespace*/
