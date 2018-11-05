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
int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	TMultiGraph *mg = new TMultiGraph();
	std::ifstream parHandle("Par.txt");
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

	std::vector<double> parVector;
	for(int i = 0 ; i < n ; i++){
		double parValue;
		parHandle >> parValue;
		parVector.push_back(parValue);
	}

	TGraph *grNew = new TGraph(materialCode.size(),&materialCode[0],&parVector[0]);
	grNew->SetMarkerStyle(kStar);
	TAxis *xax = grNew->GetXaxis();
	i = 1;
	while (i < xax->GetXmax()) {
		int bin_index = xax->FindBin(i);
		xax->SetBinLabel(bin_index, materialString[i - 1].c_str());
		i++;
	}

	grNew->Draw("AP");
	fApp->Run();
}





