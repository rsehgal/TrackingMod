/*
 * HelperFunctions.cpp
 *
 *  Created on: 16-Sep-2020
 *      Author: rsehgal
 */

#include "HelperFunctions.h"
#include "PsBar.h"
#include "includes.hh"
#include <TMath.h>



std::vector<std::vector<unsigned long int>> myhist2D;


bool CheckRange(std::vector<Point3D*> singleMuonTrack){
	for(unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
		if(std::isinf(singleMuonTrack[i]->x) || std::isinf(singleMuonTrack[i]->y) || std::isinf(singleMuonTrack[i]->z)
			// || (abs((singleMuonTrack[i]->x) <= 45.) ) || (abs((singleMuonTrack[i]->y) <= 45.) ) || (abs((singleMuonTrack[i]->z) <= 50.) )
		){
			return true;
		}
	}
	return false;
}




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

double Pol2(Double_t *x,Double_t *par) {

      double fitval = par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
      return fitval;
}

double Cos2ThetaFit(Double_t *x, Double_t *par){
	  double fitval = par[0]*pow(cos(x[0]),par[1]);
	  return fitval;

}

double NewCos2ThetaFit(Double_t *x, Double_t *par){
	  double fitval = par[0]+par[1]*pow(cos(par[2]*x[0]+par[3]),2);
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

TH1D* PlotZenithAngle(std::vector<std::vector<Point3D*>> muonTrackVec, std::string histTitle){
	TVector3 ref(0.,-1.,0.);
	int numOfBins = 100;
	TH1D *zenithAngleHist ;
	//zenithAngleHist = new TH1D("ZenithAngle", "ZenithAngle Distribution",numOfBins,0.,1.5);
	zenithAngleHist = new TH1D(histTitle.c_str(),histTitle.c_str(),numOfBins,0.,1.39626);
	int invalidCounter = 0 ;
	for(unsigned int trackIndex = 0 ; trackIndex < muonTrackVec.size() ; trackIndex++){
		std::vector<Point3D*> singleMuonTrack = muonTrackVec[trackIndex];
		//std::cout << "============================= Muon Track from : " << histTitle << "===============================" << std::endl;
		/*for (unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
			singleMuonTrack[i]->Print();
		}*/
		Point3D *startPoint = singleMuonTrack[0];
		Point3D *endPoint = singleMuonTrack[singleMuonTrack.size()-1];
		TVector3 muonDir(TVector3(endPoint->x,endPoint->y,endPoint->z)-TVector3(startPoint->x,startPoint->y,startPoint->z));
		double zenitAngle = muonDir.Angle(ref);
		//std::cout <<"Zenith Angle before condition Check for : " << histTitle <<" : " << zenitAngle << std::endl;
		if(zenitAngle > -0.0872665 && zenitAngle < 0.0872665){

		}else{
			/*if(zenitAngle > 0.96)
			{
				invalidCounter++;
				PrintPoint3DVector(singleMuonTrack);
			}else*/
			{
				if(startPoint->y > 35. && endPoint->y < -35){
					//std::cout <<"Zenith Angle for : " << histTitle <<" : " << zenitAngle << std::endl;
					zenithAngleHist->Fill(zenitAngle);
				}
			}
		}
	}
	std::cout << std::endl << "Total number of invalid angle tracks : " << invalidCounter << std::endl;
	//TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
	TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,1.39626);
	//zenithAngleHist->Fit(zenForm,"r");
	//zenithAngleHist->Draw();

	return zenithAngleHist;

}

TH1D* PlotZenithAngle_XY(std::vector<std::vector<Point3D*>> muonTrackVec, std::string histTitle){
	TVector3 ref(0.,-1.,0.);
	int numOfBins = 100;
	TH1D *zenithAngleHist ;
	//zenithAngleHist = new TH1D("ZenithAngle", "ZenithAngle Distribution",numOfBins,0.,1.5);
	zenithAngleHist = new TH1D(histTitle.c_str(),histTitle.c_str(),numOfBins,0.,1.39626);
	int invalidCounter = 0 ;
	for(unsigned int trackIndex = 0 ; trackIndex < muonTrackVec.size() ; trackIndex++){
		std::vector<Point3D*> singleMuonTrack = muonTrackVec[trackIndex];
		//std::cout << "============================= Muon Track from : " << histTitle << "===============================" << std::endl;
		/*for (unsigned int i = 0 ; i < singleMuonTrack.size() ; i++){
			singleMuonTrack[i]->Print();
		}*/
		Point3D *startPoint = singleMuonTrack[0];
		Point3D *endPoint = singleMuonTrack[singleMuonTrack.size()-1];
		TVector3 muonDir(TVector3(endPoint->x,endPoint->y,endPoint->z)-TVector3(startPoint->x,startPoint->y,startPoint->z));
		double zenitAngle = muonDir.Angle(ref);
		//std::cout <<"Zenith Angle before condition Check for : " << histTitle <<" : " << zenitAngle << std::endl;
		if(zenitAngle > -0.0872665 && zenitAngle < 0.0872665){

		}else{
			if(zenitAngle > 0.96){
				invalidCounter++;
				PrintPoint3DVector(singleMuonTrack);
			}else{
				if(startPoint->y > 35. && endPoint->y < -35){
					//std::cout <<"Zenith Angle for : " << histTitle <<" : " << zenitAngle << std::endl;
					//zenithAngleHist->Fill(zenitAngle);
					double zenithAngle = atan(fabs(endPoint->x - startPoint->x)/fabs(endPoint->y - startPoint->y));
					zenithAngleHist->Fill(zenithAngle);
				}
			}
		}
	}
	std::cout << std::endl << "Total number of invalid angle tracks : " << invalidCounter << std::endl;
	//TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
	TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,1.39626);
	//zenithAngleHist->Fit(zenForm,"r");
	//zenithAngleHist->Draw();

	return zenithAngleHist;

}

void PrintPoint3DVector(std::vector<Point3D*> vect){
	std::cout << "=================================================" << std::endl;
	for(unsigned short int i = 0 ; i < vect.size() ; i++){
		vect[i]->Print();
	}
}


HelperFunctions::HelperFunctions() {
	// TODO Auto-generated constructor stub

}

HelperFunctions::~HelperFunctions() {
	// TODO Auto-generated destructor stub
}
