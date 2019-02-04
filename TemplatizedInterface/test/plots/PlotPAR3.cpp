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

	TCanvas *can = new TCanvas();
    //can->Divide(2,3);

	//TMultiGraph *mg = new TMultiGraph();
	//std::ifstream parHandle("Par.txt");
	std::ifstream parHandle(argv[1]);
	std::vector<double> x, y;
	int n = 0;
	std::string numOfMaterials="";
	parHandle >> numOfMaterials >> n;
	std::cout << numOfMaterials << " : " << n << std::endl;
	int i = 0;

	std::vector<double> yNew;

	std::vector<std::string> materialString;
	for(int i = 0 ; i < n ; i++){
		std::string material;
		parHandle >> material;
		std::cout << material << " ";
		materialString.push_back(material);
	}
	std::cout << std::endl;

	std::vector<double> materialCode;
	for(int i = 1 ; i <= n ; i++ ){
		materialCode.push_back(i);
	}

	int numOfEventsSteps = 0;
	std::string numOfStepsString="";
	parHandle >> numOfStepsString>> numOfEventsSteps;
	std::cout << "numOfStepsString :  " << numOfStepsString << " : " << numOfEventsSteps << std::endl;
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

/*
	std::vector<char*> legends;
	legends.push_back("Aluminium");
	legends.push_back("Iron");
	legends.push_back("Lead");
	legends.push_back("Uranium");
*/

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

			//leg->AddEntry(gr,legends[i],"lp");
			gr->SetMarkerStyle(20+i);
			gr->SetMarkerColor(i+1);
			gr->SetLineColor(i+1);
			mg->Add(gr);
		}

	}

/*
	can->cd(1);
	mg->Draw("alp");
	leg->Draw();
*/

/*
	for(int i = 3 ; i <= 6 ; i++){
		can->cd(i);
		graphsVector[i-3]->Draw("alp");
	}
*/

	graphsVector[0]->Draw("alp");

//	can->cd(2);
//	hist->SetMarkerStyle(20);
//	hist->Draw("E1");

	fApp->Run();
}





