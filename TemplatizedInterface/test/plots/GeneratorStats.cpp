#include <TH1F.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>

int main(){

    TApplication *fApp = new TApplication("Test", NULL, NULL);
    TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
    c->Divide(3,2);
    TH1F *muonAngle = new TH1F("AngularDistribution From CRY","AngularDistribution From CRY",100,0.,M_PI/2.);
    TH1F *muonEnergy = new TH1F("EnergyDistribution From CRY","EnergyDistribution From CRY",100,-100.,10000.);
    std::ifstream infile("StatsFromGenerator.txt");
    double angle = 0., energy = 0.;
    while(!infile.eof()){
        infile >> angle >> energy ; 
        muonAngle->Fill(angle);
        if(energy > 10)
            muonEnergy->Fill(energy);
    }

    std::ifstream infile2("StatsFromEventAction.txt");
    TH1F *muonAngleEventAction = new TH1F("AngularDistribution From EventAction","AngularDistribution From EventAction",100,0.,M_PI/2.);
    TH1F *muonEnergyEventAction = new TH1F("EnergyDistribution From EventAction","EnergyDistribution From EventAction",100,-100.,10000.);
    TH1F *fittedIncomingMuonAngle = new TH1F("Fitted Incoming AngularDistribution","Fitted Incoming AngularDistribution",100,0.,M_PI/2.);
    TH1F *fittedOutgoingMuonAngle = new TH1F("Fitted Outgoing AngularDistribution","Fitted Outgoing AngularDistribution",100,0.,M_PI/2.);
    double incoming = 0., outgoing = 0., diff = 0., ener = 0, incomingFitted = 0., outgoingFitted = 0., diffFitted = 0.;
    while(!infile2.eof()){
        infile2 >> incoming >> outgoing >> diff >> incomingFitted >> outgoingFitted >> diffFitted >>  ener ; 
        muonAngleEventAction->Fill(incoming);
        fittedIncomingMuonAngle->Fill(incomingFitted);
        fittedOutgoingMuonAngle->Fill(outgoingFitted);
        if(energy > 10)
            muonEnergyEventAction->Fill(ener);
    }

    c->cd(1);
    muonAngle->Draw();
    c->cd(2);
    muonEnergy->Draw();
    c->cd(3);
    muonAngleEventAction->Draw();
    c->cd(4);
    muonEnergyEventAction->Draw();
    c->cd(5);
    fittedIncomingMuonAngle->Draw();
    c->cd(6);
    fittedOutgoingMuonAngle->Draw();
    fApp->Run();

    return 0;
}