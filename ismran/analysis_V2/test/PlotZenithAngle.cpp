#include <iostream>
#include <fstream>
#include "Point3D.h"
#include "HardwareNomenclature.h"
#include "Plotter.h"
#include <TCanvas.h>
#include <TH1F.h>
#include <TApplication.h>
int main(int argc, char *argv[])
{
  GenerateScintMatrixXYCenters();
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  std::ifstream infile(argv[1]);
  double layer9x = -10000., layer9y = -10000., layer9z = -10000.;
  double layer8x = -10000., layer8y = -10000., layer8z = -10000.;
  double layer7x = -10000., layer7y = -10000., layer7z = -10000.;
  std::vector<std::vector<lite_interface::Point3D *>> smtVec;
  TH1F *zenithAngleHist = new TH1F("zenithAngleHist", "zenithAngleHist", 200, 0., M_PI / 2);
  while (!infile.eof()) {
    infile >> layer9x >> layer9z >> layer8x >> layer8z >> layer7x >> layer7z;
    layer9y = GetYOfLayer(9);
    layer8y = GetYOfLayer(8);
    layer7y = GetYOfLayer(7);
    std::vector<lite_interface::Point3D *> smt;
    smt.push_back(new lite_interface::Point3D(layer9x, layer9y, layer9z));
    smt.push_back(new lite_interface::Point3D(layer8x, layer8y, layer8z));
    smt.push_back(new lite_interface::Point3D(layer7x, layer7y, layer7z));
    smtVec.push_back(smt);
    double angle = (lite_interface::GetZenithAngle(smt));
    zenithAngleHist->Fill(angle);

    /*std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    std::cout << "Zenith Angle : " << (M_PI - lite_interface::GetZenithAngle(smt)) << std::endl;
    for (unsigned int i = 0; i < smt.size(); i++) {
      smt[i]->Print();
    }*/
  }

  // TH1F *zenithAngleHist = lite_interface::PlotZenithAngle(smtVec);

  new TCanvas("ZenthAngleHist_ML", "ZenithAngleHist_ML");
  zenithAngleHist->Draw();

  fApp->Run();
}
