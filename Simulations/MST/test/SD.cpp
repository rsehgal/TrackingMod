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
	c->Divide(4,4);
	std::vector<TH1F*> histogram;
	int numOfEnergies = 1;
	int numOfMaterials = 1;
    int numOfSteps = 1;
	int nxbins = 80;
	
 	histogram.resize(numOfSteps*numOfMaterials);
 	for(int i = 0 ; i < histogram.size() ; i++){
 		std::stringstream ss;
 		ss << i;
 		//std::cout<<"ss.str() : " << ss.str() << std::endl;
 		std::string str = "hist";
 		str += ss.str();
 		//std::cout<< "STR : " << str <<std::endl;
	  histogram[i] = new TH1F(str.c_str(), "Data", nxbins, -50, 250);
      
    }


    //Stuff of reading from run.txt and filling the histogram
    std::ifstream runfile ("run.txt");
    std::string material="";
    double thickness = 0.;
    double energy = 0.;
    double scatteringAngle = 0.;
    int n = 0;

    //Opening a file to store Standard deviation for histograms
    std::ofstream sd;
    sd.open("sd.txt",std::ios::app);
    
    
    while(n!=numOfSteps*numOfMaterials){
	std::stringstream ss,tk;
    	c->cd(n+1);
    int numOfEvents = std::atoi(argv[1]);
    runfile >> energy >> material >> thickness;
    ss << energy/1000.;
    tk << thickness;
    std::string title = material+"_"+tk.str()+"_mm_"+ss.str()+"GeV";
    histogram[n]->SetTitle(title.c_str());
    histogram[n]->SetName(title.c_str());

    sd << ss.str()+"GeV" << " " << material << " " << thickness/10. << " " ;
    
    std::cout<<"Energy : " << energy << ":: material : " << material << std::endl;
    while(numOfEvents){
    	runfile >> scatteringAngle;
    	histogram[n]->Fill(scatteringAngle*1000.);

    	numOfEvents--;
    }

    sd << histogram[n]->GetStdDev() << std::endl;
    //if(n==0)
    histogram[n]->Draw();
    //else	
    //histogram[n]->Draw("same");

	std::cout<<"Material Num : " << n << std::endl;
    n++;
    }


    sd.close();
    std::cout<<"--- Execution Completed ----" << std::endl;
    GenerateGraph();
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
