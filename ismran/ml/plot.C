#include <iostream>
#include <fstream>
#include <string>

void plot()
{
  std::ifstream infile("output.txt");
  double x = 0, y = 0;
  std::string name   = "Test";
  TH2F *hist2D_Layer = new TH2F(name.c_str(), name.c_str(), 200, -50, 50, 200, -50, 50);

  while (!infile.eof()) {
    infile >> x >> y;
    hist2D_Layer->Fill(x, y);
  }
  hist2D_Layer->Draw("colz");
}
