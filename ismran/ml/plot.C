#include <iostream>
#include <fstream>
#include <string>
#include <TStyle.h>
void plot(char *filename)
{
  gStyle->SetOptStat(0);
  std::ifstream infile(filename);
  double x = 0, y = 0;
  std::string name   = "Test";
  TH2F *hist2D_Layer = new TH2F(name.c_str(), name.c_str(), 320, -80, 80, 320, -80, 80);

  while (!infile.eof()) {
    infile >> x >> y;
    hist2D_Layer->Fill(x, y);
  }
  hist2D_Layer->Draw("colz");
}
