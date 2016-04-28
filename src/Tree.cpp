#include "Tree.h"
#include <TFile.h>
#include <TBranch.h>
#include <iostream>

Tree::Tree(){
	numOfEvents = 100;
	rootFile = "test.root";
}

Tree::Tree(std::string rootFl){

  rootFile = rootFl;
  f = TFile::Open(rootFile.c_str(),"READ");
  if (!f) { return; }
  f->GetObject("BSC_DATA_TREE",t);
  numOfEvents = t->GetEntries();
  //f->Close();

}

Tree::Tree(std::string rootFl, std::string treeName){

  rootFile = rootFl;
  f = TFile::Open(rootFile.c_str(),"READ");
  if (!f) { return; }
  f->GetObject(treeName.c_str(),t);
  numOfEvents = t->GetEntries();
  //f->Close();

}

Tree::~Tree(){ f->Close(); }

void Tree::TreeW(){

	TFile f(rootFile.c_str(),"RECREATE");
	TTree t("testTree","A Tree with STL vectors");
    //t.Branch("channel",ch, "ch[3]/I");
    t.Branch("channel",&ch);

    for(int i= 0 ; i < numOfEvents ; i++){

    	ch.clear(); // clearing the vector
    	ch.push_back(1);
    	ch.push_back(8);
   		ch.push_back(3);

    	t.Fill();
    }

    f.Write();
 
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
