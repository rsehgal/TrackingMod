/*
 * TestEfficiencyPlot.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: rsehgal
 */

#include <iostream>
#include <TApplication.h>
#include "Tree.h"
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <string>
#include <fstream>
#include "DetectorMapping.h"
//typedef Tomography::Properties Detector;
//using namespace Tomography;

struct Data{
	std::string sDetectorName;
	std::vector<double> sVoltage;
	std::vector<double> sTemperature;
	std::vector<double> sPressure;
	std::vector<double> sHumidity;
	std::vector<double> sEffTopOn;
	std::vector<double> sEffBottomOn;
	std::vector<double> sEffBothOn;
};

class EfficiencyGraph{
	TCanvas *fCanvas;
	TMultiGraph *fMg;
	TGraph *fBothOn;
	TGraph *fTopOn;
	TGraph *fBottomOn;
	std::string fName;
	std::string fFileName;
	int fNumOfSteps;

	bool fVerbose;


public:
	Data fData;

	EfficiencyGraph(){

	}

	EfficiencyGraph(std::string name){
		fVerbose = false;
		fNumOfSteps = 18;
		fName = name;
		fFileName = name;
		fCanvas = new TCanvas(fName.c_str(), fName.c_str(), 800, 600);
		fMg = new TMultiGraph();
		fTopOn = new TGraph();
		fBottomOn = new TGraph();
		fBothOn = new TGraph();

		fBothOn->SetTitle("w.r.t both");
		fBothOn->SetMarkerStyle(21);
		fBothOn->SetLineColor(kRed);
		fBothOn->SetLineWidth(2);
		fBothOn->GetXaxis()->SetRangeUser(8000, 11000);

		fTopOn->SetTitle("w.r.t TOP");
		fTopOn->SetMarkerStyle(21);
		fTopOn->SetLineColor(kGreen);
		fTopOn->SetLineWidth(2);
		fTopOn->GetXaxis()->SetRangeUser(8000, 11000);

		fBottomOn->SetTitle("w.r.t BOTTOM");
		fBottomOn->SetMarkerStyle(21);
		fBottomOn->SetLineColor(kBlue);
		fBottomOn->SetLineWidth(2);
		fBottomOn->GetXaxis()->SetRangeUser(8000, 11000);

		fMg->Add(fTopOn);
		fMg->Add(fBottomOn);
		fMg->Add(fBothOn);


	}

	TGraph* GetTopOnGraph() const {return fTopOn;}
	TGraph* GetBottomOnGraph() const {return fBottomOn;}
	TGraph* GetBothOnGraph() const {return fBothOn;}
	TCanvas* GetCanvas() const {return fCanvas;}
	int GetNumOfSteps()const {return fNumOfSteps;}

	void Draw(){
		fMg->Draw("ACP");
		fCanvas->Modified();
		fCanvas->Update();
		fCanvas->SaveAs((fName+".gif").c_str());

	}

	void cd(){
		fCanvas->cd();
	}

	void ReadFile(){
		std::ifstream infile(fFileName+".txt");
		std::string detectorName;
		double voltage = 0., temperature = 0.,pressure=0., humidity = 0., efftopon = 0., effbottomon = 0., effbothon = 0.;

		for(int i = 0 ; i < fNumOfSteps ; i++){
			infile >> detectorName >> voltage >> temperature >> pressure >> humidity
				   >> efftopon >> effbottomon >> effbothon ;
			if(fVerbose)
				std::cout << detectorName << "  " << voltage << "  " <<  temperature <<"  " <<  pressure <<"  " <<  humidity
							   << "  " << efftopon << "  " << effbottomon << "  " << effbothon << std::endl;

			fData.sVoltage.push_back(voltage);
			fData.sTemperature.push_back(temperature);
			fData.sPressure.push_back(pressure);
			fData.sHumidity.push_back(humidity);
			fData.sEffTopOn.push_back(efftopon);
			fData.sEffBottomOn.push_back(effbottomon);
			fData.sEffBothOn.push_back(effbothon);
		}
		fData.sDetectorName = detectorName;
		infile.close();

	}

};

int main(int argc, char *argv[]) {
  TApplication *app = new TApplication("Random", NULL, NULL);
/*

  TF1 *sigmoid = new TF1("sigmoid"," [0]/(1 + exp( [1] * ([2] - x) ) )");
  sigmoid->SetParNames("Efficiency","Slope","HV @ 50 efficiency");
  sigmoid->SetParameters(80.0, 0.01, 11000.0);
  sigmoid->SetLineColor(2);
*/

  Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
  //std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
  std::vector<Tomography::Mapping::Detector*> detectorVector = detectorMap->GetDetectorVector();
  //for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
  for(int i = 0 ; i < detectorVector.size()-1; i++){
  EfficiencyGraph *effGr = new EfficiencyGraph(detectorVector[i]->sDetectorName);
  effGr->ReadFile();
  effGr->cd();
  std::cout<<"========================================================" << std::endl;
  for(int i = 0 ; i < effGr->GetNumOfSteps() ; i++ ){
	  effGr->GetTopOnGraph()->SetPoint(i, effGr->fData.sVoltage[i], effGr->fData.sEffTopOn[i]);
	  effGr->GetBottomOnGraph()->SetPoint(i,effGr->fData.sVoltage[i], effGr->fData.sEffBottomOn[i]);
	  effGr->GetBothOnGraph()->SetPoint(i, effGr->fData.sVoltage[i], effGr->fData.sEffBothOn[i]);
	  //std::cout<< effGr->fData.sEffBothOn[i] << std::endl;

  }
  effGr->Draw();
  delete effGr;
  }




/*

  //TCanvas *c2 = new TCanvas();
  //effGr2->GetCanvas()->cd();
  effGr2->cd();
  for(int i = 0 ; i < 20 ; i++ ){
  	  effGr2->GetBothOnGraph()->SetPoint(i, x[i], y[i]+10);
  	  effGr2->GetTopOnGraph()->SetPoint(i, x[i], y[i]-10);
  	  effGr2->GetBottomOnGraph()->SetPoint(i, x[i], y[i]-20);
    }
  //effGr2->Draw(c2);
  effGr2->Draw();
*/

/*
  both_on1->Fit("sigmoid","EM");
  TF1 *fit = both_on1->GetFunction("sigmoid");
  mg1->Add(both_on1);
  mg1->Draw();
*/

/*
  both_on1->Draw();
  c1->Update();
  c1->Modified();
*/

  app->Run();

  return 0;
}





