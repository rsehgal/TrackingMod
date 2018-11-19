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
int main(int argc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);

	//TMultiGraph *mg = new TMultiGraph();
	//std::ifstream parHandle("Par.txt");
	std::ifstream parHandle(argv[1]);
	std::vector<double> x, y;
	int n = 0;
	std::string numOfMaterials="";
	parHandle >> numOfMaterials >> n;
	int i = 0;

	std::vector<double> yNew;

	std::vector<std::string> materialString;
	for(int i = 0 ; i < n ; i++){
		std::string material;
		parHandle >> material;
		materialString.push_back(material);
	}

	std::vector<double> materialCode;
	for(int i = 1 ; i <= n ; i++ ){
		materialCode.push_back(i);
	}

	int numOfEventsSteps = 0;
	std::string numOfEventsString="";
	parHandle >> numOfEventsString>> numOfEventsSteps;
	std::cout << "numOfEventsString :  " << numOfEventsString << " : " << numOfEventsSteps << std::endl;
	std::vector<double> numOfEventsVector;

	{
		for(int i = 0 ; i < numOfEventsSteps ; i++){
			double numOfEvents = 0 ;
			parHandle >> numOfEvents;
			std::cout << "NumOfEvents : " << numOfEvents << std::endl;
			numOfEventsVector.push_back(numOfEvents);
		}
	}


	TMultiGraph *mg = new TMultiGraph();
	TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
	leg->SetFillColor(0); leg->SetHeader("PoCA Accuray Ratio");
//	leg->AddEntry(gr1, "graph 1", "lp"); leg->AddEntry(gr2, "graph 2", "lp"); leg->AddEntry(gr3, "graph 3", "lp"); leg->Draw();

	std::vector<char*> legends;
	legends.push_back("Aluminium");
	legends.push_back("Iron");
	legends.push_back("Lead");
	legends.push_back("Uranium");

	std::vector<double> parVector;

	for(int i = 0 ; i < n ; i++){

		{	parVector.clear();
			for(int i = 0 ; i < numOfEventsSteps ; i++){
				double parVal = 0. ;
				parHandle >> parVal;
				parVector.push_back(parVal);
			}

			TGraph *gr = new TGraph(numOfEventsVector.size(),&numOfEventsVector[0],&parVector[0]);
			leg->AddEntry(gr,legends[i],"lp");
			gr->SetMarkerStyle(20+i);
			gr->SetMarkerColor(i+1);
			gr->SetLineColor(i+1);
			mg->Add(gr);
		}

	}

	mg->Draw("alp");
	leg->Draw();


	fApp->Run();
}




