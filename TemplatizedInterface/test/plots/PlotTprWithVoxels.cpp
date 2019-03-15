/*
 * PlotPAR.cpp
 *
 *  Created on: 05-Nov-2018
 *      Author: rsehgal
 */

#include <fstream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TApplication.h>
#include <cstring>
#include <TCanvas.h>
#include <TAxis.h>
#include <vector>
#include <TLegend.h>
#include <TH1F.h>
#include <TGraphErrors.h>

int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);

//	TH1F *hist = new TH1F("Hist","Hist",8,0,8);

//	TCanvas *can = new TCanvas();
	std::ifstream parHandle(argv[1]);

/*
	TMultiGraph *mg = new TMultiGraph();
	TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
	leg->SetFillColor(0); leg->SetHeader("PoCA Accuray Ratio");
*/

	std::string numOfVoxelsS="";
	std::string tprS="";
	std::string errorS="";
	parHandle >> numOfVoxelsS >> tprS >> errorS;

	std::vector<double> tprVector;
	std::vector<double> errorVector;
	std::vector<double> xVector;
	std::vector<double> ex;

	std::vector<TGraphErrors*> graphsVector;

	while(!parHandle.eof()){

		double x, tpr, err;
		parHandle >> x >> tpr >> err;
		xVector.push_back(x);
		tprVector.push_back(tpr);
		errorVector.push_back(err);
		ex.push_back(0.);
	}

	TGraphErrors *gr = new TGraphErrors(xVector.size(),&xVector[0],&tprVector[0],&ex[0],&errorVector[0]);
	gr->SetMarkerStyle(20);
	gr->SetMarkerColor(1);
	gr->SetLineColor(1);

	gr->Draw("alp");

	fApp->Run();
}





