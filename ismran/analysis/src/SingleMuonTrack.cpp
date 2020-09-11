/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 07-Sep-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"

int SingleMuonTrack::fMuonTrackNum = 0;

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub
	fTotalEnergyDeposited = 0.;
	fIsValid = false;

}

SingleMuonTrack::~SingleMuonTrack() {
	// TODO Auto-generated destructor stub
}

void SingleMuonTrack::push_back(ScintillatorBar_V2 *hittedBar){
	fSingleMuonTrack.push_back(hittedBar);
}

void SingleMuonTrack::CalculateTotalEnergyDeposited(){
	fTotalEnergyDeposited = 0.;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		//fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMean;
		fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMeanCorrected;
		if(i < (fSingleMuonTrack.size()-1))
			fSingleMuonTrack[i]->pathLength = fSingleMuonTrack[i]->hitPosition.Distance(fSingleMuonTrack[i+1]->hitPosition);
		else
			fSingleMuonTrack[i]->pathLength = 0.;
	}
	//std::cout << "*********** Total Energy Deposited : " << fTotalEnergyDeposited << " **************" << std::endl;
	if(fTotalEnergyDeposited > minEnergyDepositedByMuonTrack && fTotalEnergyDeposited < maxEnergyDepositedByMuonTrack){
		fIsValid = true;
	}
}

bool SingleMuonTrack::CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->barIndex > i2->barIndex);
}

void SingleMuonTrack::Sort(){
	std::sort(fSingleMuonTrack.begin(), fSingleMuonTrack.end(),CompareBarIndexInScintillator);
}

void SingleMuonTrack::clear(){
	fSingleMuonTrack.clear();
}

unsigned long int SingleMuonTrack::size(){
	fSingleMuonTrack.size();
}

void SingleMuonTrack::Print(){
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}

void SingleMuonTrack::PlotTrack(){
	std::vector<Double_t> xvec, zvec;
	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
		//if((singleMuonTrack[index]->hitPosition).x <= 100 && (singleMuonTrack[index]->hitPosition).y <=50){
		xvec.push_back((fSingleMuonTrack[index]->hitPosition).x);
		zvec.push_back((fSingleMuonTrack[index]->hitPosition).z);
		//}
	}
	std::string trackname = "MuonTrack-" + std::to_string(fMuonTrackNum);
	std::cout << "TrackName : " << trackname << " : Size : "
			<< fSingleMuonTrack.size() << std::endl;
	/*for(unsigned int i = 0 ; i< singleMuonTrack.size() ; i++){
	 (singleMuonTrack[i]->hitPosition).Print();
	 }*/
	TCanvas *c = new TCanvas(trackname.c_str(), trackname.c_str(), 800, 800);
	TGraph *gr = new TGraph(xvec.size(), &zvec[0], &xvec[0]);
	gr->SetMarkerStyle(8);
	//gr->SetTitle("Muon Hit Points in different layers");
	DrawGrid(trackname, 10, 10);
	gr->Draw("p");
	fMuonTrackNum++;
}

/*
 * Function to Draw customized Grid Lines
 */
void SingleMuonTrack::DrawGrid(std::string t, Int_t ngx, Int_t ngy)
{
//	std::cout << "DrawGrid Called........: " << t << " : ....." << std::endl;
   //new TCanvas();
   Double_t x1 = -50;
   Double_t x2 = 50;
   Double_t y1 = 0;
   Double_t y2 = 100;
   Double_t xs = (x2-x1)/ngx;
   Double_t ys = (y2-y1)/ngy;
   Int_t i;

   TH1F *h = gPad->DrawFrame(x1, y1, x2, y2);
   h->GetXaxis()->SetNdivisions(20);
   h->GetYaxis()->SetNdivisions(20);
   h->GetYaxis()->SetTickLength(0.);
   h->GetXaxis()->SetTickLength(0.);
   h->GetXaxis()->SetLabelSize(0.025);
   h->GetYaxis()->SetLabelSize(0.025);
   h->SetTitle(t.c_str());
   gPad-> Update();

   TLine l;
   l.SetLineColor(kGray);
   Double_t x = x1+xs;
   for (i = 0; i<ngx-1; i++) {
      l.DrawLine(x,y1,x,y2);
      x = x + xs;
   }

   Double_t y = y1+xs;
   for (i = 0; i<ngy-1; i++) {
      l.DrawLine(x1,y,x2,y);
      y = y +ys;
   }
}

