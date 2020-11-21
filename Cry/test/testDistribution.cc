/* 

Copyright (c) 2007-2012, The Regents of the University of California. 
Produced at the Lawrence Livermore National Laboratory 
UCRL-CODE-227323. 
All rights reserved. 
 
For details, see http://nuclear.llnl.gov/simulations
Please also read this http://nuclear.llnl.gov/simulations/additional_bsd.html
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
 
1.  Redistributions of source code must retain the above copyright
notice, this list of conditions and the disclaimer below.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the disclaimer (as noted below) in
the documentation and/or other materials provided with the
distribution.

3. Neither the name of the UC/LLNL nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OF
THE UNIVERSITY OF CALIFORNIA, THE U.S. DEPARTMENT OF ENERGY OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "CRYGenerator.h"
#include "CRYSetup.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>  // For Ubuntu Linux

#include <TH1F.h>
#include <TVector3.h>
#include <TApplication.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TStyle.h>
int main( int argc, const char *argv[]) {
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	TCanvas *c = new TCanvas("TestCanvas", "ScatteringAngleCanvas", 800, 600);
	c->Divide(2,2);
	gStyle->SetOptStat(0);
  int nEv=1000; //....default number of cosmic-ray events to produce

  if ( argc < 2 ) {
    std::cout << "usage " << argv[0] << " <setup file name> <N events>\n";
    std::cout << "N events = " << nEv << " by default\n";
    return 0;
  }

  if ( argc > 2 ) nEv=atoi(argv[2]);

  // Read the setup file into setupString
  std::ifstream inputFile;
  inputFile.open(argv[1],std::ios::in);
  char buffer[1000];

  std::string setupString("");
  while ( !inputFile.getline(buffer,1000).eof()) {
    setupString.append(buffer);
    setupString.append(" ");
  }

  // Parse the contents of the setup file
  CRYSetup *setup=new CRYSetup(setupString,"./data");

  // Setup the CRY event generator
  CRYGenerator gen(setup);

  TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.,M_PI/2.);
  cosSqr->SetParameters(0,0.1);
  cosSqr->SetParameters(1,2);
  int numOfBins = 40;
  TH1F *solidAngleHist = new TH1F("AngularDistribution From CRY","AngularDistribution From CRY",numOfBins,0.,M_PI/2.);
  solidAngleHist->GetYaxis()->SetTitle("#frac{dN}{d#theta}");
  solidAngleHist->GetXaxis()->SetTitle("#theta (radian)");
  TVector3 ref(0.,-1.,0.);

  // Generate the events
  std::vector<CRYParticle*> *ev=new std::vector<CRYParticle*>;
  for ( int i=0; i<nEv; i++) {
    ev->clear();
    gen.genEvent(ev);

    // Write the event information to standard output
    std::cout << "Event: " << i << std::endl;
    for ( unsigned j=0; j<ev->size(); j++) {
      CRYParticle* p = (*ev)[j];
      std::cout << "Secondary " << j 
                << " " << CRYUtils::partName(p->id()) 
                << " ke=" << p->ke() 
                << " (x,y,z)=" 
                << " " << p->x()
                << " " << p->y()
                << " " << p->z()
                << " (m)"
                << "\n";
      if(std::string(CRYUtils::partName(p->id())) == "muon")
    	  solidAngleHist->Fill(TVector3(p->u(),p->w(),p->v()).Angle(ref));

      delete (*ev)[j];
    }
  }


  TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution From CRY","Solid angle corrected AngularDistribution From CRY",numOfBins,0.,M_PI/2.);
  solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
  solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");
  std::ofstream outfile("zenithAngle.txt");
      for(int i =0  ; i < numOfBins ; i++){
      	double binCenter = solidAngleHist->GetXaxis()->GetBinCenter(i);
      	double binContent = solidAngleHist->GetBinContent(i);
      	outfile << binCenter <<" " << binContent << std::endl;
      	std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
      	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
      }
  outfile.close();

  std::cout << "Run completed.\n";
  std::cout << "Total time simulated: " << gen.timeSimulated() << " seconds\n";

  c->cd(1);
  solidAngleHist->Draw();

  c->cd(2);
 // solidAngleCorrectedHist->Set
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

  new TCanvas();
  solidAngleHist->Draw();

  new TCanvas();

  solidAngleCorrectedHist->Draw();
  solidAngleCorrectedHist->Scale(1/solidAngleCorrectedHist->Integral());
  solidAngleCorrectedHist->Fit("cosSqr");

  fApp->Run();

  return 0;
}
