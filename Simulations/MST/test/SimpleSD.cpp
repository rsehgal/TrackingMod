#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>


void GenerateGraph();

int main(int argc, char *argv[]){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
	TH1F  *histogram = new TH1F("SD", "Data", 1000, -0.2, 0.2);
      


    //Stuff of reading from run.txt and filling the histogram
    std::ifstream runfile ("run.txt");
    std::string material="";
    double thickness = 0.;
    double energy = 0.;
    double scatteringAngle = 0.;
    int n = 0;

    int numOfEvents = std::atoi(argv[1]);
    runfile >> energy >> material >> thickness;
    std::cout<<"Energy : " << energy << ":: material : " << material << std::endl;
#if(1)
    while(numOfEvents){
    	runfile >> scatteringAngle;
    	//std::cout << "Scattering Angle : " << scatteringAngle << std::endl;
    	histogram->Fill(scatteringAngle);
    	numOfEvents--;
    }
#endif


    std::cout<<"--- Execution Completed ----" << std::endl;
    double sd = histogram->GetStdDev();
    histogram->Fit("gaus","","",-2*sd, 2*sd);
    TF1 *fit = histogram->GetFunction("gaus");
    double chi2 = fit->GetChisquare();
    double p1 = fit->GetParameter(2);
    std::cout<<"Par2 : " << p1 << std::endl;
   // double e1 = fit->GetParError(1);
    std::cout << "SD : " << sd << std::endl;
    //sd = 0.01704;
//    /sd = 0.0201;


    int bin1 = histogram->FindFirstBinAbove(histogram->GetMaximum()/2);
    int bin2 = histogram->FindLastBinAbove(histogram->GetMaximum()/2);
    double fwhm = histogram->GetBinCenter(bin2) - histogram->GetBinCenter(bin1);
    std::cout<<"FWHM for histogram : " << n <<" : " << fwhm <<std::endl;

    //sd = fwhm;
    //sd = .0092901;
    //double radLen = 0.0136*0.0136*10./(sd*sd);
    double radLen = ((15.*15.)/(3000*3000))*(10/(p1*p1));//(fwhm*fwhm);
    //double radLen = ((13.6*13.6)/(3000*3000))*(10/(fwhm*fwhm));
    std::cout << "RadLength : " << radLen << std::endl;
    histogram->Draw();
    fApp->Run();

	return 0;
}

void GenerateGraph(){
    std::string energy="";
    std::string material="";
    double thickness = 0.;
    double stdDev = 0.;
    std::ifstream sd;
    sd.open("sd.txt");
    int numOfSteps = 8;
    TCanvas *cmat = new TCanvas("cmat", "SD", 800, 600);
    cmat->Divide(1,1);
    cmat->cd(1);
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("SD w.r.t thickness");

    TGraph *mat1 = new TGraph();
  mat1->SetTitle("Fe");
  mat1->SetMarkerStyle(20);
  mat1->SetMarkerColor(kRed);
  mat1->SetLineWidth(2);

  int pNo = 0;
  int numOfMaterials = 2;
  //while(numOfMaterials){
    numOfSteps = 8;
    while(numOfSteps){
        pNo++;
        sd >> energy >> material >> thickness >> stdDev ;
        std::cout<< stdDev << std::endl;
        mat1->SetPoint(pNo,thickness,stdDev);
        numOfSteps--;
    }
   // numOfMaterials--;
  //}
    std::cout<<"--------------------------------"<<std::endl;

TGraph *mat2 = new TGraph();
  mat2->SetTitle("Pb");
  mat2->SetMarkerStyle(21);
  mat2->SetMarkerColor(kGreen);
  mat2->SetLineWidth(2);
numOfSteps = 8;
    while(numOfSteps){
        pNo++;
       sd >> energy >> material >> thickness >> stdDev ;
       std::cout<< stdDev << std::endl;
        mat2->SetPoint(pNo,thickness,stdDev);
        numOfSteps--;
    }

sd.close();
  //mat1->GetXaxis()->SetRangeUser(8000, 11000);
  
  mg->Add(mat1);
  mg->Add(mat2);
  mg->Draw("AP");
}
