/*
 * HelperFunctions.cpp
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#include "HelperFunctions.h"
#include "PsBar.h"
#include "includes.hh"


std::vector<std::vector<unsigned long int>> myhist2D;
void HistInitializer(){
for(unsigned int i = 0 ; i < numOfLayers ; i++){
	std::vector<unsigned long int> yvec;
	for(unsigned int j = 0 ; j < numOfBarsInEachLayer ; j++){
		yvec.push_back(0);
	}
	myhist2D.push_back(yvec);
}
}

void Fill2DHist(unsigned int x, unsigned int y){
	/*unsigned long int count = myhist2D[x][y];
	count++;
	myhist2D[x][y] = count;*/
	myhist2D[x][y]++;

}

Tracking::Vector3D<double> ConvertToTomoVector3D(Point3D pt){
	return Tracking::Vector3D<double>(pt.x,pt.y,pt.z);
}

/*
template<typename T>
void ResetVector(std::vector<T*> vecToReset){
	if(vecToReset.size()!=0){
		for(unsigned int i = 0 ; i < vecToReset.size() ; i++){
			delete vecToReset[i];
		}
	}
	vecToReset.clear();
}
*/

double LinearFit(Double_t *x,Double_t *par) {

      double fitval = par[0] + par[1]*x[0];
      return fitval;
}


//std::vector<double> GetFittedXorZ(TGraph *gr){
void PlotFittedLine(TGraph *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"rq");
	double c = formula->GetParameter(0);
	double m = formula->GetParameter(1);
	delete formula;

	/*std::vector<double> vecOfEstimatedXorZ;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		vecOfEstimatedXorZ.push_back( ((fSingleMuonTrack[i]->hitPosition).y - c)/m );
	}
	return vecOfEstimatedXorZ;*/

}

void DrawGrid(std::string t, Int_t ngx, Int_t ngy)
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

HelperFunctions::HelperFunctions() {
	// TODO Auto-generated constructor stub

}

HelperFunctions::~HelperFunctions() {
	// TODO Auto-generated destructor stub
}

