/*
 * RadioGraph.cpp
 *
 *  Created on: Jan 10, 2018
 *      Author: rsehgal
 *
 *  This program read the data from a file (x,y)
 *  and plot 2D histogram using different drawing option
 *  this results in radiograph of detector
 */


#include <TApplication.h>
#include <TStyle.h>
#include <TH2F.h>
#include <iostream>
#include <fstream>
#include <TCanvas.h>

int main(int argc, char *argv[]) {

	TApplication *fApp = new TApplication("Test", NULL, NULL);

	gStyle->SetPalette(1);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);

	TCanvas *c1 = new TCanvas("c1","earth_projections",700,700);
	c1->Divide(2,2);

	int nbins =32;
	TH2F *hlego2 = new TH2F("ha","Lego2",    nbins, -50, 50 , nbins, -50, 50);
	TH2F *hcolz = new TH2F("ha","COLZ", nbins   , -50, 50 , nbins, -50, 50);
	TH2F *haitoff = new TH2F("ha","Aitoff",    nbins, -50, 50 , nbins, -50, 50);
	TH2F *hcont4 = new TH2F("ha","CONT4",    nbins, -50, 50 , nbins, -50, 50);

	std::ifstream in("earth.dat");

	float x,y,z;
	while (1) {
		in >> x >> y ;
		if (!in.good()) break;
		int binNum = x*nbins+y;
		//if(hprof2d->GetBinEntries(binNum) < 2400)
		//if(NEW IF CONDITION USING OUR NEW HISTOGRAM CLASSES)
		{
		  hlego2->Fill(x,y);
		  hcolz->Fill(x,y);
		  haitoff->Fill(x,y);
		  hcont4->Fill(x,y);
		}
	}

	in.close();
	c1->cd(1); hlego2->Draw("lego2");
	c1->cd(2); hcolz->Draw("COLZ");
	c1->cd(3); haitoff->Draw("aitoff");
	c1->cd(4); hcont4->Draw("CONT4");

	fApp->Run();

}





