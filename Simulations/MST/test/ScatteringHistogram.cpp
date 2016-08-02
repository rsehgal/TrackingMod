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
	c->Divide(2,2);
	std::vector<TH1F*> histogram;
	int numOfEnergies = 2;
	int numOfMaterials = 4;
	int nxbins = 80;
	
 	histogram.resize(numOfEnergies*numOfMaterials);
 	for(int i = 0 ; i < histogram.size() ; i++){
 		std::stringstream ss;
 		ss << i;
 		//std::cout<<"ss.str() : " << ss.str() << std::endl;
 		std::string str = "hist";
 		str += ss.str();
 		//std::cout<< "STR : " << str <<std::endl;
	  histogram[i] = new TH1F(str.c_str(), "Data", nxbins, -50, 150);
      
    }


    //Stuff of reading from run.txt and filling the histogram
    std::ifstream runfile ("run.txt");
    std::string material="";
    double energy = 0.;
    double scatteringAngle = 0.;
    int n = 0;
    
    while(n!=4){
    	c->cd(n+1);
    int numOfEvents = std::atoi(argv[1]);
    runfile >> energy >> material;
    std::cout<<"Energy : " << energy << ":: material : " << material << std::endl;
    while(numOfEvents){
    	runfile >> scatteringAngle;
    	histogram[n]->Fill(scatteringAngle*1000.);
    	numOfEvents--;
    }
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