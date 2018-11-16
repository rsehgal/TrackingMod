#include "GlassRpc.h"
#include "TriggeringPlane.h"
#include <iostream>
#include <TCanvas.h>
#include "Tree.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include <TApplication.h>
#include <fstream>
#include <TStyle.h>
#include <TCanvas.h>
#include "Voxelator_Evolution.h"
#include <G4SystemOfUnits.hh>
typedef Tomography::Properties Detector;
using namespace Tomography;

void GenerateTwoDViewOfPoca(std::string);

int main(int argc, char **argv) {
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *can = new TCanvas();
	can->Divide(2,2);

	//std::string filename = std::to_string(atoi(argv[1]));
	const char* filename = argv[1];
	//GenerateTwoDViewOfPoca(temp_str);

	int xlow = -500;
	int xhigh = 500;
	int nxbins = 20;
	int ylow = -500;
	int yhigh = 500;
	int nybins = 20;
	int zlow = -450;
	int zhigh = 450;
	int nzbins = 20;

	TH2F *h2d = new TH2F("xy", "TwoD XY view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh);
	TH2F *h2dxz = new TH2F("xz", "TwoD XZ view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh);
	TH2F *h2dyz = new TH2F("yz", "TwoD YZ view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh);
	//TH2F *h2xyRL = new TH2F("xyRL", "TwoD XYRL view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh);
	TH3F *h3dxyz = new TH3F("xyz", "ThreeD XYZ view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh,nzbins,zlow,zhigh);

	gStyle->SetPalette(55);
	//gStyle->SetPalette(kDarkBodyRadiator,NULL);
	h2d->SetContour(128);
	h2d->SetStats(0);

	std::ifstream ft(filename);
	//std::ifstream ft("filteredPocaPtRunHelper-Exact.txt");
	double x=0.,y=0.,z=0;
	double color=0;
	int count = 10;
	while(!ft.eof()){
	//while(count){
		ft >> x >> y >> z >> color;
		std::cout << x << "  " << y << std::endl;
		h2d->Fill(x,y);
		h2dxz->Fill(x,z);
		h2dyz->Fill(y,z);
		count--;
	}

	h2d->Smooth();
	h2d->SetContour(60);
	h2dxz->Smooth();
	h2dxz->SetContour(60);
	h2dyz->Smooth();
	h2dyz->SetContour(60);
	//h2e->Draw("contz");

	ft.close();

	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,5*cm,5*cm,4.5*cm);
	std::ifstream voxelHandle("filteredVoxelsRunHelper-Exact.txt");
	while(!voxelHandle.eof()){
		double x = 0., y=0., z=0., sd=0., rl=0.;
		voxelHandle >> x >> y >> z >> sd >> rl ;
		int binNum = Tomography::evolution::Voxelator::instance()->GetVoxelNumber(x,y,z);
		//h3dxyz->UpdateBinContent(binNum,rl);
		h3dxyz->AddBinContent(binNum,rl);
	}



	can->cd(1);
	//h2d->Draw("contz");
	h2d->Draw("SURF3");
	can->cd(2);
	//h2dxz->Draw("colz");
	h2dxz->Draw("contz");
	can->cd(3);
	//h2dyz->Draw("colz");
	h2dyz->Draw("contz");
	can->cd(4);
	h3dxyz->Draw("LEGO2");


    fApp->Run();
}

void GenerateTwoDViewOfPoca(std::string filename){
	//Histogram Parameters should be taken from Voxelator
	int xlow = -500;
	int xhigh = 500;
	int nxbins = 20;
	int ylow = -500;
	int yhigh = 500;
	int nybins = 20;
	TH2F *h2d = new TH2F("h2d", "TwoD view of PoCA Points", nxbins, xlow, xhigh, nybins, ylow, yhigh);
	std::ifstream ft(filename);
	double x=0.,y=0.,z=0;
	double color=0;
	int count = 10;
	//while(!ft.eof()){
	while(count){
		ft >> x >> y >> z >> color;
		std::cout << x << "  " << y << std::endl;
		h2d->Fill(x,y);
		count--;
	}

	h2d->Draw();

}

