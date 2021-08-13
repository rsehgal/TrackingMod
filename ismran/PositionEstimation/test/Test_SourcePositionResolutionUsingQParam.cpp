/*
**	Filename : Test_SourcePositionResolutionUsingQParam.cpp
**	2021-07-09
**	username : rsehgal
*/
// DOCBEGIN
/*
** This test program will map the 'Q' to 'Z', by taking data from all the source postion
** of bar. This is to compare the resolution, that we get just by using QParam.
** Ideally we should be able to see all the peaks correponding to all the source
** locations. But due to resolution in 'Z' (~18 cm), we may get less peaks.
** This program is just to visualize that how many peaks we can resove just
** using 'Q'
*/

// DOCEND

#include "includes.h"
#include <iostream>
int main(int argc, char *argv[])
{
  TApplication *fApp  = new TApplication("Test", NULL, NULL);
  TFile *fpCalib      = new TFile("completeCalib2.root", "r");
  std::string barName = argv[2];
  TF1 *paramQFormula  = (TF1 *)fpCalib->Get(("fQparam_" + barName).c_str());

  TH1F *zHist = new TH1F("Estimated Z using Q", "Estimate Z Using Q", 150, -75., 75.);
  double q, delt, z, pos;
  std::ifstream infile(argv[1]);

  unsigned int counter = 0;
  // std::vector<double> zEst;
  while (!infile.eof()) {
    counter++;
    if (!(counter % 100000)) std::cout << "Event Processed : " << counter << std::endl;
    infile >> q >> delt >> z >> pos;
    // zEst.push_back(paramQFormula->Eval(q));
    zHist->Fill(paramQFormula->Eval(q));
    //if(counter > 1000000)
    //	break;
  }

  zHist->Draw();
  fApp->Run();
}
