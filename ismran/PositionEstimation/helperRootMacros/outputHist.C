#include<fstream>
void outputHist(){

TH1F *hist = new TH1F("hist","hist",100,-50.,50.);
std::ifstream infile("outputHist.csv");
double q,delt,z;
while(!infile.eof()){
infile >> q >> delt >> z;
//std::cout << z << std::endl;
hist->Fill(z);
}
hist->Draw();
}
