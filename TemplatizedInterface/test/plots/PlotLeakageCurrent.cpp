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

	TH1F *hist = new TH1F("Hist","Hist",8,0,8);

	std::ifstream parHandle(argv[1]);
	std::vector<double> x, y;
	int n = 29;
	std::string voltage="";
	std::vector<double>  voltageVal;
	std::string topgap="";
	std::vector<double> topGapVal;
	std::string bottomgap="";
	std::vector<double> bottomGapVal;

	parHandle >> voltage >> topgap >> bottomgap;

	for(int i = 0 ; i < n ; i++){
		double voltageval = 0., topval = 0., bottomval = 0.;
		parHandle >> voltageval >> topval >> bottomval;
		voltageVal.push_back(voltageval);
		topGapVal.push_back(topval);
		bottomGapVal.push_back(bottomval);
	}
	std::cout << std::endl;

	TMultiGraph *mg = new TMultiGraph();
	TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
	leg->SetFillColor(0); leg->SetHeader("Leakage Current");



	std::vector<char*> legends;
	legends.push_back("Top Gap");
	legends.push_back("Bottom Gap");

	TGraph *grTop = new TGraph(voltageVal.size(), &voltageVal[0],&topGapVal[0]);
	TGraph *grBottom = new TGraph(voltageVal.size(), &voltageVal[0],&bottomGapVal[0]);

	leg->AddEntry(grTop, legends[0], "lp");
	leg->AddEntry(grBottom, legends[1], "lp");

	grTop->SetMarkerStyle(20);
	grTop->SetMarkerColor(1);
	grTop->SetLineColor(1);

	grBottom->SetMarkerStyle(21);
	grBottom->SetMarkerColor(2);
	grBottom->SetLineColor(2);

	mg->Add(grTop);
	mg->Add(grBottom);
	mg->Draw("acp");
	leg->Draw();
	/*
	std::vector<char*> legends;
	legends.push_back("Aluminium");
	legends.push_back("Iron");
	legends.push_back("Lead");
	legends.push_back("Uranium");


	std::vector<double> parVector;
	std::vector<double> errorVector;
	std::vector<double> ex;

	std::vector<TGraphErrors*> graphsVector;
	//n = 1;
	for(int i = 0 ; i < n ; i++){

		{	parVector.clear();
			errorVector.clear();
			ex.clear();
			for(int j = 0 ; j < numOfEventsSteps ; j++){
				double parVal = 0. , errorVal = 0.;
				parHandle >> parVal >> errorVal;
				std::cout << "PAR : " << parVal << " :: Error : " << errorVal << std::endl;
				parVector.push_back(parVal);
				errorVector.push_back(errorVal);
				ex.push_back(0.);
				//hist->SetBinContent(j+1,parVal);
			}

			//TGraph *gr = new TGraph(numOfEventsVector.size(),&numOfEventsVector[0],&parVector[0]);
			TGraphErrors *gr = new TGraphErrors(numOfEventsVector.size(),&numOfEventsVector[0],&parVector[0],&ex[0],&errorVector[0]);
			graphsVector.push_back(gr);

			leg->AddEntry(gr,legends[i],"lp");
			gr->SetMarkerStyle(20+i);
			gr->SetMarkerColor(i+1);
			gr->SetLineColor(i+1);
			mg->Add(gr);
		}

	}


	can->cd(1);
	mg->Draw("alp");
	leg->Draw();

	for(int i = 3 ; i <= 6 ; i++){
		can->cd(i);
		graphsVector[i-3]->Draw("alp");
	}

//	can->cd(2);
//	hist->SetMarkerStyle(20);
//	hist->Draw("E1");
*/


	fApp->Run();
}





