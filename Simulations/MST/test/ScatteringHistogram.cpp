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
	//c->Divide(1,1);
	std::vector<TH1F*> histogram;
	int numOfEnergies = atoi(argv[2]);
	int numOfMaterials = atoi(argv[3]);
	int numOfThicnesses = atoi(argv[4]);
	int totalNumOfHist = numOfEnergies*numOfMaterials*numOfThicnesses;
	int columns = 4;
        int rows = totalNumOfHist/columns;
    std::cout<<"Rows : " << rows <<" : Columns : " << columns << std::endl;
	if(totalNumOfHist%columns)
	  columns++;
	c->Divide(rows,columns);
	int nxbins = 1010;
	

 	histogram.resize(numOfEnergies*numOfMaterials*numOfThicnesses);
 	for(int i = 0 ; i < histogram.size() ; i++){
 		std::stringstream ss;
 		ss << i;
 		//std::cout<<"ss.str() : " << ss.str() << std::endl;
 		std::string str = "hist";
 		str += ss.str();
 		//std::cout<< "STR : " << str <<std::endl;
	  histogram[i] = new TH1F(str.c_str(), "Data", nxbins, -10, 1000);
      
    }


    //Stuff of reading from run.txt and filling the histogram
    std::ifstream runfile ("run.txt");
    std::string material="";
    double energy = 0.;
    double scatteringAngle = 0.;
    int thickness = 0;
    int n = 0;
    
    while(n!=totalNumOfHist){
	std::stringstream ss;
    	c->cd(n+1);
    int numOfEvents = std::atoi(argv[1]);
    runfile >> energy >> material >> thickness;
    ss << energy/1000.;
    std::string title = material+"_"+std::to_string(thickness)+"mm_"+ss.str()+"GeV";
    histogram[n]->SetTitle(title.c_str());
    histogram[n]->SetName(title.c_str());
    
    std::cout<<"Energy : " << energy << ":: material : " << material << std::endl;
    while(numOfEvents){
    	runfile >> scatteringAngle;
//    	/if(scatteringAngle < 10.)
    		histogram[n]->Fill(scatteringAngle*1000.);
    	numOfEvents--;
    }
    int bin1 = histogram[n]->FindFirstBinAbove(histogram[n]->GetMaximum()/2);
    int bin2 = histogram[n]->FindLastBinAbove(histogram[n]->GetMaximum()/2);
    double fwhm = histogram[n]->GetBinCenter(bin2) - histogram[n]->GetBinCenter(bin1);
    std::cout<<"FWHM for histogram : " << n <<" : " << fwhm <<std::endl;

    histogram[n]->Fit("gaus");
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
