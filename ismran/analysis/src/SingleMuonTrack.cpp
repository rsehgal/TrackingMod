/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 07-Sep-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"

int SingleMuonTrack::fMuonTrackNum = 0;

double LinearFit(Double_t *x,Double_t *par) {

      double fitval = par[0] + par[1]*x[0];
      return fitval;
}

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
	return fSingleMuonTrack.size();
}

void SingleMuonTrack::Print(){
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}


std::vector<double> SingleMuonTrack::GetFittedXorZ(TGraph *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"r");
	double c = formula->GetParameter(0);
	double m = formula->GetParameter(1);
	delete formula;

	std::vector<double> vecOfEstimatedXorZ;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		vecOfEstimatedXorZ.push_back( ((fSingleMuonTrack[i]->hitPosition).y - c)/m );
	}
	return vecOfEstimatedXorZ;
}
/*
std::vector<double> SingleMuonTrack::GetFittedZ(TGraph *grzy){
	TF1 *formulaZY = new TF1("FormulaZY",LinearFit,-45,45,2);
	grzy->Fit(formulaZY,"r");
	double c1 = formulaZY->GetParameter(0);
	double m1 = formulaZY->GetParameter(1);

}
*/
std::vector<Point3D*> SingleMuonTrack::PlotTrack(){

	//TMultiGraph *mg = new TMultiGraph();

	std::vector<Double_t> xvec, yvec, zvec;
	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
		//if((singleMuonTrack[index]->hitPosition).x <= 100 && (singleMuonTrack[index]->hitPosition).y <=50){
		xvec.push_back((fSingleMuonTrack[index]->hitPosition).x);
		yvec.push_back((fSingleMuonTrack[index]->hitPosition).y);
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
	c->Divide(2,2);
	//TGraph *gr = new TGraph(xvec.size(), &zvec[0], &xvec[0]);
	c->cd(1);
	TGraph *grxy = new TGraph(xvec.size(), &xvec[0], &yvec[0]);
	grxy->SetMarkerStyle(8);
	grxy->SetTitle("Muon Track in XY plane; X axis ; Y axis");
	DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
	grxy->Draw("p");
	//mg->Add(grxy);

	c->cd(2);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	TGraph *grxFitted = new TGraph(fittedX.size(), &fittedX[0], &yvec[0]);
	grxFitted->SetMarkerStyle(8);
	grxFitted->SetTitle("Fitted Muon Track in XY plane; X axis ; Y axis");
	grxFitted->SetMarkerColor(2);
	DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
	grxFitted->Draw("p");


	c->cd(3);
	TGraph *grzy = new TGraph(zvec.size(), &zvec[0], &yvec[0]);
	grzy->SetMarkerStyle(8);
	grzy->SetTitle("Muon Track in ZY plane; Z axis ; Y axis");
	DrawGrid("Muon Track in ZY plane;Z axis ; Y axis", 9, 9);
	grzy->Draw("p");

	c->cd(4);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);
	TGraph *grzFitted = new TGraph(fittedZ.size(), &fittedZ[0], &yvec[0]);
	grzFitted->SetMarkerStyle(8);
	grzFitted->SetTitle("Fitted Muon Track in ZY plane; Z axis ; Y axis");
	grzFitted->SetMarkerColor(2);
	DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
	grzFitted->Draw("p");

	for (unsigned int i = 0; i < fSingleMuonTrack.size(); i++) {
		fSingleMuonTrackFitted.push_back(new Point3D(fittedX[i],yvec[i],fittedZ[i]));
	}

	/*c->cd(3);
	TGraph *grzx = new TGraph(zvec.size(), &zvec[0], &xvec[0]);
	grzx->SetMarkerStyle(8);
	grzx->SetTitle("Muon Track in ZX plane; Z axis; X axis");
	DrawGrid("Muon Track in ZX plane; Z axis; X axis", 9, 9);
	grzx->Draw("p");*/

	fMuonTrackNum++;
	return fSingleMuonTrackFitted;
}

/*
 * Function to Draw customized Grid Lines
 */
void SingleMuonTrack::DrawGrid(std::string t, Int_t ngx, Int_t ngy)
{
//	std::cout << "DrawGrid Called........: " << t << " : ....." << std::endl;
   //new TCanvas();
   Double_t x1 = -45;
   Double_t x2 = 45;
   Double_t y1 = -45;
   Double_t y2 = 45;
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
