/// \file
/// \ingroup tutorial_fit
/// \notebook -js
/// Get in memory an histogram from a root file and fit a user defined function.
/// Note that a user defined function must always be defined
/// as in this example:
///  - first parameter: array of variables (in this example only 1-dimension)
///  - second parameter: array of parameters
/// Note also that in case of user defined functions, one must set
/// an initial value for each parameter.
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \author Rene Brun

Double_t fitf(Double_t *x, Double_t *par)
{
   Double_t arg = 0;
   //unsigned int parLength = sizeof(par)/sizeof(*par);
   if (par[2] != 0) arg = (x[0] - par[1])/par[2];

   Double_t fitval = par[0]*TMath::Exp(-0.5*arg*arg);
   /*
   Double_t fitval = 0.;
   
   for (int i = 0 ; i < parLength ; i+=3){
      arg = (x[0] - par[i+1])/par[i+2];
      fitval += par[i]*TMath::Exp(-0.5*arg*arg);

  }
  */
   return fitval;
}

Double_t FitMultiGausis(Double_t *x, Double_t *pars){
return fitf(x,pars)+fitf(x,&pars[3]);

}

void fit1d()
{
   /*
   TString dir = gROOT->GetTutorialDir();
   dir.Append("/hsimple.C");
   dir.ReplaceAll("/./","/");
   if (!gInterpreter->IsLoaded(dir.Data())) gInterpreter->LoadMacro(dir.Data());
   TFile *hsimpleFile = (TFile*)gROOT->ProcessLineFast("hsimple(1)");
   if (!hsimpleFile) return;

   TCanvas *c1 = new TCanvas("c1","the fit canvas",500,400);

   TH1F *hpx = (TH1F*)hsimpleFile->Get("hpx");
   */

   double xmin=-20.,xmax=100.;
   TH1F *hpx=new TH1F("Gauss","Gauss",1000, xmin,xmax);
   TRandom t;
   double mean1=5, sigma1=3;
   double mean2=20, sigma2=4;
   for(int i = 0 ; i < 10000; i++){
	double val1=t.Gaus(mean1,sigma1);
	double val2=t.Gaus(mean2,sigma2);
 	if(val1 < xmax && val1 > xmin)
		hpx->Fill(val1);
        if(val2 < xmax && val2 > xmin)
                hpx->Fill(val2);
  
   }

// Creates a Root function based on function fitf above
   //TF1 *func = new TF1("fitf",fitf ,-4,4,3);
   //TF1 *func = new TF1("fitf",fitf ,xmin,xmax,3);
   TF1 *func = new TF1("fitf",FitMultiGausis ,xmin,xmax,6);

// Sets initial values and parameter names
   func->SetParameters(100,0,1,110,0,2);
   func->SetParNames("Constant","Mean_value","Sigma","Constant2","Mean_value2","Sigma2");

// Fit histogram in range defined by function
   hpx->Fit(func,"r");

// Gets integral of function between fit limits
   printf("Integral of function = %g\n",func->Integral(-2,2));
}
