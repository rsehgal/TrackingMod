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
	  FillBranchNamesVector();
	  Initialize();
	  /*
	  vpx.resize(fBranchNamesVector.size());
	  bvpx.resize(fBranchNamesVector.size());
	  for(int i = 0 ; i<fBranchNamesVector.size() ; i++){
		  t->SetBranchAddress(fBranchNamesVector[i],&vpx[i],&bvpx[i]);
	  }
	  */
}

void Tree::Initialize(){
	for(int i = 0 ; i < fBranchNamesVector.size() ; i++){
		// t->SetBranchAddress(bName.c_str(),&vpx,&bvpx);
		std::string bName = fBranchNamesVector[i];
		std::size_t pos = 0;
		pos = bName.find("NUMBER_OF_CHANNELS");
		//std::cout<<"Position : " << pos << std::endl;
		if(!pos || pos > 20)
		t->SetBranchAddress(fBranchNamesVector[i].c_str(),&fBranchMap[bName].vpx,&fBranchMap[bName].bvpx);
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

/*
 * Previous Version
Channel* Tree::GetEntry(std::string bName, int evNo){

  Channel *vpx = 0;
  TBranch *bvpx = 0;
  t->SetBranchAddress(bName.c_str(),&vpx,&bvpx);
  Long64_t tentry = t->LoadTree(evNo);
  bvpx->GetEntry(tentry);
  t->ResetBranchAddresses();
  return vpx;

}
*/
/*
 * Idea is to Initialize the tree first.
 *
 * Then get the value for particular event using branch name
 */
Channel* Tree::GetEntry(std::string bName, int evNo){

	fBranchMap[bName].tentry = t->LoadTree(evNo);
	fBranchMap[bName].bvpx->GetEntry(fBranchMap[bName].tentry);
	return fBranchMap[bName].vpx;

}

void Tree::FillBranchNamesVector(){
  TObjArray *listOfBranches = t->GetListOfBranches();
  TIter iObj(listOfBranches);
  while (TObject *obj = iObj()) {
	  std::string name = (std::string)obj->GetName();
      fBranchNamesVector.push_back(name);
  }
}

void Tree::PrintBranchNames() {
  for (int i = 0; i < fBranchNamesVector.size(); i++) {
    std::cout << fBranchNamesVector[i] << std::endl;
  }
}
} /*end of Tomography namespace*/
