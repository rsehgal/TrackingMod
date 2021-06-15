/* 
---------
DOCBEGIN
---------
** Macro to plot the 2D histogram between delT and Z Predict and Z actual
** Need a text file as an comnad line argument, having 3 columns,
** 1)delT 2) Z_predicted 3) Z_actual
**
** usage : root -l
**         .x hist2.C("filename")
**
** output : 2D histograms, as described above
--------
DOCEND
--------
*/


#include <fstream>
#include <iostream>

void hist(char *filename)
{

  TH2F *h1 = new TH2F("h1", "delt Vs Zpred", 1000, -25., 50., 1000, -60., 60.);
//  TH2F *h2 = new TH2F("h2", "delt Vs Zact", 1000, -25., 25., 1000, -60., 60.);
  std::ifstream inp;
  double x, y,yact;
  inp.open(filename);
  //   for (int i=1; i<=1000; i++)
  while (!inp.eof()) {
    inp >> x >> y;// >> yact;
    h1->Fill(x/1000., y);
    //h2->Fill(x, yact);
  }
  new TCanvas("DelTVsZPred","DelTVsZPred");
  h1->Draw();
  //new TCanvas("DelTVsZAct","DelTVsZAct");
  //h2->Draw();

}
