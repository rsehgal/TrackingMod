#include<fstream>
#include <vector>

double Pol3(Double_t *x, Double_t *par)
{

  double fitval = par[0] + par[1] * x[0] + par[2] * pow(x[0], 2) + par[3] * pow(x[0], 3);
  return fitval;
}


void outputGr(){

TH2F *hist = new TH2F("hist","hist",1000,-2.,2.,1000,-50.,50.);
//TH2F *histQdelT = new TH2F("hist","hist",1000,-2.,2.,1000,-50.,50.);
std::ifstream infile("outputHist.csv");
double q,delt,z;
std::vector<double> qvect;
std::vector<double> zvect;
std::vector<double> deltvect;
while(!infile.eof()){
infile >> q >> delt >> z;
qvect.push_back(q);
zvect.push_back(z);
deltvect.push_back(delt);
//std::cout << z << std::endl;
hist->Fill(q,z);
//histQdelT->Fill(q,delt);
}
hist->Draw();

new TCanvas("QDelT","QDelT");
//histQdelT->Draw("colz");
TFile paramFile("fQparam_temp.root","RECREATE");
new TCanvas("Graph","Graph");
TGraph *gr=new TGraph(qvect.size(),&qvect[0],&zvect[0]);
TF1 *formula = new TF1("fQparam_temp", Pol3, -5., 5., 4);
    gr->Fit(formula, "rq");
    paramFile.cd();
    formula->Write();
paramFile.Close();

gr->Draw("ap");

new TCanvas("GraphDelt","GraphDelt");
TGraph *grD=new TGraph(deltvect.size(),&deltvect[0],&zvect[0]);
grD->Draw("ap");
}
