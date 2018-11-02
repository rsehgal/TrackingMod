#include <TH1F.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include <TF1.h>
#include <TGraph.h>
int main(){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
    TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
   // TCanvas *c2 = new TCanvas("TestCanvas2", "ScatteringAngleCanvas2", 800, 600);
    c->Divide(3,2);
    int numOfBins = 40;
    double xhigh = 0.215;
    //Creating fitting function
    //TF1 *cosSqr = new TF1("cosSqr", "[2]*x*x+[1]*x+[0]*cos(x)*cos(x)", 0, M_PI/2.);
    double halfpi = M_PI/2.;
    TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.,xhigh);
    cosSqr->SetParameters(0,0.1);
    cosSqr->SetParameters(1,2);



    TH1F *solidAngleHist = new TH1F("AngularDistribution From CRY","AngularDistribution From CRY",numOfBins,0.,halfpi);
    TH1F *xAngleHist = new TH1F("AngularDistribution XZ plane From CRY","AngularDistribution XZ plane From CRY",100,0.,halfpi);
//    TH1F *yAngleHist = new TH1F("AngularDistribution YZ plane From CRY","AngularDistribution YZ plane From CRY",100,0.,M_PI/2.);
    TH1F *muonEnergy = new TH1F("EnergyDistribution From CRY","EnergyDistribution From CRY",100,-100.,10000.);
    std::ifstream infile("StatsFromGenerator.txt");
    std::string particleName = "";
    double solidAngle = 0., energy = 0.;
    while(!infile.eof()){
        infile >> particleName >> solidAngle >> energy ;
        {
            muonEnergy->Fill(energy);
            solidAngleHist->Fill(solidAngle);
        }
    }

    std::vector<double> x , y;

    TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution From CRY","Solid angle corrected AngularDistribution From CRY",numOfBins,0.,halfpi);
    for(int i =0  ; i < numOfBins ; i++){
    	double binCenter = solidAngleHist->GetXaxis()->GetBinCenter(i);
    	double binContent = solidAngleHist->GetBinContent(i);
    	std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
    	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
    	x.push_back(binCenter);
    	y.push_back(binContent/(std::sin(binCenter)));
    }

    TGraph *gr = new TGraph(x.size(),&x[0],&y[0]);
    gr->SetMarkerStyle(kStar);


    std::ifstream infile2("StatsFromEventAction.txt");
    TH1F *muonAngleEventAction = new TH1F("AngularDistribution From EventAction","AngularDistribution From EventAction",numOfBins,0.,M_PI/2.);
//    TH1F *muonAngleEventActionX = new TH1F("AngularDistribution XZ plane From EventAction","AngularDistribution XZ plane From EventAction",100,0.,M_PI/2.);
//    TH1F *muonAngleEventActionY = new TH1F("AngularDistribution YZ plane From EventAction","AngularDistribution YZ plane From EventAction",100,0.,M_PI/2.);
    TH1F *muonEnergyEventAction = new TH1F("EnergyDistribution From EventAction","EnergyDistribution From EventAction",numOfBins,-100.,10000.);
    TH1F *fittedIncomingMuonAngle = new TH1F("Fitted Incoming AngularDistribution","Fitted Incoming AngularDistribution",numOfBins,0.,M_PI/2.);
    TH1F *fittedOutgoingMuonAngle = new TH1F("Fitted Outgoing AngularDistribution","Fitted Outgoing AngularDistribution",numOfBins,0.,M_PI/2.);
    double incoming = 0., outgoing = 0., diff = 0., ener = 0, incomingFitted = 0., outgoingFitted = 0., diffFitted = 0.;
    double incomingX = 0., incomingY = 0.;
    while(!infile2.eof()){
        infile2 >> incomingX >> incomingY >> incoming >> outgoing >> diff >> incomingFitted >> outgoingFitted >> diffFitted >>  ener ;
        muonAngleEventAction->Fill(incoming);
        //muonAngleEventActionX->Fill(incomingX);
        //muonAngleEventActionY->Fill(incomingY);
        fittedIncomingMuonAngle->Fill(incomingFitted);
        fittedOutgoingMuonAngle->Fill(outgoingFitted);
        if(energy > 10)
            muonEnergyEventAction->Fill(ener);
    }
    TH1F *solidAngleCorrectedEventAction = new TH1F("AngularDistributionEventAction From CRY / Sin Theta","AngularDistributionEventAction From CRY  / Sin Theta",100,0.,M_PI/2.);
    for(int i =0  ; i < numOfBins ; i++){
    	double binCenter = muonAngleEventAction->GetXaxis()->GetBinCenter(i);
    	double binContent = muonAngleEventAction->GetBinContent(i);
    	std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
    	solidAngleCorrectedEventAction->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
    }


    c->cd(1);
    solidAngleHist->Draw();

    c->cd(2);
    solidAngleCorrectedHist->Draw();
    solidAngleCorrectedHist->Fit("cosSqr");
	{
		double ndf = cosSqr->GetNDF();
		double chi2 = cosSqr->GetChisquare();
		double prob = cosSqr->GetProb();
		std::cout << "CHI2 = " << chi2 << " : ndf : " << ndf << " : Prob : "
				<< prob << std::endl;
		std::cout << "CHI2/ndf = " << (chi2 / ndf) << std::endl;
	}


    c->cd(3);
    solidAngleCorrectedEventAction->Draw();
    solidAngleCorrectedEventAction->Fit("cosSqr");
    {
        double ndf = cosSqr->GetNDF();
        double chi2 = cosSqr->GetChisquare();
        double prob = cosSqr->GetProb();
        std::cout << "CHI2 = " << chi2 <<" : ndf : " << ndf << " : Prob : " << prob << std::endl;
        std::cout << "CHI2/ndf = " << (chi2/ndf) << std::endl;
    }

    c->cd(4);
    //gr->Draw("AP");
    //gr->Fit("cosSqr");
    muonAngleEventAction->Draw();

    c->cd(5);
    fittedIncomingMuonAngle->Draw();

    c->cd(6);
    fittedOutgoingMuonAngle->Draw();

    fApp->Run();

    return 0;
}
