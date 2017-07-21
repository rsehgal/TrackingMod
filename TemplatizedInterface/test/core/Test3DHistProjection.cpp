/*
 * Test3DHistProjection.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: rsehgal
 */


#include <iostream>
#include <TH3F.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TRandom.h>

int main(int argc, char **argv){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	int xbins = 10;
	int ybins = 10;
	int zbins = 10;
	int xlow = 0, xhigh = 10.;
	int ylow = 0, yhigh = 10.;
	int zlow = 0, zhigh = 10.;
	TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
	c->Divide(4,2);
	TH3F *hist = new TH3F("Test3DHist","Test3DHist",xbins,xlow,xhigh,ybins,ylow,yhigh,zbins,zlow,zhigh);
	for(int i = 0 ; i < 10 ; i++){
		for(int j = 0 ; j < 10; j++){
			for(int k = 0 ; k < 10 ; k++){
				hist->Fill(2,3,4);
				//hist->Fill(2,3,8);
			}
		}
	}


	TRandom t;
	TH3F *histValue = new TH3F("Test3DHistvalue","Test3DHistvalue",xbins,xlow,xhigh,ybins,ylow,yhigh,zbins,zlow,zhigh);
	int count = 0;
	float sum = 0.;
	for(int i = 0 ; i < 2 ; i++){
			for(int j = 0 ; j < 3; j++){
				for(int k = 0 ; k < 3 ; k++){
					count++;
					float val = t.Uniform(1,1010);
					std::cout<<"Count : " << count <<" : Val : "<< val << std::endl;
					sum += val;
					histValue->Fill(2,3,4,val);
					//hist->Fill(2,3,8);
				}
			}
		}

	std::cout<<"Sum : " << sum <<" : Average : " << (sum/1000) << std::endl;



	c->cd(1);
	hist->Draw();
	c->cd(2);
	hist->Project3D("x")->Draw();
	c->cd(3);
	hist->Project3D("y")->Draw();
	c->cd(4);
	hist->Project3D("yz")->Draw("0lego1 PFC");
	c->cd(5);
	histValue->Draw();
	histValue->Divide(hist);
	c->cd(6);
	histValue->Draw();

	sum = 0.;
	count = 0;
	TH2F *hist2DValue = new TH2F("Test2DHistvalue","Test2DHistvalue",xbins,xlow,xhigh,ybins,ylow,yhigh);
	for(int j = 0 ; j < 3; j++){
					for(int k = 0 ; k < 3 ; k++){
						count++;
						float val = t.Uniform(1,1010);
						std::cout<<"Count : " << count <<" : Val : "<< val << std::endl;
						sum += val;
						hist2DValue->Fill(2,3,val);
						//hist->Fill(2,3,8);
					}
				}

	std::cout<<"Sum : " << sum <<" : Average : " << (sum/count) << std::endl;

	c->cd(7);
	hist2DValue->Draw();

	fApp->Run();

}

