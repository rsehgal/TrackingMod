#include <TH1F.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include "DetectorMapping.h"

int main(){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
    TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
    Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
    int numOfRpcs = detectorMap->GetNumOfDetectors("GLASS");
    std::cout << "Num Of Rpc :  "<< numOfRpcs << std::endl;
    int cols = 4;
    int rows = numOfRpcs / cols;
    if(numOfRpcs % cols)
        rows++;
    c->Divide(cols,rows);

    std::vector<TH1F*> histVector;
    
    int xmin = -35;
    int xmax = 35;
    int numOfBins = xmax - xmin;

    for(int i = 0 ;  i < numOfRpcs ; i++){
        std::string rpcName = "RPC-"+std::to_string(i+1);
        histVector.push_back(new TH1F( rpcName.c_str(), rpcName.c_str(), numOfBins , -35, 35 ));
    }

    std::ifstream infile("ActualAndFittedHits.txt");
    while(!infile.eof()){
        int counter = numOfRpcs;
        double actHitX = 0.,actHitY = 0.,actHitZ = 0.;
        double fittedHitX = 0.,fittedHitY = 0.,fittedHitZ = 0.;
        int i = 0;
        while(counter){
            infile >> actHitX >> actHitY >> actHitZ
                   >> fittedHitX >> fittedHitY >> fittedHitZ;
            double deviation = fittedHitX - actHitX;
            histVector[i]->Fill(deviation);
            i++;
            counter--;
        }
    }

    for(int i = 0 ; i < numOfRpcs ; i++){
        c->cd(i+1);
        histVector[i]->Draw();
    }
    fApp->Run();
}
