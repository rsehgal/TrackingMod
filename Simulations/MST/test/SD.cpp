#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>

int main(int argc, char *argv[]){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
	c->Divide(4,4);
	std::vector<TH1F*> histogram;
	int numOfEnergies = 3;
	int numOfMaterials = 2;
    int numOfSteps = 8;
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



    std::cout<<"--- Execution Completed ----" << std::endl;
	fApp->Run();

	return 0;
}
